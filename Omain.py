import numpy as np
import argparse
from Streaming import VideoStreamOut, VideoStreamIn
import IPM
import gabor
import HT
import GTN
import LSD
import time

'''***********Arguments************'''
arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str,help="Video source")
arg.add_argument('-s', '--show', type=str,help="Choose only one algorithm to be displayed")
arg.add_argument('-m', '--show2', type=str,help="Choose only two algorithms to be displayed")
args = vars(arg.parse_args())
'''**********************************'''

'''***************main code**************'''
print "[INFO] Start of streaming.."
'''*****************************Initialization*******************************'''
if args['show2'] is not None:
    frames = args['show2'].split(',')

if args['video'].endswith("sample7.mp4"):
    pts = np.array([[259, 147], [384, 147], [470, 226], [87, 226]], dtype="float32")
elif args['video'].endswith("sample8.mp4"):
    pts = np.array([[236, 205], [406, 205], [498, 253], [86, 253]], dtype="float32")
elif args['video'].endswith("sample9.mp4"):
    pts = np.array([[230, 211], [342, 211], [373, 238], [160, 238]], dtype="float32")
elif args['video'].endswith("sample10.mp4"):
    pts = np.array([[253, 219], [352, 219], [444, 288], [60, 288]], dtype="float32")
elif args['video'].endswith("washintgon2.mp4"):
    pts = np.array([[241, 205], [360, 205], [486, 256], [40, 256]], dtype="float32")
else:
    pts = np.array([[269, 206], [384, 206], [596, 297], [1, 297]], dtype="float32")
    
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
        houghInput = gtnOutput
        lines = HT.HoughTransform(houghInput, frame.shape[0])
        houghOutput = IPM.DrawLanes(lines, ipmOutput.copy())
    except:
        pass
    '''**********************Line Segment Detector***************************'''
    #lsdInput = houghOutput.copy()
    #lsdOutput = LSD.LineSegmentDetector(lsdInput)
    '''*************************Final Step***********************************'''
    # @amal @amany: call the required function here - your input is houghOutput
    #finalOutput = IPM.inverse(ipmInput,houghOutput,pts)
    #finalOutput = IPM.inverse(ipmInput, lsdOutput, pts)
    '''**************************Displaying Videos***************************'''
    display = {'gabor': gaborOutput,'gtn': gtnOutput, 'ipm': ipmOutput,
               'original': frame, 'ht': houghOutput}
    if args['show'] is not None:
        video_out.showFrame(display[args['show']])
    elif args['show2'] is not None:
        video_out.showTwoFrames(display[frames[0]], display[frames[1]])
    else:
        print 'No algorithm\'s output is selected!'
    '''**************************End time - Calculate time*******************'''
    end = time.time()
    print 'Time is:', end - start

#End of program