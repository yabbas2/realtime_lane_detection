from processing import *
import re
import argparse
import time

arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str, help="video source")
args = vars(arg.parse_args())
rightLaneStatus = 0
leftLaneStatus = 0
left_counter = 0
right_counter = 0
old_left_points = []
old_right_points = []
stream = cv2.VideoCapture(args['video'])
sample = re.findall("[a-z A-Z0-9\\\-_?&()#@/]+(sample[0-9]+).[a-z0-9]+", args['video'])
width = stream.get(cv2.CAP_PROP_FRAME_WIDTH)
height = stream.get(cv2.CAP_PROP_FRAME_HEIGHT)
fps = stream.get(cv2.CAP_PROP_FPS)
pts, dst = determinePtsAndDst(width, height, ''.join(sample))

prev_left_points = np.array([])
prev_right_points = np.array([])

while True:
    (grabbed, normalFrame) = stream.read()
    if not grabbed:
        break
    if cv2.waitKey(int(fps)) & 0xFF == ord('q'):
        break
    start = time.time()
    ipmFrame, homo = fourPointTransform(normalFrame, pts, dst)
    magdy = np.zeros(ipmFrame.shape, dtype=np.uint8)
    lines = lineSegmentDetector(ipmFrame)
    lines = eliminateFalseDetection(lines)

    hsv = cv2.cvtColor(ipmFrame, cv2.COLOR_BGR2HSV)
    lower_blue = np.array([0, 175, 0])
    upper_blue = np.array([200, 255, 200])
    mask = cv2.inRange(hsv, lower_blue, upper_blue)
    mask = cv2.bitwise_not(mask)
    ipmFrame = cv2.bitwise_and(ipmFrame, ipmFrame, mask=mask)
    lines = eliminateFalseDetection2(lines, mask, width, height)

    left_seed_line, right_seed_line = findSeedLines(lines, ipmFrame.shape[1])
    left_region = leftRegionGrowing(lines, left_seed_line)
    right_region = rightRegionGrowing(lines, right_seed_line)
    for line in lines:
        cv2.line(magdy, (int(line[0]), int(line[1])), (int(line[2]), int(line[3])), (255, 255, 255), 1, cv2.LINE_AA)
    for line in left_region:
        cv2.line(magdy, (int(line[0]), int(line[1])), (int(line[2]), int(line[3])), (0, 255, 255), 1, cv2.LINE_AA)
    for line in right_region:
        cv2.line(magdy, (int(line[0]), int(line[1])), (int(line[2]), int(line[3])), (255, 255, 0), 1, cv2.LINE_AA)
    left_points, right_points = linesToPoints(left_region, right_region)
    for point in left_points:
        cv2.circle(magdy, (int(point[0]), int(point[1])), 4, (0, 255, 0), 1, cv2.LINE_AA)
    for point in right_points:
        cv2.circle(magdy, (int(point[0]), int(point[1])), 4, (0, 255, 0), 1, cv2.LINE_AA)
    left_points, right_points = enhanceCurveFitting(left_points, right_points, ipmFrame.shape[0])

    if removeSuddenChange(old_left_points, left_points) or left_counter >= 10:
        old_left_points = left_points
        left_counter = 0
    else:
        left_points = old_left_points
        left_counter += 1
    if removeSuddenChange(old_right_points, right_points) or right_counter >= 10:
        old_right_points = right_points
        right_counter = 0
    else:
        right_points = old_right_points
        right_counter += 1

    if left_seed_line != 0:
        cv2.line(magdy, (int(left_seed_line[0]), int(left_seed_line[1])), (int(left_seed_line[2]), int(left_seed_line[3])), (255, 0, 255), 1, cv2.LINE_AA)
    if right_seed_line != 0:
        cv2.line(magdy, (int(right_seed_line[0]), int(right_seed_line[1])), (int(right_seed_line[2]), int(right_seed_line[3])), (55, 0, 255), 1, cv2.LINE_AA)

    lstatus = "magdy"
    rstatus = "magdy"

    if isDashed(left_region, left_seed_line):
        leftLaneStatus += 1
    else:
        leftLaneStatus -= 1
    if leftLaneStatus > 0:
        lstatus = "d"
        if leftLaneStatus > 10:
            leftLaneStatus = 10
    elif leftLaneStatus < 0:
        lstatus = "s"
        if leftLaneStatus < -10:
            leftLaneStatus = -10

    if isDashed(right_region, right_seed_line):
        rightLaneStatus += 1
    else:
        rightLaneStatus -= 1
    if rightLaneStatus > 0:
        rstatus = "d"
        if rightLaneStatus > 10:
            rightLaneStatus = 10
    elif rightLaneStatus < 0:
        rstatus = "s"
        if rightLaneStatus < -10:
            rightLaneStatus = -10
    left_points = doInverse(left_points, homo)
    right_points = doInverse(right_points, homo)

    left_points = kalman(left_points, "l")
    right_points = kalman(right_points, "r")

    debug_draw(left_points, normalFrame, lstatus)
    debug_draw(right_points, normalFrame, rstatus)
    end = time.time()
    print(end-start)

    cv2.imshow('ipm', ipmFrame)
    cv2.imshow('m', magdy)
    cv2.imshow('videonormal', normalFrame)
    cv2.imshow('mask', mask)
stream.release()
cv2.destroyAllWindows()
