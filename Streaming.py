import threading, cv2

thread_lock = threading.Lock()

'''*************Streaming In Thread****************'''
class VideoStreamIn(threading.Thread):
    def __init__(self, src):
        threading.Thread.__init__(self)
        self.stream = cv2.VideoCapture(src)
        self.stop = False
        self.frame = None
    def stopStream(self):
        self.stop = True
    def run(self):
        while True:
            if(self.stop):
                print "[INFO] End of streaming.."
                return
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
        self.frame = None
        self.EndFlag = False
        self.EndStream = False
    def run(self):
        while True:
            thread_lock.acquire()
            if self.frame is not None:
                cv2.imshow('video', self.frame)
            thread_lock.release()
            if cv2.waitKey(1) & 0xFF == ord('q') or self.EndStream:
                break
        cv2.destroyAllWindows()
        self.EndFlag = True
    def showFrame(self, f):
        self.frame = f
    def getShowFlag(self):
        return self.EndFlag
    def setEndStream(self):
        self.EndStream = True
'''**********************************************'''