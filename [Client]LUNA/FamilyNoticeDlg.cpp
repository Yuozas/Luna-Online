#include "stdafx.h"
#include "./Interface\cTextArea.h"
#include "WindowIDEnum.h"
#include "FamilyNoticeDlg.h"
//#include "FamilyManager.h"

CFamilyNoticeDlg::CFamilyNoticeDlg(void)
{
}

CFamilyNoticeDlg::~CFamilyNoticeDlg(void)
{
}

void CFamilyNoticeDlg::Linking()
{
	m_pNoticeText = (cTextArea*)GetWindowForID(FNotice_TEXTREA);
	
	m_pNoticeText->SetEnterAllow(FALSE);
	m_pNoticeText->SetScriptText("");
}

void CFamilyNoticeDlg::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		switch(lId)
		{
		case GNotice_SENDOKBTN:
			{
				// 070801 웅주, 기존 패밀리 제거하며 주석처리

				//char notice[MAX_FAMILY_NOTICE+1] = {0,};

				//m_pNoticeText->GetScriptText(notice);
				//FAMILYMGR->SetFamilyNotice(notice);

				SetActive(FALSE);
			}
			break;
		case GNotice_CANCELBTN:
			{
				SetActive(FALSE);
			}
			break;
		}
	}
}

void CFamilyNoticeDlg::SetActive(BOOL val)
{
	// 070801 웅주, 기존 패밀리 제거하며 주석처리

	//if(val == TRUE)
	//{
	//	if(FAMILYMGR->GetFamilyNotice())
	//		m_pNoticeText->SetScriptText(FAMILYMGR->GetFamilyNotice());
	//}

	cDialog::SetActive(val);
}
