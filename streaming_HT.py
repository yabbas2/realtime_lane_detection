import cv2
import numpy as np
import argparse
import threading

'''***********Arguments************'''
arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str,help="Your video")
arg.add_argument('-t', '--threading', type=str, default="no", help="Enable or disable Multi-thread mode")
args = vars(arg.parse_args())
'''**********************************'''
'''**********Streaming Thread*********'''
class VideoStream(threading.Thread):
    def __init__(self, src=0):
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
                print "[INFO] End of streaming.."
                self.stopStream()
    def readFrame(self):
        return self.frame
    def streamFlag(self):
        return self.stop
'''**************************************'''
'''***************main code**************'''
print "[INFO] Starting streaming.."
video = VideoStream(src=0) #decide video source
video.start() #start thread of streaming
while True:
    frame = video.readFrame()
    if frame is None:
        continue
    cv2.imshow('frame', frame)
    if cv2.waitKey(1) & 0xFF == ord('q') or video.streamFlag():
        break

print "[INFO] End of stream.."
cv2.destroyAllWindows() #End of code
video.stopStream()