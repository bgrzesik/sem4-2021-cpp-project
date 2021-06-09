![Banner](https://raw.githubusercontent.com/bgrzesik/sem4-2021-cpp-project/master/triggerbox.jpeg "Banner")

# TriggerBox
Library enabling creation of simple pipelines ment to notify when certain conditions occur.

## Example
A simple example writing to standard output whenever an magenta colored object enters a portion of the screen.

```cpp
using namespace tb;

cv::VideoCapture cap;
cap.open(0);

auto context = SingleThreadedContext();
auto frames_node = std::make_shared<FrameSource>();
auto color_spotter = std::make_shared<ColorSpotter>(cv::Scalar(145, 60, 60), cv::Scalar(155, 255, 255));
auto rect_filter = std::make_shared<RectangleFilter>(box, CollisionType::BoundingBoxOverlap);

auto stream_notifier = std::make_shared<StreamNotifier<const SpottedObject *>>(std::cout, "Object detected!\n");

frames_node | color_spotter | rect_filter | stream_notifier;

context.process(frames_node, &cap);

```
