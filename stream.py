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
        self.normalFrame2 = None
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
            self.normalFrame2 = self.normalFrame.copy()
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
            if len(self.right_points) == 0 or len(self.left_points) == 0:
                return
            bottom_points = list()
            top_points = list()
            if len(self.right_points) != 0:
                top_points.append(self.right_points[0])
                bottom_points.append(self.right_points[0])
            if len(self.left_points) != 0:
                top_points.append(self.left_points[0])
                bottom_points.append(self.left_points[0])
            top_points = np.array(top_points, dtype="int32")
            bottom_points = np.array(bottom_points, dtype="int32")
            cv2.polylines(self.normalFrame2, np.int32([top_points]), False, (255, 127, 127), 2, cv2.LINE_AA)
            cv2.polylines(self.normalFrame2, np.int32([bottom_points]), False, (255, 127, 127), 2, cv2.LINE_AA)
            cv2.polylines(self.normalFrame2, np.int32([self.left_points]), False, (255, 127, 127), 2, cv2.LINE_AA)
            cv2.polylines(self.normalFrame2, np.int32([self.right_points]), False, (255, 127, 127), 2, cv2.LINE_AA)
            mask = cv2.inRange(self.normalFrame2, (255, 127, 127), (255, 127, 127))
            im2, contours, hierarchy = cv2.findContours(mask, 1, 2)
            if len(contours) == 0:
                contours = self.prev_contours
            if len(contours) != 0:
                cv2.fillConvexPoly(self.normalFrame2, contours[0], (0, 255, 0))
            cv2.addWeighted(self.normalFrame2, 0.5, self.frameToShow, 1 - 0.5, 0, self.frameToShow)
            self.prev_contours = contours


