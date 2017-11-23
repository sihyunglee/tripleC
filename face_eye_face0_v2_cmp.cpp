/* 
 * ���� : ��/�� ��ġ ��� �� ���� �� �� ù ��° �� ����� ������ �� ����� ���缭 warp
 * ������ �κ� : �� ��ġ�� ���� �̹��� rotation �� warping �ϵ��� ����
 */

#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace cv::face;
using namespace std;

int main(int argc, const char *argv[]) {
	string fn_haar = string("E:/opencv_train_xml_data/haarcascades/haarcascade_frontalface_alt.xml");
	string eyes = string("E:/opencv_train_xml_data/haarcascades/haarcascade_eye.xml");

	Mat testSample = imread("test-seonae.jpg");

	Mat origin = testSample.clone();
	Mat gray;

	cvtColor(origin, gray, CV_BGR2GRAY);

	CascadeClassifier haar_cascade, eye_cas;
	haar_cascade.load(fn_haar);
	eye_cas.load(eyes);

	//	eye_cas_l.load(leye_haar);
	//	eye_cas_r.load(reye_haar);

	vector<Rect_<int> > faces;
	vector<Rect_<int> > eyes_;

	haar_cascade.detectMultiScale(gray, faces/*, 1.2, 3, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30)*/);

	printf("face count : %d\n", faces.size());
	Rect face_i;

	// �̹������� �󱼺κи� �ڸ���. 
	// ����ϴ� FR �˰��� ���� ������ ���� ���� �߻� ����.
	Mat face;
	Mat face_resized;

	// ������ �� ��.(ù��° ����)
	Mat std = origin(faces[0]);
	//Mat sample = origin(faces[1]);
	//Mat input = sample.clone();
	Mat output;
	imshow("std", std);
	//imshow("sample", sample);

	
	/**************** �� �ȿ��� ��ã�� ****************/
	for (int i = 0; i < faces.size(); i++) {
		Rect face_i = faces[i];
		Mat face = gray(face_i);
		
		// �� ��ġ ã��
		eye_cas.detectMultiScale(face, eyes_, 1.2, 3, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
		
		for (int j = 0; j < eyes_.size(); j++) {
			// �� ��ġ�� �簢�� �׸���. 
			// �簢���� �����(���� Ȯ���� ������ ��ġ��� ����)�� ���׶�� �׸���
			// -> �� �ڵ忡�� ������ ��ġ ��� ���
			// Point(faceface_i.x + eyes_[j].x + eyes_[j].width / 2, face_i.y + eyes_[j].y + eyes_[j].height / 2) 
			rectangle(origin, Rect(face_i.x + eyes_[j].x, face_i.y + eyes_[j].y, eyes_[j].width, eyes_[j].height), CV_RGB(0, 255, 0), 2);
			circle(origin, Point(face_i.x + eyes_[j].x + eyes_[j].width / 2, face_i.y + eyes_[j].y + eyes_[j].height / 2), 3, Scalar(0, 0, 255), 3);
		}
		/***************************************************/
		
		//Mat trans = Mat::zeros(std.rows, std.cols, sample.type());

		while (waitKey(0) != 27);

		// Warping �� Point
		Point2f before[4];
		printf("%d\n", faces[0].x);
		printf("%d\n", faces[1].x);
		printf("%d\n", sample.cols);
		before[0] = Point2f(faces[1].x, faces[1].y);
		before[1] = Point2f(faces[1].x + faces[1].width, faces[1].y);
		before[2] = Point2f(faces[1].x + faces[1].width, faces[1].y + faces[1].height);
		before[3] = Point2f(faces[1].x, faces[1].y + faces[1].height);

		// Warping �� Point
		Point2f after[4];
		after[0] = Point2f(0, 0);
		after[1] = Point2f(std.cols, 0);
		after[2] = Point2f(std.cols, std.rows);
		after[3] = Point2f(0, std.rows);
		Size warpsize(faces[0].width, faces[0].height);
		Mat warpImg(warpsize, sample.type());

		// ������ �� �󱼿� �簢�� �׸��� 
		rectangle(origin, faces[0], CV_RGB(0, 255, 0), 2);

		Mat trans = getPerspectiveTransform(before, after);
		warpPerspective(origin, warpImg, trans, warpImg.size());

		/* for (int i = 0;i < sizeof(before);i++) {
			circle(origin, before[i], 3, Scalar(0, 0, 255), 3);
			circle(origin, after[i], 3, Scalar(0, 0, 255), 3);
		}*/

		imshow("origin", origin);
		imshow("image", warpImg);
		if (waitKey(0) == 27);
		return 0;
	}
}