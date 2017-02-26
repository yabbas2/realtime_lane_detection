import threading
import cv2


class stream(threading.Thread):
    def __init__(self, src):
        threading.Thread.__init__(self)
        self.stream = cv2.VideoCapture(src)
        self.fps = self.stream.get(cv2.CAP_PROP_FPS)
        self.afps = 0
        self.stop = False
        self.normalFrame = None
        self.frameToShow = None
        self.lines = None
        self.arrow = None
        self.lanesNum = "zero"
        self.lineMargin = 0
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
            return self.normalFrame.copy()
        else:
            return None

    def setInfo(self, detectedLanes, arrow, margin, lanesNumber, afps):
        self.locker = False
        self.lines = detectedLanes
        self.arrow = arrow
        self.lanesNum = lanesNumber
        self.lineMargin = margin
        self.afps = afps
        self.locker = True

    def draw(self):
        if self.lines is not None and self.locker:
            for line in self.lines:
                cv2.line(self.frameToShow, (int(line[0]), int(line[1])), (int(line[2]), int(line[3])), (255, 0, 0),
                         3, cv2.LINE_AA)
            cv2.rectangle(self.frameToShow, (0, 0), (200, 50), (0, 0, 0), 1, cv2.LINE_AA)
            cv2.putText(self.frameToShow, "status: ", (10, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.4
                        , (255, 0, 0), 1, cv2.LINE_AA)
            cv2.putText(self.frameToShow, "AFPS: ", (10, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.4
                        , (255, 0, 0), 1, cv2.LINE_AA)
            if self.lineMargin == 0:
                cv2.putText(self.frameToShow, "searching for lanes..", (60, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.4
                            , (0, 0, 255), 1, cv2.LINE_AA)
            if self.lineMargin > 0:
                cv2.putText(self.frameToShow, self.lanesNum+"-lane road" , (60, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.4
                            , (0, 255, 0), 1, cv2.LINE_AA)
            if self.afps >= self.fps:
                cv2.putText(self.frameToShow, str(self.afps), (60, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.4
                            , (0, 255, 0), 1, cv2.LINE_AA)
            else:
                cv2.putText(self.frameToShow, str(self.afps), (60, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.4
                            , (0, 0, 255), 1, cv2.LINE_AA)
            if self.arrow is not None and self.lineMargin > 0:
                cv2.arrowedLine(self.frameToShow, (int(self.arrow[0]), int(self.arrow[1]))
                                , (int(self.arrow[2]), int(self.arrow[3])), (0, 255, 0), 5, cv2.LINE_AA, 0, 0.3)
