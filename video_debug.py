from processing import *
import numpy.polynomial.polynomial as poly
import argparse
import time


def draw(image, lines):
    for line in lines:
        cv2.line(image, (int(line[0]), int(line[1])), (int(line[2]), int(line[3])), (0, 0, 255), 3, cv2.LINE_AA)


def draw2(image, points):
    cv2.polylines(image, np.int32([points]), False, (0, 0, 255), 3, cv2.LINE_AA)

arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str, help="image source")
arg.add_argument('-r', '--res', type=str, help='video resolution')
args = vars(arg.parse_args())

pts, dst = determinePtsAndDst(args, ".mp4")
stream = cv2.VideoCapture(args['video'])
fps = stream.get(cv2.CAP_PROP_FPS)
xPoints = []
yPoints = []
while True:
    (grabbed, normalFrame) = stream.read()
    if not grabbed:
        break
    if cv2.waitKey(int(fps)) & 0xFF == ord('q'):
        break
    start = time.time()
    ipmFrame, homo = fourPointTransform(normalFrame, pts, dst)
    # cv2.rectangle(ipmFrame, (0, 0), (20, 640), (0, 255, 0), 1, cv2.LINE_AA)
    # cv2.rectangle(ipmFrame, (170, 0), (190, 640), (0, 255, 0), 1, cv2.LINE_AA)
    # cv2.rectangle(ipmFrame, (340, 0), (360, 640), (0, 255, 0), 1, cv2.LINE_AA)
    lines = lineSegmentDetector(ipmFrame)
    lines = eliminateFalseDetection(lines)
    # left_lines, right_lines = combineLineSegments(lines, int(ipmFrame.shape[1]/2), ipmFrame)
    # draw(ipmFrame, left_lines)
    # draw(ipmFrame, right_lines)
    left_points, right_points = combineLineSegments(lines, int(ipmFrame.shape[1]/2), ipmFrame)
    draw2(ipmFrame, left_points)
    draw2(ipmFrame, right_points)
    # draw(ipmFrame, lines)
    # lines = list(lines)
    # for i in range(0, 360, 1):
    #    lines.append([[i, 0, i, 640]])
    # for i in range(0, 360, 20+150):
    #     xPoints.clear()
    #     yPoints.clear()
    #     for line in lines:
    #         for x1, y1, x2, y2 in line:
    #             if int(x1) in range(i, i+20) and int(x2) in range(i, i+20):
    #                 xPoints.append(x1)
    #                 xPoints.append(x2)
    #                 yPoints.append(y1)
    #                 yPoints.append(y2)
    #     if len(xPoints) != 0 and len(yPoints) != 0:
    #         coeff = poly.polyfit(yPoints, xPoints, 2)
    #         ffit = poly.Polynomial(coeff)
    #         # y_new = np.linspace(0, 640)
    #         # z = np.polyfit(yPoints, xPoints, 2)
    #         # p = np.poly1d(z)
    #         for j in range(0, 640, 1):
    #             x_new = ffit(j)
    #             cv2.circle(ipmFrame, (int(x_new), int(j)), 3, (0, 255, 0), 3)
    end = time.time()
    print(end-start)
    cv2.imshow('video', ipmFrame)
stream.release()
cv2.destroyAllWindows()
