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
while True:
    start = time.time()
    if not video.getStreamStatus():
        print("[INFO] application ends")
        exit(0)
    frame = video.readFrame()
    if frame is None:
        continue
    ipmFrame, homo = fourPointTransform(frame, pts, dst)
    lines = lineSegmentDetector(ipmFrame)
    lines = eliminateFalseDetection(lines)
    left_points, right_points = combineLineSegments(lines, ipmFrame)
    left_points, right_points = enhanceCurveFitting(left_points, right_points, ipmFrame)
    left_points = doInverse(left_points, homo)
    right_points = doInverse(right_points, homo)
    end = time.time()
    afps = int(1 / (end - start))
    video.setInfo(left_points, right_points, afps)
    # print('[INFO]   Time is: %s' % (end-start))
