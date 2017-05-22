#include "StdAfx.h"
#include "improve.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#pragma warning ( disable : 4018)

int GetAsh(BYTE** imageBuf, int x, int y)
{
	int clr;
	clr = (imageBuf[y][x*4] + imageBuf[y][x*4+1]
			+imageBuf[y][x*4+2]) / 3;
	return clr;
}

/********************************************************
* �����δ洢������ת��Ϊ��ά������ʽ
* ����: image ���δ洢�����أ� width,height ͼ��ĳ���
********************************************************/
BYTE** CreatImage(BYTE* image, unsigned int width, unsigned int height, int bt=4)
{
	BYTE** imageBuf = (BYTE**)malloc(sizeof(BYTE*)*(height));
	for(int y=0; y<height; y++)
	{
		//ʹimageBuf��ÿ��ָ��ֱ�ָ�����±��ʾ���е����׵�ַ
		imageBuf[y] = image+y*width*bt; 
	}
	return imageBuf;
}

/**************************************************
* ����: �趨ָ��λ�õ����ػҶ�
* ����: imageBufΪĿ��ͼ�� x,yΪҪ�趨���ص�����
**************************************************/
void SetPixelXY(BYTE** imageBuf1, int x, int y, int a)
{
	imageBuf1[y][x*4] = a;
	imageBuf1[y][x*4+1] = a;
	imageBuf1[y][x*4+2] = a;
	imageBuf1[y][x*4+3]= 255;
}

/**************************************************
* ����: ʹ��ģ��Բ�ɫͼ�����������
* ����: imageBufΪĿ��ͼ�� w��hΪͼ���С
*       templtΪģ�� twΪ�����С 
*		x,yΪҪȡ�����ص�����
*       cnΪ��ɫ������� 0Ϊ��ɫ 1Ϊ��ɫ 2Ϊ��ɫ
**************************************************/
int TempltExcuteCl(BYTE** imageBuf0, int w, int h, int* templt, int tw, int x, int y, int cn)
{
	int i,j;                        //ѭ������
	int m=0;                      //������ż�Ȩ��
	int px,py;   
	//���ζ�������ÿ�����ؽ�������
	for(i=0; i<tw; i++)
	{
		for(j=0; j<tw; j++)
		{
			//�����Ӧģ����λ�õ�������ԭͼ���е�λ��
			py=y-tw/2+i;
			px=x-tw/2+j;
			//��Ȩ���
			m+=imageBuf0[py][px*4+cn] * templt[i*tw+j];
		}
	}
	return m;                     //���ؽ��
}

/*****************************************************************
* ����: ʹ��ģ��ԻҶ�ͼ����������� 
* ����: imageBufΪĿ��ͼ�� w��hΪͼ���С
*       templtΪģ�� twΪ�����С 
*		x,yΪҪȡ�����ص�����
******************************************************************/
int TempltExcuteAsh(BYTE** imageBuf0, int w, int h, 
					int* templt, int tw, int x, int y)
{
	int i,j;                        //ѭ������
	int m=0;                      //������ż�Ȩ��
	int px,py;   
	//���ζ�������ÿ�����ؽ�������
	for(i=0; i<tw; i++)
	{
		for(j=0; j<tw; j++)
		{
			//�����Ӧģ����λ�õ�������ԭͼ���е�λ��
			py=y-tw/2+i;
			px=x-tw/2+j;
			//��Ȩ���
			m+=GetAsh(imageBuf0,px,py) * templt[i*tw+j];
		}
	}
	return m;                     //���ؽ��
}

/******************************************************************
* ����: �Ҷ�ͼ��ļ�ƽ������
* ����: image0Ϊԭͼ�Σ�image1Ϊƽ�������
*		w��hΪͼ��Ŀ�͸�
*       sizeΪ����ƽ��������߳�
******************************************************************/
void SmoothAsh(BYTE* image0, BYTE* image1, 
			   unsigned int w, unsigned int h, unsigned int size)
{
	//��ͼ��ת��Ϊ������ʽ
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//����ģ��
	int* templt;
	int x,y;
	int a;
	int scale;

	//���������С�趨ģ��
	templt = new int[size * size];
	for(x=0; x<size*size; x++)
	{
		templt[x]=1;
	}
	//�趨˥������
	scale = size*size;

	//���ζ�ԭͼ���ÿ�����ؽ��д���
	for(y=size/2; y<h-size/2; y++)
	{
		for(x=size/2; x<w-size/2; x++)
		{
			a=TempltExcuteAsh(imageBuf0,w,h,templt,size,x,y);
			a/= scale;
			//���޴���
			a = a>255?255:a;    
			a = a<0?0:a;
			SetPixelXY(imageBuf1,x,y,a);
		}
	}

	//�����ڴ�
	delete[] templt;
	free(imageBuf0);
	free(imageBuf1);
}

/******************************************************************
* ����: ��ɫͼ��ļ�ƽ������
* ����: image0Ϊԭͼ�Σ�image1ƽ�������
*		w��hΪͼ��Ŀ�͸�
*       sizeΪ����ƽ��������߳�
******************************************************************/
void SmoothCl(BYTE* image0, BYTE* image1, 
			  unsigned int w, unsigned int h, unsigned int size)
{
	//��ͼ��ת��Ϊ������ʽ
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//����ģ��
	int* templt;
	int x,y,c;
	int a;
	int scale;

	//���������С�趨ģ��
	templt = new int[size * size];
	for(x=0; x<size*size; x++)
	{
		templt[x]=1;
	}
	//�趨˥������
	scale = size*size;

	//���ζ�ԭͼ���ÿ�����ؽ��д���
	for(y=size/2; y<h-size/2; y++)
	{
		for(x=size/2; x<w-size/2; x++)
		{
			//���ζԲ�ɫͼ������������ҶȽ��д���
			for(c=0; c<3; c++)
			{
				a=TempltExcuteCl(imageBuf0,w,h,templt,size,x,y,c);
				a/= scale;
				//���޴���
				a = a>255?255:a;    
				a = a<0?0:a;
				imageBuf1[y][x*4+c]=a;
			}
		}
	}

	//�����ڴ�
	delete[] templt;
	free(imageBuf0);
	free(imageBuf1);
}

/******************************************************************
* ����: ��ɫͼ��ĸ�˹ƽ������
* ����: image0Ϊԭͼ�Σ�image1ƽ�������
*		w��hΪͼ��Ŀ�͸�
******************************************************************/
void SmoothGaussCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h)
{
	//��ͼ��ת��Ϊ������ʽ
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//�趨ģ��
	int templt[9]={1,2,1,2,4,2,1,2,1};
	int x,y,c;
	int a;
	int scale;

	//�趨˥������
	scale = 16;

	//���ζ�ԭͼ���ÿ�����ؽ��д���
	for(y=1; y<h-1; y++)
		for(x=1; x<w-1; x++)
			for(c=0; c<3; c++)
			{
				//���ø�˹ģ���������д���
				a=TempltExcuteCl(imageBuf0,w,h,templt,3,x,y,c);
				a/= scale;
				//���޴���
				a = a>255?255:a;    
				a = a<0?0:a;
				imageBuf1[y][x*4+c]=a;
			}

	//�����ڴ�
	free(imageBuf0);
	free(imageBuf1);
} 

/**************************************************
* ����: ʹ��ģ��ԻҶ�ͼ��������ȡ��ֵ
* ����: imageBufΪĿ��ͼ�� w��hΪͼ���С
*       templtΪģ�� twΪ�����С 
*		x,yΪ��ǰ���������������ص�����
**************************************************/
int MedianValueAsh(BYTE** imageBuf0, int w, int h, int* templt, int tw, int x, int y)
{
	int i,j,k;                   
	int px,py,c;
	int* value;                //��������Ҫ�������ֵ
	int count;                 //��������������ڵ���������
	value= new int[tw*tw]; 
	k=0;
	//�Ӳ���������ȡ�����ػҶ�
	for(i=0; i<tw; i++)
	{
		for(j=0; j<tw; j++)
		{
			py=y-tw/2+i;
			px=x-tw/2+j;
			//���������λ�ڲ���������
			if(templt[i*tw+j]>0)
			{
				//�������ػҶ�
				value[k]=GetAsh(imageBuf0,px,py);
				k++;
			}
		}
	}
	//��¼��������ظ���
	count=k;
	//�Ա�������ػҶ����ݽ�������
	for(i=0; i<count-1; i++)
	{
		k=i;
		for(j=i+1; j<count; j++)
			if(value[j]<value[k]) k=j;
		c=value[i];
		value[i]=value[k];
		value[k]=c;
	}
	//������ֵ
	c=value[count/2];
	//�����ڴ�
	delete[] value;
	return c;
}

/******************************************************************
* ����: �Ҷ�ͼ�����ֵ�˲�ƽ������
* ����: image0Ϊԭͼ�Σ�image1ƽ�������
*		w��hΪͼ��Ŀ�͸�
*       sizeΪ����ƽ��������߳�
******************************************************************/
void SmoothMedianAsh(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, unsigned int size)
{
	//��ͼ��ת��Ϊ������ʽ
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//�趨ģ��
	int* templt;
	int x,y;
	int a;
	int scale;

	//���������С�趨ģ��
	templt = new int[size * size];
	for(x=0; x<size*size; x++) templt[x]=1;

	//�趨˥������
	scale = 1;

	//���ζ�ԭͼ���ÿ�����ؽ��д���
	for(y=size/2; y<h-size/2; y++)
		for(x=size/2; x<w-size/2; x++)
		{
			//ȡ�������������ػҶȵ���ֵ
			a=MedianValueAsh(imageBuf0,w,h,templt,size,x,y);
			a/= scale;
			//���޴���
			a = a>255?255:a;    
			a = a<0?0:a;
			SetPixelXY(imageBuf1,x,y,a);
		}

		//�����ڴ�
		delete[] templt;
		free(imageBuf0);
		free(imageBuf1);
}

/**************************************************
* ����: ʹ��ģ��Բ�ɫͼ��������ȡ��ֵ
* ����: imageBufΪĿ��ͼ�� w��hΪͼ���С
*       templtΪģ�� twΪ�����С 
*		x,yΪ��ǰ���������������ص�����
*       cnΪ��ɫ������� 0Ϊ��ɫ 1Ϊ��ɫ 2Ϊ��ɫ
**************************************************/
int MedianValueCl(BYTE** imageBuf0, int w, int h, int* templt, int tw, int x, int y, int cn)
{
	int i,j,k;                   
	int px,py,c;
	int* value;                //��������Ҫ�������ֵ
	int count;                 //��������������ڵ���������
	value = new int[tw*tw]; 
	k=0;
	//�Ӳ���������ȡ�����ػҶ�
	for(i=0; i<tw; i++)
	{
		for(j=0; j<tw; j++)
		{
			py=y-tw/2+i;
			px=x-tw/2+j;
			//���������λ�ڲ���������
			if(templt[i*tw+j]>0)
			{
				//�������ػҶ�
				value[k]=imageBuf0[py][px*4+cn];
				k++;
			}
		}
	}

	count = k;

	vector<int> nums(tw*tw);
	
	for (int i = 0; i < tw*tw; i++)
		nums[i] = value[i];

	//�Ա�������ػҶ����ݽ�������
	sort(nums.begin(), nums.end());

	//������ֵ
	c=nums[count/2];
	//�����ڴ�
	delete[] value;
	return c;
}

/******************************************************************
* ����: ��ɫͼ�����ֵ�˲�ƽ������
* ����: image0Ϊԭͼ�Σ�image1ƽ�������
*		w��hΪͼ��Ŀ�͸�
*       sizeΪ����ƽ��������߳�
******************************************************************/
void SmoothMedianCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, unsigned int size)
{
	//��ͼ��ת��Ϊ������ʽ
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//�趨ģ��
	int* templt;
	int x,y,c;
	int a;
	int scale;

	//���������С�趨ģ��
	templt = new int[size * size];

	for(x=0; x<size*size; x++)
	{
		templt[x]=1;
	}
	//�趨˥������
	scale = 1;

	//���ζ�ԭͼ���ÿ�����ؽ��д���
	for(y=size/2; y<h-size/2; y++)
	{
		for(x=size/2; x<w-size/2; x++)
		{
			for(c=0; c<3; c++)
			{
				//ȡ�������������ػҶȵ���ֵ
				a=MedianValueCl(imageBuf0,w,h,templt,size,x,y,c);
				a/= scale;
				//���޴���
				a = a>255?255:a;    
				a = a<0?0:a;
				imageBuf1[y][x*4+c]=a;
			}
		}
	}
	//�����ڴ�
	delete[] templt;
	free(imageBuf0);
	free(imageBuf1);
}


/******************************************************************
* ����: �Ҷ�ͼ���������˹�񻯴���scale = 3��
* ����: image0Ϊԭͼ�Σ�image1�񻯽����
*		w��hΪͼ��Ŀ�͸�
******************************************************************/
void SharpLaplacianAsh(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h)
{
	//��ͼ��ת��Ϊ������ʽ
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//�趨ģ��
	int templt[9]={-1,-1,-1,-1,8,-1,-1,-1,-1};
	int x,y;
	int a;
	int scale;

	scale = 3;//�趨˥������

	//���ζ�ԭͼ���ÿ�����ؽ��д���
	for(y=1; y<h-1; y++)
		for(x=1; x<w-1; x++)
		{
			//����������˹ģ���������д���
			a=TempltExcuteAsh(imageBuf0,w,h,templt,3,x,y);
			a/= scale;
			//���������ؽ�����ǿ
			a=GetAsh(imageBuf0,x,y)+a;
			//���޴���
			a = a>255?255:a;    
			a = a<0?0:a;
			SetPixelXY(imageBuf1,x,y,a);
		}

		//�����ڴ�
		free(imageBuf0);
		free(imageBuf1);
}

/******************************************************************
* ����: ��ɫͼ���������˹�񻯴���scale = 3��
* ����: image0Ϊԭͼ�Σ�image1�񻯽����
*		w��hΪͼ��Ŀ�͸�
******************************************************************/
void SharpLaplacianCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h)
{
	//��ͼ��ת��Ϊ������ʽ
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//�趨ģ��
	int templt[9]={-1,-1,-1,-1,8,-1,-1,-1,-1};
	int x,y,c;
	int a;
	int scale;

	//�趨˥������
	scale = 3;

	//���ζ�ԭͼ���ÿ�����ؽ��д���
	for(y=1; y<h-1; y++)
	{
		for(x=1; x<w-1; x++)
		{
			for(c=0; c<3; c++)
			{
				//����������˹ģ���������д���
				a=TempltExcuteCl(imageBuf0,w,h,templt,3,x,y,c);
				a/= scale;
				//���������ؽ�����ǿ
				a=imageBuf0[y][x*4+c]+a;
				//���޴���
				a = a>255?255:a;    
				a = a<0?0:a;
				imageBuf1[y][x*4+c]=a;
			}
		}
	}
	//�����ڴ�
	free(imageBuf0);
	free(imageBuf1);
}

void SetPixel(BYTE* image1, unsigned int n, int a)
{
	image1[n] = a;
	image1[n+1] = a;
	image1[n+2] = a;
	image1[n+3] = 255;
}     


/***********************************************************************
* Sobel��Ե���  ��scale=0.5��
* ����: image0Ϊԭͼ�Σ�image1Ϊ��Ե�������w��hΪͼ��Ŀ�͸�
* ��typeΪtrueʱ����ֽ��ȡˮƽ�ʹ�ֱ�������нϴ��ߣ�����ȡƽ��ֵ
************************************************************************/
void SideSobel(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, bool type)
{
	int x, y, a, aHr, aHg, aHb, aVr, aVg, aVb, aH, aV;
	long n;
	double scale = 0.5;
	//���δ���ÿ������
	for(y = 1; y < h-1; y++)
		for(x = 1; x < w-1; x++)
		{
			//�������ص�ƫ��λ��
			n = (y*w+x)*4;
			//�����ɫ����ˮƽ�ҶȲ�
			aHr = abs( (image0[n-w*4-4]+image0[n-4]*2+image0[n+w*4-4])
				- (image0[n-w*4+4]+image0[n+4]*2+image0[n+w*4+4]) );
			//�����ɫ������ֱ�ҶȲ�
			aVr = abs( (image0[n-w*4-4]+image0[n-w*4]*2+image0[n-w*4+4])
				- (image0[n+w*4-4]+image0[n+w*4]*2+image0[n+w*4+4]) );
			//������ɫ����ˮƽ�ҶȲ�
			aHg = abs( (image0[n-w*4-4+1]+image0[n-4+1]*2+image0[n+w*4-4+1])
				- (image0[n-w*4+4+1]+image0[n+4+1]*2+image0[n+w*4+4+1]) );
			//������ɫ������ֱ�ҶȲ�
			aVg = abs( (image0[n-w*4-4+1]+image0[n-w*4+1]*2+image0[n-w*4+4+1])
				- (image0[n+w*4-4+1]+image0[n+w*4+1]*2+image0[n+w*4+4+1]) );
			//������ɫ����ˮƽ�ҶȲ�
			aHb = abs( (image0[n-w*4-4+2]+image0[n-4+2]*2+image0[n+w*4-4+2])
				- (image0[n-w*4+4+2]+image0[n+4+2]*2+image0[n+w*4+4+2]) );
			//������ɫ������ֱ�ҶȲ�
			aVb = abs( (image0[n-w*4-4+2]+image0[n-w*4+2]*2+image0[n-w*4+4+2])
				- (image0[n+w*4-4+2]+image0[n+w*4+2]*2+image0[n+w*4+4+2]) );

			//����ˮƽ�ۺϻҶȲ�
			aH = aHr + aHg + aHb;
			//���㴹ֱ�ۺϻҶȲ�
			aV = aVr + aVg + aVb;

			if(type)
			{
				//ȡˮƽ�ʹ�ֱ�������нϴ���
				if(aH > aV) a = aH;
				else a = aV;
			}
			else
			{
				//ȡˮƽ�ʹ�ֱ�����ֵ�ƽ��ֵ
				a = (aH + aV)/2;
			}
			a = int(a*scale);
			a = a>255?255:a;
			//���ɱ�Եɨ����
			SetPixel(image1,n,a);
		}
}

/******************************************************************
* ����: ��ͼ����ж�ֵ������
* ����: image0Ϊԭͼ�Σ�image1Ϊ����Ľ��ͼ��
*		w��hΪͼ��Ŀ�͸�
*       KΪ��ֵ
******************************************************************/
void ToTwoValue(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, int K)
{
	//��ͼ��ת��Ϊ������ʽ
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	int x,y;

	//���ζ�ԭͼ���ÿ�����ؽ��д���
	for(y=0; y<h; y++)
		for(x=0; x<w; x++)
		{
			//�����ǰ���Ѿ�Ϊ���� ���ڽ��ͼ���ú�ɫ���
			if( GetAsh(imageBuf0,x,y) >=K )
			{
				SetPixelXY (imageBuf1,x,y,255 );
			}
			//�����ǰ�㲻�ǵ��� ���ڽ��ͼ���ð�ɫ���
			else
			{
				SetPixelXY(imageBuf1,x,y,0);
			}
		}

	//�����ڴ�
	free(imageBuf0);
	free(imageBuf1);
}

/******************************************************************
* ����: ������ͼ����м�������
* ����: image0Ϊԭͼ�Σ�image1ΪҪ��ȥ��ͼ��
*		w��hΪͼ��Ŀ�͸�
******************************************************************/
void Subtract(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h)
{
	//��ͼ��ת��Ϊ������ʽ
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	int x,y;
	int a;

	//���ζ�ԭͼ���ÿ�����ؽ��д���
	for(y=0; y<h; y++)
		for(x=0; x<w; x++)
		{
			//ȡ��ԭͼ��Ҷ�
			a=GetAsh(imageBuf0,x,y);
			//���м�������
			a=a-GetAsh(imageBuf1,x,y);
			//���޴���
			a = a>255?255:a;    
			a = a<0?0:a;
			SetPixelXY(imageBuf1,x,y,a);
		}

		//�����ڴ�
		free(imageBuf0);
		free(imageBuf1);
}

/******************************************************************
* ����: ������ͼ����мӷ�����
* ����: image0Ϊԭͼ�Σ�image1ΪҪ��ȥ��ͼ��
*		w��hΪͼ��Ŀ�͸�
******************************************************************/
void AshAdd(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h)
{
	//��ͼ��ת��Ϊ������ʽ
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	int x,y;
	int a;

	//���ζ�ԭͼ���ÿ�����ؽ��д���
	for(y=0; y<h; y++)
		for(x=0; x<w; x++)
		{
			//ȡ��ԭͼ��Ҷ�
			a=GetAsh(imageBuf0,x,y);
			//���м�������
			a=a+GetAsh(imageBuf1,x,y);
			//���޴���
			a = a>255?255:a;    
			a = a<0?0:a;
			SetPixelXY(imageBuf1,x,y,a);
		}

		//�����ڴ�
		free(imageBuf0);
		free(imageBuf1);
}