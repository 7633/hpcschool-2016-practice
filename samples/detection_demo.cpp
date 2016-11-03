#include <iostream>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "detection.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
  "{ v video        | <none> | video to process                         }"
  "{ c camera       | <none> | camera to get video from                 }"
  "{ m model        | <none> | path to detector file                    }"
  "{ d detector     | <node> | detector type (cascade)                  }"
  "{ h ? help usage |        | print help message                       }";

int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

  const string kWindowName = "video";
  const int kWaitKeyDelay = 20;
  const int kEscapeKey = 27;
  const Scalar kColorCyan = CV_RGB(0, 255, 255);
  const Scalar kColorGreen = CV_RGB(0, 255, 0);
  const int kLineThickness = 2;

  namedWindow(kWindowName);

  if (!parser.has("detector") || !parser.has("model")) {
    parser.printMessage();
    return 0;
  }
  shared_ptr<Detector> detector =
    Detector::CreateDetector(parser.get<string>("detector"));
  string detector_model_file_path = parser.get<string>("model");
  if (!detector->Init(detector_model_file_path)) {
    std::cerr << "Failed to load detector from file '"
      << detector_model_file_path << "'";
    return 0;
  }

  VideoCapture video;
  if (parser.has("video")) {
    string video_path = parser.get<string>("video");
    video.open(video_path);
    if (!video.isOpened()) {
      cout << "Failed to open video file '" << video_path << "'" << endl;
      return 0;
    }
  }
  else if (parser.has("camera")) {
    int camera_id = parser.get<int>("camera");
    video.open(camera_id);
    if (!video.isOpened()) {
      cout << "Failed to capture video stream from camera " << camera_id
        << endl;
      return 0;
    }
  }

  Mat frame;
  video >> frame;

  while (!frame.empty()) {
    vector<Rect> objects;
    vector<double> scores;
    detector->Detect(frame, objects, scores);
    for (const auto& object : objects) {
      rectangle(frame, object, kColorCyan, kLineThickness);
    }
    imshow(kWindowName, frame);
    int key = waitKey(kWaitKeyDelay) & 0x00FF;
    if (key == kEscapeKey) {
      break;
    }
    video >> frame;
  }

  return 0;
}
