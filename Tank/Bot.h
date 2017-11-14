#pragma once
#include "TankEntry.h"

class CBot : public CTankEntry 
{
public:
	CBot();
	CBot(float x, float y, PCTSTR szImgName);
	// ̹��ͼƬ ����ʱ
	Image *m_imgTank;
	// ̹��ͼƬ ��ըʱ
	Image *m_imgExplosion[12];
	mutable int m_explosionIndex{ 0 };
	// �ӵ�
	std::list<CBullet> m_lstBullets;
	
	PCTSTR imgName;
	float xn;
	float yn;

	void getImage();
	virtual ~CBot();
};

