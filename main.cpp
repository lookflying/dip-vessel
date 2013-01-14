#include <iostream>
#include "utility.h"
#include "filter.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ostream>
#include <vector>
#include <iostream>
using namespace std;
using namespace cv;
Mat process_image(Mat image, int i){
    if (!image.empty()){
        ostringstream os;
        Mat gray;
        Mat cut;
        Mat resized;
        Mat dst;
        Mat equalized;
        Mat blured;
        Mat normalized;
        Mat thresholded;
        os << i;
        os << ".bmp";
        Utility::add_to_display(os.str(), image);

        vector<Mat> layers;
        split(image, layers);
        gray = layers[1];
//        cvtColor(image, gray, CV_BGR2GRAY);
//            Utility::add_to_display(os.str() + "-gray", gray);

//        int t = Utility::get_average(gray);
//        cut = Utility::cut_black_edge(gray, t / 2);
//        Utility::add_to_display(os.str() + "-cut", cut);
        cut = gray;

        resized = Utility::resize_according_to_height(cut, 400);
        Utility::add_to_display(os.str() + "-resized", resized);


        equalizeHist(resized, equalized);
        Utility::add_to_display(os.str() + "-equalized", equalized);

        GaussianBlur(equalized, blured, Size(3, 3), 1.5);
        Utility::add_to_display(os.str() + "-blur", blured);

        Filter::gaussian_filter(blured, dst, 9, 2, 0);
//            Utility::show_nonzero(os.str() + "-dst", dst);
        normalized = Utility::normalize_image(dst);
        Utility::add_to_display("normalized", normalized);


        return normalized;
    }
}
Mat process_drive_image(Mat image, int i, Mat gt, Mat mask){
    if (!image.empty()){
        ostringstream os;
        Mat gray;
        Mat cut;
        Mat resized;
        Mat dst;
        Mat equalized;
        Mat blured;
        Mat normalized;
        Mat thresholded;
        os << i;
        os << ".bmp";
        Utility::add_to_display(os.str(), image);

        vector<Mat> layers;
        split(image, layers);
        gray = layers[1];

        bitwise_and(gray, mask, gray);
//        cvtColor(image, gray, CV_BGR2GRAY);
//            Utility::add_to_display(os.str() + "-gray", gray);

//        int t = Utility::get_average(gray);
//        cut = Utility::cut_black_edge(gray, t / 2);
//        Utility::add_to_display(os.str() + "-cut", cut);
        cut = gray;

//        resized = Utility::resize_according_to_height(cut, 400);
//        Utility::add_to_display(os.str() + "-resized", resized);
        resized = cut;

        equalizeHist(resized, equalized);
        Utility::add_to_display(os.str() + "-equalized", equalized);

        GaussianBlur(equalized, blured, Size(3, 3), 1.5);
        Utility::add_to_display(os.str() + "-blur", blured);

        Filter::gaussian_filter(blured, dst, 9, 2, 1.5);
//            Utility::show_nonzero(os.str() + "-dst", dst);
        normalized = Utility::normalize_image(dst);
        Utility::add_to_display("normalized", normalized);


        return normalized;
    }
}

Mat process_ir_image(Mat image, int i){
    if (!image.empty()){
        ostringstream os;
        Mat gray;
        Mat cut;
        Mat resized;
        Mat dst;
        Mat equalized;
        Mat blured;
        Mat normalized;
        Mat thresholded;
        os << i;
        os << ".bmp";
        Utility::add_to_display(os.str(), image);

        vector<Mat> layers;
        split(image, layers);
        gray = layers[1];
//        cvtColor(image, gray, CV_BGR2GRAY);
//            Utility::add_to_display(os.str() + "-gray", gray);

        medianBlur(gray, gray, 9);

        int t = Utility::get_average(gray);
        cut = Utility::cut_black_edge(gray, t / 2);
        Utility::add_to_display(os.str() + "-cut", cut);
//        cut = gray;

        resized = Utility::resize_according_to_height(cut, 400);
        Utility::add_to_display(os.str() + "-resized", resized);

//        adaptiveThreshold(resized, thresholded, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 9, 0);
//        Utility::add_to_display("thresholded", thresholded);

        thresholded = resized;

        equalizeHist(thresholded, equalized);
        Utility::add_to_display(os.str() + "-equalized", equalized);

        GaussianBlur(equalized, blured, Size(3, 3), 1.5);
        Utility::add_to_display(os.str() + "-blur", blured);

        Filter::gaussian_filter(blured, dst, 11, 3, 0);
//            Utility::show_nonzero(os.str() + "-dst", dst);
        normalized = Utility::normalize_image(dst);
        Utility::add_to_display("normalized", normalized);


        return normalized;
    }
}

int main()
{

//    Utility::set_display_switch(true);
//    for (int i = 0; i < 100; ++i){
//        Mat image;
//        image = Utility::get_oct_image(i);
//        if (!image.empty()){
//            Utility::save_oct_result(i, process_image(image, i));
//            cout << "processed oct"<< i << ".bmp"<<endl;
//            Utility::wait();
//            Utility::clear_display();
//        }

//    }
    Utility::set_display_switch(true);
    for (int i = 0; i < 100; ++i){
        Mat image;
        image = Utility::get_ir_image(i);
        if (!image.empty()){
            Utility::save_ir_result(i, process_ir_image(image, i));
            cout << "processed ir"<< i << ".bmp"<<endl;
            Utility::wait();
            Utility::clear_display();
//            break;
        }
    }
     Utility::set_display_switch(true);
    for (int i = 0; i < 100; ++i){
        Mat image, gt, mask;
        image = Utility::get_training_image(i, gt, mask);
        if (!image.empty()){
            CV_Assert(!mask.empty());
            CV_Assert(!gt.empty());
            Mat result = process_drive_image(image, i, gt, mask);
            Utility::save_training_result(i, result);
            Mat display;
            Mat white(result.size(), CV_8UC1, Scalar(0));
            vector<Mat> l;
            l.push_back(gt);
            l.push_back(white);
            l.push_back(result);
            bitwise_and(result, gt, result);
            merge(l, display);
            Utility::add_to_display("compare", display);
             Utility::add_to_display("and", result);
            cout << "processed training"<< i << ".tif"<<endl;
            Utility::wait();
            Utility::clear_display();
        }
    }
//    for (int i = 0; i < 100; ++i){
//        Mat image;
//        image = Utility::get_test_image(i);
//        if (!image.empty()){
//            Utility::save_test_result(i, process_drive_image(image, i));
//            cout << "processed test"<< i << ".tif"<<endl;
//            Utility::wait();
//            Utility::clear_display();
//        }
//    }


    return 0;
}

