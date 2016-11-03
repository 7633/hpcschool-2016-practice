#include "detection.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using namespace cv;
using namespace std;

shared_ptr<Detector> Detector::CreateDetector(const string& name) {
  if (name == "cascade") {
    return std::make_shared<CascadeDetector>();
  }
  return nullptr;
}

bool CascadeDetector::Init(const std::string& model_file_path) {
  return detector.load(model_file_path);
}

void CascadeDetector::Detect(const cv::Mat& frame,
  std::vector<cv::Rect>& objects, std::vector<double>& scores) {
  if (detector.empty()) {
    cerr << "Detector has not been initialized yet." << endl;
  }
  else {
    vector<int> numDetections;
    const double scaleFactor = 1.1;
    const int minNeighbors = 3;
    detector.detectMultiScale(frame, objects, numDetections,
      scaleFactor, minNeighbors);
    scores.resize(numDetections.size());
    copy(numDetections.begin(), numDetections.end(), scores.begin());
  }
}
