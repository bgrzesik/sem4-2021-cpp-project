#include <iostream>
#include <vector>
#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "Constants.hpp"
#include "Spotters.hpp"


int main(int argc, char **argv)
{
    cv::setUseOptimized(true);
    cv::VideoCapture cap;
    cap.open(0);

//    cap.open("tcp://192.168.0.178:3333/");

    if (!cap.isOpened()) {
        return -1;
    }

    cv::namedWindow("edges", 1);

    cv::Rect box(1000, 100, 200, 200);

    tb::QRCodeSpotter qr_spotter;
    tb::FaceSpotter face_spotter;


    while (true) {
        cv::Mat frame;
        cap >> frame;

        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        auto faces = face_spotter.detect(frame);
        auto qr_codes = qr_spotter.detect(frame);

        for (const auto &obj : faces) {
            const auto center = obj->getCenter();
            const auto points = obj->getPoints();

            cv::Rect rect(points[0], points[1]);

            if (center.inside(box)) {
                cv::circle(frame, center, 7, tb::Colors::red, 10);
                cv::rectangle(frame, rect, tb::Colors::red, 2);
            } else {
                cv::circle(frame, center, 7, tb::Colors::green, 10);
                cv::rectangle(frame, rect, tb::Colors::green, 2);
            }

        }


        for (const auto &obj : qr_codes) {
            const auto qr_obj = reinterpret_cast<const tb::QRSpottedObject *>(obj.get());
            const auto &obj_points = qr_obj->getPoints();
            cv::Rect rect = cv::boundingRect(obj_points);

            auto center = qr_obj->getCenter();

            if (center.inside(box)) {
                cv::circle(frame, center, 7, tb::Colors::red, 10);
                cv::rectangle(frame, rect, tb::Colors::red, 2);
            } else {
                cv::circle(frame, center, 7, tb::Colors::green, 10);
                cv::rectangle(frame, rect, tb::Colors::green, 2);
            }

            cv::putText(frame, qr_obj->getCode(), center, cv::FONT_HERSHEY_SIMPLEX, 1.0, tb::Colors::black, 0);
        }

        cv::rectangle(frame, box, tb::Colors::red, 0);


        cv::imshow("edges", frame);
        if (cv::waitKey(30) >= 0)break;
    }

    return 0;
}
