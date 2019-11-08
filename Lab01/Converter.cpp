#include "Converter.h"

int Converter::RGB2GrayScale(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.type() == CV_8UC1)
	{
		destinationImage = sourceImage;
		return 0;
	}
	if (sourceImage.type() != CV_8UC3)
		return 1;

	int rows = sourceImage.rows;
	int cols = sourceImage.cols;

	destinationImage.create(sourceImage.size(), CV_8UC1); // tạo ảnh dich mới với kích thước bằng ảnh cũ

	for (int i = 0; i < rows; i++)
	{
		const uchar* src = sourceImage.ptr<uchar>(i); // trả về con trỏ mảng
		uchar* res = destinationImage.ptr<uchar>(i);

		for (int j = 0; j < cols; j++, src += 3)
		{
			//res =  0.299⋅R+0.587⋅G+0.114⋅B
			res[j] = (uchar)(src[0] * 0.114f + src[1] * 0.587f + src[2] * 0.299f);
		}
	}
	return 0;
}

int Converter::GrayScale2RGB(const Mat& sourceImage, Mat& destinationImage)
{
	int rows = sourceImage.rows;
	int cols = sourceImage.cols;

	destinationImage.create(sourceImage.size(), CV_8UC3);

	for (int i = 0; i < rows; i++)
	{
		const uchar* src = sourceImage.ptr<uchar>(i);
		uchar* res = destinationImage.ptr<uchar>(i);

		for (int j = 0; j < cols; j++, res += 3)
		{
			res[0] = (uchar)(src[j]);
			res[1] = (uchar)(src[j]);
			res[2] = (uchar)(src[j]);
		}
	}
	return 0;
}
void rgb2hsv(float r, float g, float b, float & h, float & s, float & v)
{
		float min = std::min(std::min(r, g), b);
		float max = std::max(std::max(r, g), b);
		v = max;               

		float delta = max - min;

		if (max != 0.0)
			s = delta / max;      
		else {
			
			s = 0.0;
			h = 0.0;

		}
		if (max != min)
		{
			if (r == max)
				h = (g - b) / delta + (g < b ? 6 : 0);
			else if (g == max)
				h = 2.0 + (b - r) / delta;
			else
				h = 4.0 + (r - g) / delta;

			h = h  / 6;
		}

		h *= 100;
		s *= 100;
		v *= 100;
		/*if (h < 0.0)
			h += 360.0;*/

}
int Converter::RGB2HSV(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.type() != CV_8UC3)
		return 0;

	int rows = sourceImage.rows;
	int cols = sourceImage.cols;


	destinationImage.create(sourceImage.size(), CV_8UC3);

	float h, s, v;
	float r, g, b;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			b = (float)sourceImage.at<cv::Vec3b>(i, j)[0] / 255;
			g = (float)sourceImage.at<cv::Vec3b>(i, j)[1] / 255;
			r = (float)sourceImage.at<cv::Vec3b>(i, j)[2] / 255;

			rgb2hsv(r, g, b, h, s, v);
			/*float Max = max(max(r, b), g);
			float Min = min(min(r, b), g);
			v = Max;
			s = (v == 0) ? 0 : (v - Min) / v;
			h = (v == Min) ? 0 : ((v == r) ? 60 * (g - b) / (v - Min) : ((v == g) ? (120 + 60 * (b - r) / (v - Min)) : (240 + 60 * (r - g) / (v - Min))));
			h = (h < 0) ? h + 360 : h;
			*/

			destinationImage.at<cv::Vec3b>(i, j)[0] = (uchar)(h);
			destinationImage.at<cv::Vec3b>(i, j)[1] = (uchar)(s);
			destinationImage.at<cv::Vec3b>(i, j)[2] = (uchar)(v);
		}
	}

	return 1;
}
void hsv2rgb(float h, float s, float v, float & r, float & g, float & b)
{
	int i = floor(h * 6);
	float f = h * 6 - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);
	switch (i % 6)
	{
	case 0: r = v, g = t, b = p; break;
	case 1: r = q, g = v, b = p; break;
	case 2: r = p, g = v, b = t; break;
	case 3: r = p, g = q, b = v; break;
	case 4: r = t, g = p, b = v; break;
	case 5: r = v, g = p, b = q; break;
	}
}
int Converter::HSV2RGB(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.type() != CV_8UC3)
		return 0;

	int rows = sourceImage.rows;
	int cols = sourceImage.cols;

	destinationImage.create(sourceImage.size(), CV_8UC3);

	float h, s, v;
	float r, g, b;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			h = (float)sourceImage.at<cv::Vec3b>(i, j)[0]/100;
			s = (float)sourceImage.at<cv::Vec3b>(i, j)[1]/100;
			v = (float)sourceImage.at<cv::Vec3b>(i, j)[2]/100;


			hsv2rgb(h, s, v, r, g, b);
			/*r = v - v*s*max((min(k(5, h), min(4 - k(5, h), (float)1))), (float)0);
			g = v - v*s*max((min(k(3, h), min(4 - k(3, h), (float)1))), (float)0);
			b = v - v*s*max((min(k(1, h), min(4 - k(1, h), (float)1))), (float)0);
			*/
			destinationImage.at<cv::Vec3b>(i, j)[0] = (uchar)(b * 255);
			destinationImage.at<cv::Vec3b>(i, j)[1] = (uchar)(g * 255);
			destinationImage.at<cv::Vec3b>(i, j)[2] = (uchar)(r * 255);
		}
	}

	return 1;
}

int Converter::Convert(Mat& sourceImage, Mat& destinationImage, int type)
{
	switch (type)
	{
	case 0:
		return RGB2GrayScale(sourceImage, destinationImage);
		break;
	case 1:
		return GrayScale2RGB(sourceImage, destinationImage);
		break;
	case 2:
		return RGB2HSV(sourceImage, destinationImage);
		break;
	case 3:
		return HSV2RGB(sourceImage, destinationImage);
		break;
	default:
		break;
	}
	return 1;
}

int Converter::Convert(IplImage* sourceImage, IplImage* destinationImage, int type)
{
	return 0;
}

Converter::Converter()
{
}

Converter::~Converter()
{
}
