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
	temp[1] = _strdup("--drawhist");
	temp[2] = _strdup("test_gray.png");
	temp[3] = _strdup("commandArgs");
	argv = temp;
#pragma endregion

	char* command, *inputPath, *interpolate, *cmdArguments;

	try
	{
		if (argc < 3)
		{
			//Print exception
			throw "Missing parameter!";
		}

		command = argv[1];
		inputPath = argv[2];

		Mat input = imread(inputPath, -1);
		Mat output;
		int isSuccess =0;

		// Command in LAB 1
		if (strcmp(command, "--rgb2gray") == 0)
		{
			Converter converter;
			isSuccess = converter.Convert(input, output, 0);
		}
		else if (strcmp(command, "--gray2rgb") == 0)
		{
			Converter converter;
			isSuccess = converter.Convert(input, output, 1);
		}
		else if (strcmp(command, "--rgb2hsv") == 0)
		{
			Converter converter;
			isSuccess = converter.Convert(input, output, 2);

		}
		else if (strcmp(command, "--hsv2rgb") == 0)
		{
			Converter converter;
			isSuccess = converter.Convert(input, output, 0);
		}
		else if (strcmp(command, "--bright") == 0)
		{
			ColorTransformer ColorTrans;
			uchar b = atoi(argv[3]);
			isSuccess = ColorTrans.ChangeBrighness(input, output, b);
		}
		else if (strcmp(command, "--contrast") == 0)
		{
			ColorTransformer ColorTrans;
			uchar c = atoi(argv[3]);
			isSuccess = ColorTrans.ChangeContrast(input, output, c);
		}
		else if (strcmp(command, "--hist") == 0)
		{	
			ColorTransformer ColorTrans;
			isSuccess = ColorTrans.CalcHistogram(input, output);
 		}
		else if (strcmp(command, "--equalhist") == 0)
		{
			ColorTransformer ColorTrans;
 			isSuccess = ColorTrans.HistogramEqualization(input, output);
 		}
		else if (strcmp(command, "--drawhist") == 0)
		{
			ColorTransformer ColorTrans;
			isSuccess = ColorTrans.DrawHistogram(input, output);
		}
		else if (strcmp(command, "--compare") == 0)
		{
			// so sanh
		}
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
