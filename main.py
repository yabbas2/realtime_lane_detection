import numpy as np
import argparse
from Streaming import VideoStreamOut, VideoStreamIn
import IPM
import gabor
import HT
import GTN

'''***********Arguments************'''
arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str,help="Video source")
arg.add_argument('-s', '--show', type=str,help="Choose algorithm to be displayed")
args = vars(arg.parse_args())
'''**********************************'''

'''***************main code**************'''
print "[INFO] Start of streaming.."
'''*****************************Initialization*******************'''
if args['video'].endswith("sample7.mp4"):
    pts = np.array([[263, 142], [394, 142], [485, 209], [107, 208]], dtype="float32")
'''elif args['video'].endswith("sample5_cut_1.mp4"):
    pts =
elif args['video'].endswith("sample5_cut_2.mp4"):
    pts =
elif args['video'].endswith("sample5_cut_3.mp4"):
    pts =
'''
houghOutput = np.zeros((200, 200, 3), dtype=np.uint8)
if args['video'] is not None:
    video_in = VideoStreamIn(src=args['video'])
else:
    video_in = VideoStreamIn(src=0)
video_out = VideoStreamOut()

video_in.start()
video_out.start()
'''**************************************************************'''
'''********************************Algorithm*********************************'''
while True:
    '''**************************Reading Frames*******************************'''
    if video_in.streamFlag():
        video_out.setEndStream()
    if video_out.getShowFlag():
        video_in.stopStream()
        exit()
    frame = video_in.readFrame()
    if frame is None:
        continue
    '''*******************************IPM************************************'''
    ipmInput = frame.copy()
    ipmOutput = IPM.four_point_transform(ipmInput, pts)
    '''****************************Gabor Filter******************************'''
    gaborInput = ipmOutput
    gabor_filter = gabor.build_gabor_filter()
    gaborOutput = gabor.process(gaborInput, gabor_filter)
    '''****************Gaussian blur - threshold - noise removal*************'''
    gtnInput = gaborOutput
    gtnOutput = GTN.process(gtnInput)
    '''***************************Hough Transform****************************'''
    try:
        houghInput1 = gtnOutput
        houghInput2 = ipmOutput.copy()
        houghOutput = HT.HoughTransform(houghInput1, houghInput2)
    except:
        pass
    '''*************************Final Step***********************************'''
    finalOutput = IPM.inverse(ipmInput,houghOutput,pts)
    '''**************************Displaying Videos***************************'''
    if args['show'] == 'gabor':
        video_out.showFrame(gaborOutput)
    elif args['show'] == 'ipm':
        video_out.showFrame(ipmOutput)
    elif args['show'] == 'ht':
        video_out.showFrame(houghOutput)
    elif args['show'] == 'all':
        video_out.showFrame(finalOutput)
    elif args['show'] == 'original':
        video_out.showFrame(frame)

#End of program