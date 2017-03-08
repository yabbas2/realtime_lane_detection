import threading
import cv2
import numpy as np


class stream(threading.Thread):
    def __init__(self, src):
        threading.Thread.__init__(self)
        self.stream = cv2.VideoCapture(src)
        self.fps = self.stream.get(cv2.CAP_PROP_FPS)
        self.height = self.stream.get(cv2.CAP_PROP_FRAME_HEIGHT)
        self.width = self.stream.get(cv2.CAP_PROP_FRAME_WIDTH)
        self.frameCount = self.stream.get(cv2.CAP_PROP_FRAME_COUNT)
        self.afps = 0
        self.left_points = []
        self.right_points = []
        self.normalFrame = None
        self.frameToShow = None
        self.locker = True
        self.streamStatus = True

    def run(self):
        while True:
            (grabbed, self.normalFrame) = self.stream.read()
            if not grabbed:
                break
            self.frameToShow = self.normalFrame.copy()
            self.draw()
            if cv2.waitKey(int(self.fps)) & 0xFF == ord('q'):
                break
            cv2.imshow('video', self.frameToShow)
        self.stream.release()
        cv2.destroyAllWindows()
        self.streamStatus = False
        exit(0)

    def getStreamStatus(self):
        return self.streamStatus

    def readFrame(self):
        if self.normalFrame is not None:
            return self.normalFrame
        else:
            return None

    def getInfo(self):
        return self.fps, self.width, self.height, self.frameCount

    def setInfo(self, lps, rps, afps):
        self.locker = False
        if len(lps) != 0:
            self.left_points = lps
        if len(rps) != 0:
            self.right_points = rps
        self.afps = afps
        self.locker = True

    def draw(self):
        if self.locker:
            if len(self.right_points) != 0:
                cv2.polylines(self.frameToShow, np.int32([self.right_points]), False, (0, 255, 0), 2, cv2.LINE_AA)
            if len(self.left_points) != 0:
                cv2.polylines(self.frameToShow, np.int32([self.left_points]), False, (0, 255, 0), 2, cv2.LINE_AA)
            cv2.rectangle(self.frameToShow, (0, 0), (200, 50), (0, 0, 0), 1, cv2.LINE_AA)
            cv2.putText(self.frameToShow, "status: ", (10, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.4
                        , (255, 0, 0), 1, cv2.LINE_AA)
            cv2.putText(self.frameToShow, "AFPS: ", (10, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.4
                        , (255, 0, 0), 1, cv2.LINE_AA)
            if self.afps >= self.fps:
                cv2.putText(self.frameToShow, str(self.afps), (60, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.4
                            , (0, 255, 0), 1, cv2.LINE_AA)
            else:
                cv2.putText(self.frameToShow, str(self.afps), (60, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.4
                            , (0, 0, 255), 1, cv2.LINE_AA)
