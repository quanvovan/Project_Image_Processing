#include "Convolution.h"
#include "EdgeDetector.h"
#include "Blur.h"
#include <string.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
#pragma region ONLY_FOR_DEBUG
	argc = 5;
	char* temp[5];
	temp[0] = _strdup("main.exe");
	temp[1] = _strdup("--zoom");
	temp[2] = _strdup("--bl");
	temp[3] = _strdup("test.png");
	temp[4] = _strdup("commandArgs");
	argv = temp;
#pragma endregion

	char *command, *inputPath, *Interpolate, *cmdArguments;

	try
	{
		command = argv[1];
		inputPath = argv[2];

		// khởi tạo mặc định 
		int isSuccess = 0;
		Mat srcImg = imread(inputPath, IMREAD_GRAYSCALE);
		Mat dstImg;

		Blur blur;
		EdgeDetector EdgeD;
		//-----------------------------------------------

		if (strcmp(command, "--mean") == 0)
		{
			int kWidth = atoi(argv[3]);
			int kHeight = atoi(argv[4]);
			isSuccess = blur.BlurImage(srcImg, dstImg, kWidth, kHeight, 0);
		}
		else if (strcmp(command, "--median") == 0)
		{
			int kWidth = atoi(argv[3]);
			int kHeight = atoi(argv[4]);
			isSuccess = blur.BlurImage(srcImg, dstImg, kWidth, kHeight, 1);
		}
		else if (strcmp(command, "--gauss") == 0)
		{
			int kWidth = atoi(argv[3]);
			int kHeight = atoi(argv[4]);
			isSuccess = blur.BlurImage(srcImg, dstImg, kWidth, kHeight, 2);
		}
		else if (strcmp(command, "--sobel") == 0)
		{
			//kWidth = 3, kHeight = 3
			isSuccess = EdgeD.DetectEdge(srcImg, dstImg, 3, 3, 1);
		}
		else if (strcmp(command, "--prewitt") == 0)
		{
			isSuccess = EdgeD.DetectEdge(srcImg, dstImg, 3, 3, 2);
		}
		else if (strcmp(command, "--laplace") == 0)
		{
			isSuccess = EdgeD.DetectEdge(srcImg, dstImg, 3, 3, 3);
		}

		//in kết quả
		if (isSuccess)
		{
			imshow("Source Image", srcImg);
			imshow("Destination Image", dstImg);
			waitKey(0);
		}
		else
		{
			throw "Error:..........";
		}
		//----------------------------------
	}
	catch (const char* msg)
	{
		cout << msg << endl;
		system("pause");
	}


#pragma region ONLY_FOR_DEBUG
	free(temp[0]);
	free(temp[1]);
	free(temp[2]);
	free(temp[3]);
	free(temp[4]);
#pragma endregion

	//system("pause");
	return 0;
}
