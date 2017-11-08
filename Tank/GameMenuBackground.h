#pragma once
#include "GameEntryMoveable.h"
class CGame;
class CGameMenuBackground :
	public CGameEntryMoveable
{
public:
	CGameMenuBackground();
	~CGameMenuBackground();
	void SetParent(CGame *g)
	{
		m_pParent = g;
	};

	CGame *m_pParent{ nullptr };

	virtual void Draw(Graphics &gh) const; 						// 画自己的函数

	RectF m_rect{ 0, 0, 800, 600 };           						// 自己的范围

	Gdiplus::Image *m_imgBackground;                  						// 背景图
	void getImage();
};

