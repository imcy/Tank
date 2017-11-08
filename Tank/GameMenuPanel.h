#pragma once
/*ѡ��˵�����ʾ����Ӧ*/
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
	// ���һ��CGame���ָ���ڱ���
	void SetParent(CGame *g)
	{
		m_pParent = g;
	};

	CGame *m_pParent{ nullptr };                   // ���һ��CGame���ָ���ڱ���

	virtual void Draw(Graphics &gh) const;        // ���Լ�

	RectF m_rect{ 0, 0, 800, 600 };                  // �Լ��ķ�Χ

	void OnMouseMove(UINT nFlags, CPoint point);  // ���� ����ƶ��¼�

	void OnLButtonUp(UINT nFlags, CPoint point);  // ���� ������̧���¼�

	Image *m_imgBackground;                         // ����ͼ
	void getImage();								//���캯���л�ȡͼƬ�����⣬���Ե���дһ����ȡͼƬ�ĺ���
	struct {                                          // �˵���
		vector<tuple<Image *, RectF>> vImgInfoPtr;
		int index;
	} m_menuItems[2];
};

