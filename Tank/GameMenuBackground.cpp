#include "stdafx.h"
#include "GameMenuBackground.h"


CGameMenuBackground::CGameMenuBackground()
{
	m_imgBackground = Image::FromFile(_T("menu_background.png"));
}


CGameMenuBackground::~CGameMenuBackground()
{
}

void CGameMenuBackground::Draw(Graphics &gh) const
{

	// ªÊ÷∆±≥æ∞Õº∆¨	
	gh.DrawImage(m_imgBackground, m_rect);  
}

void CGameMenuBackground::getImage()
{
	if (m_imgBackground == NULL) {
		m_imgBackground = Image::FromFile(_T("./images/menu_background.png"));
	}
}
