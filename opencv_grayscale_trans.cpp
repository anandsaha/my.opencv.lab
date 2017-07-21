#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>

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




int main()
{
    Mat image = imread("images/Lenna.jpg", IMREAD_GRAYSCALE);
    vector<uchar> vi = getIdentityTx();
    vector<uchar> vn = getNegativeTx();
    vector<uchar> vp = getPiecewiseLinearTx(60, 55, 195, 200);

    processImage(image.clone(), vi, "Identity transformation");
    processImage(image.clone(), vn, "Negative transformation");
    processImage(image.clone(), vp, "Piecewise linear transformation");

    return 0;
}
