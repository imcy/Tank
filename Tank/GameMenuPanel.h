#pragma once
/*选择菜单的显示和响应*/
#include "GameEntryMoveable.h"
#include <tuple>
#include <vector>
using namespace std;

class CGame;
class CGameMenuPanel :
	public CGameEntryMoveable
{
public:
	CGameMenuPanel();
	~CGameMenuPanel();
	// 存放一个CGame类的指针在本中
	void SetParent(CGame *g)
	{
		m_pParent = g;
	};

	CGame *m_pParent{ nullptr };                   // 存放一个CGame类的指针在本中

	virtual void Draw(Graphics &gh) const;        // 画自己

	RectF m_rect{ 0, 0, 800, 600 };                  // 自己的范围

	void OnMouseMove(UINT nFlags, CPoint point);  // 处理 鼠标移动事件

	void OnLButtonUp(UINT nFlags, CPoint point);  // 处理 鼠标左键抬起事件

	Image *m_imgBackground;                         // 背景图
	void getImage();								//构造函数中获取图片有问题，所以单独写一个获取图片的函数
	struct {                                          // 菜单项
		vector<tuple<Image *, RectF>> vImgInfoPtr;
		int index;
	} m_menuItems[2];
};

