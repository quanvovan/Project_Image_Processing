#include "ColorTransformer.h"

int ColorTransformer::ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, short b)
{
	int cols = sourceImage.cols;
	int rows = sourceImage.rows;


	destinationImage = sourceImage.clone();

	for (int i = 0; i < rows; i++)
	{
		const uchar* src = sourceImage.ptr<uchar>(i);
		uchar* res = destinationImage.ptr<uchar>(i);

		for (int j = 0; j < cols; j++, src++, res++)
		{
			float blue = src[0] + b;
			float green = src[1] + b;
			float red = src[2] + b;

			blue = blue < 255 ? blue : 255;
			green = green < 255 ? green : 255;
			red = red < 255 ? red : 255;

			res[0] = (uchar)blue;
			res[1] = (uchar)green;
			res[2] = (uchar)red;
		}
	}
	return 1;
}

int ColorTransformer::ChangeContrast(const Mat& sourceImage, Mat& destinationImage, float c)
{
	if (!sourceImage.data) {
		return 0;
	}
	destinationImage.create(sourceImage.size(), sourceImage.type());
	float r, g, b;
	int rows = sourceImage.rows;
	int cols = sourceImage.cols;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			b = (float)sourceImage.at<cv::Vec3b>(i, j)[0] * c;
			g = (float)sourceImage.at<cv::Vec3b>(i, j)[1] * c;
			r = (float)sourceImage.at<cv::Vec3b>(i, j)[2] * c;
			b = (b > 255) ? 255 : b;
			g = (g > 255) ? 255 : g;
			r = (r > 255) ? 255 : r;
			destinationImage.at<cv::Vec3b>(i, j)[0] = (uchar)b;
			destinationImage.at<cv::Vec3b>(i, j)[1] = (uchar)g;
			destinationImage.at<cv::Vec3b>(i, j)[2] = (uchar)r;
		}
	}

	return 1;
}

int ColorTransformer::CalcHistogram(const Mat& sourceImage, Mat& histMatrix)
{
	if (sourceImage.empty()) //Kiểm tra hình đâu vào và thoát ra nếu lỗi
		return 0;

	vector<Mat> img_rgb; //Vector chứa các ma trận được tách ra theo channel màu

	int w = 512, h = 400; //Kích thước khung histogram
	int sizeHist = 256; //Số lượng thành phần của histogram của histogram
	split(sourceImage, img_rgb); //Tách thành các channel

	int red, green, blue;
	histMatrix = Mat::zeros(3, 256, CV_32F);

	for (int x = 0; x < sourceImage.cols; x++)
		for (int y = 0; y < sourceImage.rows; y++)
		{
			// truy xuất đến toạ độ ảnh
			blue = img_rgb[0].at<uchar>(y, x);
			histMatrix.at<float>(0, blue)++; //Tăng số lượng điểm ảnh có tại màu blue

			green = img_rgb[1].at<uchar>(y, x);
			histMatrix.at<float>(1, green)++; //Tăng số lượng điểm ảnh có tại màu green

			red = img_rgb[2].at<uchar>(y, x);
			histMatrix.at<float>(2, red)++; //Tăng số lượng điểm ảnh có tại màu red
		}

	return 1;
}

vector<int> CalculateEqualizeHistogramVector(vector<int> hist, Mat newmap)
{
	vector <float> xacsuat;
	xacsuat.resize(256);
	//Tinh xac suat cua tung gia tri hist, hist co 256 gia tri tu 1->256 tuong ung
	for (int i = 0; i < 256; i++) xacsuat[i] = hist[i] * 1.0 / (newmap.rows * newmap.cols);

	//Tinh cdf (Compact Density Fiber Board), la tinh tong tang dan cua moi xac suat hist
	vector<float> cdf;
	cdf.resize(256);
	cdf[0] = xacsuat[0];
	for (int i = 1; i < 256; i++) cdf[i] = cdf[i - 1] + xacsuat[i];
	
	//Sau cung tinh Equalize Hist de phan phoi xac suat hop li
	vector<int> EqualizeHist;
	EqualizeHist.resize(256);
	for (int i = 0; i < 256; i++) EqualizeHist[i] = (int)(255 * cdf[i]);
	return EqualizeHist;
}
// Tinh histogram anh xam
vector <int> GrayImageCalculate(Mat a)
{
	//Doc anh a, moi diem anh co gia tri tu 1-256, tinh tan so cua tung gia tri
	vector<int> histogram;
	histogram.resize(256);
	for (int i = 0; i < 256; i++) histogram[i] = 0;

	for (int i = 0; i < a.rows; i++)
	{
		for (int k = 0; k < a.cols; k++)
		{
			uchar intensity = a.at<uchar>(i, k);
			histogram[static_cast<int>(intensity)] ++;
		}
	}
	return histogram;
}
// Can bang anh xam
Mat EqualizeGrayHistogram(Mat a)
{
	// Lay vector histogram anh xam, equalize no, sau do quang nguoc tro lai anh goc
	Mat newmap = a.clone();
	vector<int> hist = GrayImageCalculate(a);
	vector<int> newLevel = CalculateEqualizeHistogramVector(hist, newmap);

	for (int i = 0; i < newmap.rows; i++)
	{
		for (int k = 0; k < newmap.cols; k++)
		{
			uchar &intensity = newmap.at<uchar>(i, k);
			int num = static_cast<int>(intensity);
			intensity = newLevel[num];
		}
	}
	return newmap;
}

// Tinh histogram anh mau
vector<vector<int>> ColorImageCalculate(Mat a)
{
	// Tuong tu nhu tinh Histogram anh xam, nhung tach ra thanh 3 kenh R,G,B
	// Tuong ung voi histogram[0], histogram[1], histogram[2]
	vector<vector<int>> histogram;
	histogram.resize(3);
	for (int i = 0; i < 3; i++)
	{
		histogram[i].resize(256);
		for (int k = 0; k < 256; k++) histogram[i][k] = 0;
	}

	for (int i = 0; i < a.rows; i++)
	{
		for (int k = 0; k < a.cols; k++)
		{
			Vec3b intensity = a.at<Vec3b>(i, k);
			for (int j = 0; j < 3; j++) histogram[j][static_cast<int>(intensity[j])] ++;
		}
	}
	return histogram;
}
// Can bang anh mau
Mat EqualizeColorHistogram(Mat a)
{
	// Tuong tu voi can bang anh xam, nhung tach ra thanh 3 kenh R,G,B
	// Sau do equalize tung kenh va dua lai vao anh goc
	Mat newmap = a.clone();
	vector<vector<int>>hist;
	hist = ColorImageCalculate(a);

	vector<vector<int>> newLevel;
	newLevel.resize(3);
	for (int i = 0; i < 3; i++)
	{
		newLevel[i].resize(256);
		vector<int> temp = CalculateEqualizeHistogramVector(hist[i], newmap);
		for (int k = 0; k < 256; k++)
		{
			newLevel[i][k] = temp[k];
		}
	}

	for (int i = 0; i < newmap.rows; i++)
	{
		for (int k = 0; k < newmap.cols; k++)
		{
			Vec3b &intensity = newmap.at<Vec3b>(i, k);
			for (int j = 0; j < 3; j++) 
			{
				int num = static_cast<int>(intensity[j]);
				intensity.val[j] = newLevel[j][num];
			}
		}
	}
	return newmap;
}

int ColorTransformer::HistogramEqualization(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.empty())
		return 0;

	if (sourceImage.type() == 0) destinationImage = EqualizeGrayHistogram(sourceImage);

	else destinationImage = EqualizeColorHistogram(sourceImage);

	return 1;
}

int ColorTransformer::DrawHistogram(const Mat& histMatrix, Mat& histImage)
{
	if (histMatrix.empty()) //Kiểm tra hình đầu vào và thoát ra nếu lỗi
		return 0;

	int w = 512, h = 400; //Kích thước khung histogram
	int sizeHist = 256; //Kích thước của histogram

	//Tạo ra một hình để hiện histogram
	int bin = cvRound((double)w / sizeHist);
	histImage = Mat(h, w, CV_8UC3, Scalar(0, 0, 0));

	Mat img_b = histMatrix.row(0), img_g = histMatrix.row(1), img_r = histMatrix.row(2);

	//Chuẩn hoá giá trị
	normalize(img_b, img_b, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(img_g, img_g, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(img_r, img_r, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < sizeHist; i++)
	{
		line(histImage, Point(bin*(i - 1), h - cvRound(img_b.at<float>(i - 1))),
			Point(bin*(i), h - cvRound(img_b.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin*(i - 1), h - cvRound(img_g.at<float>(i - 1))),
			Point(bin*(i), h - cvRound(img_g.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin*(i - 1), h - cvRound(img_r.at<float>(i - 1))),
			Point(bin*(i), h - cvRound(img_r.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	return 1;
}

int CalcHistToCmp(const Mat& sourceImage, Mat& histMatrix)
{
	if (sourceImage.empty())
		return 0;
	int size[] = { 256,256,256};
	histMatrix = Mat(3, size, CV_32F, cv::Scalar(0));
	for (int i=0;i<sourceImage.rows;i++)
		for (int j = 0; j < sourceImage.cols; j++)
		{
			Vec3b bgr = sourceImage.at <Vec3b>(i, j);
			histMatrix.at<float>(bgr[0], bgr[1], bgr[2]) += 1;
		}
	return 1;
}

float ColorTransformer::CompareImage(const Mat& image1, Mat& image2)
{
	Mat hist1, hist2;
	CalcHistToCmp(image1, hist1);
	normalize(hist1, hist1, 0, 1, NORM_MINMAX, -1, Mat());
	CalcHistToCmp(image2, hist2);
	normalize(hist2, hist2, 0, 1, NORM_MINMAX, -1, Mat());
	float res = 0.0f;

	for (int i = 0; i < 256; i++)
		for (int j = 0; j < 256; j++)
			for (int k = 0; k < 256; k++)
			{
				float ai = hist1.at<float>(i, j, k);
				float bi = hist2.at<float>(i, j, k);
				if (ai != 0)
					res += (ai - bi) * (ai - bi) / ai;
			}

	return res;
}

ColorTransformer::ColorTransformer()
{
}

ColorTransformer::~ColorTransformer()
{
}
