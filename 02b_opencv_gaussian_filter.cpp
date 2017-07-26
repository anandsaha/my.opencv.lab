#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    Mat gaussian_kernel = getGaussianKernel(7, 1.0);
    int rows = gaussian_kernel.rows;
    int cols = gaussian_kernel.cols;


    if (false && gaussian_kernel.isContinuous()) {
        cols = cols * rows;
        rows = 1;
    }

    cout << "Gaussian Kernel" << endl;
    cout << "Rows: " << rows << endl;
    cout << "Cols: " << cols << endl;

    for (int r = 0; r < rows; ++r) {
        double* rowptr = gaussian_kernel.ptr<double>(r);
        for(int c = 0; c < cols; ++c) {
            cout << rowptr[c] << " ";
        }
        cout << endl;
    }
    return 0;
}
