#include <iostream>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "detection.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
"This is an empty application that can be treated as a template for your "
"own doing-something-cool applications.";

const char* kOptions =
"{ d detector     | <none> | detector type (takes value \"cascade\")  }"
"{ i image        | <none> | image to process                         }"
"{ v video        | <none> | video to process                         }"
"{ c camera       | <none> | camera to get video from                 }"
"{ m model        | <none> | path to detector file                    }"
"{ h ? help usage |        | print help message                       }";


int main(int argc, const char** argv) {
	// Parse command line arguments.
	CommandLineParser parser(argc, argv, kOptions);
	parser.about(kAbout);

	// If help option is given, print help message and exit.
	if (parser.has("help")) {
		parser.printMessage();
		return 0;
	}

	// Let's do it!
	std::string detector_type = parser.get<std::string>("detector");
	std::string model_path = parser.get<std::string>("model");
	std::string img_path, video_path;
	std::size_t camera_number;
	if (parser.has("image")) {
		img_path = parser.get<std::string>("image");
	}
	else if (parser.has("video")) {
		video_path = parser.get<std::string>("video");
	}
	else {
		camera_number = parser.get<std::size_t>("camera");
	}

	if (!parser.check()) {
		parser.printErrors();
		return 0;
	}

	CascadeDetector cadet;
	cadet.CreateDetector(detector_type);
	cv::Mat img;
	std::vector<cv::Rect> objects;
	std::vector<double> scores;
	VideoCapture cap;

	if (video_path != "") {
		cap.open(video_path);
	}
	else {
		cap.open(camera_number);
	}

	cv::namedWindow("result");

	if (img_path != "") {
		img = cv::imread(img_path);
		if (cadet.Init(model_path)) {
			cadet.Detect(img, objects, scores);
			cout << "Totally detected: " << objects.size() << " objects" << endl;

			for (size_t i = 0; i < objects.size(); i++)
			{
				cv::rectangle(img, objects[i], cv::Scalar(0, 255, 0));
			}

			cv::imshow("result", img);
			while (27 == waitKey()) {
				return 0;
			}
		}
	}


	if (cadet.Init(model_path)) {
		int key;
		while (true) {
			cap >> img;
			cadet.Detect(img, objects, scores);
			cout << "Totally detected: " << objects.size() << " objects" << endl;

			for (size_t i = 0; i < objects.size(); i++)
			{
				cv::rectangle(img, objects[i], cv::Scalar(0, 255, 0));
			}

			cv::imshow("result", img);
			key = waitKey(33);
			if (27 == key) {
				return 0;
			}
		}

	}
	return 0;
}
