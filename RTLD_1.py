import cv2
import numpy as np
import argparse
from Streaming import VideoStreamOut, VideoStreamIn
import IPM
import gabor


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
    '''***********************************IPM************************************'''
    ipmInput = frame
    h, w, channels = frame.shape
    pts = np.array([[0, h / 2],
                    [w, h / 2],
                    [w, h],
                    [0, h]], dtype="float32")
    # apply the four point transform to obtain a "birds eye view" of image
    ipmOutput = IPM.four_point_transform(ipmInput, pts)
    '''********************************Gabor Filter******************************'''
    gaborInput = ipmOutput
    gabor_filter = gabor.build_gabor_filter()  # build gabor filter
    gaborOutput = gabor.process(gaborInput, gabor_filter)  # processing on image
    '''********************************Hough Transform****************************'''
    #using SHT
    houghInput = cv2.cvtColor(gaborOutput, cv2.COLOR_BGR2GRAY)
    lines = cv2.HoughLines(houghInput, 1, np.pi/180, 300)
    for line in lines:
        for rho, theta in line:
            if not theta == np.pi/2:#(theta < 100 * np.pi / 180 and theta > 80 * np.pi / 180): #between 100 degree and 80 degree
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
            cv2.line(ipmOutput, (x1, y1), (x2, y2), (0, 0, 255), 1)
    '''******************************************************************************'''
    video_out.showFrame(ipmOutput)


#End of program