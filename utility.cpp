#include "utility.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <sstream>
#include <limits>
using namespace std;
using namespace cv;

const string Utility::ir_prefix = "./Images/IR/";
const string Utility::oct_prefix = "./Images/OCT/";
const string Utility::training_prefix = "./Images/training/images/";
const string Utility::test_prefix = "./Images/test/images/";

Utility::Utility()
{
}

static Mat read_image(string prefix, int num, string ext){
    ostringstream os;
    os << num;
    return imread(prefix + os.str() + ext);
}

Mat Utility::get_ir_image(int num){
    return read_image(ir_prefix, num, ".bmp");
}

Mat Utility::get_oct_image(int num){
    return read_image(oct_prefix, num, ".bmp");
}

Mat Utility::get_test_image(int num){
    int n = num / 10;
    int m = num % 10;
    ostringstream os;
    os <<m;
    os <<"_test.tif";
    return read_image(test_prefix, n, os.str());
}

Mat Utility::get_training_image(int num){
    int n = num / 10;
    int m = num % 10;
    ostringstream os;
    os <<m;
    os <<"_training.tif";
    return read_image(training_prefix, n, os.str());
}

uchar Utility::get_average(Mat image){
    unsigned int count = 0, sum = 0;
    unsigned int threshold = numeric_limits<unsigned int>::max() - 256;
    for (int i = 0; i < image.rows; ++i){
        for (int j = 0; j < image.rows; ++j){
            ++count;
            sum += static_cast<unsigned int>(image.at<uchar>(i, j));
            CV_Assert(sum < threshold);
        }
    }
    return static_cast<uchar>(sum / count);
}

Mat Utility::   cut_black_edge(Mat image, uchar threshold){
    CV_Assert(image.channels() == 1 && image.elemSize() == 1);
    int top = image.rows - 1;
    int bottom = 0;
    int left = image.cols - 1;
    int right = 0;
    for (int i = 0; i < image.rows; ++i){
        for (int j = 1; j < image.rows - 1; ++j){
            if (image.at<uchar>(i, j) > threshold){
                top = i;
                goto from_bottom;
            }
        }
    }

    from_bottom:
    for (int i = image.rows - 1; i >= 0; --i){
        for (int j = 1; j < image.cols - 1; ++j){
            if (image.at<uchar>(i, j) > threshold){
                bottom = i;
                goto from_left;
            }
        }
    }

    from_left:
    for (int j = 0; j < image.cols; ++j){
        for (int i = 1; i < image.rows - 1; ++i){
            if (image.at<uchar>(i, j) > threshold){
                left = j;
                goto from_right;
            }
        }
    }

    from_right:
    for (int j = image.cols - 1; j >= 0; --j){
        for (int i = 1; i < image.rows - 1; ++i){
            if (image.at<uchar>(i, j) > threshold){
                right = j;
                goto done;
            }
        }
    }

    done:
    CV_Assert(top <= bottom && left <= right);
    Mat cut;
    image(Rect(left, top, right - left + 1, bottom - top + 1)).copyTo(cut);
    return cut;
}

Mat Utility::resize_according_to_height(Mat image, int height){
    int old_width = image.cols;
    int old_height = image.rows;
    int width = old_width * height / old_height;
    Mat resized;
    resize(image, resized, Size(width, height), 0, 0, INTER_CUBIC);
    return resized;
}
static void save_image(string prefix, int num, string ext, Mat image){
    ostringstream os;
    os << prefix;
    os << num;
    os << ext;
    imwrite(os.str(), image);
}

void Utility::save_ir_result(int num, Mat result){
    save_image(ir_prefix, num, "-result.bmp", result);
}

void Utility::save_oct_result(int num, Mat result){
    save_image(oct_prefix, num, "-result.bmp", result);
}

void Utility::save_test_result(int num, Mat result){
    int n = num / 10;
    int m = num % 10;
    ostringstream os;
    os <<m;
    os <<"_test_result.tif";
    return save_image(test_prefix, n, os.str(), result);
}

void Utility::save_training_result(int num, Mat result){
    int n = num / 10;
    int m = num % 10;
    ostringstream os;
    os <<m;
    os <<"_training_result.tif";
    return save_image(training_prefix, n, os.str(), result);
}

void Utility::show_nonzero(string window_name, Mat image){
    CV_Assert(image.channels() == 1);
    Mat non_zero(image.rows, image.cols, CV_8UC1, Scalar(0));
    if (image.elemSize() == 1){
        for (int i = 0; i < image.rows; ++i){
            for (int j = 0; j < image.cols; ++j){
                if (image.at<uchar>(i, j) != 0){
                    non_zero.at<uchar>(i, j) = 255;
                }
            }
        }
    }else if (image.elemSize() == 2){
        for (int i = 0; i < image.rows; ++i){
            for (int j = 0; j < image.cols; ++j){
                if (image.at<short>(i, j) != 0){
                    non_zero.at<uchar>(i, j) = 255;
                }
            }
        }
    }else if (image.elemSize() == 4){
        for (int i = 0; i < image.rows; ++i){
            for (int j = 0; j < image.cols; ++j){
                if (image.at<float>(i, j) != 0){
                    non_zero.at<uchar>(i, j) = 255;
                }
            }
        }
    }else if (image.elemSize() == 8){
        for (int i = 0; i < image.rows; ++i){
            for (int j = 0; j < image.cols; ++j){
                if (image.at<double>(i, j) != 0){
                    non_zero.at<uchar>(i, j) = 255;
                }
            }
        }
    }else{
        return;
    }
    namedWindow(window_name, WINDOW_NORMAL);
    imshow(window_name, non_zero);
}

bool Utility::display_switch = true;
void Utility::set_display_switch(bool s){
    display_switch = s;
}

int Utility::display_count = 0;
void Utility::add_to_display(string window_name, Mat image){
    if (display_switch){
        static int width = 1200;
        static int height = 720;
        static int window_width = 400;
        static int window_height = 360;

        namedWindow(window_name, WINDOW_NORMAL);
        imshow(window_name, image);
        int h = display_count * window_width / width * window_height;
        int w = display_count * window_width % width;
        moveWindow(window_name, w, h);
        display_count ++;
    }
}

void Utility::wait(){
    if (display_switch){
        char c = waitKey(0);
        if (c == 27){
            exit(0);
        }
    }
}

void Utility::clear_display(){
    if (display_switch){
        display_count = 0;
        destroyAllWindows();
    }

}

Mat Utility::normalize_image(Mat image){
    Mat dst;
    normalize(image, dst, 0, 255, CV_MINMAX, CV_8UC1);
    return dst;
}
