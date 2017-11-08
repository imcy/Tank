#include "stdafx.h"
#include "Game.h"
#include <algorithm>

CGame::CGame()
{
	GameInitOne2Bot(); //��ʼ���˻���ս��Ϸ
}


CGame::~CGame()
{
}

void CGame::SetHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}

bool CGame::EnterFrame(DWORD dwTime) //��Ϸ֡����
{
	GameRunDraw(); //ÿ��֡�������ͼƬ
	GameRunLogic();
	return false;
}

void CGame::OnMouseMove(UINT nFlags, CPoint point)
{
	m_menuSelect.getImage();
	m_menuSelect.OnMouseMove(nFlags, point); //�����µ��õ�menu�еķ���
}

void CGame::OnButtonUp(UINT nFlags, CPoint point)
{
	m_menuSelect.getImage();
	m_menuSelect.OnLButtonUp(nFlags, point);
}

bool CGame::GameInitOne2Bot()
{
	//��Ϸ��ʼ��
	m_map.LoadMap();//�����ͼ
	//���һ
	{
		PointF ptCenter;
		if (!m_map.FindRandomPosition(ptCenter)) {
			AfxMessageBox(_T("�������1λ��ʧ��"));
		}
		else
		{
			m_player01 = CPlayer(ptCenter.X, ptCenter.Y, _T("images/tank_player1.png"));
			//m_player01.SetCenterPoint(ptCenter);
		}
	}
	//�����ӵ�
	m_lstBullets.clear();
	return true;
}

void CGame::GameRunDraw()
{
	HDC hdc = ::GetDC(m_hWnd);//��ȡ�ͻ�����С
	CRect rc;
	GetClientRect(m_hWnd, &rc);
	CDC *dc = CClientDC::FromHandle(hdc);
	CDC m_dcMemory;  //˫��������ͼ
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
	m_dcMemory.CreateCompatibleDC(dc);
	CBitmap *pOldBitmap = m_dcMemory.SelectObject(&bmp);

	Graphics gh(m_dcMemory.GetSafeHdc()); //����Graphics����
	gh.Clear(Color::White); //�������
	gh.ResetClip();
	//����һ��ͼƬ
	//Gdiplus::Image *img = Image::FromFile(_T("./images/menu_background.png"));
	//gh.DrawImage(img, rc.left, rc.top, rc.Width(), rc.Height());
	/*{
		m_menu.getImage(); //�Ȼ�ȡ����ͼƬ
		m_menu.Draw(gh);
		m_menuSelect.getImage();
		m_menuSelect.Draw(gh); //�Ȼ�ȡ����ͼƬ
	}*/
	{
		m_map.Draw(gh);
		m_player01.getImage();  //�����1
		m_player01.Draw(gh);
		for (auto &blt : m_lstBullets) {
			blt.Draw(gh);
		}
	}
	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(),
		m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	dc->DeleteDC();

	return;
}

void CGame::GameRunLogic()
{
#define KEYDOWN(vk) (GetAsyncKeyState(vk) & 0x8000)
	//��������
	{
		if (KEYDOWN(VK_LEFT))          // �����̰���
		{
			m_player01.RotateLeft(); // ���һ����ת
		}
		if (KEYDOWN(VK_RIGHT))          			// �ҷ����̰���
		{
			m_player01.RotateRight();			// ���һ����ת
		}
		if (KEYDOWN(VK_UP))            			// �Ϸ����̰���
		{
			if (m_map.IsHitTheWall(m_player01, true)) {
				m_player01.ChangeDirection(true); //�жϵ�ײǽ����Ҫ�ı䷽��
			}
			else
			{
				m_player01.Forward();     			// ���һ��ǰ��

			}
			

		}
		if (KEYDOWN(VK_DOWN))          			// �·����̰���
		{
			if (m_map.IsHitTheWall(m_player01, true)) {
				m_player01.ChangeDirection(true); //�жϵ�ײǽ����Ҫ�ı䷽��
			}
			else
			{
				m_player01.Backward(); 		// ���һ�����
			}
		}
		if (KEYDOWN('M'))                		// ����M��
		{
			CBullet blt;
			if (m_player01.Fire(blt)) {        	// ����
				m_lstBullets.push_back(blt); 	// ���뵽��ͼ�б���
			}
		}
		//�ӵ�ײǽ����
		for (auto &blt : m_lstBullets) {
			m_map.HitWallProcess(blt);
			blt.Move();
		}
		//����ӵ��Ƿ����̹�ˣ�������̹�˱�ը
		for (auto &blt : m_lstBullets) {
			if (m_player01.IsHitted(blt)) {
				m_player01.Bomb();
			}
			break;
		}
	}

	for (auto &blt : m_lstBullets) {          	// �����ӵ�������ƶ�
		blt.Move();                            	// �ӵ���ǰ�ƶ�
	}

	// �Ƴ���ʱ���ӵ�
	{
		// ���ҳ�ʱ���ӵ�
		auto itRemove = std::remove_if(m_lstBullets.begin(),
			m_lstBullets.end(),
			[](CBullet & blt)->bool {
			return blt.IsTimeout();
		});
		for (auto it = itRemove; it != m_lstBullets.end(); ++it) {
			it->SetActive(false);				// ����Ϊ��Ч
			it->GetOwner()->AddBullet(*it);	// ����Ӧ��̹�������ӵ�
		}
		// �ӱ��� ɾ�� �ӵ�
		m_lstBullets.erase(itRemove, m_lstBullets.end());
	}
}

void CGame::DrawFps(Graphics & gh)
{
	static int fps = 0; //���徲̬����
	m_fps++;  //�Ѿ����˶���֡
	static DWORD dwLast = GetTickCount(); //��¼�ϴ����fpsʱ��
	if (GetTickCount() - dwLast >= 1000)  //�ж�ʱ�����Ƿ񳬹�1s
	{

		fps = m_fps;
		m_fps = 0;
		dwLast = GetTickCount();  //����ʱ��
	}
	//���Fps
	{
		CString s;
		s.Format(_T("FPS:%d"), fps);
		SolidBrush brush(Color(0x00, 0x00, 0xFF));//��ɫ��ˢ
		Gdiplus::Font font(_T("����"), 10.0); //�����������
		CRect rc;
		::GetClientRect(m_hWnd, &rc); //���ǻ�ȡ���ڴ�С����λ�������λ��
		PointF origin(static_cast<float>(rc.right - 50),
			static_cast<float>(rc.top + 2));
		gh.DrawString(s.GetString(), -1, &font, origin, &brush); //��ʽ�������

	}
}
