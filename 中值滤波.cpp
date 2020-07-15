/*
*┌────────────────────────────────────────────────┐
*│　描    述：根据原理编写程序，实现中值滤波													   │
*│　作    者：牟峙桦|BestBlade																	   │
*│　版    本：1.0																			   	   │
*│　创建时间：2020.07.13																		   │
*└────────────────────────────────────────────────┘
*/

#include <iostream>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace cv;

void median_fliter(Mat &origin_pic, Mat &result_pic, Size ksize)								/*	灰度图像中值滤波	*/
{
	result_pic = origin_pic.clone();
	Mat during_pic = result_pic.clone();

	int row = result_pic.rows;
	int col = result_pic.cols;

	int krow = ksize.width;
	int kcol = ksize.height;

	int median = krow * kcol / 2;																/*	计算中值，由于数组从0开始，因此此处不必-1	*/

	if ((row < krow) && (col < kcol))
	{
		cerr << "ERROR:图片尺寸过小" << endl;													/*	图片尺寸过小	*/
	}
	else
		if ((krow % 2 == 0) || (kcol % 2 == 0))
		{
			cerr << "ERROR:滑窗口尺寸错误" << endl;												/*	平滑窗口尺寸错误	*/
		}
		else
		{
			int row_start = krow / 2;
			int col_start = kcol / 2;

			if (origin_pic.channels() == 1)														/*	单通道图像	*/
			{
				for (int i = row_start; i < row - row_start; i++)
				{
					for (int j = col_start; j < col - col_start; j++)
					{
						int n = { 0 };
						vector<int> channel(krow * kcol);

						for (int p = i - row_start; p <= i + row_start; p++)
						{
							for (int q = j - col_start; q <= j + col_start; q++)
							{
								channel[n++] = during_pic.at<uchar>(p, q);						/*	记录像素(i,j)周围模板的每一个像素值	*/
							}
						}

						std::sort(channel.begin(), channel.end());								/*	排序	*/		/*	std库和cv库都有sort函数，因此要注明	*/

						result_pic.at<uchar>(i, j) = channel[median];							/*	将中值赋给结果图第(i,j)个像素，完成中值滤波	*/
					}
				}
			}
			else if (origin_pic.channels() == 3)												/*	对于三通道图像	*/
			{
				for (int i = row_start; i < row - row_start; i++)
				{
					for (int j = col_start; j < col - col_start; j++)
					{
						int n = { 0 };
						vector<int> channel_r(krow * kcol);
						vector<int> channel_g(krow * kcol);
						vector<int> channel_b(krow * kcol);

						for (int p = i - row_start; p <= i + row_start; p++)
						{
							for (int q = j - col_start; q <= j + col_start; q++)
							{
								channel_r[n] = during_pic.at<Vec3b>(p, q)[2];					/*	记录像素(i,j)周围模板的每一个像素值	*/
								channel_g[n] = during_pic.at<Vec3b>(p, q)[1];
								channel_b[n] = during_pic.at<Vec3b>(p, q)[0];
								n++;
							}
						}

						std::sort(channel_r.begin(), channel_r.end());							/*	排序	*/
						std::sort(channel_g.begin(), channel_g.end());
						std::sort(channel_b.begin(), channel_b.end());

						result_pic.at<Vec3b>(i, j)[0] = channel_b[median];						/*	将中值赋给结果图第(i,j)个像素，完成中值滤波	*/
						result_pic.at<Vec3b>(i, j)[1] = channel_g[median];
						result_pic.at<Vec3b>(i, j)[2] = channel_r[median];
					}
				}
			}
		}
	
}

int main()
{
	Mat origin_pic = imread("C:/Users/Chrysanthemum/Desktop/2.JPG",-1);						/*	三通道测试图	*/
	//Mat origin_pic = imread("C:/Users/Chrysanthemum/Desktop/1.jpg", -1);						/*	单通道测试图	*/

	Mat result_pic;
	Size ksize(3, 3);

	cout << "图片通道数为" << origin_pic.channels() << endl;

	median_fliter(origin_pic, result_pic, ksize);

	imshow("原图", origin_pic);
	imshow("5x5平滑结果图", result_pic);

	waitKey(0);

	return 0;
}