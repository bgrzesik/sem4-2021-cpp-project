#include <iostream>
#include <vector>
#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "Spotters.hpp"


int main(int argc, char **argv)
{
    cv::setUseOptimized(true);
    cv::VideoCapture cap;
    cap.open(0);
//    cap.open("tcp://192.168.0.178:3333/");
    if (!cap.isOpened())
        return -1;

    cv::namedWindow("edges", 1);


//    cv::QRCodeDetector detector;


    const cv::Scalar red(0, 0, 255);
    const cv::Scalar green(0, 255, 0);
    cv::Rect box(1000, 100, 200, 200);

    auto filter = cv::samples::findFile("haarcascades/haarcascade_frontalface_default.xml");
//    auto cascade = cv::CascadeClassifier(filter);

    tb::QRCodeSpotter qr_spotter;


    while (true) {
        cv::Mat frame;
        cap >> frame;

        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        std::vector<cv::Rect> faces;

//        cascade.detectMultiScale(gray, faces);
//        for (int i = 0; i < faces.size(); i++) {
//            cv::rectangle(frame, faces[i], green, 0);
//        }

        auto objects = qr_spotter.detect(frame);
//        std::vector<cv::Point> points;
//        std::vector<std::string> codes;

//        detector.detectAndDecodeMulti(gray, codes, points);
        for (const auto &obj : objects) {
            const auto &obj_points = obj->getPoints();
            auto qr_obj = reinterpret_cast<std::unique_ptr<tb::QRSpottedObject> *>(&obj);

            int min_x = cv::min({obj_points[0].x, obj_points[1].x, obj_points[2].x, obj_points[3].x});
            int min_y = cv::min({obj_points[0].y, obj_points[1].y, obj_points[2].y, obj_points[3].y});
            int max_x = cv::max({obj_points[0].x, obj_points[1].x, obj_points[2].x, obj_points[3].x});
            int max_y = cv::max({obj_points[0].y, obj_points[1].y, obj_points[2].y, obj_points[3].y});
            auto center = (cv::Point(min_x, min_y) + cv::Point(max_x, max_y)) / 2.;

            if (center.inside(box)) {
                cv::circle(frame, center, 10, red, 5);
            } else {
                cv::circle(frame, center, 10, green, 5);
            }

//            cv::putText(frame, qr_obj->get()., center, cv::FONT_HERSHEY_SIMPLEX, 1.0, red, 10);
            cv::rectangle(frame, cv::Point(min_x, min_y), cv::Point(max_x, max_y), red, 0);
        }

        cv::rectangle(frame, box, red, 0);


        cv::imshow("edges", frame);
        if (cv::waitKey(30) >= 0)break;
    }

    return 0;
}
