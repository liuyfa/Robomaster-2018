#include "rune.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <stdlib.h>
#include <stdio.h>


using namespace cv;
using namespace std;
using namespace cv::ml;




int pic_count=1;
Ptr<cv::ml::KNearest>ledRecognizer;
int LedRecognize::ledRecognize(const Mat& ledImage, vector<Rect>&led_rects,vector<int>&save) {
	Mat result,res_save;
	char pic_name[40];
	sort(led_rects.begin(), led_rects.end(), [](const Rect& a,const Rect& b) {
		return a.x < b.x;
	});
	if(led_rects.size()!=5){
        return -1;
	}
	for (int i = 0; i < 5; i++) {
		Mat cell(ledImage, led_rects[i]);//感兴趣区域
		Mat ledcell;
		cell.copyTo(ledcell);
		resize(ledcell, ledcell, Size(20, 35));//
		imshow("led", ledcell);
		ledcell.copyTo(res_save);
		//waitKey(300);
		cout<<"shape is"<<ledcell.rows<<" "<<ledcell.cols<<endl;

		ledcell = ledcell.reshape(0, 1);//与训练集的格式一样
		ledcell.convertTo(ledcell, 5);//uchar型转换为cv_32f
		ledRecognizer->findNearest(ledcell, 1, result);//获得一个最紧邻的数据

        cout << "result is " << result.at<float>(0,0) << endl;//uode mat类型的0行0列就是一个数据
		//cout << result << endl;
		save.push_back(result.at<float>(0,0));
		cout<<"size="<<save.size()<<endl;

		// if(result.at<float>(0,0)==7)
		// {
		// 	sprintf(pic_name,"7_%d.jpg",pic_count);//为pic_name命名，将res_save保存到相片中。
		// 	imwrite(pic_name,res_save);//生成图片保存，提高训练样本。
		// 	pic_count++;
		// }
		// if(result.at<float>(0,0)==3)
		// {
		// 	sprintf(pic_name,"3_%d.jpg",pic_count);
		// 	imwrite(pic_name,res_save);
		// 	pic_count++;
		// }
	}
	return 0;
}

bool LedRecognize::isEqual(const vector<int>& x,const vector<int>& y)
{
    assert(x.size() == y.size());//如果条件返回错误，终止程序运行
    if(x==y)
        return true;
    else
        return false;
}


void LedRecognize::getLedTarget(const cv::Mat & src,vector<int>& save2) {
	Mat src_gray, dst;

	//Mat src, src_gray, dst;
	// string traindata_path = "C:\\Users\\lijing\\Desktop\\jianxin_CV\\train_data.csv";
	string traindata_path = "/home/deepin/RM/train_data.csv";
	Ptr<cv::ml::TrainData>trainingData = cv::ml::TrainData::loadFromCSV(traindata_path, 0);//一行为一个样本
	ledRecognizer = cv::ml::KNearest::create();//创建knn分类器；
	ledRecognizer->train(trainingData);
	// VideoCapture capture("C:\\Users\\lijing\\Desktop\\jianxin_CV\\1.avi");
	//VideoCapture capture("/media/ruanqi/TOSHIBA EXT/LED_promote/LED_promote/1.avi");
	//if (!capture.isOpened())
	//	cout << "fail toopen!" << endl;

    cout <<endl <<  "*******run into LedRecognize::getLedTarget in imgconsumeproduce.cpp ********" << endl << endl;
    //imshow("1yuanshitu", src);
    //waitKey(500);
	long seq = 0;
	bool res_checkSudoku;

		double startTime = (double)cvGetTickCount();
		//Mat frame;
		//capture >> frame;
		//frame.copyTo(src);

		//cvtColor(src, src_gray, CV_BGR2GRAY);


		vector<Mat> channels;
		split(src,channels);
		Mat Red=channels.at(2);
		threshold(Red, dst, 200, 255, THRESH_OTSU);


		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
		erode(dst, dst, element);
		dilate(dst, dst, element);
		imshow("1erodebinary", dst);
		//waitKey(20);
		vector<vector<Point2i>> contours;
		vector<Vec4i> hierarchy;
		findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//找轮廓

		//LedRecognize sudokucheck;
		LedRecognize ledcheck;
		vector<cv::Rect> led_rects;
		double preprocessEndTime = (double)cvGetTickCount();

		ledcheck.ledCheck(contours, led_rects);//找到5个数字图像的轮廓
		ledRecognize(dst, led_rects,save2);

        //cout <<endl <<  "*******run before waitKey(10) in rune.cpp ********" << endl << endl;
		/*++seq;
		double recoStart = (double)cvGetTickCount();
		double endTime = (double)cvGetTickCount();
		printf("Preprocess time = %g ms, Recognize time = %g ms, total time = %gms\n",
			(preprocessEndTime - startTime) / (cvGetTickFrequency() * 1000),
			(endTime - recoStart) / (cvGetTickFrequency() * 1000),
			(endTime - startTime) / (cvGetTickFrequency() * 1000));*/
}



bool LedRecognize::ledCheck(const vector<vector<Point2i>> & contours, std::vector<cv::Rect> & led_rects) {

	if (contours.size() < 9)
		return 0;
	float ledwidth = led_width;
	float ledheight = led_height;
	int led = 0;
	float low_threshold = 0.6;
	float high_threshold = 1.4;

	size_t i = 0;
	for (; i < contours.size(); i++) {
		Rect rect = boundingRect(contours[i]);
		const Size2f & t = rect.size();
		float ledratio_cur = float(t.width) / float(t.height);
		if (//ledratio_cur > 0.8 * ledratio && ledratio_cur < 1.2 * ledratio &&
			t.width > low_threshold * ledwidth && t.width < high_threshold * ledwidth &&
			t.height > low_threshold * ledheight && t.height < high_threshold * ledheight
			&& rect.tl().y < 240 || t.width > 5 && t.width < 12 && t.height > low_threshold * ledheight && t.height < high_threshold * ledheight && rect.tl().y < 240)
		{
			rect.x -= 2;

			rect.y -= 2;

			rect.width += 4;

			cout<<"rect.x"<<rect.x<<"rect.width"<<rect.width<<endl;
			rect.height += 4;
            if(rect.x<0){
                rect.x=0;
			}
            if(rect.y<0){
                rect.y=0;
			}
            if(rect.x+rect.width>640){
                rect.width = 640-rect.x;
			}
			if(rect.y+rect.height>480)
			{
                rect.height = 480-rect.y;
			}
			led_rects.push_back(rect);
			++led;
		}
	}
	cout << "led:   " << led << endl;
	if (led > 5 || led < 5)
		return false;
	return true;
}



RotatedRect LedRecognize::adjustRRect(const RotatedRect & rect) {
	const Size2f & s = rect.size;
	if (s.width > s.height)
		return rect;
	return RotatedRect(rect.center, Size2f(s.height, s.width), rect.angle + 90.0);
}

