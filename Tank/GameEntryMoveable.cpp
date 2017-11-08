#include "stdafx.h"
#include "GameEntryMoveable.h"


CGameEntryMoveable::CGameEntryMoveable()
{
}

CGameEntryMoveable::CGameEntryMoveable(const CGameEntryMoveable & rhs)
{
	if (this != &rhs) {
		this->m_bActive = rhs.m_bActive;				// �Ƿ���Ч
		this->m_rect = rhs.m_rect;						// ռ�õ�����
		this->m_fSpeed = rhs.m_fSpeed;					// �ƶ��ٶ�
		this->m_direction = rhs.m_direction;			// ��ǰ�ĽǶ�
		this->m_directionTurn = rhs.m_directionTurn; 	// ÿ����ת�ĽǶ�
	}
}


CGameEntryMoveable::~CGameEntryMoveable()
{
}

CGameEntryMoveable & CGameEntryMoveable::operator=(const CGameEntryMoveable & rhs)
{
	if (this != &rhs) {
		this->m_bActive = rhs.m_bActive;				// �Ƿ���Ч
		this->m_rect = rhs.m_rect;						// ռ�õ�����
		this->m_fSpeed = rhs.m_fSpeed;					// �ƶ��ٶ�
		this->m_direction = rhs.m_direction; 			// ��ǰ�ĽǶ�
		this->m_directionTurn = rhs.m_directionTurn; 	// ÿ����ת�ĽǶ�
	}
	return *this;
}

void CGameEntryMoveable::RotateRight()
{
	m_direction += m_directionTurn; // ��ǰ�Ƕ�����
	if (m_direction >= 360) {         // ������һ��֮�󣬼�ȥ360�ȣ�ʹ��ʼ�ձ�����С�Ƕ�
		m_direction -= 360;
	}
}

void CGameEntryMoveable::RotateLeft()
{
	m_direction -= m_directionTurn; // ��ǰ�Ƕȼ���
	if (m_direction < 0) {            // ʹ��ʼ�ձ����ڴ���0�ĽǶ�
		m_direction = static_cast<float>(360 - m_directionTurn);
	}
}

void CGameEntryMoveable::Forward()
{
	// ���ݵ�ǰ�Ƕȣ���x,y�����Ϸֱ��������
	m_rect.X += static_cast<Gdiplus::REAL>((m_fSpeed)* sin(GetDirectionArc()));
	m_rect.Y -= static_cast<Gdiplus::REAL>((m_fSpeed)* cos(GetDirectionArc()));
}

void CGameEntryMoveable::Backward()
{
	// ���ݵ�ǰ�Ƕȣ���x,y�����Ϸֱ��������
	m_rect.X -= static_cast<Gdiplus::REAL>((m_fSpeed)* sin(GetDirectionArc()));
	m_rect.Y += static_cast<Gdiplus::REAL>((m_fSpeed)* cos(GetDirectionArc()));
}

float CGameEntryMoveable::GetXSpeed() const
{
	return static_cast<Gdiplus::REAL>((m_fSpeed)* sin(GetDirectionArc()));
}

float CGameEntryMoveable::GetYSpeed() const
{
	return -static_cast<Gdiplus::REAL>((m_fSpeed)* cos(GetDirectionArc()));
}

RectF CGameEntryMoveable::ForwardNextRect() const
{
	RectF rc = m_rect;
	rc.X += static_cast<Gdiplus::REAL>((m_fSpeed)* sin(GetDirectionArc()));
	rc.Y -= static_cast<Gdiplus::REAL>((m_fSpeed)* cos(GetDirectionArc()));
	return rc;
}

RectF CGameEntryMoveable::BackwardNextRect() const
{
	RectF rc = m_rect;
	rc.X -= static_cast<Gdiplus::REAL>((m_fSpeed)* sin(GetDirectionArc()));
	rc.Y += static_cast<Gdiplus::REAL>((m_fSpeed)* cos(GetDirectionArc()));
	return rc;
}

PointF CGameEntryMoveable::GetHeadPos() const
{
	PointF ptCenter = GetCenterPoint();
	PointF ptHead = ptCenter;
	float fRadius = std::sqrt(std::pow(m_rect.Width / 2, 2) +   // ����뾶
		std::pow(m_rect.Height / 2, 2));

	ptHead.X += fRadius * sin(GetDirectionArc());                // ����ͷ������x
	ptHead.Y -= fRadius * cos(GetDirectionArc());                // ����ͷ������y
	return ptHead;                                                  // ����ͷ������
}

PointF CGameEntryMoveable::GetCenterPoint() const
{
	PointF center = PointF(m_rect.X + m_rect.Width / 2,
		m_rect.Y + m_rect.Height / 2);
	return center;
}

void CGameEntryMoveable::SetCenterPoint(const PointF & ptCenter)
{
	PointF cen = GetCenterPoint();
	cen.X = ptCenter.X - cen.X;
	cen.Y = ptCenter.Y - cen.Y;
	m_rect.Offset(cen);
}

void CGameEntryMoveable::SetDirectionArc(float dir)
{
	m_direction = dir * 180.0f / PI;
}

void CGameEntryMoveable::SetDirection(float dir)
{
	m_direction = dir;
}

float CGameEntryMoveable::GetDirectionArc() const
{
	return PI * m_direction / 180.0f;
}

float CGameEntryMoveable::GetDirection() const
{
	return m_direction;
}

void CGameEntryMoveable::SetDirectionTurnArc(float dir)
{
	m_directionTurn = PI * dir / 180.0f;
}

void CGameEntryMoveable::SetDirectionTurn(float dir)
{
	m_directionTurn = dir;
}

float CGameEntryMoveable::GetDirectionTurnArc() const
{
	return PI * m_directionTurn / 180.0f;
}

float CGameEntryMoveable::GetDirectionTurn() const
{
	return m_direction;
}

bool CGameEntryMoveable::IsActive() const
{
	return m_bActive;
}

void CGameEntryMoveable::SetActive(bool bActive)
{
	m_bActive = bActive;
}

void CGameEntryMoveable::SetRect(const RectF rect)
{
	m_rect = rect;
}

RectF CGameEntryMoveable::GetRect() const
{
	return m_rect;
}

void CGameEntryMoveable::SetSpeed(float speed)
{
	m_fSpeed = speed;
}

float CGameEntryMoveable::GetSpeed() const
{
	return m_fSpeed;
}
