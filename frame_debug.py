from processing import *
import argparse
import time


arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str, help="image source")
arg.add_argument('-r', '--res', type=str, help='video resolution')
args = vars(arg.parse_args())


def draw(frameToShow, lines, lineMargin, lanesNum, afps, fps):
    if lines is not None:
        if lanesNum != "none":
            for line in lines:
                cv2.line(frameToShow, (int(line[0]), int(line[1])), (int(line[2]), int(line[3])), (255, 0, 0),
                         3, cv2.LINE_AA)
        cv2.rectangle(frameToShow, (0, 0), (200, 50), (0, 0, 0), 1, cv2.LINE_AA)
        cv2.putText(frameToShow, "status: ", (10, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.4
                    , (255, 0, 0), 1, cv2.LINE_AA)
        cv2.putText(frameToShow, "AFPS: ", (10, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.4
                    , (255, 0, 0), 1, cv2.LINE_AA)
        if lineMargin == 0:
            cv2.putText(frameToShow, "searching for lanes..", (60, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.4
                        , (0, 0, 255), 1, cv2.LINE_AA)
        if lineMargin > 0:
            cv2.putText(frameToShow, lanesNum + "-lane road", (60, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.4
                        , (0, 255, 0), 1, cv2.LINE_AA)
        if afps >= fps:
            cv2.putText(frameToShow, str(afps), (60, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.4
                        , (0, 255, 0), 1, cv2.LINE_AA)
        else:
            cv2.putText(frameToShow, str(afps), (60, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.4
                        , (0, 0, 255), 1, cv2.LINE_AA)
        if arrow is not None and lineMargin > 0:
            cv2.arrowedLine(frameToShow, (int(arrow[0]), int(arrow[1]))
                            , (int(arrow[2]), int(arrow[3])), (0, 255, 0), 5, cv2.LINE_AA, 0, 0.3)

print("[INFO] application starts")

frame = cv2.imread(args['video'])
print(frame[100][100][0], frame[100][100][1], frame[100][100][2])
pts, dst = determinePtsAndDst(args, ".png")
start = time.time()
ipmFrame, homo = fourPointTransform(frame, pts, dst)
lines = lineSegmentDetector(ipmFrame)
margin, lanesNum = calcMargin(lines, 40, ipmFrame.shape[1])
print(margin)
lines = filterAndAverage(lines, 0, ipmFrame.shape[1], 40, margin, ipmFrame.shape[:2])
lines, arrow = doInverse(lines, homo, args)
draw(frame, lines, margin, lanesNum, 100, 30)
end = time.time()
print('Time is: %s' % (end - start))
cv2.imshow('image', ipmFrame)
cv2.waitKey(0)
cv2.destroyAllWindows()
