#include "EdgeDetector.h"
#include "Convolution.h"

int EdgeDetector::DetectEdge(const Mat & sourceImage, Mat & destinationImage, int kWidth, int kHeight, int method)
{
	if (sourceImage.empty()) //Kiểm tra hình đâu vào và thoát ra nếu lỗi
		return 0;
	if (method == 1) // Sobel Edge Detection
	{
		destinationImage.create(sourceImage.size(), sourceImage.type());
		vector<vector<int>> matrix;
		matrix.resize(sourceImage.rows);
		for (int i = 0; i < sourceImage.rows; i++)
		{// Lưu ma trận của ảnh sourceImage
			matrix[i].resize(sourceImage.cols);
			for (int k = 0; k < sourceImage.cols; k++)
			{
				matrix[i][k] = static_cast<int>(sourceImage.at<uchar>(i, k));
			}
		}

		for (int i = 1; i < sourceImage.rows - 1; i++)
		{// Tính toán theo mặt nạ Sobel để có được biên cạnh
			for (int k = 1; k < sourceImage.cols - 1; k++)
			{
				int Gx = matrix[i - 1][k + 1] + 2 * matrix[i][k + 1] + matrix[i + 1][k + 1];
				Gx -=    matrix[i - 1][k - 1] + 2 * matrix[i][k - 1] + matrix[i + 1][k - 1];
				int Gy = matrix[i + 1][k - 1] + 2 * matrix[i + 1][k] + matrix[i + 1][k + 1];
				Gy -=    matrix[i - 1][k - 1] + 2 * matrix[i - 1][k] + matrix[i - 1][k + 1];
				
				float alpha = round(sqrt(Gx*Gx + Gy*Gy));// Có thể chọn alpha = |Gx| + |Gy|
				destinationImage.at<uchar>(i, k) = static_cast<uchar>(alpha);
			}
		}
	}
	
	
	Convolution conv;
	Mat fx, fy;
	vector<float> Wx = { -1/3, 0, 1/3,
						 -1/3, 0, 1/3,
						 -1/3, 0, 1/3};
	conv.SetKernel(Wx, 3, 3);
	
	int calCov = conv.DoConvolution(sourceImage, fx);

	if (calCov == 0)
		return 0;

	vector<float> Wy = { -1/3, -1/3, -1/3,
						  0, 0, 0,
						  1/3, 1/3, 1/3 };
	conv.SetKernel(Wy, 3, 3);
	
	calCov = conv.DoConvolution(sourceImage, fy);

	if (calCov == 0)
		return 0;
	return 0;
}

EdgeDetector::EdgeDetector()
{
}

EdgeDetector::~EdgeDetector()
{
}
