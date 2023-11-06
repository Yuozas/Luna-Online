// MunpaDoc.cpp : implementation file
//

#include "stdafx.h"

#include "DS_RMTool.h"
#include "PetDoc.h"
#include "Pet.h"
#include "RMNetwork.h"
#include "PetView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PetDoc.cpp : 구현 파일입니다.
//

IMPLEMENT_DYNCREATE(CPetDoc, CBaseDoc)

CPetDoc::CPetDoc()
{
	m_dwLoadingFlag = eAL_None;
	m_dwSaveFlag = eCF_None;
	
	m_bReadOnly = FALSE;

	m_ItemOptionInfo.Initialize(100);
	m_OptionKeyGenerator.Init(2000, 100);

	memset(&m_sOperator, 0, sizeof(m_sOperator));

	m_sUserInfo = NULL;

}

BOOL CPetDoc::OnNewDocument()
{
	if (!CBaseDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CPetDoc::~CPetDoc()
{
}


BEGIN_MESSAGE_MAP(CPetDoc, CBaseDoc)
END_MESSAGE_MAP()


// CPetDoc 진단입니다.

#ifdef _DEBUG
void CPetDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPetDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CPetDoc serialization입니다.

void CPetDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CPetDoc 명령입니다.

void CPetDoc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CBaseDoc::OnCloseDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CAdminDoc commands

void CPetDoc::NetworkMsgParse(BYTE Protocol, void* pMsg)
{

	CPetView* pView = (CPetView*)GetView(0);
	if( !pView )	return;

	switch(Protocol)
	{
	case MP_RMTOOL_PETSEARCH_NACK:		// 펫 검색 에러
		{
			pView->MessageBox("No Data!!", "Error");
		}
		break;
	case MP_RMTOOL_PETSEARCH_ACT:		// 펫 검색 성공
		{
			TMSG_PETINFO* pmsg = (TMSG_PETINFO*)pMsg;
			m_dwPetDBIdx = pmsg->PetInfo[0].dwPetDBIdx;
			pView->SetPetInfo( pmsg );
			m_sBaseInfo.dwObjectID = pmsg->PetInfo[0].dwCharIdx;
			strcpy(m_sBaseInfo.ObjectName, pmsg->PetInfo[0].Loginid);
		}
		break;
	case MP_RMTOOL_PETITEMIFNO:			// 펫 인벤 검색 결과
		{
			TMSG_CHARACTER_ITEMBASE* pmsg = (TMSG_CHARACTER_ITEMBASE*)pMsg;
			pView->SetPetWareItemInfo( pmsg );
		}
		break;
	case MP_RMTOOL_PETACCIFNO:			// 펫 악세서리 검색 결과
		{
			TMSG_CHARACTER_ITEMBASE* pmsg = (TMSG_CHARACTER_ITEMBASE*)pMsg;
			pView->SetPetAccItemInfo( pmsg );
		}
		break;
	case MP_RMTOOL_PETLIST_ACT:	// pet 캐릭터별 리스트 결과			
		{
			TMSG_PETLISTRESULT* pmsg = (TMSG_PETLISTRESULT*)pMsg;
			pView->SetPetList( pmsg->Count, pmsg );

		}
		break;
	case MP_RMTOOL_PETLIST_NACK:// 펫 리스트 결과 오류
		{
		}
		break;

	case MP_RMTOOL_PETUSERLOGINCHK:	// 펫 소유자(캐릭터) 로그인 결과			
		{
			TMSG_USERLOGINCHK* msg = (TMSG_USERLOGINCHK*)pMsg;
			pView->SetUserLoginChk( msg );
		}
		break;
	case MP_RMTOOL_PET_MEMBERNUM_ACK:				// 1
		{

			TMSG_DWORD* pmsg = (TMSG_DWORD*)pMsg;

	
			// 서버에 검색 요청(팻 캐릭터별 리스트 가져옴)
			TMSG_PETLISTSEARCH PetListSearch;
			PetListSearch.Category = MP_RMTOOL_PET;
			PetListSearch.Protocol = MP_RMTOOL_PETLIST_SYN;
			PetListSearch.dwTemplateIdx = GetDocumentIdx();
			PetListSearch.wServer = MASINFOMGR->GetCurMasInfo()->nServerSet;
			PetListSearch.loginidx = pmsg->dwData;		//PetListSearch.loginidx = 사용자가 검색 요청한 아이디index
			
			NETWORK->Send( &PetListSearch, sizeof(PetListSearch) );

		}
		break;
	case MP_RMTOOL_PET_MEMBERNUM_NACK:
		{
			pView->MessageBox("No Data!!", "Error");
		}
		break;
	}

	pView->SearchbtnEnable(TRUE);
}


void CPetDoc::SetOperatorTotalInfo(WORD count, OPERATOR* pOper, BOOL bEnd)
{
	m_sOperator.wOperCount = 0;
	memset( m_sOperator.pOper, 0, sizeof(OPERATOR)*100 );

	memcpy(&m_sOperator.pOper[m_sOperator.wOperCount], pOper, sizeof(OPERATOR)*count);
	m_sOperator.wOperCount = count;

	SetLoadingFlag(eAL_OperatorInfo);
}

void CPetDoc::OpenData()
{
	// 컨트롤 초기화 지시..
	CPetView* pView = (CPetView*)GetView(0);

	pView->GetPetPage()->InitControl();
}

void CPetDoc::SetUserInfo(USERINFO* user)
{
	SAFE_DELETE(m_sUserInfo);

	m_sUserInfo = new USERINFO;
	memcpy(m_sUserInfo, user, sizeof(USERINFO));
	memcpy(&m_sUserInfoOrg, user, sizeof(USERINFO));
}


void CPetDoc::SaveData()
{

	if( IsSetSaveFlag(eCF_PetInfo) )
	{
		PetInfoSaveDataSend();
	}

	m_dwSaveFlag = eCF_None;

	Sleep(3000);

	SetInitData(m_dwPetDBIdx); //  저장된 펫 index로 다시 검색

}

// 펫 인덱스 기준으로 데이타 저장
void CPetDoc::PetInfoSaveDataSend()
{

	CPetView* pView = (CPetView*)GetView(0);
	TMSG_PET_UPDATEBASEINFO Msg;
	Msg.Category = MP_RMTOOL_PET;
	Msg.Protocol = MP_RMTOOL_PET_UPDATE_BASEINFO;
	Msg.dwTemplateIdx = GetDocumentIdx();
	Msg.dwPetDBIdx = m_dwPetDBIdx; // 펫 인덱스
	
	pView->GetPetPage()->GetChangeBaseInfo(&Msg);
	
	NETWORK->Send(&Msg, sizeof(Msg));
}

void CPetDoc::UpdateData(DWORD dwParam1, DWORD dwParam2)
{
	CPetView* pView = (CPetView*) GetView(0);

	WORD pos = HIWORD(dwParam1);
	WORD kind = LOWORD(dwParam1);

	switch(kind)
	{
	case eTK_PetInfo:
	case eTK_PetItem:
		{
			pView->GetPetPage()->m_StatusEdit[pos].ResetData();
			SetSaveFlag(eCF_PetInfo);
		}
		break;
	}

}

void CPetDoc::SetInitData( DWORD dwPetDBIdx )
{
	m_dwPetDBIdx = dwPetDBIdx;

	// 서버에 검색 요청
	TMSG_PETSEARCH PetSearch;
	PetSearch.Category = MP_RMTOOL_PET;
	PetSearch.Protocol = MP_RMTOOL_PETSEARCH_SYN;
	PetSearch.dwTemplateIdx = m_dwDocumentIdx;
	PetSearch.wServer = MASINFOMGR->GetCurMasInfo()->nServerSet;
	PetSearch.dwPetIndex = m_dwPetDBIdx;

	NETWORK->Send(&PetSearch, sizeof(PetSearch));

}

ITEM_OPTION_INFO* CPetDoc::GetItemOptionInfo(DWORD dwOptionIdx)
{
	return m_ItemOptionInfo.GetData(dwOptionIdx);
}

ITEM_OPTION_INFO* CPetDoc::GetItemOptionInfoOrigin(DWORD dwOptionIdx)
{
	return m_ItemOptionInfo_org.GetData(dwOptionIdx);
}

ITEM_RARE_OPTION_INFO* CPetDoc::GetRareItemOptionInfo( DWORD dwRareIdx )
{
	return m_ItemRareOptionInfo.GetData(dwRareIdx);
}

void CPetDoc::DelLocalOptionInfo(DWORD dwIndex)
{
	ITEM_OPTION_INFO* pOption = m_ItemOptionInfo.GetData(dwIndex);

	if( pOption )
	{
		delete pOption;

		m_ItemOptionInfo.Remove(dwIndex);
		m_OptionKeyGenerator.ReleaseIndex(dwIndex);
	}
}

DWORD CPetDoc::AddLocalOptionInfo( ITEM_OPTION_INFO* pLocalOption, BOOL bNew )
{
	ITEM_OPTION_INFO* pOption = NULL;
	DWORD Index = 0;

	pOption = new ITEM_OPTION_INFO;
	memcpy(pOption, pLocalOption, sizeof(ITEM_OPTION_INFO));

	if( bNew )
		Index = CreateLocalOptionIndex();
	else
		Index = pLocalOption->dwOptionIdx;

	if( Index == 0 )
	{
		delete pOption;
		return 0;
	}

	pOption->dwOptionIdx = Index;

	m_ItemOptionInfo.Add(pOption, Index);

	return Index;
}

DWORD CPetDoc::CreateLocalOptionIndex()
{
	DWORD dwIndex = (DWORD)m_OptionKeyGenerator.GenerateIndex();

	while( m_ItemOptionInfo.GetData(dwIndex) )
	{
		dwIndex = (DWORD)m_OptionKeyGenerator.GenerateIndex();
		if( dwIndex == 0 )
		{
			ASSERT(0);
			return 0;
		}
	}
	
	return dwIndex;
}