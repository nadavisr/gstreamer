#include <gst/gst.h>

int
main (int argc, char *argv[])
{
  GstElement *pipeline;
  GstBus *bus;
  GstMessage *msg;
  char* cmd = "rtpbin name=rtpbin \
				udpsrc address=127.0.0.1 port=50000 caps=\"application/x-rtp, media=(string)video, encoding-name=(string)H264, clock-rate=(int)90000\" ! rtpbin.recv_rtp_sink_0 \
				udpsrc address=127.0.0.1 port=50001 caps=\"application/x-rtcp\" ! rtpbin.recv_rtcp_sink_0 \
				udpsrc address=127.0.0.1 port=50002 caps=\"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)RAW, sampling=YCbCr-4:2:0,depth=(string)8,width=(string)320, height=(string)120,colorimetry=(string)BT601-5, payload=(int)96, framerate=(fraction)30/1\" ! rtpbin.recv_rtp_sink_1 \
				udpsrc address=127.0.0.1 port=50003 caps=\"application/x-rtcp\" ! rtpbin.recv_rtcp_sink_1 \
				rtpbin. ! rtph264depay ! queue ! avdec_h264 ! autovideosink sync=true \
				rtpbin. ! rtpvrawdepay ! queue ! autovideosink sync=true ";

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Build the pipeline */
  
  pipeline =
      gst_parse_launch (cmd, NULL);
      //("udpsrc address=127.0.0.1 port=5000 ! application/x-rtp,clock-rate=90000,payload=96 ! rtph264depay ! avdec_h264 ! videoconvert ! autovideosink", NULL);
      //("videotestsrc ! video/x-raw, framerate=500/1 ! timeoverlay font-desc=\"Sans 60\" !  videoconvert ! x264enc ! video/x-h264 ! tee name=t t. ! queue ! rtph264pay pt=96 ! udpsink host=127.0.0.1 port=5000 sync=true  t. ! queue ! avdec_h264 ! videoconvert ! autovideosink", NULL);
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
