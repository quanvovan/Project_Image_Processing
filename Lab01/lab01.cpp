#define _CRT_SECURE_NO_WARNINGS
#include "Converter.h"
#include "ColorTransformer.h"
#include <string.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
#pragma region ONLY_FOR_DEBUG
	argc = 4;
	char* temp[4];
	temp[0] = _strdup("Lab01.exe");
	temp[1] = _strdup("--hist");
	temp[2] = _strdup("test_color.png");
	temp[3] = _strdup("f");
	argv = temp;
#pragma endregion

	char* command, *inputPath, *interpolate, *cmdArguments;

	try
	{
		command = argv[1];
		inputPath = argv[2];

		Mat input = imread(inputPath, -1);
		Mat output;
		int isSuccessConvert  =0;
		int isSuccessTrans = 0;
		// Command in LAB 1
		if (strcmp(command, "--rgb2gray") == 0)
		{
			Converter converter;
			isSuccessConvert  = converter.Convert(input, output, 0);
		}
		else if (strcmp(command, "--gray2rgb") == 0)
		{
			Converter converter;
			isSuccessConvert  = converter.Convert(input, output, 1);
		}
		else if (strcmp(command, "--rgb2hsv") == 0)
		{
			Converter converter;
			isSuccessConvert  = converter.Convert(input, output, 2);

		}
		else if (strcmp(command, "--hsv2rgb") == 0)
		{
			Converter converter;
			isSuccessConvert  = converter.Convert(input, output, 0);
		}
		else if (strcmp(command, "--bright") == 0)
		{
			ColorTransformer ColorTrans;
			uchar b = atoi(argv[3]);
			isSuccessTrans = ColorTrans.ChangeBrighness(input, output, b);
		}
		else if (strcmp(command, "--contrast") == 0)
		{
			ColorTransformer ColorTrans;
			uchar c = atoi(argv[3]);
			isSuccessTrans = ColorTrans.ChangeContrast(input, output, c);
		}
		else if (strcmp(command, "--hist") == 0)
		{	
			ColorTransformer ColorTrans;
			isSuccessTrans = ColorTrans.CalcHistogram(input, output);
 		}
		else if (strcmp(command, "--equalhist") == 0)
		{
			ColorTransformer ColorTrans;
 			isSuccessTrans = ColorTrans.HistogramEqualization(input, output);
 		}
		else if (strcmp(command, "--drawhist") == 0)
		{
			ColorTransformer ColorTrans;
			Mat hist;
			isSuccessTrans = ColorTrans.CalcHistogram(input, hist);

			if (isSuccessTrans == 1)
				isSuccessTrans = ColorTrans.DrawHistogram(hist, output);
		}
		else if (strcmp(command, "--compare") == 0)
		{
			// so sanh
		}

		if (strcmp(command, "--rgb2gray") == 0 || strcmp(command, "--gray2rgb")==0 || strcmp(command, "--rgb2hsv") == 0 || strcmp(command, "--hsv2rgb") == 0)
		{
			//in kết quả
			if (isSuccessConvert == 0)
			{
				imshow("Source Image", input);
				imshow("Destination Image", output);
				waitKey(0);
			}
			else
			{
				throw "Error:..........";
			}
		}
		if (strcmp(command, "--bright") == 0 || strcmp(command, "--contrast") == 0 || strcmp(command, "--hist") == 0 || strcmp(command, "--hist") == 0 || strcmp(command, "--equalhist") == 0 || strcmp(command, "--drawhist") == 0 || strcmp(command, "--compare") == 0)
		{
			if (isSuccessTrans==1)
			{
				imshow("Source Image", input);
				imshow("Destination Image", output);
				waitKey(0);
			}
			else
			{
				throw "Error:..........";
			}
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
#pragma endregion

	//system("pause");
	return 0;
}
