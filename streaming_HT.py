import cv2
import numpy as np
import argparse
import threading
from Streaming import VideoStreamOut, VideoStreamIn
import time

'''***********Arguments************'''
arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str,help="Your video")
args = vars(arg.parse_args())
'''**********************************'''

'''***************main code**************'''
print "[INFO] Start of streaming.."
if args['video'] is not None: #decide video source
    video_in = VideoStreamIn(src=args['video'])
else:
    video_in = VideoStreamIn(src=0)
video_out = VideoStreamOut()
video_in.start()    #start thread of streaming in
video_out.start()   #start thread of streaming out
while True:
    if video_in.streamFlag():
        video_out.setEndStream()
    if video_out.getShowFlag():
        video_in.stopStream()
        exit()
    frame = video_in.readFrame()
    if frame is None:
        continue
    '''********************************Algorithm*********************************'''
    frame_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    frame_canny = frame_gray
    frame_canny = cv2.GaussianBlur(frame_canny, (7, 7), 0)
    mu, sigma = cv2.meanStdDev(frame_canny)
    frame_canny = cv2.Canny(frame_canny, mu - sigma, mu + sigma)
    h, w = frame_canny.shape[:2]
    center = (w / 2, h / 2)
    M = cv2.getRotationMatrix2D(center, 180, 1)
    frame_canny = cv2.warpAffine(frame_canny, M, (w, h))
    lines = cv2.HoughLines(frame_canny, 1, np.pi / 180, 100)
    image_canny = cv2.cvtColor(frame_canny, cv2.COLOR_GRAY2BGR)
    for line in lines:
        for rho, theta in line:
            if (theta <= np.pi / 2 and theta > 80 * np.pi / 180) or rho < 10:
                continue
            # print rho, theta
            a = np.cos(theta)
            b = np.sin(theta)
            x0 = a * rho
            y0 = b * rho
            x1 = int(x0 + 400 * (-b))
            y1 = int(y0 + 400 * (a))
            x2 = int(x0 - 400 * (-b))
            y2 = int(y0 - 400 * (a))
            cv2.line(image_canny, (x1, y1), (x2, y2), (0, 0, 255), 1)
    frame = cv2.warpAffine(image_canny, M, (w, h))
    '''******************************************************************************'''
    video_out.showFrame(frame)


#End of program