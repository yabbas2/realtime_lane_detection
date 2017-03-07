from processing import *
import argparse
import time
import re


arg = argparse.ArgumentParser()
arg.add_argument('-i', '--image', type=str, help="image source")
args = vars(arg.parse_args())

print("[INFO] application starts")

frame = cv2.imread(args['image'])
imageFile = re.findall("[a-z A-Z0-9\\\-_?&()#@/]+(sample[0-9]+).[a-z0-9]+", args['image'])
pts, dst = determinePtsAndDst(frame.shape[1], frame.shape[0], imageFile)
start = time.time()
ipmFrame, homo = fourPointTransform(frame, pts, dst)
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
print('[INFO] Time is: %s' % (end - start))
cv2.imshow('image', ipmFrame)
cv2.waitKey(0)
cv2.destroyAllWindows()
print("[INFO] application ends")
