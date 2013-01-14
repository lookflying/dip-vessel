#include "filter.h"
#include "utility.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>

using namespace cv;
Filter::Filter()
{
}

void Filter::matched_filter(cv::Mat image, cv::Mat &filtered){

}

void Filter::sobel_filter(cv::Mat image, cv::Mat &filtered){
    CV_Assert(image.channels() == 1);
    Sobel(image, filtered, CV_16S, 1, 1, 5, 1, 0, BORDER_DEFAULT);

}



void Filter::gaussian_filter(Mat image, Mat &filtered, int l, double sigma, double threshold_scale){
    CV_Assert(image.channels() == 1 && image.elemSize() == 1);
    Mat kernel = generate_matched_filter_kernel(l, sigma);
    Mat middle_result(image.rows, image.cols, CV_32FC1, Scalar(0));
    for (int angle = 0; angle < 180; angle += 15){
        Mat rotated_kernel = rotate_mat(kernel, angle);
        Mat temp;
        run(image, temp, rotated_kernel);
        middle_result = cv::max(temp, middle_result);
    }
    double average = sum(middle_result)[0] / middle_result.rows / middle_result.cols;
    for (int i = 0; i < middle_result.rows; ++i){
        for (int j = 0; j < middle_result.cols; ++j){
            if (middle_result.at<float>(i, j) < average * threshold_scale){
                middle_result.at<float>(i, j) = 0;
            }
        }
    }
    middle_result.copyTo(filtered);
}

void Filter::run(Mat image, Mat &filtered, Mat kernel){
    filter2D(image, filtered, CV_32F, kernel, Point(-1, -1), 0, BORDER_DEFAULT);
}

Mat Filter::generate_matched_filter_kernel(int l, double sigma){
    int size = 6 * static_cast<int>(sigma) + 1;
    Mat kernel_g = getGaussianKernel(size, sigma, CV_32F);
    float average = sum(kernel_g)[0] / size;
    for (int i = 0; i < kernel_g.rows; ++i){
        kernel_g.at<float>(i, 0) = -kernel_g.at<float>(i, 0) + average;
//        kernel_g.at<float>(i, 0) = zero_point -  kernel_g.at<float>(i, 0);
//         kernel_g.at<float>(i, 0) *= 10;
    }
    transpose(kernel_g, kernel_g);
    int diag = std::sqrt(l * l + kernel_g.cols * kernel_g.cols);
    Mat big_kernel(diag, diag, CV_32FC1, Scalar(0));
    int row_offset = (diag - l) / 2;
    int col_offset = (diag - kernel_g.cols) / 2;
    for (int i = 0; i < l; ++i){
        for (int j = 0; j < kernel_g.cols; ++j){
            big_kernel(Rect(col_offset, row_offset, kernel_g.cols, l)).at<float>(i, j)
                    = kernel_g.at<float>(0, j);
        }
    }
    return big_kernel;
}

Mat Filter::rotate_mat(Mat mat, double angle){
    Mat rot_mat = getRotationMatrix2D(Point2f(mat.cols / 2, mat.rows / 2), angle, 1);
    Mat dst(mat.size(), mat.type(), Scalar(0));
    warpAffine(mat, dst, rot_mat, dst.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar::all(0));
    return dst;
}
