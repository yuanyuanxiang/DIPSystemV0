#include "StdAfx.h"
#include "GeoOperator.h"
#include <algorithm>
using namespace std;
#include <cmath>

#include <iostream>
#include <ctime>

/******************************************************************************
*	����:			ƽ�Ʊ任����
*	����:		
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
*		xx			ƽ�Ʊ任��ˮƽƫ����
*		yy			ƽ�Ʊ任�Ĵ�ֱƫ����
*	����ֵ:			�Ƿ�ɹ�
******************************************************************************/
bool MoveTransNormal(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
					 int xx, int yy)
{
	// ���ͼ���С
	int size = width * height * 4;

	tempPixel = new BYTE[size];
	memset(tempPixel, 255, size);

	// ƫ�����Ƿ��ںϷ���Χ��
	if (xx >= width || yy >= height || xx <= -width || yy <= -height)
		return false;

	int		y = 0;							// �任�������������
	BYTE*	copyPixel = NULL;				// ָ��ԭʼͼ������Ҫ�������ص���ʼλ��
	BYTE*	objPixel = NULL;				// ָ�򱻸������ص���ʼλ��
	int		copyWidth = width - abs(xx);	// ��Ҫ����ͼ��Ŀ��

	for (int j = 0; j < height; j++)
	{
		// ָ��ԭʼͼ���j�е���ʼλ��
		copyPixel = pixel + j * width * 4;

		// �������������ص���ʼλ��
		if (xx < 0)
			copyPixel += abs(xx) * 4;

		// ����任�������������
		y = j + yy;

		if (y >= 0 && y < height)
		{
			objPixel = tempPixel + y * width * 4;

			if (xx > 0)
				objPixel += abs(xx) * 4;

			// ����һ��ͼ��
			memcpy(objPixel, copyPixel, copyWidth * 4);
		}
	}

	return true;
}

/******************************************************************************
*	����:			ƽ�Ʊ任����
*	����:		
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
*		xx			ƽ�Ʊ任��ˮƽƫ����
*		yy			ƽ�Ʊ任�Ĵ�ֱƫ����
*		outWidth	[out]���ͼ��Ŀ��
*		outHeight	[out]���ͼ��ĸ߶�
******************************************************************************/
void MoveTransSize(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
				   int xx, int yy, UINT& outWidth, UINT& outHeight)
{
	outWidth = width + abs(xx);
	outHeight = height + abs(yy);

	int size = outWidth * outHeight * 4;

	tempPixel = new BYTE[size];
	memset(tempPixel, 255, size);

	int		x = 0;							// ÿ�и������ĺ�����
	int		y = 0;							// ÿ�и�������������
	BYTE*	copyPixel = NULL;				// ָ��ԭʼͼ������Ҫ�������ص���ʼλ��
	BYTE*	objPixel = NULL;				// ָ�򱻸������ص���ʼλ��

	if (xx > 0)
		x = xx;
	if (yy > 0)
		y = yy;

	for (int j = 0; j < height; j++)
	{
		copyPixel = pixel + j * width * 4;
		objPixel = tempPixel + y * outWidth * 4 + x * 4;
		y++;
		
		memcpy(objPixel, copyPixel, width * 4);
	}
}

/******************************************************************************
*	����:			ˮƽ����任����
*	����:		
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
******************************************************************************/
void HMirrorTrans(BYTE* pixel, BYTE*& tempPixel, int width, int height)
{
	int size = width * height * 4;

	tempPixel = new BYTE[size];
	memset(tempPixel, 255, size);


	// ����һ	ͨ��ӳ���ϵ���б任
	int		x = 0;							// �任������غ�����
	int		y = 0;							// �任�������������
	BYTE*	copyPixel = NULL;				// ָ��ԭʼͼ������Ҫ�������ص���ʼλ��
	BYTE*	objPixel = NULL;				// ָ�򱻸������ص���ʼλ��

	for (int j = 0; j < height; j++)
	{
		y = j;

		for (int i = 0; i < width; i++)
		{
			x = width - i - 1;

			copyPixel = pixel + j * width * 4 + i * 4;
			objPixel = tempPixel + y * width * 4 + x * 4;
			memcpy(objPixel, copyPixel, 4);
		}
	}

/*	// ������	ͨ���������ؽ��б任
	memcpy(tempPixel, pixel, size);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width / 2; j++) 
		{
			swap(tempPixel[j*4 + i*width*4],	 tempPixel[(i+1)*width*4 - j*4 - 4]);
			swap(tempPixel[j*4 + i*width*4 + 1], tempPixel[(i+1)*width*4 - j*4 - 3]);
			swap(tempPixel[j*4 + i*width*4 + 2], tempPixel[(i+1)*width*4 - j*4 - 2]);
			swap(tempPixel[j*4 + i*width*4 + 3], tempPixel[(i+1)*width*4 - j*4 - 1]);
		}
	}*/
}

/******************************************************************************
*	����:			��ֱ����任����
*	����:		
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
******************************************************************************/
void VMirrorTrans(BYTE* pixel, BYTE*& tempPixel, int width, int height)
{
	int size = width * height * 4;

	tempPixel = new BYTE[size];
	memset(tempPixel, 255, size);

	// ����һ	ͨ��ӳ���ϵ���б任
	int		x = 0;							// �任������غ�����
	int		y = 0;							// �任�������������
	BYTE*	copyPixel = NULL;				// ָ��ԭʼͼ������Ҫ�������ص���ʼλ��
	BYTE*	objPixel = NULL;				// ָ�򱻸������ص���ʼλ��

	for (int j = 0; j < height; j++)
	{
		y = height - j - 1;

		for (int i = 0; i < width; i++)
		{
			x = i;

			copyPixel = pixel + j * width * 4 + i * 4;
			objPixel = tempPixel + y * width * 4 + x * 4;
			memcpy(objPixel, copyPixel, 4);
		}
	}

/*	// ������	ͨ���������ؽ��б任
	// ����ԭʼͼ��
	memcpy(tempPixel, pixel, size);

	for(int i = 0; i < width * 4; i+=4) 
	{
		for (int j = 0; j < height / 2; j++) 
		{
			swap(tempPixel[j*width*4 + i],	   tempPixel[(height-j-1)*width*4 + i]);
			swap(tempPixel[j*width*4 + i + 1], tempPixel[(height-j-1)*width*4 + i + 1]);
			swap(tempPixel[j*width*4 + i + 2], tempPixel[(height-j-1)*width*4 + i + 2]);
			swap(tempPixel[j*width*4 + i + 3], tempPixel[(height-j-1)*width*4 + i + 3]);
		}
	}*/
}


/******************************************************************************
*	����:			ͼ���ת�ú���
*	����:		
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
*		outWidth	[out]���ͼ��Ŀ��
*		outHeight	[out]���ͼ��ĸ߶�
******************************************************************************/
void Transpose(BYTE* pixel, BYTE*& tempPixel, int width, int height, UINT& outWidth, UINT& outHeight)
{
	// ת�ú�ͼ��Ŀ�Ⱥ͸߶�
	outWidth = (UINT)height;
	outHeight = (UINT)width;

	int size = outWidth * outHeight * 4;

	tempPixel = new BYTE[size];
	memset(tempPixel, 0, size);

	BYTE*	copyPixel = NULL;	// ָ��ԭʼͼ������Ҫ�������ص���ʼλ��
	BYTE*	objPixel = NULL;	// ָ�򱻸������ص���ʼλ��
	int		x = 0;				// �任������غ�����
	int		y = 0;				// �任�������������
	int		tempJ = 0;			// �洢�м�ֵ,��ߺ����ٶ�

	for (UINT j = 0; j < outHeight; j++)
	{
		x = j;

		// ������ǰ������i�޹ص��м�ֵ
		// ���Դ������ظ�����
		tempJ = j * outWidth * 4;
		for (UINT i = 0; i < outWidth; i++) 
		{
			y = i;
			copyPixel = pixel + y * width * 4 + x * 4;
			objPixel = tempPixel + tempJ + i * 4;
			memcpy(objPixel, copyPixel, 4);
		}
	}
}

/******************************************************************************
*	����:			ͼ������ź���(���ٽ���ֵ��)
*	����:		
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
*		outWidth	[out]���ͼ��Ŀ��
*		outHeight	[out]���ͼ��ĸ߶�
*		fx			ˮƽ����ϵ��
*		fy			��ֱ����ϵ��
******************************************************************************/
void ZoomNormal(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
				UINT& outWidth, UINT& outHeight, double fx, double fy)
{
	// �������ź��ͼ���С
	outWidth = (UINT)(width * fx);
	outHeight = (UINT)(height * fy);
	
	int size = outWidth * outHeight * 4;
	
	tempPixel = new BYTE[size];
	memset(tempPixel, 255, size);

	BYTE*	copyPixel = NULL;	// ָ��ԭʼͼ������Ҫ�������ص���ʼλ��
	BYTE*	objPixel = NULL;	// ָ�򱻸������ص���ʼλ��
	int		x = 0;				// �任������غ�����
	int		y = 0;				// �任�������������
	long	tempY;				// �洢�м�ֵ,��ߺ����ٶ�
	long	tempJ;				// �洢�м�ֵ,��ߺ����ٶ�

	for (UINT j = 0; j < outHeight; j++)
	{
		// ����ٽ����ص�������
		y = (int)(j / fy + 0.5);

		// ��������
		if (y >= height)
			y--;

		// ������i,x�޹ص��м�ֵ
		tempY = y * width * 4;
		tempJ = j * outWidth * 4;

		for (UINT i = 0; i < outWidth; i++) 
		{
			// ����ٽ����صĺ�����
			x = (int)(i / fx + 0.5);

			// ��������
			if (x >= width)
				x--;
			copyPixel = pixel + tempY + x * 4;
			objPixel = tempPixel + tempJ + i * 4;
			memcpy(objPixel, copyPixel, 4);
		}
	}
}


/******************************************************************************
*	����:			ͼ������ź���(˫���Բ�ֵ��)
*	����:		
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
*		outWidth	[out]���ͼ��Ŀ��
*		outHeight	[out]���ͼ��ĸ߶�
*		fx			ˮƽ����ϵ��
*		fy			��ֱ����ϵ��
******************************************************************************/
void ZoomInterpolation(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
					   UINT& outWidth, UINT& outHeight, double fx, double fy)
{
	// �������ź��ͼ���С
	outWidth = (UINT)(width * fx + 0.5);
	outHeight = (UINT)(height * fy + 0.5);

	int size = outWidth * outHeight * 4;

	tempPixel = new BYTE[size];
	memset(tempPixel, 255, size);

	BYTE*	copyPixel = NULL;	// ָ��ԭʼͼ������Ҫ�������ص���ʼλ��
	BYTE*	objPixel = NULL;	// ָ�򱻸������ص���ʼλ��
	double	x = 0.0;			// �任������غ�����
	double	y = 0.0;			// �任�������������
	long	tempJ = 0;			// �洢�м�ֵ,��ߺ����ٶ�

	for (UINT j = 0; j < outHeight; j++)
	{
		y = j / fy;
		tempJ = j * outWidth * 4;

		for (UINT i = 0; i < outWidth; i++) 
		{
			x = i / fx;
			objPixel = tempPixel + tempJ + i * 4;

			// ���뵱ǰ��������ĸ�������
			int	x1, x2, y1, y2;

			x1 = (int)x;
			x2 = x1 + 1;
			y1 = (int)y;
			y2 = y1 + 1;

			// ��Ӧ�ĸ��������ص�ȡֵ
			BYTE clr1, clr2, clr3, clr4;
			double u, v;

			u = x - x1;
			v = y - y1;

			// ����32λ���ɫ��Ҫ�ֱ������ԭɫ
			for (int k = 0; k < 3; k++)
			{
				// ���������ͼ������½�
				if ( (x >= width - 1) && (y >= height - 1) )
				{
					clr1 = pixel[(y1 * width + x1) * 4 + k];	// (x1, y1)
					objPixel[k] = clr1; 
				}
				// ���ͼ�������һ�����
				else if ( x >= width - 1 )
				{
					clr1 = pixel[(y1 * width + x1) * 4 + k];	// (x1, y1)
					clr3 = pixel[(y2 * width + x1) * 4 + k];	// (x1, y2)
					objPixel[k] = (BYTE)(clr1 * (1 - v) + clr3 * v); 
				}			
				// ���ͼ�������һ�����
				else if ( y >= height - 1 )
				{
					clr1 = pixel[(y1 * width + x1) * 4 + k];	// (x1, y1)
					clr2 = pixel[(y1 * width + x2) * 4 + k];	// (x2, y1)
					objPixel[k] = (BYTE)(clr1 * (1 - u) + clr2 * u);
				}
				else
				{
					clr1 = pixel[(y1 * width + x1) * 4 + k];	// (x1, y1)
					clr2 = pixel[(y1 * width + x2) * 4 + k];	// (x2, y1)
					clr3 = pixel[(y2 * width + x1) * 4 + k];	// (x1, y2)
					clr4 = pixel[(y2 * width + x2) * 4 + k];	// (x2, y2)

					double f1, f2;

					f1 = clr1 * (1 - u) + clr2 * u;
					f2 = clr3 * (1 - u) + clr4 * u;
					objPixel[k] = (BYTE)(f1 * (1 - v) + f2 * v);
				}
			}
		}
	}
}

/******************************************************************************
*	����:			ͼ�����ת����(���ٽ���ֵ��)
*	����:		
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
*		outWidth	[out]���ͼ��Ŀ��
*		outHeight	[out]���ͼ��ĸ߶�
*		angle		��ת����,�Ի��ȱ�ʾ
******************************************************************************/
void RotateNormal(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
				  UINT& outWidth, UINT& outHeight, double angle)
{
	// ԭʼͼ���ĸ��ǵ�����
	int srcX1, srcX2, srcX3, srcX4;
	int srcY1, srcY2, srcY3, srcY4;

	srcX1 = 0;
	srcY1 = 0;
	srcX2 = width - 1;
	srcY2 = 0;
	srcX3 = 0;
	srcY3 = height - 1;
	srcX4 = width - 1;
	srcY4 = height - 1;

	// ������ת�Ƕȵ����Һ�����ֵ
	double fSin = sin(angle);
	double fCos = cos(angle);

	// ͼ�񾭹���ת���ĸ��ǵ�����
	double tranX1, tranX2, tranX3, tranX4;
	double tranY1, tranY2, tranY3, tranY4;

	tranX1 = fCos * srcX1 + fSin * srcY1;
	tranY1 = -fSin * srcX1 + fCos * srcY1;
	tranX2 = fCos * srcX2 + fSin * srcY2;
	tranY2 = -fSin * srcX2 + fCos * srcY2;
	tranX3 = fCos * srcX3 + fSin * srcY3;
	tranY3 = -fSin * srcX3 + fCos * srcY3;
	tranX4 = fCos * srcX4 + fSin * srcY4;
	tranY4 = -fSin * srcX4 + fCos * srcY3;

	// ������ת��ͼ��Ĵ�С
	outWidth = (UINT)( max( fabs(tranX4-tranX1), fabs(tranX3-tranX2) ) + 1.5 );
	outHeight = (UINT)( max( fabs(tranY4-tranY1), fabs(tranY3-tranY2) ) + 1.5 );

	tempPixel = new BYTE[outWidth * outHeight * 4];
	memset(tempPixel, 255, outWidth * outHeight * 4);

	// ������������
	double num1 = -0.5*outWidth*fCos - 0.5*outHeight*fSin + 0.5*width;
	double num2 = 0.5*outWidth*fSin - 0.5*outHeight*fCos + 0.5*height;

	BYTE*	copyPixel = NULL;	// ָ��ԭʼͼ������Ҫ�������ص���ʼλ��
	BYTE*	objPixel = NULL;	// ָ�򱻸������ص���ʼλ��
	int		x = 0;				// �任������غ�����
	int		y = 0;				// �任�������������

	for (long j = 0; j < (long)outHeight; j++)
	{
		for (long i = 0; i < (long)outWidth; i++) 
		{
			x = (int)(i * fCos + j * fSin + num1 + 0.5);
			y = (int)(-i * fSin + j * fCos + num2 + 0.5);

			// ��������
			if (x == width)
				x--;
			if (y == height)
				y--;

			copyPixel = pixel + y * width * 4 + x * 4;
			objPixel = tempPixel + j * outWidth * 4 + i * 4;

			if (x >= 0 && x < width && y >= 0 && y < height)
				memcpy(objPixel, copyPixel, 4);
		}
	}
}

/******************************************************************************
*	����:			ͼ�����ת����(˫���Բ�ֵ��)
*	����:		
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
*		outWidth	[out]���ͼ��Ŀ��
*		outHeight	[out]���ͼ��ĸ߶�
*		angle		��ת����,�Ի��ȱ�ʾ
******************************************************************************/
void RotateInterpolation(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
						 UINT& outWidth, UINT& outHeight, double angle)
{
	// ԭʼͼ���ĸ��ǵ�����
	int srcX1, srcX2, srcX3, srcX4;
	int srcY1, srcY2, srcY3, srcY4;

	srcX1 = 0;
	srcY1 = 0;
	srcX2 = width - 1;
	srcY2 = 0;
	srcX3 = 0;
	srcY3 = height - 1;
	srcX4 = width - 1;
	srcY4 = height - 1;

	// ������ת�Ƕȵ����Һ�����ֵ
	double fSin = sin(angle);
	double fCos = cos(angle);

	// ͼ�񾭹���ת���ĸ��ǵ�����
	double tranX1, tranX2, tranX3, tranX4;
	double tranY1, tranY2, tranY3, tranY4;

	tranX1 = fCos * srcX1 + fSin * srcY1;
	tranY1 = -fSin * srcX1 + fCos * srcY1;
	tranX2 = fCos * srcX2 + fSin * srcY2;
	tranY2 = -fSin * srcX2 + fCos * srcY2;
	tranX3 = fCos * srcX3 + fSin * srcY3;
	tranY3 = -fSin * srcX3 + fCos * srcY3;
	tranX4 = fCos * srcX4 + fSin * srcY4;
	tranY4 = -fSin * srcX4 + fCos * srcY3;

	// ������ת��ͼ��Ĵ�С
	outWidth = (UINT)( max( fabs(tranX4-tranX1), fabs(tranX3-tranX2) ) + 1.5 );
	outHeight = (UINT)( max( fabs(tranY4-tranY1), fabs(tranY3-tranY2) ) + 1.5 );

	tempPixel = new BYTE[outWidth * outHeight * 4];
	memset(tempPixel, 255, outWidth * outHeight * 4);

	//�������(2.9)�е��������������������Ժ�ÿ�ζ�������

	double num1 = -0.5*outWidth*fCos - 0.5*outHeight*fSin + 0.5*width;
	double num2 = 0.5*outWidth*fSin - 0.5*outHeight*fCos + 0.5*height;

	BYTE*	copyPixel = NULL;	// ָ��ԭʼͼ������Ҫ�������ص���ʼλ��
	BYTE*	objPixel = NULL;	// ָ�򱻸������ص���ʼλ��
	double	x = 0.0;			// �任������غ�����
	double	y = 0.0;			// �任�������������
	long	tempJ = 0;			// �洢�м�ֵ,��ߺ����ٶ�

	for (long j = 0; j < (long)outHeight; j++)
	{
		tempJ = j * outWidth * 4;

		for (long i = 0; i < (long)outWidth; i++) 
		{
			x = i * fCos + j * fSin + num1;
			y = -i * fSin + j * fCos + num2;

			// �����������һ������
			if (x > width || x < 0 || y > height || y < 0)
				continue;

			objPixel = tempPixel + tempJ + i * 4;

			// ���뵱ǰ��������ĸ�������
			int	x1, x2, y1, y2;

			x1 = (int)x;
			x2 = x1 + 1;
			y1 = (int)y;
			y2 = y1 + 1;

			// ��Ӧ�ĸ��������ص�ȡֵ
			BYTE clr1, clr2, clr3, clr4;
			double u, v;

			u = x - x1;
			v = y - y1;

			// ����32λ���ɫ��Ҫ�ֱ������ԭɫ
			for (int k = 0; k < 3; k++)
			{
				// ���������ͼ������½�
				if ( (x >= width - 1) && (y >= height - 1) )
				{
					clr1 = pixel[(y1 * width + x1) * 4 + k];	// (x1, y1)
					objPixel[k] = clr1; 
				}
				// ���ͼ�������һ�����
				else if ( x >= width - 1 )
				{
					clr1 = pixel[(y1 * width + x1) * 4 + k];	// (x1, y1)
					clr3 = pixel[(y2 * width + x1) * 4 + k];	// (x1, y2)
					objPixel[k] = (BYTE)(clr1 * (1 - v) + clr3 * v); 
				}			
				// ���ͼ�������һ�����
				else if ( y >= height - 1 )
				{
					clr1 = pixel[(y1 * width + x1) * 4 + k];	// (x1, y1)
					clr2 = pixel[(y1 * width + x2) * 4 + k];	// (x2, y1)
					objPixel[k] = (BYTE)(clr1 * (1 - u) + clr2 * u);
				}
				else
				{
					clr1 = pixel[(y1 * width + x1) * 4 + k];	// (x1, y1)
					clr2 = pixel[(y1 * width + x2) * 4 + k];	// (x2, y1)
					clr3 = pixel[(y2 * width + x1) * 4 + k];	// (x1, y2)
					clr4 = pixel[(y2 * width + x2) * 4 + k];	// (x2, y2)

					double f1, f2;

					f1 = clr1 * (1 - u) + clr2 * u;
					f2 = clr3 * (1 - u) + clr4 * u;
					objPixel[k] = (BYTE)(f1 * (1 - v) + f2 * v);
				}
			}
		}
	}
}