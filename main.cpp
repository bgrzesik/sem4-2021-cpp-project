#include <iostream>
#include <vector>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <chrono>

#include "Constants.hpp"
#include "Spotters.hpp"

#include "PipelineNode.hpp"


class DebugDrawer: public tb::PipelineInputNode<const tb::SpottedObject *>
{
public:
    explicit DebugDrawer(cv::Mat *frame)
        : frame(frame)
    {
    }

protected:
    cv::Mat *frame;

    void process(const tb::SpottedObject *object) override
    {
        const cv::Rect box(100, 100, 200, 200);

        const auto &obj_points = object->getPoints();
        cv::Rect rect = cv::boundingRect(obj_points);

        auto center = object->getCenter();

        if (center.inside(box)) {
            cv::circle(*frame, center, 7, tb::Colors::red, 10);
            cv::rectangle(*frame, rect, tb::Colors::red, 2);
        } else {
            cv::circle(*frame, center, 7, tb::Colors::green, 10);
            cv::rectangle(*frame, rect, tb::Colors::green, 2);
        }

        if (auto *qr_obj = dynamic_cast<const tb::QRSpottedObject *>(object)) {
            cv::putText(*frame, qr_obj->getCode(), center, cv::FONT_HERSHEY_SIMPLEX, 1.0, tb::Colors::magenta, 3);
        }
    }
};

int main(int argc, char **argv)
{
    cv::setUseOptimized(true);
    cv::VideoCapture cap;
//    cap.open(0);

// gst-launch-1.0 -ve v4l2src device=/dev/video0 ! 'video/x-raw,width=640,height=480' !  x264enc pass=qual quantizer=20 tune=zerolatency ! rtph264pay ! udpsink host=192.168.0.171 port=1234
//    cap.open("udpsrc port=1234 ! application/x-rtp,payload=127 ! rtph264depay ! h264parse ! avdec_h264 ! autovideoconvert ! appsink sync=false", cv::CAP_GSTREAMER);

// raspivid -t 0 -hf -n -h 768 -w 1080 -fps 5 -o - | nc -lk -p 5001
//    cap.open("tcpclientsrc host=192.168.0.178 port=5001 ! queue2 max-size-buffers=1 ! decodebin ! autovideoconvert ! appsink sync=false", cv::CAP_GSTREAMER);

    cap.open(
        " rtspsrc location=rtsp://192.168.0.178:8554/test latency=0 buffer-mode=auto ! decodebin ! videoconvert ! appsink sync=false",
        cv::CAP_GSTREAMER);

    if (!cap.isOpened()) {
        return -1;
    }

    cv::namedWindow("edges", 1);

    cv::Rect box(100, 100, 200, 200);
    cv::Mat frame;
    cv::Mat gray;

    tb::BroadcasterNode<const cv::Mat *> framesNode;
    auto qr_spotter = std::make_shared<tb::QRCodeSpotter>();
    auto face_spotter = std::make_shared<tb::FaceSpotter>();
    auto debug_drawer = std::make_shared<DebugDrawer>(&frame);

    framesNode.outputTo(qr_spotter);
    framesNode.outputTo(face_spotter);

    qr_spotter->outputTo(debug_drawer);
    face_spotter->outputTo(debug_drawer);

    auto last_frame = std::chrono::system_clock::now();
    long fps = 0;

    while (true) {
        cap >> frame;
        fps += 1;

        auto now = std::chrono::system_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_frame).count() >= 1000) {
            std::cout << "Frame cam " << cap.get(cv::CAP_PROP_FPS) << " fps sim " << fps << " fps " << std::endl;
            last_frame = now;
            fps = 0;
        }

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        framesNode.addPending(&gray);
        framesNode.processPending();
        qr_spotter->processPending();
        face_spotter->processPending();

        debug_drawer->processPending();

        cv::rectangle(frame, box, tb::Colors::red, 0);

        qr_spotter->cleanup();
        face_spotter->cleanup();

        cv::imshow("edges", frame);
        if (cv::waitKey(30) >= 0) break;
    }

    return 0;
}
