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
counter = 0
windowSize = 20
lanesNum = "none"
oldMargin = 0
oldLanesNum = "none"
margin = 0
while True:
    start = time.time()
    if not video.getStreamStatus():
        print("[INFO]   application ends")
        exit(0)
    frame = video.readFrame()
    if frame is None:
        continue
    counter += 1
    ipmFrame, homo = fourPointTransform(frame, pts, dst)
    lines = lineSegmentDetector(ipmFrame)
    if counter == fps:
        margin, lanesNum = calcMargin(lines, windowSize, ipmFrame.shape[1])
        if margin == 0:
            margin = oldMargin
            lanesNum = oldLanesNum
        else:
            oldMargin = margin
            oldLanesNum = lanesNum
        # print(margin)
        counter = 0
    lines = filterAndAverage(lines, 0, ipmFrame.shape[1], windowSize, margin, ipmFrame.shape[:2])
    lines, arrow = doInverse(lines, homo, args)
    video.setInfo(lines, arrow, margin, lanesNum, afps)
    end = time.time()
    afps = int(1 / (end - start))
    # print('[INFO]   Time is: %s' % (end - start))
