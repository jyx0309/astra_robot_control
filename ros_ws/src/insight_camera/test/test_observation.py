from insight_camera.node import choose_views


class Header:
    class Stamp:
        sec = 0
        nanosec = 0

    stamp = Stamp()


class Image:
    header = Header()


def image(stamp):
    msg = Image()
    msg.header = Header()
    msg.header.stamp = Header.Stamp()
    msg.header.stamp.sec, msg.header.stamp.nanosec = divmod(stamp, 10**9)
    return msg


def test_three_view_span_and_missing_stream():
    frames = {'rgb': [image(120)], 'left': [image(111)], 'right': [image(129)]}
    assert choose_views(frames, 100, 10, ['left', 'right', 'rgb']) is None
    assert choose_views(frames, 100, 20, ['left', 'right', 'rgb']) is not None
    frames['right'] = []
    assert choose_views(frames, 100, 20, ['left', 'right', 'rgb']) is None


def test_view_selection_uses_latest_complete_set():
    frames = {
        'rgb': [image(100), image(200)],
        'left': [image(101), image(201)],
        'right': [image(99), image(199)],
    }
    selected = choose_views(frames, 100, 5, ['left', 'right', 'rgb'])
    assert selected['rgb'].header.stamp.nanosec == 200


def test_observation_schema_is_three_images():
    metadata = {
        'views': [
            {'role': 'left', 'file': 'left.png'},
            {'role': 'right', 'file': 'right.png'},
            {'role': 'rgb', 'file': 'rgb.png'},
        ]
    }
    assert [view['file'] for view in metadata['views']] == [
        'left.png', 'right.png', 'rgb.png']


def test_boundary_rejects_motion_frames_and_reuses_post_action_cache():
    frames = {name: [image(100), image(200)] for name in ('left', 'right', 'rgb')}
    assert choose_views(frames, 200, 5, ['left', 'right', 'rgb']) is None
    assert choose_views(frames, 150, 5, ['left', 'right', 'rgb'])['rgb'].header.stamp.nanosec == 200
