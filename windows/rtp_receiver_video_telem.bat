gst-launch-1.0 rtpbin name=rtpbin ^
udpsrc address=127.0.0.1 port=50000 caps="application/x-rtp, media=(string)video, encoding-name=(string)H264, clock-rate=(int)90000" ! rtpbin.recv_rtp_sink_0 ^
udpsrc address=127.0.0.1 port=50001 caps="application/x-rtcp" ! rtpbin.recv_rtcp_sink_0 ^
udpsrc address=127.0.0.1 port=50002 caps="application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)RAW, sampling=YCbCr-4:2:0,depth=(string)8,width=(string)320, height=(string)120,colorimetry=(string)BT601-5, payload=(int)96, framerate=(fraction)30/1" ! rtpbin.recv_rtp_sink_1 ^
udpsrc address=127.0.0.1 port=50003 caps="application/x-rtcp" ! rtpbin.recv_rtcp_sink_1 ^
rtpbin. ! rtph264depay ! queue ! avdec_h264 ! autovideosink sync=true ^
rtpbin. ! rtpvrawdepay ! queue ! autovideosink sync=true 