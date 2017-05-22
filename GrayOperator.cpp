#include "StdAfx.h"
#include "GrayOperator.h"
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;


/******************************************************************************
*	����:			���Ա任����
*	����:		
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
*		slope		���Ժ�����б��
*		inter		���Ժ����Ľؾ�
*	��ע:			�˺������ڲ�ɫͼͬ������
******************************************************************************/
void LineTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, 
			  double slope, double inter)
{
	// �Ҷ�ӳ���
	BYTE map[256];

	// ������������ʱֵ
	double dTemp;

	for (int i = 0; i < 256; i++)
	{
		// ���㵱ǰ���ر任���ֵ
		dTemp = slope * i + inter;

		// ����������޸���ֵ
		if (dTemp < 0)
			dTemp = 0.0;
		else if (dTemp > 255)
			dTemp = 255;

		// ��������
		map[i] = int(dTemp + 0.5);
	}

	// ���Ա任���ֱֵ����ӳ����в���
	for (UINT i = 0; i < width * height; i++)
	{	
		for (int j = 0; j < 4; j++)
			tempPixel[i*4+j] = map[ pixel[i*4+j] ];
	}
}

/******************************************************************************
*	����:			�����任����
*	����:		
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
*		a			���Ʋ���,��ʾ���ߵ�����ƫ����
*		b			���Ʋ���,��ʾ���ߵ������̶�
*	��ע:			�˺������ڲ�ɫͼͬ������
******************************************************************************/
void LogTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, 
			  double a, double b)
{
	// ӳ���,����256�ֻҶȱ任���ֵ
	BYTE map[256];

	// ������������ʱֵ
	double dTemp;

	for (int i = 0; i < 256; i++)
	{
		// ���㵱ǰ���ر任���ֵ
		dTemp = log((double)i + 1.0) / b + a;

		// ����������޸���ֵ
		if (dTemp < 0)
			dTemp = 0.0;
		else if (dTemp > 255)
			dTemp = 255;

		// ��������
		map[i] = int(dTemp + 0.5);
	}

	for (UINT i = 0; i < width * height; i++)
	{	
		for (int j = 0; j < 4; j++)
			tempPixel[i*4+j] = map[ pixel[i*4+j] ];
	}
}

/******************************************************************************
*	����:			�ݴα任����
*	����:		
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
*		b			���Ʋ���,��ʾ���ߵ�����ƫ����
*		c			���Ʋ���,��ʾ���ߵ������̶�
*		r			���Ʋ���,��ʾ�������ݴ�
*	��ע:			�˺������ڲ�ɫͼͬ������
******************************************************************************/
void PowerTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, 
			    double b, double c, double r)
{
	// ӳ���,����256�ֻҶȱ任���ֵ
	BYTE map[256];

	// ������������ʱֵ
	double dTemp;

	for (int i = 0; i < 256; i++)
	{
		// ���㵱ǰ���ر任���ֵ
		dTemp = c * pow(i / 255.0, r) * 255 + b;

		// ����������޸���ֵ
		if (dTemp < 0)
			dTemp = 0.0;
		else if (dTemp > 255)
			dTemp = 255;

		// ��������
		map[i] = int(dTemp + 0.5);
	}

	for (UINT i = 0; i < width * height; i++)
	{	
		for (int j = 0; j < 4; j++)
			tempPixel[i*4+j] = map[ pixel[i*4+j] ];
	}
}

/******************************************************************************
*	����:			ָ���任����
*	����:		
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
*		a			���Ʋ���,��ʾ���ߵ�����ƫ����
*		b,c			���Ʋ���,��ʾ���ߵ������̶�
*	��ע:			�˺������ڲ�ɫͼͬ������
******************************************************************************/
void ExpTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, 
				double a, double b, double c)
{
	// ӳ���,����256�ֻҶȱ任���ֵ
	BYTE map[256];

	// ������������ʱֵ
	double dTemp;

	for (int i = 0; i < 256; i++)
	{
		// ���㵱ǰ���ر任���ֵ
		dTemp = pow(b, c * (i - a)) - 1;

		// ����������޸���ֵ
		if (dTemp < 0)
			dTemp = 0.0;
		else if (dTemp > 255)
			dTemp = 255;

		// ��������
		map[i] = int(dTemp + 0.5);
	}

	for (UINT i = 0; i < width * height; i++)
	{	
		for (int j = 0; j < 4; j++)
			tempPixel[i*4+j] = map[ pixel[i*4+j] ];
	}
}


/******************************************************************************
*	����:			�Ҷ���ֵ�任����
*	����:		
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
*		thr			��ֵ
******************************************************************************/
void ThresholdTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, int thr)
{
	// �Ҷ�ӳ���
	BYTE map[256];

	for (int i = 0; i < 256; i++)
	{
		if (i >= thr)
			map[i] = 255;
		else
			map[i] = 0;
	}

	// �任���ֱֵ����ӳ����в���
	for (UINT i = 0; i < width * height; i++)
	{
		int x = pixel[i * 4];

		tempPixel[i*4] = tempPixel[i*4+1] = tempPixel[i*4+2] = map[x];
		tempPixel[i*4+3] = 255;
	}
}

/******************************************************************************
*	����:			�Ҷ����캯��
*	����:		
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		X1,Y1		���Ƶ�1������
*		X2,Y2		���Ƶ�2������
******************************************************************************/
void GrayStretch(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, 
				 float X1, float Y1, float X2, float Y2)
{
	// ӳ���,����256�ֻҶȱ任���ֵ
	BYTE map[256];

	// ������������ʱֵ
	double dTemp;

	for (int x = 0; x < 256; x++)
	{
		// ����ڵ�һ��ֱ����
		if ( x < X1)
		{
			if ( fabs(X1 - 0) > 0.0001 )
				dTemp = (float)Y1 / (float)X1 * x;
			else
				dTemp = 0;
		}
		// ����ڵڶ���ֱ����
		else if ( x <= X2 )
		{
			if ( fabs(X2 - X1) > 0.0001 )
				dTemp = (float)(Y2 - Y1) / (float)(X2 - X1) * (x - X1) + Y1;
			else
				dTemp = x;
		}
		// ����ڵ�����ֱ����
		else
		{
			if ( fabs(Y2 - 255) > 0.0001 )
				dTemp = (float)(255 - Y2) / (float)(255 - X2) * (x - X2) + Y2;
			else
				dTemp = 255;
		}

		// ��������
		map[x] = int(dTemp + 0.5);
	}

	// �任���ֱֵ����ӳ����в���
	for (UINT i = 0; i < width * height; i++)
	{
		int x = pixel[i * 4];

		tempPixel[i*4] = tempPixel[i*4+1] = tempPixel[i*4+2] = map[x];
		tempPixel[i*4+3] = 255;
	}
}

/******************************************************************************
*	����:			�ҶȾ��⺯��
*	����:		
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
******************************************************************************/
void GrayEqualize(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height)
{

	// �Ҷ�ӳ���
	BYTE map[256];
	long lCounts[256];

	memset(lCounts, 0, sizeof(long) * 256);

	// ������Ҷ�ֵ����
	for (UINT i = 0; i < width * height; i++)
	{
		int x = pixel[i * 4];
		lCounts[x]++;
	}

	// ���������е���ʱֵ
	long lTemp;

	for (int i = 0; i < 256; i++)
	{
		lTemp = 0;
		for (int j = 0; j <= i; j++)
			lTemp += lCounts[j];

		map[i] = (BYTE)(lTemp * 255.0f / width / height);
	}

	// �任���ֱֵ����ӳ����в���
	for (UINT i = 0; i < width * height; i++)
	{
		int x = pixel[i * 4];

		tempPixel[i*4] = tempPixel[i*4+1] = tempPixel[i*4+2] = map[x];
		tempPixel[i*4+3] = 255;
	}
}