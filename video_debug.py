from processing import *
import numpy.polynomial.polynomial as poly
import argparse
import time


def draw(image, lines):
    for line in lines:
        cv2.line(image, (int(line[0]), int(line[1])), (int(line[2]), int(line[3])), (0, 0, 255), 3, cv2.LINE_AA)


def draw2(image, points):
    cv2.polylines(image, np.int32([points]), False, (0, 0, 255), 3, cv2.LINE_AA)

arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str, help="video source")
arg.add_argument('-r', '--res', type=str, help="video resolution")
args = vars(arg.parse_args())

pts, dst = determinePtsAndDst(args, ".mp4")
stream = cv2.VideoCapture(args['video'])
fps = stream.get(cv2.CAP_PROP_FPS)
xPoints = []
yPoints = []
while True:
    (grabbed, normalFrame) = stream.read()
    if not grabbed:
        break
    if cv2.waitKey(int(fps)) & 0xFF == ord('q'):
        break
    start = time.time()
    ipmFrame, homo = fourPointTransform(normalFrame, pts, dst)
    lines = lineSegmentDetector(ipmFrame)
    lines = eliminateFalseDetection(lines)
    left_points, right_points = combineLineSegments(lines, int(ipmFrame.shape[1]/2), ipmFrame)
    left_points = doInverse(left_points, homo)
    right_points = doInverse(right_points, homo)
    draw2(normalFrame, left_points)
    draw2(normalFrame, right_points)
    end = time.time()
    print(end-start)
    cv2.imshow('video', normalFrame)
stream.release()
cv2.destroyAllWindows()
