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
    left_points, right_points = enhanceCurveFitting(left_points, right_points, homo, ipmFrame)
    draw(left_points, ipmFrame)
    draw(right_points, ipmFrame)
    end = time.time()
    print(end-start)
    cv2.imshow('video', ipmFrame)
stream.release()
cv2.destroyAllWindows()
