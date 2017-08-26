#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include "opencv2/bgsegm.hpp"

// https://github.com/opencv/opencv/blob/master/samples/cpp/tutorial_code/TrackingMotion/goodFeaturesToTrack_Demo.cpp

using namespace std;
using namespace cv;


void shi_tomasi()
{
    Mat frame, dst, dst_norm, dst_norm_scaled, frameGray, frameDiff;

    char ch;
    VideoCapture cap(0);

    if(!cap.isOpened())
        return;

    namedWindow("Frame");
    float scalingFactor = 0.75;

    while(true)
    {
        // Capture the current frame
        cap >> frame;

        // Resize the frame
        resize(frame, frame, Size(), scalingFactor, scalingFactor,
                INTER_AREA);

        // Convert to grayscale
        cvtColor(frame, frameGray, COLOR_BGR2GRAY );

        // Initialize the parameters for Shi-Tomasi algorithm
        vector<Point2f> corners;
        double qualityThreshold = 0.02;
        double minDist = 15;
        int blockSize = 5;
        bool useHarrisDetector = false;
        double k = 0.07;
        int numCorners = 100;
        RNG rng(12345);


        // Clone the input frame
        Mat frameCopy;
        frameCopy = frame.clone();

        // Apply corner detection
        goodFeaturesToTrack(frameGray, corners, numCorners,
                qualityThreshold, minDist, Mat(), blockSize,
                useHarrisDetector, k);

        // Parameters for the circles to display the corners
        int radius = 8; // radius of the cirles
        int thickness = 2; // thickness of the circles
        int lineType = 8;

        // Draw the detected corners using circles
        for(size_t i = 0; i < corners.size(); i++)
        {
            Scalar color = Scalar(rng.uniform(0,255),
                    rng.uniform(0,255), rng.uniform(0,255));
            circle(frameCopy, corners[i], radius, color,
                    thickness, lineType, 0);
        }

        /// Show what you got
        imshow("Shi Tomasi", frameCopy);

        ch = waitKey( 30 );
        if (ch == 27) {
            break;
        }
    }

    cap.release();
    destroyAllWindows();

}

int main(int argc, char* argv[])
{
    shi_tomasi();
    return 0;
}
