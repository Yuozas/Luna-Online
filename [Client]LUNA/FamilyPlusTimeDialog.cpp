#include "StdAfx.h"
#include "./Interface\cStatic.h"
#include "./Interface\cListDialog.h"
#include "WindowIDEnum.h"
#include "FamilyPlusTimeDialog.h"
#include "../Input/Mouse.h"
//#include "FamilyManager.h"
#include "ChatManager.h"

CFamilyPlusTimeDlg::CFamilyPlusTimeDlg()
{
	m_CurrentSelectedItem = -1;
}

CFamilyPlusTimeDlg::~CFamilyPlusTimeDlg()
{
}

void CFamilyPlusTimeDlg::Linking()
{
	m_pCurrentHavePoint = (cStatic*)GetWindowForID(FD_POINT);
	//m_pCurrentHavePoint->SetStaticText("15");

	m_pPlusItemList = (cListDialog*)GetWindowForID(FD_PLUSTIMELIST);
	m_pPlusItemList->SetShowSelect(TRUE);

	LoadPlustimeList();
	//m_pPlusItemList->SetHeight(170);

	SetFamilyPointText(0);
	/*
	m_pPlusItemList->AddItem("������0",0xffffffff,0);
	m_pPlusItemList->AddItem("������1",0xffffffff,1);
	m_pPlusItemList->AddItem("������2",0xffffffff,2);
	m_pPlusItemList->AddItem("������3",0xffffffff,3);
	m_pPlusItemList->AddItem("������4",0xffffffff,4);
	m_pPlusItemList->AddItem("������5",0xffffffff,5);
	m_pPlusItemList->AddItem("������6",0xffffffff,6);
	m_pPlusItemList->AddItem("������7",0xffffffff,7);
	m_pPlusItemList->AddItem("������8",0xffffffff,8);
	m_pPlusItemList->AddItem("������9",0xffffffff,9);
	m_pPlusItemList->AddItem("������10",0xffffffff,10);
	m_pPlusItemList->AddItem("������11",0xffffffff,11);
	*/

}

//�����÷���Ÿ�� ���̾�α��� �޽����� �޴´�.
void CFamilyPlusTimeDlg::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		switch(lId)
		{
		case GD_PLUSTIMESTART:
			{
				// 070801 ����, ���� �йи� �����ϸ� �ּ�ó��
				
				//if(m_CurrentSelectedItem != -1)
				//{
				//	//���� ���õ� �׸��� �÷��� Ÿ�� ����
				//	FAMILYMGR->UseFamilyPointSyn(ForFamilyPlusTime, m_CurrentSelectedItem + 1);	 //����Ʈ�� 1����..
				//}				
			}
			break;
		case CMI_CLOSEBTN:
			{
				//â�� �ݴ´�.
				SetActive(FALSE);
			}
			break;
		}
	}
}

DWORD CFamilyPlusTimeDlg::ActionEvent(CMouse* mouseInfo)
{

	int idx = m_pPlusItemList->GetCurSelectedRowIdx();

	DWORD we = WE_NULL;
	if( !m_bActive )		return we;

	we = cDialog::ActionEvent(mouseInfo);

	if(m_pPlusItemList->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) != -1)
	{
		if(we & WE_LBTNCLICK)
		{
			//if( WINDOWMGR->IsMouseDownUsed() == FALSE)
			{		
				int Idx = m_pPlusItemList->GetCurSelectedRowIdx();
				if(Idx != -1)
				{
					//���� ���õ� ��ȣ�� ����ȴ�.
					m_CurrentSelectedItem = Idx;
				}
			}
		}
	}

	int a = 0;

	return we;
}

void CFamilyPlusTimeDlg::SetActive(BOOL val)
{
	if(val == TRUE)
	{

	}

	cDialog::SetActive(val);
}

void CFamilyPlusTimeDlg::SetFamilyPointText( DWORD FamilyPoint )
{
	char buf[64];
	itoa((int)FamilyPoint, buf, 10);
	AddComma( buf );
	m_pCurrentHavePoint->SetStaticText(buf);
}

void CFamilyPlusTimeDlg::LoadPlustimeList()
{
	// 070801 ����, ���� �йи� �����ϸ� �ּ�ó��

	//FAMILYPLUSTIME_INFO* pGPlustimeInfo = FAMILYMGR->GetFamilyPointPlustimeList();

	//if(!pGPlustimeInfo)
	//	return;

	//char buf[256] = {0,};

	//for( int n = 0; n < eFamilyPlusTime_Max; ++n )
	//{
	//	DWORD Kind = pGPlustimeInfo[n].PlusTimeKind;
	//
	//	switch( Kind )
	//	{
	//	case eGPT_SuRyun:
	//		{
	//			sprintf( buf, CHATMGR->GetChatMsg(1377), pGPlustimeInfo[n].AddData, pGPlustimeInfo[n].NeedPoint );
	//		}
	//		break;
	//	case eGPT_Skill:
	//		{
	//			sprintf( buf, CHATMGR->GetChatMsg(1378), pGPlustimeInfo[n].AddData, pGPlustimeInfo[n].NeedPoint );
	//		}
	//		break;
	//	case eGPT_Exp:
	//		{
	//			//sprintf( buf, CHATMGR->GetChatMsg(1379), float(pGPlustimeInfo[n].AddData)/100, pGPlustimeInfo[n].NeedPoint );
	//			sprintf( buf, CHATMGR->GetChatMsg(1379), pGPlustimeInfo[n].AddData, pGPlustimeInfo[n].NeedPoint );
	//		}
	//		break;
	//	case eGPT_DamageUp:
	//		{
	//			sprintf( buf, CHATMGR->GetChatMsg(1380), pGPlustimeInfo[n].AddData, pGPlustimeInfo[n].NeedPoint );
	//		}
	//		break;
	//	default:
	//		break;
	//	}

	//	m_pPlusItemList->AddItem(buf , RGB_HALF(255, 255, 255));
	//}
}