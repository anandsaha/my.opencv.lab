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

void frame_diff()
{
    Mat frame, prevFrame, curFrame, nextFrame;
    char ch;
    VideoCapture cap(0);

    if(!cap.isOpened())
        return;

    namedWindow("Frame");

    float scalingFactor = 0.75;

    prevFrame = getFrame(cap, scalingFactor);
    curFrame = getFrame(cap, scalingFactor);
    nextFrame = getFrame(cap, scalingFactor);

    while(true)
    {
        // Show the object movement
        imshow("Object Movement", frameDiff(prevFrame, curFrame,
                    nextFrame));

        // Update the variables and grab the next frame
        prevFrame = curFrame;
        curFrame = nextFrame;
        nextFrame = getFrame(cap, scalingFactor);

        // Get the keyboard input and check if it's 'Esc'

        // 27 -> ASCII value of 'Esc' key
        ch = waitKey( 30 );
        if (ch == 27) {
            break;
        }
    }

    // Release the video capture object
    cap.release();

    // Close all windows
    destroyAllWindows();


}

void mog()
{
    Mat frame, prevFrame, curFrame, nextFrame;
    Mat fgMaskMOG, fgMaskMOG2;

    char ch;
    VideoCapture cap(0);

    if(!cap.isOpened())
        return;

    namedWindow("Frame");
    float scalingFactor = 0.75;

    Ptr<bgsegm::BackgroundSubtractorMOG> pMOG;
    Ptr<BackgroundSubtractor> pMOG2;

    pMOG = bgsegm::createBackgroundSubtractorMOG();
    pMOG2 = createBackgroundSubtractorMOG2(); //MOG2 approach

    while(true)
    {
        cap >> frame;

        resize(frame, frame, Size(), scalingFactor, scalingFactor,
                INTER_AREA);

        pMOG->apply(frame, fgMaskMOG);
        pMOG2->apply(frame, fgMaskMOG2);
        imshow("FG Mask MOG", fgMaskMOG);
        imshow("FG Mask MOG 2", fgMaskMOG2);
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
    mog();
    //frame_diff();
    return 0;
}
