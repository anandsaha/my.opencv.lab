#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

// Common processing function
void processImage(Mat im, vector<uchar>& LUT, const char* title) {
    for(int r = 0; r < im.rows; ++r)
        for(int c = 0; c < im.cols; ++c)
            im.at<uchar>(r, c) = LUT[im.at<uchar>(r, c)];

    imshow(title, im);
    waitKey(0);
}

// Identity Transformation
vector<uchar> getIdentityTx() {
    vector<uchar> LUT(256, 0);

    for(int i = 0; i < 256; ++i)
        LUT[i] = uchar(i);

    return LUT;
}

// Negative Transformation
vector<uchar> getNegativeTx() {
    vector<uchar> LUT(256, 0);

    for(int i = 0; i < 256; ++i)
        LUT[i] = uchar(255 - i);

    return LUT;
}

// Piecewise Linear Transformation
vector<uchar> getPiecewiseLinearTx(int r1, int s1, int r2, int s2) {
    vector<uchar> LUT(256, 0);

    for(int i = 0; i < 256; ++i) {

        int x = uchar(255 - i);

        if (i < r1) {
            int m = (s1) / (r1);
            LUT[i] = m * x;
        } else if (i >= r1 && i < r2) {
            int m = (s2 - s1) / (r2 - r1);
            int c = s2  - m * r2;
            LUT[i] = m * x + c;
        } else {
            int m = (255 - s1) / (255 - r1);
            int c = 255 - m * 255;
            LUT[i] = m * x + c;
        }

    }

    return LUT;
}

// Log Transformation
vector<uchar> getLogTx(int maxVal) {

    double c = 255 / log10(maxVal + 1);
    vector<uchar> LUT(256, 0);

    for(int i = 0; i < 256; ++i)
        LUT[i] = (int) round(c * log10(i + 1));

    return LUT;
}

double base = 1.02;

// Inverse-Log (Exponential) Transformation
vector<uchar> getExpTx(int maxVal) {

    double c = 255.0 / (pow(base, maxVal) + 1);
    vector<uchar> LUT(256, 0);

    for(int i = 0; i < 256; ++i)
        LUT[i] = (int) round(c * (pow(base, i) - 1));

    return LUT;
}


// Log transformation using opencv
void doLogTx(Mat image, const char* title, int type /*1 = Log, 2 = Exp*/) {

    cv::destroyAllWindows();

    Mat processed_img;
    image.convertTo(processed_img, CV_32F);
    processed_img += 1;

    if (type == 1)
        cv::log(processed_img, processed_img);
    else
        cv::pow(processed_img, base, processed_img); // Not working as expected

    
    cv::normalize(processed_img, processed_img, 0, 255, cv::NORM_MINMAX);
    cv::convertScaleAbs(processed_img, processed_img);
    imshow(title, processed_img);
    waitKey(0);
}

int main()
{
    Mat image = imread("images/Lenna.jpg", IMREAD_GRAYSCALE);
    vector<uchar> vi = getIdentityTx();
    vector<uchar> vn = getNegativeTx();
    vector<uchar> vp = getPiecewiseLinearTx(60, 55, 195, 200);

    double maxVal;
    minMaxLoc(image, NULL, &maxVal);
    vector<uchar> vl = getLogTx(maxVal);
    vector<uchar> ve = getExpTx(maxVal);

    processImage(image.clone(), vi, "Identity transformation");
    processImage(image.clone(), vn, "Negative transformation");
    processImage(image.clone(), vp, "Piecewise linear transformation");
    processImage(image.clone(), vl, "Log transformation");
    processImage(image.clone(), ve, "Exponential transformation");

    doLogTx(image.clone(), "Log tx using OpenCV", 1);
    doLogTx(image.clone(), "Exp tx using OpenCV", 2);

    return 0;
}
