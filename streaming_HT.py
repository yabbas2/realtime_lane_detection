import cv2
import numpy as np
import argparse
import threading
from Streaming import VideoStreamOut, VideoStreamIn
import time

'''***********Arguments************'''
arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str,help="Video source")
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
    #using SHT
    '''lines = cv2.HoughLines(frame, 1, np.pi/180, 100)
    frame = cv2.cvtColor(frame, cv2.COLOR_GRAY2BGR)
    for line in lines:
        for rho, theta in line:
            if not (theta < 100 * np.pi / 180 and theta > 80 * np.pi / 180): #between 100 degree and 80 degree
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
            cv2.line(frame, (x1, y1), (x2, y2), (0, 0, 255), 1)'''
    '''******************************************************************************'''
    video_out.showFrame(frame)


#End of program