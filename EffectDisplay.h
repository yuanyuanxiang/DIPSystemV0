#pragma once

// ��Ч��ʾö��
enum EffectDislayType
{
	EDT_SCANDOWN = 0,		// ����ɨ��
	EDT_VSCAN,				// ��ֱ˫��ɨ��
	EDT_MOVERIGHT,			// �����ƶ�
	EDT_HSMOVE,				// ˮƽ˫���ƶ�
	EDT_VBLIND,				// ��ֱ��Ҷ��
	EDT_HBLIND,				// ˮƽ��Ҷ��
	EDT_VRASTER,			// ��ֱդ��
	EDT_HRASTER,			// ˮƽդ��
	EDT_MOSAIC,				// ������
	EDT_RAINDROP,			// ���
	
	EDT_NONE,				// ����Ч
	EDT_RAND				// �����Ч
};

class EffectDisplay
{
public:
	static int	s_nOffsetX;		// ͼ�����ڴ�CDC�е�xƫ����
	static int	s_nOffsetY;		// ͼ�����ڴ�CDC�е�yƫ����
	static int	s_nPicWidth;	// ͼ��Ŀ��
	static int	s_nPicHeight;	// ͼ��ĸ߶�
	static int	s_nCDCWidth;	// �ڴ�CDC�Ŀ��
	static int	s_nCDCHeight;	// �ڴ�CDC�ĸ߶�

	static void DelayTime(DWORD dwMilliseconds);

	static void ScanDownDisplay(CDC* pDC, CDC* pMemDC);		// ����ɨ��
	static void VSScanDisplay(CDC* pDC, CDC* pMemDC);		// ��ֱ˫��ɨ��
	static void MoveRightDisplay(CDC* pDC, CDC* pMemDC);	// �����ƶ�
	static void HSMoveDisplay(CDC* pDC, CDC* pMemDC);		// ˮƽ˫���ƶ�
	static void VBlindDisplay(CDC* pDC, CDC* pMemDC);		// ��ֱ��Ҷ��
	static void HBlindDisplay(CDC* pDC, CDC* pMemDC);		// ˮƽ��Ҷ��
	static void	VRasterDisplay(CDC* pDC, CDC* pMemDC);		// ��ֱդ��
	static void	HRasterDisplay(CDC* pDC, CDC* pMemDC);		// ˮƽդ��
	static void	MosaicDisplay(CDC* pDC, CDC* pMemDC);		// ������
	static void	RaindropDisplay(CDC* pDC, CDC* pMemDC);		// ���
};