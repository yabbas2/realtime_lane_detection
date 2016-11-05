import cv2
import numpy as np
import argparse
import threading
import time

'''***********Arguments************'''
arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str,help="Your video")
arg.add_argument('-t', '--threading', type=str, default="no", help="Enable or disable Multi-thread mode")
args = vars(arg.parse_args())
'''**********************************'''
'''**********Streaming In Thread*********'''
class VideoStreamIn(threading.Thread):
    def __init__(self, src):
        threading.Thread.__init__(self)
        self.stream = cv2.VideoCapture(src)
        self.stop = False
        self.frame = None
    def stopStream(self):
        #self.stream.release()
        self.stop = True
    def run(self):
        while True:
            if(self.stop):
                return
            (self.grabbed, self.frame) = self.stream.read()
            if not self.grabbed:
                print "[INFO] End of streaming video.."
                self.stopStream()
    def readFrame(self):
        return self.frame
    def streamFlag(self):
        return self.stop
'''**************************************'''
'''**************Streaming Out Thread*********'''
class VideoStreamOut(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.frame = None
    def run(self):
        while True:
            thread_lock.acquire()
            if self.frame is not None:
                cv2.imshow('video', self.frame)
            thread_lock.release()
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
        cv2.destroyAllWindows()
        video_in.stopStream()
    def showFrame(self, f):
        self.frame = f
'''**********************************************'''
'''***************main code**************'''
print "[INFO] Starting streaming.."
thread_lock = threading.Lock()
if args['video'] is not None: #decide video source
    video_in = VideoStreamIn(src=args['video'])
else:
    video_in = VideoStreamIn(src=0)
video_out = VideoStreamOut()
video_in.start()    #start thread of streaming in
video_out.start()   #start thread of streaming out
while True:
    frame = video_in.readFrame()
    if frame is None:
        continue
    if video_in.streamFlag():
        break
    video_out.showFrame(frame)


print "[INFO] End of program!"