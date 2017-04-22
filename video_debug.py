from processing import *
import re
import argparse
import time

arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str, help="video source")
args = vars(arg.parse_args())
rightLaneStatus = 0
leftLaneStatus = 0
stream = cv2.VideoCapture(args['video'])
sample = re.findall("[a-z A-Z0-9\\\-_?&()#@/]+(sample[0-9]+).[a-z0-9]+", args['video'])
width = stream.get(cv2.CAP_PROP_FRAME_WIDTH)
height = stream.get(cv2.CAP_PROP_FRAME_HEIGHT)
fps = stream.get(cv2.CAP_PROP_FPS)
pts, dst = determinePtsAndDst(width, height, ''.join(sample))

prev_left_points = np.array([])
prev_right_points = np.array([])
prev_left = 0
prev_right = 0



while True:
    (grabbed, normalFrame) = stream.read()
    if not grabbed:
        break
    if cv2.waitKey(int(fps)) & 0xFF == ord('q'):
        break
    start = time.time()
    normalFrame2 = normalFrame.copy()
    ipmFrame, homo = fourPointTransform(normalFrame, pts, dst)
    magdy = np.zeros(ipmFrame.shape, dtype=np.uint8)
    lines = lineSegmentDetector(ipmFrame)
    lines = eliminateFalseDetection(lines)

    hsvFrame = cv2.cvtColor(ipmFrame, cv2.COLOR_BGR2HSV)
    # lower_blue = np.array([0, 100, 0])
    # upper_blue = np.array([200, 255, 200])
    # mask = cv2.inRange(hsvFrame, lower_blue, upper_blue)
    # mask = cv2.bitwise_not(mask)
    # mask = cv2.cvtColor(mask, cv2.COLOR_GRAY2BGR)
    # ipmFrame = cv2.bitwise_and(ipmFrame, mask)
    # lines = eliminateFalseDetection2(lines, mask, width, height)
    lines = removeShadow(hsvFrame, ipmFrame, lines)

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

    for point in left_points:
        cv2.circle(magdy, (int(point[0]), int(point[1])), 4, (0, 255, 0), 1, cv2.LINE_AA)
    for point in right_points:
        cv2.circle(magdy, (int(point[0]), int(point[1])), 4, (0, 255, 0), 1, cv2.LINE_AA)

    # left_points = removeShadowPoints(hsvFrame, ipmFrame, left_points)
    # right_points = removeShadowPoints(hsvFrame, ipmFrame, right_points)

    left_points, right_points = enhanceCurveFitting(left_points, right_points, ipmFrame.shape[0])

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
        if leftLaneStatus > 30:
            leftLaneStatus = 30
    elif leftLaneStatus < 0:
        lstatus = "s"
        if leftLaneStatus < -30:
            leftLaneStatus = -30

    if isDashed(right_region, right_seed_line):
        rightLaneStatus += 1
    else:
        rightLaneStatus -= 1
    if rightLaneStatus > 0:
        rstatus = "d"
        if rightLaneStatus > 30:
            rightLaneStatus = 30
    elif rightLaneStatus < 0:
        rstatus = "s"
        if rightLaneStatus < -30:
            rightLaneStatus = -30
    left_points = doInverse(left_points, homo)
    right_points = doInverse(right_points, homo)

    # if left_points.size == 0 and prev_left < 15:
    #     left_points = prev_left_points
    #     prev_left += 1
    # elif left_points.size != 0:
    #     prev_left = 0
    # if right_points.size == 0 and prev_right < 15:
    #     right_points = prev_right_points
    #     prev_right += 1
    # elif right_points.size != 0:
    #     prev_right = 0

    if left_points.size > 0:
        left_points = kalman(left_points, "l")
        # prev_left_points = left_points
    if right_points.size > 0:
        right_points = kalman(right_points, "r")
        # prev_right_points = right_points

    bottom_points = list()
    top_points = list()
    top_points.append(right_points[0])
    top_points.append(left_points[0])
    bottom_points.append(left_points[0])
    bottom_points.append(right_points[0])
    top_points = np.array(top_points, dtype="int32")
    bottom_points = np.array(bottom_points, dtype="int32")

    debug_draw(top_points, normalFrame2, lstatus)
    debug_draw(bottom_points, normalFrame2, lstatus)
    debug_draw(left_points, normalFrame2, lstatus)
    debug_draw(right_points, normalFrame2, lstatus)

    mask = cv2.inRange(normalFrame2, (255, 127, 127), (255, 127, 127))
    im2, contours, hierarchy = cv2.findContours(mask, 1, 2)
    cv2.fillConvexPoly(normalFrame2, contours[0], (255, 127, 127))
    cv2.addWeighted(normalFrame2, 0.5, normalFrame, 1 - 0.5, 0, normalFrame)

    end = time.time()
    print(end-start)

    # cv2.imshow('ipm', ipmFrame)
    # cv2.imshow('used lines', magdy)
    cv2.imshow('detected', normalFrame)
    # cv2.imshow('mask', mask)
    # cv2.imshow('hsv', hsvFrame)
stream.release()
cv2.destroyAllWindows()
