#include "kalman.h"

Kalman::Kalman()
{
    KalmanFilter KF(4,2);
//    kalman.measurementMatrix = np.array([[1, 0, 0, 0], [0, 1, 0, 0]], np.float32)
//    kalman.transitionMatrix = np.array([[1, 0, 1, 0], [0, 1, 0, 1], [0, 0, 1, 0], [0, 0, 0, 1]], np.float32)
//    kalman.processNoiseCov = np.array([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]], np.float32) * 0.0003
    KF.measurementMatrix = (Mat <int>(4,2)<< 1, 0, 0, 0, 0, 1, 0, 0);
    KF.transitionMatrix = (Mat <int>(4,4)<< 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
    KF.processNoiseCov = (Mat <int>(4,4)<< 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) * 0.0003;
    for (int i = 0; i < 20; ++i) {
        leftKalman[i] = KF;
        rightKalman[i] = KF;
    }
}
