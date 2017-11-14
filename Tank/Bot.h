#pragma once
#include "TankEntry.h"

class CBot : public CTankEntry 
{
public:
	CBot();
	CBot(float x, float y, PCTSTR szImgName);
	// 坦克图片 正常时
	Image *m_imgTank;
	// 坦克图片 爆炸时
	Image *m_imgExplosion[12];
	mutable int m_explosionIndex{ 0 };
	// 子弹
	std::list<CBullet> m_lstBullets;
	
	PCTSTR imgName;
	float xn;
	float yn;

	void getImage();
	virtual ~CBot();
};

