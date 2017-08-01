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
#include "opencv2/xfeatures2d.hpp"


using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

//Constants
double base = 1.02;

// Apply lookup table transformation
void apply_lut(Mat im, vector<uchar>& LUT, const char* title) {
    for(int r = 0; r < im.rows; ++r)
        for(int c = 0; c < im.cols; ++c)
            im.at<uchar>(r, c) = LUT[im.at<uchar>(r, c)];
}

// Log Transformation
vector<uchar> getLogTx(int maxVal) {

    double c = 255 / log10(maxVal + 1);
    vector<uchar> LUT(256, 0);

    for(int i = 0; i < 256; ++i)
        LUT[i] = (int) round(c * log10(i + 1));

    return LUT;
}

// Inverse-Log (Exponential) Transformation
vector<uchar> getExpTx(int maxVal) {

    double c = 255.0 / (pow(base, maxVal) + 1);
    vector<uchar> LUT(256, 0);

    for(int i = 0; i < 256; ++i)
        LUT[i] = (int) round(c * (pow(base, i) - 1));

    return LUT;
}


void preprocess(Mat frame, unsigned int blur_factor) {

    Mat hsv;
    //cvtColor(frame,hsv,CV_BGR2HSV);
    //frame = hsv;
    medianBlur(frame, frame, blur_factor);
    double maxVal;
    minMaxLoc(frame, NULL, &maxVal);
    vector<uchar> ve = getExpTx(maxVal);
    apply_lut(frame, ve, "Exponential transformation");
}

Mat align_images(Mat first_image, Mat second_image) {
    int minHessian = 400;

    Ptr<SURF> detector = SURF::create( minHessian );
    std::vector<KeyPoint> keypoints_1, keypoints_2;

    detector->detect(first_image, keypoints_1);
    detector->detect(second_image, keypoints_2);

    //-- Draw keypoints
    Mat img_keypoints_1; Mat img_keypoints_2;

    drawKeypoints(first_image, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    drawKeypoints(second_image, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT );

    //-- Show detected (drawn) keypoints
    imshow("Keypoints 1", img_keypoints_1 );
    imshow("Keypoints 2", img_keypoints_2 );

    waitKey(0);
    return first_image;
}

Mat detect_change(Mat image1, Mat image2, unsigned int blur_factor = 7) {

    preprocess(image1, blur_factor);
    preprocess(image2, blur_factor);

    Mat                         fgMaskMOG2; 
    Ptr<BackgroundSubtractor>   pMOG2;

    pMOG2 = createBackgroundSubtractorMOG2();
    pMOG2->apply(image1, fgMaskMOG2);
    pMOG2->apply(image2, fgMaskMOG2);
    return fgMaskMOG2;

}

void show_img(const char* title, Mat image)
{
    imshow(title, image);
    waitKey(0);
    cv::destroyAllWindows();
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        cout << "Usage: pipeline <image1> <image2> [<blur factor>]" << endl;
        return -1;
    }
    unsigned int blur_factor = 7;
    if(argc == 4)
        blur_factor = atoi(argv[3]);


    char* rawimage1name = argv[1];
    char* rawimage2name = argv[2];

    cout << "Image 1: " << rawimage1name << endl;
    cout << "Image 2: " << rawimage2name << endl;

    Mat rawimage1 = imread(rawimage1name, cv::IMREAD_GRAYSCALE);
    Mat rawimage2 = imread(rawimage2name, cv::IMREAD_GRAYSCALE);

    if(rawimage1.empty()){
        cerr << "Unable to open first image frame: " << rawimage1name << endl;
        exit(-1);
    }

    if(rawimage2.empty()){
        cerr << "Unable to open second image frame: " << rawimage2name << endl;
        exit(-1);
    }


    show_img("Original Image 1", rawimage1);
    show_img("Original Image 2", rawimage2);

    // Step 1: Align the first image wrt the second image
    rawimage1 = align_images(rawimage1, rawimage2);
    // Step 2: Detect changes
    // Mat mask = detect_change(rawimage1, rawimage2, blur_factor);
    // show_img("The Mask", mask);

    return 0;
}


