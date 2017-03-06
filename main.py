import argparse
from stream import *
from processing import *
import time


arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str, help="Video source")
arg.add_argument('-r', '--res', type=str, help='video resolution')
args = vars(arg.parse_args())


print("[INFO]   application starts")

if args['video'] is not None:
    video = stream(src=args['video'])
else:
    video = stream(src=0)
video.start()
fps = int(video.getInfo())
pts, dst = determinePtsAndDst(args, ".mp4")
afps = 0
while True:
    start = time.time()
    if not video.getStreamStatus():
        print("[INFO]   application ends")
        exit(0)
    frame = video.readFrame()
    if frame is None:
        continue
    ipmFrame, homo = fourPointTransform(frame, pts, dst)
    lines = lineSegmentDetector(ipmFrame)
    lines = eliminateFalseDetection(lines)
    left_points, right_points = combineLineSegments(lines, ipmFrame)
    left_points, right_points = enhanceCurveFitting(left_points, right_points, homo, ipmFrame)
    end = time.time()
    afps = int(1 / (end - start))
    video.setInfo(left_points, right_points, afps)
    print('[INFO]   Time is: %s' % (end-start))
