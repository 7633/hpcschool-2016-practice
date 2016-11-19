#include "detection.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Detector> Detector::CreateDetector(const string& name) {
	if (name == "cascade") {
		return std::make_shared<CascadeDetector>();
	}
	else {
		std::cerr << "Failed to create detector with name '" << name << "'"
			<< std::endl;
	}

	return nullptr;
}

bool CascadeDetector::Init(const std::string& file_path) {
	return detector.load(file_path);
}

void CascadeDetector::Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
	std::vector<double>& scores) {
	std::vector<int> numDet;
	detector.detectMultiScale(frame, objects, numDet);
}