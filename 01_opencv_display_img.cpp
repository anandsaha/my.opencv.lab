#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void showImage(Mat image) {
	
    if (image.isContinuous())
        cout << "Image is continuous" << endl;
    else
        cout << "Image is not continuous" << endl;

    cout << "Image size is " << image.size() << endl;

    imshow("Image Display", image);


	waitKey(0);

    vector<Mat> channels;
    split(image, channels);
    imshow("Blue", channels[0]);
	waitKey(0);
    imshow("Green", channels[1]);
	waitKey(0);
    imshow("Red", channels[2]);
	waitKey(0);

    Mat roi(image, Rect(0, 0, image.rows/2, image.cols/2));
    imshow("Region of Interest", roi);
    waitKey();

    int rows = image.rows;
    int cols = image.cols * image.channels();
    for(int i = 0; i < rows; i++) {
        uchar* img_row = image.ptr<uchar>(i);
        for (int j = 0; j < (cols - 69); j++)
            img_row[j] = img_row[j+1];
    }

    imshow("After changing pixels", image);
    waitKey(0);

    cv::destroyAllWindows();


}

void showImage2(Mat image) {
    for (int r = 0; r < image.rows; ++r) {
        for(int c = 0; c < image.cols; ++c) {
            cout << (int)image.at<Vec3b>(r, c)[1] << endl;
        }
    }
}

int main()
{
    /*
	Mat image = imread("/home/anand/work/Lenna.jpg", IMREAD_COLOR);
    showImage(image.clone());
    imshow("After calling showImage()", image);
    waitKey(0);
    cout << image.col(0).size() << endl;
    */

    Mat image2(Size(600, 400), CV_8UC3, Scalar(255, 80, 20));
    imshow("Blah", image2);
    waitKey(0);
    showImage2(image2);

    return 0;
}
