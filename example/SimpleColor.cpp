#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <chrono>

#include <tb/Constants.hpp>
#include <tb/Spotters.hpp>
#include <tb/PipelineNode.hpp>
#include <tb/SingleThreadedContext.hpp>
#include <tb/FiltersNodes.hpp>
#include <tb/DebugDraw.hpp>
#include <tb/Notifier.hpp>
#include <tb/FrameSource.hpp>

constexpr bool showWindow = false;

int main(int argc, char **argv)
{
    cv::setUseOptimized(true);
    cv::VideoCapture cap;
    cap.open(0);

    if (!cap.isOpened()) {
        return -1;
    }


    cv::Rect box(100, 100, 200, 200);

    auto context = tb::SingleThreadedContext();
    auto frames_node = std::make_shared<tb::FrameSource>();
    auto color_spotter = std::make_shared<tb::ColorSpotter>(cv::Scalar(145, 60, 60), cv::Scalar(155, 255, 255));
    auto rect_filter = std::make_shared<tb::RectangleFilter>(box, tb::CollisionType::BoundingBoxOverlap);

    auto stream_notifier = std::make_shared<tb::StreamNotifier<const tb::SpottedObject *>>(std::cout, "Test\n");

    auto debug_drawer = std::make_shared<tb::DebugDraw>(frames_node);

    frames_node | color_spotter;

    if (showWindow) {
        cv::namedWindow("edges", 1);
        color_spotter | debug_drawer;
    }

    color_spotter | rect_filter | stream_notifier;

    auto last_frame = std::chrono::system_clock::now();
    long fps = 0;

    while (true) {
        fps += 1;

        auto now = std::chrono::system_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_frame).count() >= 1000) {
            std::cout << "Frame cam " << cap.get(cv::CAP_PROP_FPS) << " fps sim " << fps << " fps " << std::endl;
            last_frame = now;
            fps = 0;
        }

        context.process(frames_node, &cap);

        if (showWindow) {
            cv::rectangle(frames_node->getFrame().rgb, box, tb::Colors::red, 0);
            cv::imshow("TriggerBox", frames_node->getFrame().rgb);
            if (cv::waitKey(30) >= 0) break;
        }
    }

    return 0;
}
