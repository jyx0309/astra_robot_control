from glob import glob
from setuptools import setup
setup(name='insight_camera', version='0.1.0', packages=['insight_camera'],
      data_files=[('share/ament_index/resource_index/packages', ['resource/insight_camera']),
                  ('share/insight_camera', ['package.xml', 'README.md']),
                  ('share/insight_camera/launch', glob('launch/*.py')),
                  ('share/insight_camera/config', glob('config/*.yaml'))],
      install_requires=['setuptools'], tests_require=['pytest'], zip_safe=True,
      maintainer='yuxuan', maintainer_email='yuxuan@example.com',
      description='Insight9 ROS 原生观察快照服务', license='Apache-2.0',
      entry_points={'console_scripts': ['observation_node = insight_camera.node:main']})
