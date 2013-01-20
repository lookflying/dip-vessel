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

        Filter::matched_filter(blured, dst, 9, 2, 0);
//            Utility::show_nonzero(os.str() + "-dst", dst);
        normalized = Utility::normalize_image(dst);
        Utility::add_to_display("normalized", normalized);


        return normalized;
    }else{
        return Mat();
    }
}
Mat process_drive_image(Mat image, Mat gt, Mat mask, std::string filename){
    if (!image.empty()){
        ostringstream os;
        Mat gray;
        Mat filtered;
        Mat cut;
        Mat resized;
        Mat dst;
        Mat equalized;
        Mat blured;
        Mat normalized;
        Mat masked;
        Mat thick;
        Mat thin;
        Mat threshold_mask;
        Mat thresholded;
        Mat se;
        Utility::add_to_display(filename, image);

        vector<Mat> layers;
        split(image, layers);
        gray = layers[1];


        bitwise_and(gray, mask, gray);
//        cvtColor(image, gray, CV_BGR2GRAY);
            Utility::add_to_display(os.str() + "-gray", gray);

//        int t = Filter::get_average(gray, mask);
//        threshold(gray, threshold_mask, t, 255, THRESH_BINARY_INV);
//        se = getStructuringElement(MORPH_RECT, Size(3, 3));

//        se *= 128;
//        cout <<"se = "<<se<<endl;
//        morphologyEx(threshold_mask, threshold_mask, MORPH_CLOSE, se);
        Filter::window_threshold(gray, threshold_mask, mask, Size(9, 9), 0.97);
        se = getStructuringElement(MORPH_CROSS, Size(3, 3));
        morphologyEx(threshold_mask, threshold_mask, MORPH_OPEN, se);
        Utility::add_to_display("threshold_mask", threshold_mask);
//        cut = Utility::cut_black_edge(gray, t / 2);
//        Utility::add_to_display(os.str() + "-cut", cut);
//        cut = gray;

////        resized = Utility::resize_according_to_height(cut, 400);
////        Utility::add_to_display(os.str() + "-resized", resized);
//        resized = cut;

////        equalizeHist(resized, equalized);
////        Utility::add_to_display(os.str() + "-equalized", equalized);
//        equalized = resized;
//        GaussianBlur(equalized, blured, Size(3, 3), 1.5);
//        Utility::add_to_display(os.str() + "-blur", blured);


//        Filter::gaussian_filter(equalized, thick, 9, 8, 1.5);
        Filter::matched_filter(gray, filtered, 9, 2, 3);
//        Filter::multi_matched_filter(gray, filtered, 9, 2, 1, 1, 6);

//            Utility::show_nonzero(os.str() + "-dst", dst);


        filtered = Utility::normalize_image(filtered);
//        se = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
//        morphologyEx(dst, dst, cv::MORPH_CLOSE, se);
        se = getStructuringElement(MORPH_CROSS, Size(9, 9));
        morphologyEx(mask, mask, cv::MORPH_ERODE, se, Point(-1, -1), 4);
        bitwise_and(filtered, mask, masked);
////        Utility::add_to_display("masked", masked);
//        masked.copyTo(dst);
//        dst = Filter::morphology_restruct(dst, thin);

        bitwise_and(masked, threshold_mask, thresholded);
        dst = thresholded;
        threshold(dst, dst, 1, 255, THRESH_BINARY);
        Utility::add_to_display("dst", dst);
        return dst;
    }else{
        return Mat();
    }
}

//Mat process_ir_image(Mat image, int i){
//    if (!image.empty()){
//        ostringstream os;
//        Mat gray;
//        Mat cut;
//        Mat resized;
//        Mat dst;
//        Mat equalized;
//        Mat blured;
//        Mat normalized;
//        Mat thresholded;
//        os << i;
//        os << ".bmp";
//        Utility::add_to_display(os.str(), image);

//        vector<Mat> layers;
//        split(image, layers);
//        gray = layers[1];
////        cvtColor(image, gray, CV_BGR2GRAY);
////            Utility::add_to_display(os.str() + "-gray", gray);

//        medianBlur(gray, gray, 9);

//        int t = Utility::get_average(gray);
//        cut = Utility::cut_black_edge(gray, t / 2);
//        Utility::add_to_display(os.str() + "-cut", cut);
////        cut = gray;

//        resized = Utility::resize_according_to_height(cut, 400);
//        Utility::add_to_display(os.str() + "-resized", resized);

////        adaptiveThreshold(resized, thresholded, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 9, 0);
////        Utility::add_to_display("thresholded", thresholded);

//        thresholded = resized;

//        equalizeHist(thresholded, equalized);
//        Utility::add_to_display(os.str() + "-equalized", equalized);

//        GaussianBlur(equalized, blured, Size(3, 3), 1.5);
//        Utility::add_to_display(os.str() + "-blur", blured);

//        Filter::gaussian_filter(blured, dst, 11, 3, 2);
////            Utility::show_nonzero(os.str() + "-dst", dst);
//        normalized = Utility::normalize_image(dst);
//        Utility::add_to_display("normalized", normalized);


//        return normalized;
//    }
//}

int main()
{
    double matched = 0;
    double found = 0;
    double missed = 0;
    double gt_total = 0;

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
//    Utility::set_display_switch(true);
//    for (int i = 0; i < 100; ++i){
//        Mat image;
//        image = Utility::get_ir_image(i);
//        if (!image.empty()){
//            Utility::save_ir_result(i, process_ir_image(image, i));
//            cout << "processed ir"<< i << ".bmp"<<endl;
//            Utility::wait();
//            Utility::clear_display();
////            break;
//        }
//    }
    Utility::set_display_switch(true);
    for (int i = 0; i < 100; ++i){
        Mat image, gt, mask;
        std::string filename;
        image = Utility::get_test_image(i, gt, mask, filename);
        if (!image.empty()){
            CV_Assert(!mask.empty());
            CV_Assert(!gt.empty());
            Mat result = process_drive_image(image, gt, mask, filename);
            Utility::save_test_result(i, result);
            Mat display;
            Mat white(result.size(), CV_8UC1, Scalar(0));
            vector<Mat> l;
            int rst_num, gt_num, match_num;
            Utility::analyze_result(gt, result, gt_num, rst_num, match_num);
            l.push_back(gt);
            l.push_back(white);
            l.push_back(result);
            bitwise_and(result, gt, result);
            merge(l, display);
            Utility::add_to_display("compare", display);
            cout << "processed "<< filename <<endl;
            cout <<"gt\t"<<gt_num<<endl;
            cout <<"match\t"<<match_num<<endl;
            cout <<"rst\t"<<rst_num<<endl;
            matched += match_num;
            found += rst_num;
            missed += (gt_num - match_num);
            gt_total += gt_num;
            Utility::wait();
            Utility::clear_display();
        }
    }
    Utility::set_display_switch(true);
    for (int i = 0; i < 100; ++i){
        Mat image, gt, mask;
        std::string filename;
        image = Utility::get_training_image(i, gt, mask, filename);
        if (!image.empty()){
            CV_Assert(!mask.empty());
            CV_Assert(!gt.empty());
            Mat result = process_drive_image(image, gt, mask, filename);
            Utility::save_training_result(i, result);
            Mat display;
            Mat white(result.size(), CV_8UC1, Scalar(0));
            vector<Mat> l;
            int rst_num, gt_num, match_num;
            Utility::analyze_result(gt, result, gt_num, rst_num, match_num);
            l.push_back(gt);
            l.push_back(white);
            l.push_back(result);
            bitwise_and(result, gt, result);
            merge(l, display);
            Utility::add_to_display("compare", display);
            cout << "processed "<< filename <<endl;
            cout <<"gt\t"<<gt_num<<endl;
            cout <<"match\t"<<match_num<<endl;
            cout <<"rst\t"<<rst_num<<endl;
            matched += match_num;
            found += rst_num;
            missed += (gt_num - match_num);
            gt_total += gt_num;
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

    cout << "accuracy\t" << matched / found * 100 << "%"<<endl;
    cout << "missed\t" << missed / gt_total * 100 << "%"<<endl;
    cout << "gt/found\t" << gt_total << " / " << found << "  " << found / gt_total * 100 << "%" <<endl;

    return 0;
}

