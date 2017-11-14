#pragma once
#include "GameEntry.h"
#include "GameEntryMoveable.h"

class CGame;
class CGameMenuPanel2 : CGameEntryMoveable
{
public:
    CGameMenuPanel2();
    ~CGameMenuPanel2();

    CGame *m_pParent{ nullptr };
	void getImage();								//���캯���л�ȡͼƬ�����⣬���Ե���дһ����ȡͼƬ�ĺ���
    // ��
    virtual void Draw(Gdiplus::Graphics &gh) const;

    // ���� ����ƶ��¼�
    void OnMouseMove(UINT nFlags, CPoint point);

    // ���� ������̧���¼�
    void OnLButtonUp(UINT nFlags, CPoint point);

    // �˵� ͼƬ
    Image *m_img{nullptr};

    // ��ͼ״̬
    bool m_bNormal{true};
};
