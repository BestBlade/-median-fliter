/*
*┌────────────────────────────────────────────────┐
*│　描    述：根据原理编写程序，实现中值滤波													   │
*│　作    者：牟峙桦|BestBlade																	   │
*│　版    本：2.0																			   	   │
*│　创建时间：2020.10.18																		   │
*└────────────────────────────────────────────────┘
*/

#include <iostream>
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp>  
#include"opencv2/imgproc/imgproc.hpp"
#include <stdio.h>

using namespace std;
using namespace cv;

Mat median_fliter(Mat img, Size temp) {
	if (temp.height % 2 == 0 || temp.width % 2 == 0) {
		cerr << "error template size" << endl;
	}
	int row_center = temp.height / 2;
	int col_center = temp.width / 2;
	int mid = temp.height * temp.width / 2;

	Mat res(img.rows, img.cols, img.type());

	for (int x = row_center; x < img.rows - row_center; x++) {
		for (int y = col_center; y < img.cols - col_center; y++) {
			if (res.channels() == 1) {
				vector<int> val;
				for (int i = 0; i < temp.height; i++) {
					for (int j = 0; j < temp.width; j++) {
						val.push_back(img.at<uchar>(x + i - row_center, y + j - col_center));
					}
				}
				sort(val.begin(), val.end());
				res.at<uchar>(x, y) = val[mid];
			}
			else {
				//空间换时间
				vector<int> val_r;
				vector<int> val_g;
				vector<int> val_b;
				for (int i = 0; i < temp.height; i++) {
					for (int j = 0; j < temp.width; j++) {
						val_r.push_back(img.at<Vec3b>(x + i - row_center, y + j - col_center)[2]);
						val_g.push_back(img.at<Vec3b>(x + i - row_center, y + j - col_center)[1]);
						val_b.push_back(img.at<Vec3b>(x + i - row_center, y + j - col_center)[0]);
					}
				}
				sort(val_r.begin(), val_r.end());
				sort(val_g.begin(), val_g.end());
				sort(val_b.begin(), val_b.end());
				res.at<Vec3b>(x, y)[0] = val_b[mid];
				res.at<Vec3b>(x, y)[1] = val_g[mid];
				res.at<Vec3b>(x, y)[2] = val_r[mid];
			}
		}
	}
	return res;
}

int main()
{
	Mat img = imread("2.JPG");

	Size temp(3, 3);

	Mat res = median_fliter(img, temp);

	imshow("原图", img);
	imshow("5x5平滑结果图", res);

	waitKey(0);

	return 0;
}
