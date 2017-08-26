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

// https://github.com/opencv/opencv/blob/master/samples/cpp/tutorial_code/TrackingMotion/cornerHarris_Demo.cpp

using namespace std;
using namespace cv;

Mat frameDiff(Mat prevFrame, Mat curFrame, Mat nextFrame)
{
    Mat diffFrames1, diffFrames2, output;
    absdiff(nextFrame, curFrame, diffFrames1);
    absdiff(curFrame, prevFrame, diffFrames2);
    bitwise_and(diffFrames1, diffFrames2, output);

    return output;
}

Mat getFrame(VideoCapture cap, float scalingFactor)
{
    Mat frame, output;
    cap >> frame;
    resize(frame, frame, Size(), scalingFactor, scalingFactor, INTER_AREA);
    cvtColor(frame, output, CV_BGR2GRAY);

    return output;
}

void harris()
{
    Mat frame, dst, dst_norm, dst_norm_scaled, frameGray, frameDiff;

    char ch;
    VideoCapture cap(0);

    if(!cap.isOpened())
        return;

    namedWindow("Frame");
    float scalingFactor = 0.75;
    float blockSize = 2;
    float apertureSize = 3;
    float k = 0.04;
    int thresh = 200;

    while(true)
    {
        cap >> frame;
        resize(frame, frame, Size(), scalingFactor, scalingFactor, INTER_AREA);
        dst = Mat::zeros(frame.size(), CV_32FC1);
        cvtColor(frame, frameGray, COLOR_BGR2GRAY);
        cornerHarris(frameGray, dst, blockSize, apertureSize, k, BORDER_DEFAULT);
        normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
        convertScaleAbs(dst_norm, dst_norm_scaled);

        // Drawing a circle around each corner
        for(int j = 0; j < dst_norm.rows ; j++)
        {
            for(int i = 0; i < dst_norm.cols; i++)
            {
                if((int)dst_norm.at<float>(j,i) > thresh)
                {
                    circle(frame, Point(i, j), 8, Scalar(0,255,0), 2, 8, 0);
                }
            }
        }

        // Showing the result
        imshow("Harris corner", frame);

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
    harris();
    return 0;
}
