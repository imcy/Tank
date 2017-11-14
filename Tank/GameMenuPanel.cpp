#include "stdafx.h"
#include "GameMenuPanel.h"
#include "Game.h"

CGameMenuPanel::CGameMenuPanel()
{
	// ����ͼ
	m_imgBackground = Image::FromFile(_T("./images/menu_background.png"));

}


CGameMenuPanel::~CGameMenuPanel()
{

}
void CGameMenuPanel::getImage()
{
	if (m_imgBackground == NULL) {
		// ����ͼ
		m_imgBackground = Image::FromFile(_T("./images/menu_background.png"));

		// �˵���һ : �˻���ս
		m_menuItems[0].index = 0;
		for (int i = 0; i < 2; ++i) {
			TCHAR path[MAX_PATH];
			_stprintf_s(path, _T("./images/menu_1player_%d.png"), i);               // ��ʽ���ļ���
			auto imgPtr = Image::FromFile(path);                            // ����ͼƬ
			RectF rc(0, 300, static_cast<float>(imgPtr->GetWidth()),
				static_cast<float>(imgPtr->GetHeight()));
			m_menuItems[0].vImgInfoPtr.push_back(make_tuple(imgPtr, rc)); // ͼƬ����������
		}

		// �˵���һ : ˫�˶�ս
		m_menuItems[1].index = 0;
		for (int i = 0; i < 2; ++i) {
			TCHAR path[MAX_PATH];
			_stprintf_s(path, _T("./images/menu_2plyaer_%d.png"), i);              // ��ʽ���ļ���
			auto imgPtr = Image::FromFile(path);                           // ����ͼƬ
			RectF rc(400, 300, static_cast<float>(imgPtr->GetWidth())
				, static_cast<float>(imgPtr->GetHeight()));
			m_menuItems[1].vImgInfoPtr.push_back(make_tuple(imgPtr, rc)); // ͼƬ����������
		}
	}
}

void CGameMenuPanel::Draw(Graphics & gh) const
{
	gh.DrawImage(m_imgBackground, m_rect);                              // ��������ͼƬ
	// ���Ӳ˵�
	for (auto menuItem : m_menuItems) {
		auto img = get<0>(menuItem.vImgInfoPtr[menuItem.index]);      // ��ȡ�˵����ͼƬ
		auto rect = get<1>(menuItem.vImgInfoPtr[menuItem.index]);     // ��ȡ�˵���Ĵ�С
		gh.DrawImage(img, rect);
	}
}

// ���� ����ƶ��¼�
void CGameMenuPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	PointF pt(static_cast<float>(point.x), static_cast<float>(point.y));
	// ���Ӳ˵�
	for (auto &menuItem : m_menuItems) {
		auto img = get<0>(menuItem.vImgInfoPtr[menuItem.index]); // ��ȡ�˵����ͼƬ
		auto rect = get<1>(menuItem.vImgInfoPtr[menuItem.index]);// ��ȡ�˵���Ĵ�С
		if (rect.Contains(pt)) {                                     // �ж��Ƿ������ǰ���λ��
			menuItem.index = 1;                                     // ��������ʾ��1��ͼƬ                                       
		}
		else {
			menuItem.index = 0;                                     // ����������ʾ��0��ͼƬ
		}
	}
}

// ���� ������̧���¼�
void CGameMenuPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	PointF pt(static_cast<float>(point.x), static_cast<float>(point.y));
	{
		auto &menuItem = m_menuItems[0];							// ��ȡ��0��ͼƬ
		auto img = get<0>(menuItem.vImgInfoPtr[menuItem.index]);	// ��ȡͼƬָ��
		auto rect = get<1>(menuItem.vImgInfoPtr[menuItem.index]);	// ��ȡͼƬ��С
		if (rect.Contains(pt)) {									// �ж��Ƿ����ͼƬ
			// �˻���ս����ʱ������
			m_pParent->SetStep(CGame::EGameTypeOne2BotMenu);
			return;
		}
	}
	{
		auto &menuItem = m_menuItems[1];							// ��ȡ��0��ͼƬ
		auto img = get<0>(menuItem.vImgInfoPtr[menuItem.index]);	// ��ȡͼƬָ��
		auto rect = get<1>(menuItem.vImgInfoPtr[menuItem.index]);	// ��ȡͼƬ��С
		if (rect.Contains(pt)) {									// �ж��Ƿ����ͼƬ
																	// ˫�˶�ս����ʱ������
			m_pParent->SetStep(CGame::EGameTypeOne2OneMenu);
			return;
		}
	}
}


