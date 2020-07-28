
import sys
import traceback
import argparse

import gi
gi.require_version('Gst', '1.0')
from gi.repository import Gst, GObject, GLib

def on_message(bus: Gst.Bus, message:Gst.Message, loop: GLib.MainLoop):
    mtype = message.type
    if mtype == Gst.MessageType.EOS:
        print('End Of Stream')
        loop.quit()
    elif mtype == Gst.MessageType.ERROR:
        err, debug = message.parse_error()
        print(err, debug)
        loop.quit()
    elif mtype == Gst.MessageType.WARNING:
        err, debug = message.parse_warning()
        print(err, debug)
    
    return True

def main():  

    Gst.init(sys.argv)

    DEFAULT_PIPELINE =  "" #"videotestsrc ! video/x-raw, framerate=30/1, width=320, height=240 ! timeoverlay font-desc=\"Times 60\" ! autovideosink"  # "videotestsrc num-buffers=100 ! autovideosink"
    ap = argparse.ArgumentParser()
    ap.add_argument("-p", "--pipeline", required=False, default=DEFAULT_PIPELINE, help="Gstreamer pipeline without gst-launch")

    args = vars(ap.parse_args())
    print(args)

    command = args["pipeline"]
    pipeline = Gst.parse_launch(command)
    bus = pipeline.get_bus()
    bus.add_signal_watch()
    pipeline.set_state(Gst.State.PLAYING)

    loop = GLib.MainLoop()
    bus.connect("message", on_message, loop)

    try:
        loop.run()
    except Exception:
        traceback.print_exc
        loop.quit()

    pipeline.set_state(Gst.State.NULL)



if __name__ == "__main__":    
    main()

