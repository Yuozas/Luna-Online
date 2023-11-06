#include "stdafx.h"
#include "FamilyRankDialog.h"
#include "WindowIDEnum.h"
#include "./interface/cTextArea.h"
#include "ChatManager.h"
//#include "FamilyManager.h"


CFamilyRankDialog::CFamilyRankDialog()
{
	m_type = WT_FAMILYRANKDLG;
	m_CurFamilyRankMode = 255;
}

CFamilyRankDialog::~CFamilyRankDialog()
{
	for(int i=0; i<eFamilyRankMode_Max;  ++i)
		m_FamilyRankCtrlList[i].RemoveAll();
}

void CFamilyRankDialog::Linking()
{
	m_pRankMemberName = (cTextArea*)GetWindowForID(FDR_MEMBERNAME);
	m_pDRankComboBox = (cComboBox*)GetWindowForID(FDR_DRANKCOMBOBOX);
	m_pRankComboBox = (cComboBox*)GetWindowForID(FDR_RANKCOMBOBOX);
	m_pDOkBtn = (cButton*)GetWindowForID(FDR_DOKBTN);
	m_pOkBtn = 	(cButton*)GetWindowForID(FDR_OKBTN);

	m_FamilyRankCtrlList[eFamilyRankMode_Dan].AddTail(m_pDRankComboBox);
	m_FamilyRankCtrlList[eFamilyRankMode_Dan].AddTail(m_pDOkBtn);

	m_FamilyRankCtrlList[eFamilyRankMode_Family].AddTail(m_pRankComboBox);
	m_FamilyRankCtrlList[eFamilyRankMode_Family].AddTail(m_pOkBtn);
}

void CFamilyRankDialog::SetActive(BOOL val)
{
	// 070801 웅주, 기존 패밀리 제거하며 주석처리

	//if(val == TRUE)
	//{
	//	if((FAMILYMGR->GetSelectedMemberID() == 0) || (FAMILYMGR->GetSelectedMemberID() == HEROID))
	//	{
	//		if(IsActive() == TRUE)
	//			cDialog::SetActive(FALSE);
	//		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(714));
	//		return;
	//	}
	//	SetName(FAMILYMGR->GetSelectedMemberName());
	//}

	cDialog::SetActive(val);
}

void CFamilyRankDialog::ShowFamilyRankMode(BYTE FamilyLvl)
{	
	BYTE ShowMode;
	if(FamilyLvl == MAX_FAMILY_LEVEL)
		ShowMode = eFamilyRankMode_Family;
	else 
		ShowMode = eFamilyRankMode_Dan;
	if(m_CurFamilyRankMode == ShowMode)
		return;

	if(m_CurFamilyRankMode != 255)
	{
		SetActiveFamilyRankMode(m_CurFamilyRankMode ,FALSE);
	}

	SetActiveFamilyRankMode(ShowMode,TRUE);
	m_CurFamilyRankMode = ShowMode;	
}

void CFamilyRankDialog::SetActiveFamilyRankMode(int showmode,BOOL bActive)
{
	PTRLISTSEARCHSTART(m_FamilyRankCtrlList[showmode],cWindow*,pWin)
		pWin->SetActive(bActive);
	PTRLISTSEARCHEND
}

void CFamilyRankDialog::SetName(char * name)
{
	char buf[128];
	sprintf(buf, CHATMGR->GetChatMsg(718), name);
	m_pRankMemberName->SetScriptText(buf);
}
