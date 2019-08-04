#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <vector>
#include <utility>
#include <cstdio>
using namespace cv;
using namespace std;

class LedRecognize{

public:
	struct Point2fWithIdx {
		cv::Point2f p;
		size_t idx;
		Point2fWithIdx(const cv::Point2f _p, size_t _idx) :p(_p), idx(_idx) {}
	};
	bool isEqual(const vector<int>& x,const vector<int>& y);
	void getLedTarget(const cv::Mat & src,vector<int>& save2);
	int ledRecognize(const Mat& ledImage, vector<Rect>&led_rects,vector<int>&);
	bool checkSudoku(const std::vector<std::vector<cv::Point2i> > & contours, std::vector<cv::RotatedRect> & sudoku_rects);

	RotatedRect adjustRRect(const cv::RotatedRect & rect);

	pair<int, int> chooseTargetPerspective(const Mat & image, const vector<RotatedRect> & sudoku_rects);

	bool ledCheck(const std::vector<std::vector<cv::Point2i> > & contours, std::vector<cv::Rect> & led_rects);
private:
	std::vector<cv::RotatedRect> sudoku_rects;
	int sudoku_width = 90;
	int sudoku_height = 50;
	int led_width = 20;
	int led_height = 35;
};


