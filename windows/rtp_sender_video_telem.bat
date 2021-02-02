gst-launch-1.0 rtpbin name=rtpbin videotestsrc ! video/x-raw, width=640, height=480, framerate=30/1, format=I420, depth=8 ! timeoverlay font-desc="Sans 60" time-mode=running-time ^
! tee name=t  ^
t. ! queue ! videoconvert ! x264enc tune=zerolatency ! video/x-h264 ! rtph264pay pt=96 ! rtpbin.send_rtp_sink_0 ^
t. ! queue ! videoscale ! video/x-raw, width=320, height=120, framerate=30/1, format=I420 ! rtpvrawpay pt=96 ! rtpbin.send_rtp_sink_1 ^
t. ! queue ! videoconvert ! autovideosink sync=true ^
rtpbin.send_rtp_src_0 ! udpsink host=127.0.0.1 port=50000 sync=true async=false ^
rtpbin.send_rtcp_src_0 ! udpsink host=127.0.0.1 port=50001 sync=false async=false ^
rtpbin.send_rtp_src_1 ! udpsink host=127.0.0.1 port=50002 sync=true async=false ^
rtpbin.send_rtcp_src_1 ! udpsink host=127.0.0.1 port=50003 sync=false async=false