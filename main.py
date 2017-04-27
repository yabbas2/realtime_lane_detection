import argparse
from stream import *
from processing import *
import re
import time


arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str, help="Video source")
args = vars(arg.parse_args())


print("[INFO] application starts")

if args['video'] is not None:
    video = stream(src=args['video'])
else:
    video = stream(src=0)
video.start()
fps, width, height, frameCount = video.getInfo()
sample = re.findall("[a-z A-Z0-9\\\-_?&()#@/]+(sample[0-9]+).[a-z0-9]+", args['video'])
pts, dst = determinePtsAndDst(width, height, ''.join(sample))
afps = 0
prev_left_points = np.array([])
prev_right_points = np.array([])
prev_left = 0
prev_right = 0
prev_contours = 0

while True:
    start = time.time()
    if not video.getStreamStatus():
        print("[INFO] application ends")
        exit(0)
    normalFrame = video.readFrame()
    if normalFrame is None:
        continue
    ipmFrame, homo = fourPointTransform(normalFrame, pts, dst)
    lines = lineSegmentDetector(ipmFrame)
    lines = eliminateFalseDetection(lines)
    hsvFrame = cv2.cvtColor(ipmFrame, cv2.COLOR_BGR2HSV)
    lines = removeShadow(hsvFrame, ipmFrame, lines)
    left_seed_line, right_seed_line = findSeedLines(lines, ipmFrame.shape[1])
    left_region = leftRegionGrowing(lines, left_seed_line)
    right_region = rightRegionGrowing(lines, right_seed_line)
    left_points, right_points = linesToPoints(left_region, right_region)
    if left_points.size < 8 and prev_left < 20:
        left_points = prev_left_points
        prev_left += 1
    elif left_points.size != 0:
        prev_left = 0
        prev_left_points = left_points

    if right_points.size < 8 and prev_right < 20:
        right_points = prev_right_points
        prev_right += 1
    elif right_points.size != 0:
        prev_right = 0
        prev_right_points = right_points
    left_points, right_points = enhanceCurveFitting(left_points, right_points, ipmFrame.shape[0])

    left_points = doInverse(left_points, homo)
    right_points = doInverse(right_points, homo)

    if left_points.size > 0:
        left_points = kalman(left_points, "l")
        # prev_left_points = left_points
    if right_points.size > 0:
        right_points = kalman(right_points, "r")
        # prev_right_points = right_points

    end = time.time()
    afps = int(1 / (end - start))
    video.setInfo(left_points, right_points, afps)
    # print('[INFO]   Time is: %s' % (end-start))
