#include "filter.h"
#include "utility.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>
#include <limits>
#include <iostream>


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
void Filter::multi_matched_filter(Mat image, Mat &filtered, int l, double sigma, double threshold_scale, double sigma_step, int repeat){
    CV_Assert(image.channels() == 1 && image.elemSize() == 1);
    int i = 0;
    filtered = Mat(image.rows, image.cols, CV_8UC1, Scalar(0));
    do{
        Mat temp;
        matched_filter(image, temp, l, sigma, threshold_scale);
        temp = Utility::normalize_image(temp);
        for (int j = 0; j < image.rows; ++j){
            for (int k = 0; k < image.cols; ++k){
                filtered.at<uchar>(j, k) = std::max(filtered.at<uchar>(j, k), temp.at<uchar>(j, k));
            }
        }

        sigma += sigma_step;
        ++i;
    }while(i < repeat);

}

void Filter::window_threshold(Mat image, Mat &thresholded, Mat mask, Size window_size, double threshod_scale){
    CV_Assert(image.channels() == 1 && image.elemSize() == 1);
    CV_Assert(mask.channels() == 1 && mask.elemSize() == 1);
    CV_Assert(image.rows == mask.rows && image.cols == mask.cols);
    Mat average;
    blur(image, average, window_size);
    thresholded = Mat(image.size(), CV_8UC1, Scalar(0));
    for (int i = 0; i < image.rows; ++i){
        for (int j = 0; j < image.cols; ++j){
            if (static_cast<int>(image.at<uchar>(i, j))
                    < (static_cast<int>(average.at<uchar>(i, j)) * threshod_scale)){
                thresholded.at<uchar>(i, j) = 255;
            }
        }
    }
}

uchar Filter::get_average(Mat image, Mat mask){
    CV_Assert(image.channels() == 1 && image.elemSize() == 1);
    CV_Assert(mask.channels() == 1 && mask.elemSize() == 1);
    CV_Assert(image.rows == mask.rows && image.cols == mask.cols);
    unsigned int count = 0, sum = 0;
    unsigned int threshold = std::numeric_limits<unsigned int>::max() - 256;
    for (int i = 0; i < image.rows; ++i){
        for (int j = 0; j < image.rows; ++j){
            if (mask.at<uchar>(i, j) > 0){
                ++count;
                sum += static_cast<unsigned int>(image.at<uchar>(i, j));
                CV_Assert(sum < threshold);
            }
        }
    }
    return static_cast<uchar>(sum / count);
}
void Filter::matched_filter(Mat image, Mat &filtered, int l, double sigma, double threshold_scale){
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

Mat Filter::morphology_restruct(Mat image, Mat mask){
    Mat temp, old_temp, se;
    image.copyTo(temp);
    do{
        temp.copyTo(old_temp);
        se = getStructuringElement(MORPH_CROSS, Size(3, 3));
        morphologyEx(image, temp, MORPH_DILATE, se);
        bitwise_and(temp, mask, temp);
    }while(countNonZero(temp - old_temp) > 0);
    return temp;

}
