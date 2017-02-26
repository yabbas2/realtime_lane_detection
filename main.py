import argparse
from stream import *
from processing import *
import time


arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str, help="Video source")
arg.add_argument('-r', '--res', type=str, help='video resolution')
args = vars(arg.parse_args())


print("[INFO] application starts")

if args['video'] is not None:
    video = stream(src=args['video'])
else:
    video = stream(src=0)
video.start()
pts, dst = determinePtsAndDst(args)
while True:
    start = time.time()
    if not video.getStreamStatus():
        exit(0)
    frame = video.readFrame()
    if frame is None:
        continue
    ipmFrame, homo = fourPointTransform(frame, pts, dst)
    lines = lineSegmentDetector(ipmFrame, 66, 40)
    print(lines)
    video.setInfo(lines, list(), 66, "three")
    end = time.time()
    # print('Time is: %s' % (end - start))
