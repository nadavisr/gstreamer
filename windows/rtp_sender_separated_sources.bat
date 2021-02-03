gst-launch-1.0 rtpbin name=rtpbin videotestsrc do-timestamp=true ! video/x-raw, framerate=30/1, format=I420, depth=8 ! ^
timeoverlay font-desc="Sans 60" time-mode=running-time ! tee name=t ^
t. ! queue ! videoconvert ! x264enc ! video/x-h264 ! rtph264pay pt=96 ! rtpbin.send_rtp_sink_0 ^
rtpbin.send_rtp_src_0 ! udpsink host=127.0.0.1 port=50000 sync=true async=false ^
rtpbin.send_rtcp_src_0 ! udpsink host=127.0.0.1 port=50001 sync=false async=false ^
videotestsrc do-timestamp=true pattern=white ! video/x-raw, width=320, height=120, framerate=30/1, format=I420, depth=8 !  ^
timeoverlay font-desc="Sans 60" time-mode=running-time !  queue ! rtpvrawpay pt=96 ! rtpbin.send_rtp_sink_1 ^
rtpbin.send_rtp_src_1 ! udpsink host=127.0.0.1 port=50002 sync=true async=false ^
rtpbin.send_rtcp_src_1 ! udpsink host=127.0.0.1 port=50003 sync=false async=false 

REM removed " t. ! queue ! videoconvert ! autovideosink "  from sender because it get freezes.
REM probably because different latencies of x264enc and autovideosink. see x264enc gstreamer documentation 
REM tried to play with queue settings : queue max-size-buffers=0 max-size-time=0  max-size-bytes=100000000 but it didn't help
REM other settings like sync, async, encoder settings and others didn't help much.
REM in linux there is no such problem. maybe related to specific x264enc or autovideosink implimentation 
REM anyway - we don't really need to show the sender side


