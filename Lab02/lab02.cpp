﻿//
#include "GeometricTransformer.h"
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
		Interpolate = argv[2];
		inputPath = argv[3];

		// khởi tạo mặc định 
		
		GeometricTransformer GTF;
		PixelInterpolate *pxInterpolate;
		int isSuccess = 0;

		
		Mat srcImg = imread(inputPath, -1); 
		Mat dstImg;
		//-----------------------------------------------

		if (strcmp(command, "--zoom") == 0)
		{
			if (argc < 5)
			{
				throw "Missing parameter!";
			}

			float sx = atof(argv[4]); //zoom theo x 
			float sy = atof(argv[5]); //zoom theo y
									  // zoom = scale
			if (Interpolate == "--bl")
			{
				pxInterpolate = new BilinearInterpolate();
				isSuccess = GTF.Scale(srcImg, dstImg, sx, sy, pxInterpolate);
			}
			else
			{
				pxInterpolate = new NearestNeighborInterpolate();
				isSuccess = GTF.Scale(srcImg, dstImg, sx, sy, pxInterpolate);
			}

		}
		else if (strcmp(command, "--resize") == 0)
		{
			if (argc < 5)
			{
				throw "Missing parameter!";
			}

			int newWidth = stoi(argv[4]); // kích thước mới 
			int newHeight = stoi(argv[5]);

			if (Interpolate == "--bl")
			{
				pxInterpolate = new BilinearInterpolate();
				isSuccess = GTF.Resize(srcImg, dstImg, newWidth, newHeight, pxInterpolate);
			}
			else
			{
				pxInterpolate = new NearestNeighborInterpolate();
				isSuccess = GTF.Resize(srcImg, dstImg, newWidth, newHeight, pxInterpolate);
			}

		}
		else if (strcmp(command, "--rotate") == 0)
		{
			if (argc < 4)
			{
				throw "Missing parameter!";
			}

			float angle = atof(argv[4]);  // góc xoay
			if (Interpolate == "--bl")
			{
				pxInterpolate = new BilinearInterpolate();
				isSuccess = GTF.RotateKeepImage(srcImg, dstImg, angle, pxInterpolate);
			}
			else
			{
				pxInterpolate = new NearestNeighborInterpolate();
				isSuccess = GTF.RotateKeepImage(srcImg, dstImg, angle, pxInterpolate);
			}

		}
		else if (strcmp(command, "--rotateN") == 0)
		{
			if (argc < 4)
			{
				throw "Missing parameter!";
			}

			float angle = atof(argv[4]);  // góc xoay
			if (Interpolate == "--bl")
			{
				pxInterpolate = new BilinearInterpolate();
				isSuccess = GTF.RotateUnkeepImage(srcImg, dstImg, angle, pxInterpolate);
			}
			else
			{
				pxInterpolate = new NearestNeighborInterpolate();
				isSuccess = GTF.RotateUnkeepImage(srcImg, dstImg, angle, pxInterpolate);
			}

		}
		else if (strcmp(command, "--flip") == 0)
		{
			if (argc < 4)
			{
				throw "Missing parameter!";
			}

			bool direction = (argv[4]);  // string to bool :)) 
			if (Interpolate == "--bl")
			{
				pxInterpolate = new BilinearInterpolate();
				isSuccess = GTF.Flip(srcImg, dstImg, direction, pxInterpolate);
			}
			else
			{
				pxInterpolate = new NearestNeighborInterpolate();
				isSuccess = GTF.Flip(srcImg, dstImg, direction, pxInterpolate);
			}

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
