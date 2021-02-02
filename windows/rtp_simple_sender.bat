gst-launch-1.0 videotestsrc ! video/x-raw, framerate=500/1 ! timeoverlay font-desc="Sans 60" !  videoconvert ! x264enc ! video/x-h264 ! tee name=t ^
 t. ! queue ! rtph264pay pt=96 ! udpsink host=127.0.0.1 port=5000 sync=true ^
 t. ! queue ! avdec_h264 ! videoconvert ! autovideosink