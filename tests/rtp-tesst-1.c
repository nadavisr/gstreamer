#include <gst/gst.h>

//"videotestsrc ! video/x-raw, framerate=500/1 ! timeoverlay font-desc=\"Sans 60\" !  videoconvert ! x264enc ! video/x-h264 ! tee name=t t. ! queue ! rtph264pay pt=96 ! udpsink host=127.0.0.1 port=5000 sync=true  t. ! queue ! avdec_h264 ! videoconvert ! autovideosink";


int
main (int argc, char *argv[])
{
  GstElement *pipeline;
  GstBus *bus;
  GstMessage *msg;
  char* cmd = "rtpbin name=rtpbin \
				videotestsrc ! video/x-raw, width=640, height=480, framerate=30/1, format=I420, depth=8 ! timeoverlay font-desc=\"Sans 60\" time-mode=running-time \
				! tee name=t  \
				t. ! queue ! videoconvert ! x264enc tune=zerolatency ! video/x-h264 ! rtph264pay pt=96 ! rtpbin.send_rtp_sink_0 \
				t. ! queue ! videoscale ! video/x-raw, width=320, height=120, framerate=30/1, format=I420 ! rtpvrawpay pt=96 ! rtpbin.send_rtp_sink_1 \
				t. ! queue ! videoconvert ! autovideosink sync=true \
				rtpbin.send_rtp_src_0 ! udpsink host=127.0.0.1 port=50000 sync=true async=false \
				rtpbin.send_rtcp_src_0 ! udpsink host=127.0.0.1 port=50001 sync=false async=false \
				rtpbin.send_rtp_src_1 ! udpsink host=127.0.0.1 port=50002 sync=true async=false \
				rtpbin.send_rtcp_src_1 ! udpsink host=127.0.0.1 port=50003 sync=false async=false";

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  
  /* Build the pipeline */  
  pipeline = gst_parse_launch (cmd, NULL);
     
  /* Start playing */
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

  /* Wait until error or EOS */
  bus = gst_element_get_bus (pipeline);
  msg =
      gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
      GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

  /* Free resources */
  if (msg != NULL)
    gst_message_unref (msg);
  gst_object_unref (bus);
  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (pipeline);
  return 0;
}
