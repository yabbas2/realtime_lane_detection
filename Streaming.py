import threading, cv2, time
import numpy as np

thread_lock = threading.Lock()

'''*************Streaming In Thread****************'''
class VideoStreamIn(threading.Thread):
    def __init__(self, src):
        threading.Thread.__init__(self)
        self.videoSource = src
        self.stream = cv2.VideoCapture(self.videoSource)
        self.stop = False
        self.frame = None
    def stopStream(self):
        self.stop = True
    def run(self):
        while True:
            if(self.stop):
                print "[INFO] End of streaming.."
                return
            time.sleep(0.1)
            (self.grabbed, self.frame) = self.stream.read()
            if not self.grabbed:
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
        self.frame1 = None
        self.frame2 = None
        self.EndFlag = False
        self.EndStream = False
    def run(self):
        while True:
            thread_lock.acquire()
            if self.frame1 is not None and self.frame2 is not None:
                #cv2.imshow('video', np.hstack([self.frame1, self.frame2]))
                cv2.imshow('video', self.frame2)
            thread_lock.release()
            if cv2.waitKey(1) & 0xFF == ord('q') or self.EndStream:
                break
        cv2.destroyAllWindows()
        self.EndFlag = True
    def showFrame(self, f1, f2):
        self.frame1 = f1
        self.frame2 = f2
    def getShowFlag(self):
        return self.EndFlag
    def setEndStream(self):
        self.EndStream = True
'''**********************************************'''