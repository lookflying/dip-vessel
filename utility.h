#ifndef UTILITY_H
#define UTILITY_H
#include <string>
#include <opencv2/core/core.hpp>

class Utility
{
public:
    Utility();
    static const std::string ir_prefix;
    static const std::string oct_prefix;
    static const std::string training_prefix;
    static const std::string test_prefix;
    static const std::string training_mask_prefix;
    static const std::string training_1st_manual;
    static const std::string test_mask_prefix;
    static const std::string test_1st_manual;
    static cv::Mat get_ir_image(int num);
    static cv::Mat get_oct_image(int num);
    static cv::Mat get_training_image(int num, cv::Mat &gt, cv::Mat &mask);
    static cv::Mat get_test_image(int num);
    static cv::Mat cut_black_edge(cv::Mat image, uchar threshold = 0);
    static uchar get_average(cv::Mat image);
    static cv::Mat resize_according_to_height(cv::Mat image, int height);
    static void save_ir_result(int num, cv::Mat result);
    static void save_oct_result(int num, cv::Mat result);
    static void save_test_result(int num, cv::Mat result);
    static void save_training_result(int num, cv::Mat result);
    static void show_nonzero(std::string window_name, cv::Mat image);
    static int display_count;
    static bool display_switch;
    static void set_display_switch(bool s);
    static void add_to_display(std::string window_name, cv::Mat image);
    static void clear_display();
    static void wait();
    static cv::Mat normalize_image(cv::Mat image);
};

#endif // UTILITY_H
