#include "stdafx.h"
#include "Game.h"
#define KEYDOWN(vk) (GetAsyncKeyState(vk) & 0x8000)  //�жϰ����Ƿ���

CGame::CGame()
{
	//GameInitOne2Bot(); //��ʼ���˻���ս��Ϸ
	m_menuSelect.m_pParent = this; //���ò˵�ѡ����ָ��ǰ����
	m_menuBackup.m_pParent = this;
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
	return true;
}

void CGame::OnMouseMove(UINT nFlags, CPoint point)
{
	m_menuSelect.getImage();
	m_menuBackup.getImage();
	if (m_eStep == EGameTypeMenu) {
		m_menuSelect.OnMouseMove(nFlags, point); //�����µ��õ�menu�еķ���
	}
	else
	{
		m_menuBackup.OnMouseMove(nFlags, point);
	}
}

void CGame::OnButtonUp(UINT nFlags, CPoint point)
{
	m_menuSelect.getImage();
	m_menuBackup.getImage();
	if (m_eStep == EGameTypeMenu) {
		m_menuSelect.OnLButtonUp(nFlags, point);
	}
	else
	{
		m_menuBackup.OnLButtonUp(nFlags, point);
	}
}
// ���õ�ǰ��Ϸ�����׶Σ����ݲ����ʼ��
void CGame::SetStep(CGame::EGameType step)
{
	m_eStep = step;
	GameInit();
}

bool CGame::GameInit()
{
	srand(GetTickCount());  //��ʼ�������������
	return (this->*m_initFunc[m_eStep])();  //���ݲ�ͬ�׶ε��ò�ͬ������,������ǰ��*��
}
//��Ϸ��ʼ����ѡ��׶�
bool CGame::GameInitMenu()
{
	return true;
}
//��Ϸ��ʼ�����˻���ս�˷��׶�
bool CGame::GameInitOne2BotMenu()
{
	RECT rc;
	GetWindowRect(m_hWnd, &rc);
	PointF pt;
	pt.X = rc.left + (rc.right - rc.left) / 2.0f;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);				// ���õ��˶�սkeyMenuλ��Ϊ��Ļ���м�
	m_keymenu01.SetStop(false);					// ���á�����ֹͣ���Ŷ���
	return true;
}

bool CGame::GameInitOne2Bot()
{
	for (;;) {
		//��Ϸ��ʼ��
		m_map.LoadMap();//�����ͼ
		//���һ
		{
			m_player01 = CPlayer(0, 0, _T("./images/tank_player1.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter)) {
				AfxMessageBox(_T("�������1λ��ʧ��"));
			}
			else
			{
				m_player01.SetCenterPoint(ptCenter);
			}
		}
		// ������
		{
			m_bot = CBot(0, 0, _T("./images/tank_bot.png"));				// ��������˶���
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter)) {				// ������ҵ�ͼ�еĿյ�
				AfxMessageBox(_T("����Botλ��ʧ��")); 				// ��ʾ����λ��ʧ��
			}
			else {
				m_bot.SetCenterPoint(ptCenter);
			}
		}
		// �ж��Ƿ�Ϸ�
		{
			// ��ȡ�����ˣ���� ���ڵ�λ��
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_bot, startX, startY) ||
				!m_map.FindObjPosition(m_player01, targetX, targetY)) {
				AfxMessageBox(_T("��ȡ̹��λ�÷�������"));
				goto __Init_End;
			}
			// �ж���Һͻ������Ƿ���ͨ������޷���ͨ�����޷�������Ϸ��������Ҫ��������
			VPath path;
			m_map.FindPath(startX, startY, targetX, targetY, path);
			if (!path.empty()) {
				goto __Init_End; // ������ͨ������ѭ����ֱ����������β������ʼ������
			}
		}
		//�����ӵ�
		m_lstBullets.clear();
	}
__Init_End:
	return true;
}
// ��Ϸ��ʼ�� : ���˶Ե��� ����
bool CGame::GameInitOne2BotEnd()
{
	return true;
}

// ��Ϸ��ʼ�� : ˫�˶�ս �˵��׶�
bool CGame::GameInitOne2OneMenu()
{
	// ���� ������ҵ� keyMenu λ�� : ��Ļ���м�
	RECT rc;
	GetWindowRect(m_hWnd, &rc);
	PointF pt;
	pt.X = rc.left + m_keymenu01.GetRect().Width / 2.0f + 100;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);							// ���øò˵����λ��
	m_keymenu01.SetStop(false);								// ���ò�ֹͣ���Ŷ���


	pt.X = rc.right - m_keymenu02.GetRect().Width / 2.0f - 100;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu02.SetCenterPoint(pt); 							// ���øò˵����λ��
	m_keymenu02.SetStop(false); 								// ���ò�ֹͣ���Ŷ���

	return true;
}

// ��Ϸ��ʼ�� : ˫�˶�ս
bool CGame::GameInitOne2One()
{
	for (;;) {
		m_map.LoadMap();										// �����ͼ
																//�м����̹��
		{
			m_player01 = CPlayer(0, 0, _T("./images/tank_player1.png"));	// �������һ����
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter)) {			// ��������Ŀյ�λ��
				AfxMessageBox(_T("����Player01λ��ʧ��"));		// ��ʾ����ʧ��
			}
			else {
				m_player01.SetCenterPoint(ptCenter); // �������һλ�õ����յ�����	
			}
		}
		{
			m_player02 = CPlayer(0, 0, _T("./images/tank_player2.png"));	// ������Ҷ�
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter)) {			// ������ҵ�ͼ�еĿյ�
				AfxMessageBox(_T("����Player02λ��ʧ��"));		// ��ʾ����ʧ��
			}
			else {
				m_player02.SetCenterPoint(ptCenter);	// ������Ҷ���λ�õ����յ�����
			}
		}

		m_lstBullets.clear();							// ����ӵ�����
														// �ж��Ƿ�Ϸ�
		{
			// ���һ����ˣ���� ���ڵ�λ��
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_player02, startX, startY) ||
				!m_map.FindObjPosition(m_player01, targetX, targetY)) {
				AfxMessageBox(_T("��ȡ̹��λ�÷�������")); 	// ��ʾ����ʧ��
				break;
			}
			// �ж���������Ƿ������ͨ
			VPath path;
			m_map.FindPath(startX, startY, targetX, targetY, path);
			if (!path.empty()) {
				break;									// ������ͨ����ѭ��,��ʼ�����
			}
			// ��������ͨ��˵�����γ�ʼ��ʧ�ܣ�������ѭ������������һ�³�ʼ��
		}
	}
	return true;
}

// ��Ϸ��ʼ�� : ˫�˶�ս ����
bool CGame::GameInitOne2OneEnd()
{
	return true;	// ����Ҫ��ʼ��������ֱ�ӷ���true��ʾ��ʼ���ɹ�
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
	

	(this->*m_drawFunc[m_eStep])(gh);  //������Ӧ��ͼ����
	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(),
		m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	::ReleaseDC(m_hWnd, hdc); 							// �ͷ�

	return;
}
// ѡ��׶�
void CGame::GameRunDrawOnMenu(Graphics &gh)
{
	m_menuSelect.getImage();
	m_menuSelect.Draw(gh);
}

//���˶Ե��� : �˵��׶�
void CGame::GameRunDrawOnOne2BotMenu(Graphics &gh)
{
	m_menu.getImage(); 
	m_menu.Draw(gh); 									// ������
	m_keymenu01.getImage();
	m_keymenu01.Draw(gh); 								// ���˵�
}

//���˶Ե���
void CGame::GameRunDrawOnOne2Bot(Graphics &gh)
{
	m_menuBackup.getImage();
	m_menuBackup.Draw(gh); 								// ���˵�
	m_map.Draw(gh); 										// ��ǽ

	m_player01.Draw(gh); 								// �����һ

	m_bot.Draw(gh); 										// ��������

	for (auto b : m_lstBullets) { 						// ���ӵ� :  ���������
		b.Draw(gh);
	}

	DrawFps(gh); 										// ��� : FPS
}

//���˶Ե��� ����
void CGame::GameRunDrawOnOne2BotEnd(Graphics &gh)
{
	m_menuBackup.Draw(gh); 								// �˵�
	m_map.Draw(gh); 										// ǽ
	m_player01.Draw(gh); 								// ���
	m_bot.Draw(gh); 										// ������

	DrawFps(gh); 										// ��� : FPS

	if (m_player01.IsBombEnd() || m_bot.IsBombEnd()) { 	// �ж���Ϸ�������
		m_eStep = EGameTypeMenu;							// ����Ϊ�˵�״̬
	}
}

// ˫�˶�ս : �˵��׶�
void CGame::GameRunDrawOnOne2OneMenu(Graphics &gh)
{
	m_menu.getImage();
	m_menu.Draw(gh); 									// ���˵�����
	m_keymenu01.getImage();
	m_keymenu01.Draw(gh); 								// ���˵���һ
	m_keymenu02.getImage();
	m_keymenu02.Draw(gh); 								// ���˵����
}

// ˫�˶�ս
void CGame::GameRunDrawOnOne2One(Graphics &gh)
{
	m_menuBackup.getImage();
	m_menuBackup.Draw(gh); 								// ���˵�

	m_map.Draw(gh); 										// ��ǽ

	m_player01.Draw(gh); 								// �����һ
	m_player02.Draw(gh); 								// ����Ҷ�

	for (auto b : m_lstBullets) { 						// ���ӵ� :  ���������
		b.Draw(gh);
	}

	DrawFps(gh); 											// ��� : FPS
}

// ˫�˶�ս ����
void CGame::GameRunDrawOnOne2OneEnd(Graphics &gh)
{
	m_menuBackup.getImage();
	m_menuBackup.Draw(gh); 									// �˵�
	m_map.Draw(gh); 											// ǽ
	m_player01.Draw(gh); 									// ���һ
	m_player02.Draw(gh); 									// ��Ҷ�
	for (auto b : m_lstBullets) { 							// ���ӵ� :  ���������
		b.Draw(gh);
	}
	DrawFps(gh);	 											// ��� : FPS

	if (m_player01.IsBombEnd() || m_player02.IsBombEnd()) { // �ж���Ϸ�������
		m_eStep = EGameTypeMenu;								// ������Ϸ״̬Ϊ�˵�״̬
	}
}

void CGame::GameRunLogic()
{
	// ���ݲ�ͬ�׶ε��ò�ͬ�Ĵ�����
	(this->*m_logicFunc[m_eStep])();
}
// ��Ϸ�߼����� : ѡ��׶�
void CGame::GameRunLogicOnMenu()
{
	// ʲôҲ��������û��ʼ��Ϸ
}

// ��Ϸ�߼����� : ���˶Ե��� �˵��׶�
void CGame::GameRunLogicOnOne2BotMenu()
{
	if (KEYDOWN('M')) { 				// ���������M����ֹͣ����״̬
		m_keymenu01.SetStop();
	}

	if (m_keymenu01.GetStop()) { 		// ����������ˣ���ʽ��ʼ��Ϸ
		SetStep(EGameTypeOne2Bot);
	}
}

// ��Ϸ�߼����� : ���˶Ե���
void CGame::GameRunLogicOnOne2Bot()
{
	// ״̬ά��
	// �ƶ����б�����Ч���ӵ�,������Ӧ��̹�������ӵ�
	RemoveTimeoutBullets();

	// ����ӵ��Ƿ����̹�� :  ����Ҫʹ̹�˱�ը(�ӵ��ɲ����Ƴ��ˣ�
	// ��Ϊ��Ϸ��������)
	for (auto &blt : m_lstBullets) {
		if (m_bot.IsHitted(blt)) {			// ���л�����
			m_bot.Bomb();						// �����˱�ը
			m_eStep = EGameTypeOne2BotEnd; 	// ��Ϸ����
			blt.SetActive(false);				// ʹ�ӵ�������Ч
		}
		if (m_player01.IsHitted(blt)) { 		// �������һ
			m_player01.Bomb();				// ���һ��ը
			m_eStep = EGameTypeOne2BotEnd; 	// ��Ϸ����
			blt.SetActive(false);				// ʹ�ӵ�������Ч
		}
		break;
	}

	ProcessHitBullets();  					// �ӵ��˶�ά��

	AI();									// ʹ�������Զ��������

											//��������
	{
		if (KEYDOWN(VK_LEFT)) {				// ���������
			m_player01.RotateLeft();			// ���һ������ת
		}
		if (KEYDOWN(VK_RIGHT)) {				// �ҷ��������
			m_player01.RotateRight();			// ���һ������ת
		}
		if (KEYDOWN(VK_UP)) {					// �Ϸ��������
												// ̹��ײǽ�����	
			{
				if (m_map.IsHitTheWall(m_player01, true)) {		// ���ײǽ��
					m_player01.ChangeDirection(true);			// �ı�̹�˷���
				}
				else {
					m_player01.Forward();						// ûײǽ������ǰ��
				}
			}
		}
		if (KEYDOWN(VK_DOWN)) {								// �·��������
															// ̹��ײǽ�����
			{
				if (m_map.IsHitTheWall(m_player01, false)) {	// ���ײǽ��
					m_player01.ChangeDirection(true);		  	// �ı�̹�˷���
				}
				else {
					m_player01.Backward();				  	// ûײǽ������ǰ��
				}
			}
		}
		if (KEYDOWN('M')) {									// M������
			CBullet blt;
			if (m_player01.Fire(blt)) {						// �����ӵ�
				m_lstBullets.push_back(blt);					// ���뵽��ͼ�б���
			}
		}
		if (KEYDOWN('I')) {						// ���¼��������I���������˲���(���Թ���)
												// �����ˣ���� ���ڵ�λ��
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_bot, startX, startY) ||
				!m_map.FindObjPosition(m_player01, targetX, targetY)) {
				return;
			}
			float fDirNext = 0; //��������һ���ķ���
			if (!m_map.FindNextDirection(&fDirNext, startX, startY,
				targetX, targetY)) {
				return;
			}
			//��ȡ������̹�˵����ĵ�
			PointF ptTankCenter = m_bot.GetCenterPoint();
			PointF ptAreaCenter = m_map.GetElementAreaCenter(startX, startY);
			RectF rc(ptAreaCenter.X - 5, ptAreaCenter.Y - 5, 10, 10);


			if (!rc.Contains(ptTankCenter)) { 		// �ж� ̹���Ƿ񼺾��ߵ������ĵ�λ����
				m_bot.Forward(); 					// û�е������ĵ㣬����ǰ��
				return;
			}
			else {
				m_bot.SetDirection(fDirNext);		// ���û����˵ķ���			
				m_bot.Forward();					// ������ǰ��
			}
		}
	}

}

// ��Ϸ�߼����� : ���˶Ե��� ����
void CGame::GameRunLogicOnOne2BotEnd()
{
	//��������
	// ���ٽ��ܰ���


	// ״̬ά��
	// �ƶ����б�����Ч���ӵ�,������Ӧ��̹�������ӵ�
	RemoveTimeoutBullets();


	// �ӵ��Ƿ�ײ��ǽ : ���ײ���ˣ��ı䷽��ȵ�
	ProcessHitBullets();
}

// ��Ϸ�߼����� : ˫�˶�ս �˵��׶�
void CGame::GameRunLogicOnOne2OneMenu()
{
	if (KEYDOWN('M')) { 									// ���������M����ֹͣ����״̬
		m_keymenu01.SetStop();
	}
	if (KEYDOWN('Q')) { 									// ���������Q����ֹͣ����״̬
		m_keymenu02.SetStop();
	}

	if (m_keymenu01.GetStop() && m_keymenu02.GetStop()) { 	// ����������ˣ���ʽ��ʼ��Ϸ
		SetStep(EGameTypeOne2One);
	}
}

// ��Ϸ�߼����� : ˫�˶�ս
void CGame::GameRunLogicOnOne2One()
{
	//��������
	{
		if (KEYDOWN(VK_LEFT)) { 								// ���������
			m_player01.RotateLeft();							// ���������ת
		}
		if (KEYDOWN(VK_RIGHT)) {								// �ҷ��������
			m_player01.RotateRight();							// ���һ������ת
		}
		if (KEYDOWN(VK_UP)) {									// �Ϸ��������
																// ̹��ײǽ�����
			{
				if (m_map.IsHitTheWall(m_player01, true)) {		// �ж����һ�Ƿ�ײǽ
					m_player01.ChangeDirection(true);			// ײǽ�����һ�ı䷽��
				}
				else {
					m_player01.Forward();						// ���һûײǽ������ǰ��
				}
			}
		}
		if (KEYDOWN(VK_DOWN)) {								// �·��������
			{
				// ̹��ײǽ�����
				{
					if (m_map.IsHitTheWall(m_player01, false)) {// �ж��Ƿ�ײǽ
						m_player01.ChangeDirection(false);
					}
					else {
						m_player01.Backward();
					}
				}
			}
		}
		if (KEYDOWN('M')) {								// �����M����
			CBullet blt;
			if (m_player01.Fire(blt)) {					// �������һ������
				m_lstBullets.push_back(blt);				// �ѷ�����ӵ������ӵ�����
			}
		}
		// ��Ҷ�
		if (KEYDOWN('A')) {								// A������
			m_player02.RotateLeft();						// ��Ҷ���������
		}
		if (KEYDOWN('D')) {								// D������
			m_player02.RotateRight();						// ��Ҷ�������ת
		}
		if (KEYDOWN('W')) {								// W������
														// ̹��ײǽ�����
			{
				if (m_map.IsHitTheWall(m_player02, true)) {// �ж���Ҷ��Ƿ�ײǽ
					m_player02.ChangeDirection(true);		// ��Ҷ��ı䷽��
				}
				else {
					m_player02.Forward();					// ��Ҷ�������ǰ��
				}
			}
		}
		if (KEYDOWN('S')) {								// S��������
			{
				// ̹��ײǽ�����
				{
					if (m_map.IsHitTheWall(m_player02, false)) {// �ж���Ҷ��Ƿ�ײǽ
						m_player02.ChangeDirection(false);	// ��Ҷ��ı䷽��
					}
					else {
						m_player02.Backward();				// ��Ҷ�������ǰ
					}
				}
			}
		}
		if (KEYDOWN('Q')) {								// Q�����£������
			CBullet blt;
			if (m_player02.Fire(blt)) {					// ������Ҷ�������
				m_lstBullets.push_back(blt);				// �ѷ�����ӵ������ӵ�����
			}
		}
		if (KEYDOWN('Z')) {								// �����õĴ��룬��ʽ������Ч
			if (m_map.IsCanKillTarget(m_player01, m_player02)) {
				AfxMessageBox(_T("���Դ�"));
			}
		}
	}

	// ���ж�״̬
	// �ƶ����б�����Ч���ӵ�,������Ӧ��̹�������ӵ�
	RemoveTimeoutBullets();

	// ����ӵ��Ƿ����̹�� :  ����Ҫʹ̹�˱�ը(�ӵ��ɲ����Ƴ��ˣ���Ϊ��Ϸ��������)
	for (auto &blt : m_lstBullets) {
		if (!blt.IsActive()) {
			continue;
		}
		if (m_player01.IsHitted(blt)) {	// �������һ
			m_player01.Bomb();			// ���һ��ը
			m_eStep = EGameTypeOne2OneEnd;// ��Ϸ����
			blt.SetActive(false);
		}
		if (m_player02.IsHitted(blt)) { 	// ������Ҷ�
			m_player02.Bomb();			// ��Ҷ���ը
			m_eStep = EGameTypeOne2OneEnd;// ��Ϸ����
			blt.SetActive(false);
		}
	}

	ProcessHitBullets();					// �ӵ�ײǽ����
}

// ��Ϸ�߼����� : ˫�˶�ս ����
void CGame::GameRunLogicOnOne2OneEnd()
{
	//��������
	// ����Ҫ��������

	RemoveTimeoutBullets();				// �ƶ����б�����Ч���ӵ�,������Ӧ��̹�������ӵ�

	ProcessHitBullets();					// �ӵ�ײǽ����
}


void CGame::RemoveTimeoutBullets()
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

void CGame::ProcessHitBullets()
{
	//�ӵ�ײǽ����
	for (auto &blt : m_lstBullets) {
		m_map.HitWallProcess(blt);
		blt.Move();
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
// ά�����Ե��Զ�Ѱ·����
void CGame::AI()
{
	// �����˶�״̬̬ά��
	static CGameTimer acTimer(-1, 150);
	if (acTimer.IsTimeval()) {
		// ��û����ˣ���� ���ڵ�λ��
		int startX, startY, targetX, targetY;
		if (!m_map.FindObjPosition(m_bot, startX, startY) ||
			!m_map.FindObjPosition(m_player01, targetX, targetY)) {
			return;
		}
		float fDirNext = 0; //��������һ���ķ���
		if (!m_map.FindNextDirection(&fDirNext,
			startX, startY,
			targetX, targetY)) {
			return;
		}
		// ��û����ˡ���ͼ�пո�����ĵ�
		PointF ptTankCenter = m_bot.GetCenterPoint();
		PointF ptAreaCenter = m_map.GetElementAreaCenter(startX, startY);
		RectF rc(ptAreaCenter.X - 5, ptAreaCenter.Y - 5, 10, 10);


		// �ж� ̹���Ƿ񼺾��ߵ������ĵ�λ����
		if (!rc.Contains(ptTankCenter)) {
			m_bot.Forward(); // û�е������ĵ㣬����ǰ��
			return;
		}
		else {
			m_bot.SetDirection(fDirNext);	// ���û�������һ�������з���
			float dir;
			// �ж��Ƿ���Դ����һ
			if (m_map.IsCanKillTarget(m_bot, m_player01, &dir)) {
				CBullet blt;
				if (m_bot.Fire(blt)) {		// �����˿��𣬽�����������
					m_lstBullets.push_back(blt);
				}
				return;
			}
			m_bot.Forward();					// ������ǰ��
		}
	}
}