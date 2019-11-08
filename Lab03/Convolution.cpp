
#include "Convolution.h"

vector<float> Convolution::GetKernel()
{
	return this->_kernel;
}

void Convolution::SetKernel(vector<float> kernel, int kWidth, int kHeight)
{
	this->_kernelWidth = kWidth;
	this->_kernelHeight = kHeight;
	copy(kernel.begin(), kernel.end(), back_inserter(this->_kernel));
}

int Convolution::DoConvolution(const Mat & sourceImage, Mat & destinationImage)
{
	int rows = sourceImage.rows;
	int cols = sourceImage.cols;
	int iStart = -_kernelWidth / 2;
	int jStart = -_kernelHeight / 2;

	Mat res(rows, cols, CV_32F);

	for (int x = 0; x < cols; x++)
	{
		for (int y = 0; y < rows; y++)
		{
			float val = 0;
			for (int k = 0; k < _kernel.size(); k++)
			{
				int i = iStart + k % _kernelWidth;
				int j = jStart + k / _kernelWidth;
				int r = y - j;
				int c = x - i;

				if (r < 0 || r >= rows || c < 0 || c >= cols)
					continue;
				val += sourceImage.at<uchar>(r, c) * _kernel[k];
			}
			res.at<float>(y, x) = val;
		}
	}

	destinationImage = res;
	return 0;
}

Convolution::Convolution()
{
	this->_kernelWidth = 0;
	this->_kernelHeight = 0;
}

Convolution::~Convolution()
{
}