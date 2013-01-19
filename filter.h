#ifndef FILTER_H
#define FILTER_H
#include <opencv2/core/core.hpp>
class Filter
{
public:
    Filter();
    static void matched_filter(cv::Mat image, cv::Mat &filtered);
    static void sobel_filter(cv::Mat image, cv::Mat &filtered);
    static void matched_filter(cv::Mat image, cv::Mat &filtered, int l, double sigma, double threshold_scale);
    static void multi_matched_filter(cv::Mat image, cv::Mat &filtered, int l, double sigma, double threshold_scale, double sigma_step = 1, int repeat = 1);
    static void run(cv::Mat image, cv::Mat &filtered, cv::Mat kernel);
    static cv::Mat generate_matched_filter_kernel(int l, double sigma);
    static cv::Mat rotate_mat(cv::Mat mat, double angle);
    static cv::Mat morphology_restruct(cv::Mat image, cv::Mat mask);
};

#endif // FILTER_H
