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

#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"


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


void preprocess(Mat frame, unsigned int blur_factor, bool increase_contrast = true) {

    //Convert BGR (Blue Green Red) to HSV (Hue Sat Val)
    //Mat hsv;
    //cvtColor(frame,hsv, cv::COLOR_BGR2HSV_FULL);
    //frame = hsv;

    // Put a threshold
    cv::threshold(frame, frame, cv::THRESH_OTSU, 100, cv::THRESH_TOZERO);

    // Blur the image
    //cv::medianBlur(frame, frame, blur_factor);
    //cv::GaussianBlur(frame, frame, cv::Size(blur_factor, blur_factor), 0);
    cv::bilateralFilter(frame.clone(), frame, blur_factor, 100, 100);


    // Increase the contrast
    double maxVal;
    minMaxLoc(frame, NULL, &maxVal);
    vector<uchar> ve;
    if (increase_contrast)
        ve = getExpTx(maxVal);
    else
        ve = getLogTx(maxVal);
    apply_lut(frame, ve, "Exponential transformation");
}

Mat align_images(Mat img_1, Mat img_2)
{    
    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian = 400;

    //SurfFeatureDetector detector( minHessian );
    Ptr<xfeatures2d::SURF> detector = xfeatures2d::SURF::create(minHessian);

    std::vector<KeyPoint> keypoints_1, keypoints_2;

    detector->detect( img_1, keypoints_1 );
    detector->detect( img_2, keypoints_2 );

    //-- Step 2: Calculate descriptors (feature vectors)
    //SurfDescriptorExtractor extractor;
    Ptr<xfeatures2d::SURF> extractor = xfeatures2d::SURF::create();

    Mat descriptors_1, descriptors_2;

    extractor->compute( img_1, keypoints_1, descriptors_1 );
    extractor->compute( img_2, keypoints_2, descriptors_2 );

    //-- Step 3: Matching descriptor vectors using FLANN matcher
    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match( descriptors_1, descriptors_2, matches );

    double max_dist = 0; double min_dist = 100;

    //-- Quick calculation of max and min distances between keypoints
    for( int i = 0; i < descriptors_1.rows; i++ )
    { 
        double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
    }

    printf("-- Max dist : %f \n", max_dist );
    printf("-- Min dist : %f \n", min_dist );


    //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
    //-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
    //-- small)
    //-- PS.- radiusMatch can also be used here.
    std::vector< DMatch > good_matches;

    for( int i = 0; i < descriptors_1.rows; i++ )
    { if( matches[i].distance <= max(2*min_dist, 0.02) )
        { good_matches.push_back( matches[i]); }
    }

    //-- Localize the object
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;

    for( int i = 0; i < good_matches.size(); i++ )
    {
        //-- Get the keypoints from the good matches
        obj.push_back( keypoints_1[ good_matches[i].queryIdx ].pt );
        scene.push_back( keypoints_2[ good_matches[i].trainIdx ].pt );
    }

    Mat H = findHomography( obj, scene, CV_RANSAC );

    Mat output;
    warpPerspective(img_1, output, H, img_2.size());
    return output;
}
/*
Mat align_images(Mat first_image, Mat second_image) {
    int minHessian = 400;

    Ptr<SURF> detector = SURF::create(minHessian);
    std::vector<KeyPoint> keypoints_1, keypoints_2;
    Mat desc1, desc2, mask;

    detector->detectAndCompute(first_image, mask, keypoints_1, desc1);
    detector->detectAndCompute(second_image, mask, keypoints_2, desc2);

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
*/

Mat detect_change(Mat image1, Mat image2, unsigned int blur_factor = 7, bool increase_contrast = true) {

    preprocess(image1, blur_factor, increase_contrast);
    preprocess(image2, blur_factor, increase_contrast);

    Mat                         fgMaskMOG2; 
    Ptr<BackgroundSubtractor>   pMOG2;

    pMOG2 = createBackgroundSubtractorMOG2();
    pMOG2->apply(image1, fgMaskMOG2);
    pMOG2->apply(image2, fgMaskMOG2);
/*
    double maxVal;
    double minVal;
    minMaxLoc(fgMaskMOG2, &minVal, &maxVal);
    cout << minVal << ", " << maxVal << endl;
    cv::threshold(fgMaskMOG2, fgMaskMOG2, cv::THRESH_OTSU, 150, cv::THRESH_TOZERO);
*/

    return fgMaskMOG2;

}

void show_img(const char* title, Mat image)
{
    imshow(title, image);
    waitKey(0);
    cv::destroyAllWindows();
}

Mat mask_img(Mat image, Mat mask)
{

    //cout << "Image size: " << image.size().height << ", " << image.size().width << endl;
    //cout << "Mask  size: " << mask.size().height << ", " << mask.size().width << endl;

    for(int r = 0; r < mask.rows; ++r) {
        for(int c = 0; c < mask.cols; ++c) {
            if ((int)mask.at<uchar>(r, c)  == 255) {
                //image.at<Vec3b>(r, c).val[0] = 100;
                //image.at<Vec3b>(r, c).val[1] = 100;
                image.at<Vec3b>(r, c).val[2] = 10;
            }
        }
    }

    /*
    // "channels" is a vector of 3 Mat arrays:
    vector<Mat> channels(3);
    // split img:
    split(image, channels);

    for(int r = 0; r < mask.rows; ++r)
        for(int c = 0; c < mask.cols; ++c) {
            cout << mask.at<uchar>(r, c) << endl;  
            if (mask.at<uchar>(r, c) > 0)
                channels[2] += mask.at<uchar>(r, c);
                image.at<Vec3b>(r, c)[1]
        }
    merge(channels, image);

    */
    /*
    Mat output;
    cv::bitwise_and(image, image, output, mask = mask);
    */
    return image;
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

    int mode = cv::IMREAD_GRAYSCALE;
    Mat rawimage1 = imread(rawimage1name, mode);
    Mat rawimage2 = imread(rawimage2name, mode);

    if(rawimage1.empty()){
        cerr << "Unable to open first image frame: " << rawimage1name << endl;
        exit(-1);
    }

    if(rawimage2.empty()){
        cerr << "Unable to open second image frame: " << rawimage2name << endl;
        exit(-1);
    }


    //show_img("Original Image 1", rawimage1);
    //show_img("Original Image 2", rawimage2);

    // Step 1: Align the first image wrt the second image
    rawimage1 = align_images(rawimage1, rawimage2);
    //show_img("Aligned image", rawimage1);
    //exit(0);

    // Step 2: Detect changes
    Mat mask1 = detect_change(rawimage1.clone(), rawimage2.clone(), blur_factor, true);
    Mat mask2 = detect_change(rawimage1.clone(), rawimage2.clone(), blur_factor, false);
    Mat mask;
    cv::bitwise_and(mask1, mask2, mask);
    cv::threshold(mask, mask, 100, 255, cv::THRESH_BINARY);

    // Step 3: Overlay the mask on the second image
    Mat rawimage2_show = imread(rawimage2name, cv::IMREAD_COLOR);
    mask_img(rawimage2_show, mask);
    show_img("The Mask", mask);
    show_img("The Difference", rawimage2_show);


    return 0;
}

