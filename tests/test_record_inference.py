import base64
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest


SCRIPT = Path(__file__).resolve().parents[1] / 'scripts' / 'record_inference.py'


class RecordingTest(unittest.TestCase):
    def test_archive_result_and_replay(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            session = root / 'session'
            def run(*args, ok=True):
                result = subprocess.run([sys.executable, str(SCRIPT), *map(str, args)],
                                        capture_output=True, text=True)
                self.assertEqual(result.returncode == 0, ok, result.stderr)
                return result.stdout.strip()
            run('init', session, '--task', '<script>test</script>')
            observation = root / 'observation'
            observation.mkdir()
            png = base64.b64decode('iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAQAAAC1HAwCAAAAC0lEQVR42mP8/x8AAwMCAO+aWZkAAAAASUVORK5CYII=')
            (observation / 'rgb.png').write_bytes(png)
            metadata = observation / 'metadata.json'
            metadata.write_text(json.dumps({'views': [{'role': 'rgb', 'file': 'rgb.png', 'stamp_ns': 123}]}))
            step = run('add', session, '--observation', metadata, '--summary', '目标可见 <目标>', '--defer-render')
            self.assertNotIn('目标可见', (session / 'index.html').read_text())
            (observation / 'rgb.png').unlink()
            self.assertEqual((session / 'steps' / step / 'view_0.png').read_bytes(), png)
            result = root / 'result.txt'
            result.write_text('success: false\n原因：IK 拒绝', encoding='utf-8')
            run('result', session, step, '--result-file', result)
            run('result', session, step, '--result-file', result, ok=False)
            run('export', session)
            page = (session / 'index.html').read_text()
            self.assertIn('IK 拒绝', page)
            self.assertIn('&lt;script&gt;', page)
            self.assertNotIn('<script>', page)
            run('add', session, '--observation', metadata, '--summary', 'missing image', ok=False)
            self.assertEqual(len(list((session / 'steps').iterdir())), 1)
            self.assertFalse(list(session.glob('.pending-*')))
            run('result', session, '..', '--result-file', result, ok=False)


if __name__ == '__main__':
    unittest.main()
