import numpy as np
import argparse
from Streaming import VideoStreamOut, VideoStreamIn
import IPM
import gabor
import HT
import GTN
import time

'''***********Arguments************'''
arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str,help="Video source")
arg.add_argument('-s', '--show', type=str,help="Choose only one algorithm to be displayed")
arg.add_argument('-m', '--show2', type=str,help="Choose only two algorithms to be displayed")
args = vars(arg.parse_args())
'''**********************************'''

'''***************main code**************'''
'''*****************************Initialization*******************************'''
houghOutput = np.zeros((300, 300, 3), dtype=np.uint8)
lsdOutput = np.zeros((300, 300, 3), dtype=np.uint8)
lines = list()
linesToDraw = list()
countFrames = 0
continueFlag = True
pauseFlag = False
numFrames = 5

if args['show2'] is not None:
    frames = args['show2'].split(',')

if args['video'].endswith("sample7.mp4"):
    pts = np.array([[270, 143], [383, 143], [430, 187], [166, 187]], dtype="float32")
elif args['video'].endswith("sample8.mp4"):
    pts = np.array([[236, 205], [406, 205], [498, 253], [86, 253]], dtype="float32")
elif args['video'].endswith("sample9.mp4"):
    pts = np.array([[230, 211], [342, 211], [373, 238], [160, 238]], dtype="float32")
elif args['video'].endswith("sample10.mp4"):
    pts = np.array([[253, 219], [352, 219], [444, 288], [60, 288]], dtype="float32")
elif args['video'].endswith("sample11.mp4"):
    pts = np.array([[197, 212], [326, 212], [348, 230], [106, 230]], dtype="float32")
elif args['video'].endswith("washintgon2.mp4"):
    pts = np.array([[241, 205], [360, 205], [486, 256], [40, 256]], dtype="float32")
else:
    pts = np.array([[269, 206], [384, 206], [596, 297], [1, 297]], dtype="float32")

print "[INFO] Start of streaming.."
if args['video'] is not None:
    video_in = VideoStreamIn(src=args['video'])
else:
    video_in = VideoStreamIn(src=0)

video_out = VideoStreamOut()
video_in.start()
video_out.start()
'''**************************************************************************'''
'''********************************Algorithm*********************************'''
while True:
    '''**************************Start time**********************************'''
    start = time.time()
    '''**************************Reading Frames******************************'''
    if video_in.streamFlag():
        video_out.setEndStream()
    if video_out.getShowFlag():
        video_in.stopStream()
        exit()
    frame = video_in.readFrame()
    if frame is None:
        continue

    countFrames += 1
    if countFrames == numFrames and continueFlag:
        continueFlag = False
        pauseFlag = True
        countFrames = 0
    elif countFrames == numFrames and pauseFlag:
        continueFlag = True
        pauseFlag = False
        countFrames = 0

    if continueFlag:
        '''*******************************IPM************************************'''
        ipmInput = frame
        ipmOutput = IPM.four_point_transform(ipmInput, pts)
        '''****************************Gabor Filter******************************'''
        gaborInput = ipmOutput
        gabor_filter = gabor.build_gabor_filter()
        gaborOutput = gabor.process(gaborInput, gabor_filter)
        '''****************Gaussian blur - threshold - noise removal*************'''
        gtnInput = gaborOutput.copy()
        gtnOutput = GTN.process(gtnInput)
        '''***************************Hough Transform****************************'''
        try:
            houghInput = gtnOutput
            lines = HT.HoughTransform(houghInput)
        except:
            pass
        '''**********************Get points of final lines***********************'''
        linesToDraw = IPM.inverse(lines)
    '''************************Draw lines over lanes*************************'''
    finalImage = IPM.DrawLanes(linesToDraw, frame.copy())
    '''**************************Displaying Videos***************************'''
    if args['show'] is not None:
        video_out.showFrame(finalImage)
    elif args['show2'] is not None:
        video_out.showTwoFrames(frame, finalImage)
    '''**************************End time - Calculate time*******************'''
    end = time.time()
    print 'Time is:', end - start

#End of program