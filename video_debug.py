from processing import *
import numpy.polynomial.polynomial as poly
import argparse
import time

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
    # draw(ipmFrame, lines)
    left_points, right_points = combineLineSegments(lines, ipmFrame)
    if left_points.size > 0:
        left_points = curveFit(left_points[:, 0], left_points[:, 1], 2, ipmFrame.shape[0], 10)
        left_points = doInverse(left_points, homo)
        draw(left_points, normalFrame)
    if right_points.size > 0:
        right_points = curveFit(right_points[:, 0], right_points[:, 1], 2, ipmFrame.shape[0], 10)
        right_points = doInverse(right_points, homo)
        draw(right_points, normalFrame)
    end = time.time()
    print(end-start)
    cv2.imshow('video', normalFrame)
stream.release()
cv2.destroyAllWindows()
