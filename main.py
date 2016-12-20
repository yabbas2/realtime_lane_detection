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
numFrames = 30

if args['show2'] is not None:
    frames = args['show2'].split(',')

if args['video'].endswith("sample7.mp4"):
    pts = np.array([[263, 142], [394, 142], [485, 209], [107, 208]], dtype="float32")
elif args['video'].endswith("sample8.mp4"):
    pts = np.array([[271, 247], [370, 247], [468, 281], [168, 281]], dtype="float32")
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
        gtnInput = gaborOutput.copy()#lsdOutput.copy()
        gtnOutput = GTN.process(gtnInput)
        '''***************************Hough Transform****************************'''
        try:
            houghInput1 = gtnOutput
            houghInput2 = ipmOutput.copy()
            lines, houghOutput = HT.HoughTransform(houghInput1, houghInput2)
        except:
            pass
        '''**********************Get points of final lines***********************'''
        linesToDraw = IPM.inverse(lines)
    '''************************Draw lines over lanes*************************'''

    '''**************************Displaying Videos***************************'''
    display = {'gabor': gaborOutput, 'ht': houghOutput, 'lsd': lsdOutput, 'gtn': gtnOutput, 'ipm': ipmOutput,
               'original': frame, 'all': finalOutput}
    if args['show'] is not None:
        video_out.showFrame(display[args['show']])
    elif args['show2'] is not None:
        video_out.showTwoFrames(display[frames[0]], display[frames[1]])
    '''**************************End time - Calculate time*******************'''
    end = time.time()
    print 'Time is:', end - start

#End of program