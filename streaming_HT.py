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
class VideoStream(object, threading.Thread):
    def __init__(self, src=0):
        self.stream = cv2.VideoCapture(src)
        (self.grabbed, self.frame) = self.stream.read()
        self.stopStream = False
    def run(self):
        while True:
            if(self.stopStream):
                return
            (self.grabbed, self.frame) = self.stream.read()
    def stopStream(self):
        self.stream.release()
        self.stopStream = True
'''**************************************'''

cv2.destroyAllWindows() #End of code