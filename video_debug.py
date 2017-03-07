from processing import *
import re
import argparse
import time

arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str, help="video source")
args = vars(arg.parse_args())

stream = cv2.VideoCapture(args['video'])
sample = re.findall("[a-z A-Z0-9\\\-_?&()#@/]+(sample[0-9]+).[a-z0-9]+", args['video'])
width = stream.get(cv2.CAP_PROP_FRAME_WIDTH)
height = stream.get(cv2.CAP_PROP_FRAME_HEIGHT)
fps = stream.get(cv2.CAP_PROP_FPS)
pts, dst = determinePtsAndDst(width, height, ''.join(sample))
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
    # for line in lines:
    #    cv2.line(ipmFrame, (int(line[0]), int(line[1])), (int(line[2]), int(line[3])), (255, 0, 0), 1, cv2.LINE_AA)
    left_points, right_points = combineLineSegments(lines, ipmFrame)
    left_points, right_points = enhanceCurveFitting(left_points, right_points, ipmFrame)
    # left_points = doInverse(left_points, homo)
    # right_points = doInverse(right_points, homo)
    debug_draw(left_points, ipmFrame)
    debug_draw(right_points, ipmFrame)
    end = time.time()
    print(end-start)
    cv2.imshow('video', ipmFrame)
stream.release()
cv2.destroyAllWindows()
