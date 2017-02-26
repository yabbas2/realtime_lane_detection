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
    lines = lineSegmentDetector(ipmFrame, 66, 40)
    lines, arrow = doInverse(lines, homo, args)
    if lines is not None and arrow is not None:
        video.setInfo(lines, arrow, 66, "two", afps)
    end = time.time()
    afps = int(1 / (end - start))
    # print('[INFO]   Time is: %s' % (end - start))
