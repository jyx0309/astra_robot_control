from insight_camera.device_clock import DeviceClock


def test_uptime_is_mapped_without_rewriting_source():
    clock=DeviceClock()
    for i in range(10):
        source=200_000_000_000+i*30_000_000
        arrival=700_000_000_000+i*30_000_000
        assert clock.update('rgb',source,arrival)[0]
    assert clock.ready(arrival)
    assert clock.mapped(source)==arrival
    assert clock.mapped(source-1_000_000_000)<arrival-500_000_000


def test_repeated_frame_does_not_refresh_clock_and_reset_requires_warmup():
    clock=DeviceClock()
    clock.update('rgb',1000,10000)
    assert clock.update('rgb',1000,20000)==(False,False)
    assert clock.count==1
    assert clock.update('rgb',500,30000)==(True,True)
    assert not clock.ready(30000)


def test_variable_transport_delay_does_not_move_old_frames_forward():
    clock=DeviceClock()
    clock.update('rgb',100_000_000,1_100_000_000)
    clock.update('rgb',200_000_000,2_200_000_000)
    assert clock.mapped(200_000_000)==1_200_000_000
