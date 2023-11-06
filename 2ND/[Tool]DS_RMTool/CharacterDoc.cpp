// CharacterDoc.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "CharacterDoc.h"
#include "Characterinfo.h"
#include "CharacterView.h"
#include "RMNetwork.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharacterDoc

IMPLEMENT_DYNCREATE(CCharacterDoc, CBaseDoc)

CCharacterDoc::CCharacterDoc()
{
	m_dwLoadingFlag = eCL_None;
	m_dwSaveFlag = eCF_None;
	m_bReadOnly = FALSE;

	m_ItemOptionInfo_org.Initialize(100);
	m_ItemOptionInfo.Initialize(100);	

	m_ItemRareOptionInfo_org.Initialize(100);
	m_ItemRareOptionInfo.Initialize(100);
	
	m_OptionKeyGenerator.Init(2000, 101);

	m_dwUserIdx = m_dwCharIdx = 0;
}

BOOL CCharacterDoc::OnNewDocument()
{
	if (!CBaseDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CCharacterDoc::~CCharacterDoc()
{
	ITEM_OPTION_INFO* pOptionInfo = NULL;

	m_ItemOptionInfo_org.SetPositionHead();
	while(pOptionInfo = m_ItemOptionInfo_org.GetData())
		delete pOptionInfo;
	m_ItemOptionInfo_org.RemoveAll();

	m_ItemOptionInfo.SetPositionHead();
	while(pOptionInfo = m_ItemOptionInfo.GetData())
		delete pOptionInfo;
	m_ItemOptionInfo.RemoveAll();

	m_OptionKeyGenerator.Release();
}


BEGIN_MESSAGE_MAP(CCharacterDoc, CBaseDoc)
	//{{AFX_MSG_MAP(CCharacterDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharacterDoc diagnostics

#ifdef _DEBUG
void CCharacterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCharacterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCharacterDoc serialization

void CCharacterDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCharacterDoc commands

void CCharacterDoc::InitData()
{
	memset( &m_sBaseInfo, 0, sizeof(m_sBaseInfo) );
	memset( &m_sTotalInfo, 0, sizeof(m_sTotalInfo) );
	memset( &m_sHTotalInfo, 0, sizeof(m_sHTotalInfo) );

	memset( &m_sInven_org, 0, sizeof(m_sInven_org) );
	memset( &m_sMugong_org, 0, sizeof(m_sMugong_org) );
	memset( &m_sPyoguk_org, 0, sizeof(m_sPyoguk_org) );
	memset( &m_sAbility_org, 0, sizeof(m_sAbility_org) );
	memset( &m_sShopWare_org, 0, sizeof(m_sShopWare_org) );

	memset( &m_sInven, 0, sizeof(m_sInven) );
	memset( &m_sMugong, 0, sizeof(m_sMugong) );
	memset( &m_sPyoguk, 0, sizeof(m_sPyoguk) );
	memset( &m_sAbility, 0, sizeof(m_sAbility) );
	memset( &m_sShopWare, 0, sizeof(m_sShopWare) );
	
	memset( &m_InvenInfoError, 0, sizeof(m_InvenInfoError) );
	memset( &m_WareError, 0, sizeof(m_WareError) );
	memset( &m_MugongError, 0 , sizeof(m_MugongError) );
}

void CCharacterDoc::OpenData()
{
	memcpy( &m_sInven, &m_sInven_org, sizeof(ITEM_TOTALINFO) );
	memcpy( &m_sMugong, &m_sMugong_org, sizeof(MUGONG_TOTALINFO) );
	memcpy( &m_sPyoguk, &m_sPyoguk_org, sizeof(PHYOKUK_TOTALINFO) );
	memcpy( &m_sAbility, &m_sAbility_org, sizeof(ABILITY_TOTALINFO) );
	memcpy( &m_sShopWare, &m_sShopWare_org, sizeof(TMSG_CHARACTER_ITEMBASE) );

	// ��Ʈ�� �ʱ�ȭ ����..
	CCharacterView* pView = (CCharacterView*)GetView(0);
	pView->GetCharacterInfoPage()->InitControl(this);
}

void CCharacterDoc::UpdateData(DWORD dwParam1, DWORD dwParam2)
{
	CCharacterView* pView = (CCharacterView*) GetView(0);

	WORD pos = HIWORD(dwParam1);
	WORD kind = LOWORD(dwParam1);

	switch(kind)
	{
	case eTK_CharacterInfo:
		{
			pView->GetCharacterInfoPage()->m_StatusEdit[pos].ResetData();
			SetSaveFlag(eCF_CharacterInfo);
		}
		break;
	case eTK_PyogukInfo:
		{
			pView->GetCharacterInfoPage()->m_StatusEdit[pos].ResetData();
			SetSaveFlag(eCF_PyogukInfo);
		}
		break;
	case eTK_AbilityInfo:
		{
			SetSaveFlag(eCF_AbilityInfo);
		}
		break;
	}
}

void CCharacterDoc::SaveData()
{
	if( IsSetSaveFlag(eCF_CharacterInfo) )
	{
		CharacterInfoSaveDataSend();
	}
	if( IsSetSaveFlag(eCF_PyogukInfo) )
	{
		PyogukInfoSaveDataSend();
	}
	if( IsSetSaveFlag(eCF_AbilityInfo) )
	{
		AbilitySaveDataSend();
	}

	m_dwSaveFlag = eCF_None;
	Sleep(3000);
	InitData();
	OpenCharacterInfo( 0, m_dwUserIdx, m_dwCharIdx );
//	CCharacterView* pView = (CCharacterView*)GetView(0);
//	(pView->GetParentFrame())->SendMessage(WM_CLOSE);
}

void CCharacterDoc::CharacterInfoSaveDataSend()
{
	CCharacterView* pView = (CCharacterView*)GetView(0);
	TMSG_CHARACTER_UPDATEBASEINFO Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_UPDATE_BASEINFO;
	Msg.dwTemplateIdx = GetDocumentIdx();
	Msg.CharIdx = m_sBaseInfo.dwObjectID;

	pView->GetCharacterInfoPage()->GetChangeBaseInfo(&Msg);
	
	NETWORK->Send(&Msg, sizeof(Msg));
}

void CCharacterDoc::PyogukInfoSaveDataSend()
{
	CCharacterView* pView = (CCharacterView*)GetView(0);
	TMSG_CHARACTER_PYOGUKINFO Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_UPDATE_PYOGUKINFO;
	Msg.dwTemplateIdx = GetDocumentIdx();
	Msg.m_dwUserIdx = m_sBaseInfo.dwUserID;
	strcpy( Msg.m_sUserName, m_sBaseInfo.ObjectName );

	pView->GetCharacterInfoPage()->GetChangePyogukInfo(&Msg);

	NETWORK->Send(&Msg, sizeof(Msg));
}

void CCharacterDoc::AbilitySaveDataSend()
{
	TMSG_CHARACTER_UPDATE_ABILITY Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_UPDATE_ABILLITYINFO;
	Msg.dwTemplateIdx = GetDocumentIdx();
	Msg.dwCharIdx = m_sBaseInfo.dwObjectID;
	strcpy( Msg.sName, m_sBaseInfo.ObjectName );
	memcpy(&Msg.sAbility, &m_sAbility, sizeof(ABILITY_TOTALINFO));

	NETWORK->Send(&Msg, sizeof(Msg));
}

void CCharacterDoc::NetworkMsgParse(BYTE Protocol, void* pMsg)
{
	switch(Protocol)
	{
	case MP_RMTOOL_CHARACTER_BASEINFO:
		{
			TMSG_CHARACTER_BASEINFO* msg = (TMSG_CHARACTER_BASEINFO*)pMsg;
			SetCharacterBaseInfo(&msg->m_sObjinfo, &msg->m_sTotalinfo, &msg->m_sHeroinfo);
		}
		break;
	case MP_RMTOOL_CHARACTER_INVENINFO:
		{
			TMSG_CHARACTER_ITEMBASE* pmsg = (TMSG_CHARACTER_ITEMBASE*)pMsg;
			SetCharacterInvenInfo( pmsg );
		}
		break;
	case MP_RMTOOL_CHARACTER_INVENINFO2:
		{
			TMSG_CHARACTER_ITEMBASE* msg = (TMSG_CHARACTER_ITEMBASE*)pMsg;
			SetCharacterInvenInfo2( msg );
		}
		break;
	case MP_RMTOOL_CHARACTER_PYOGUKINFO:
		{
			TMSG_CHARACTER_PYOGUKINFO* msg = (TMSG_CHARACTER_PYOGUKINFO*)pMsg;
			SetCharacterPyogukInfo(msg->m_wPyogukNum, msg->m_dwMoney);
		}
		break;
	case MP_RMTOOL_CHARACTER_PYOGUKITEMINFO:
		{
			TMSG_ITEMINFO_LARGECOUNT* msg = (TMSG_ITEMINFO_LARGECOUNT*)pMsg;
			SetCharacterPyogukItemInfo(msg->m_wCount, msg->m_Item, msg->m_bEnd );
		}
		break;
	case MP_RMTOOL_CHARACTER_SHOPWARE_ITEMINFO:
		{
			TMSG_CHARACTER_ITEMBASE* pmsg = (TMSG_CHARACTER_ITEMBASE*)pMsg;
			SetCharacterShopWareItemInfo( pmsg );
		}
		break;
	case MP_RMTOOL_CHARACTER_MUGONGINFO:
		{
			TMSG_CHARACTER_MUGONGINFO* msg = (TMSG_CHARACTER_MUGONGINFO*)pMsg;
			SetCharacterMugongInfo(&msg->m_sMugongInfo);
		}
		break;
	case MP_RMTOOL_CHARACTER_MUGONGINFO2:
		{
			TMSG_CHARACTER_MUGONGINFO2* msg = (TMSG_CHARACTER_MUGONGINFO2*)pMsg;
			SetCharacterMugongInfoError( msg );
		}
		break; 
	case MP_RMTOOL_CHARACTER_ABILLITYINFO:
		{
			TMSG_CHARACTER_ABILITYINFO* msg = (TMSG_CHARACTER_ABILITYINFO*)pMsg;
			SetCharacterAbillityInfo(&msg->sAbility);
		}
		break;
	case MP_RMTOOL_CHARACTERINFO_ITEMOPTIONINFO:
		{
			TMSG_ITEMOPTIONINFO* msg = (TMSG_ITEMOPTIONINFO*)pMsg;
			SetCharacterItemOptionInfo( msg->wCount, msg->OptionInfo, msg->bEnd );
		}
		break;
	case MP_RMTOOL_CHARACTERINFO_NACK:
		{
		}
		break;		
	case MP_RMTOOL_CHARACTER_MAINQUESTINFO_ACK:
		{
			TMSG_MAINQUESTINFO* pmsg = (TMSG_MAINQUESTINFO*)pMsg;
			CCharacterView* pView = (CCharacterView*)GetView(0);
			pView->UpdateMainQuestData( pmsg );
		}
		break;
	case MP_RMTOOL_CHARACTER_SUBQUESTINFO_ACK:
		{
			TMSG_SUBQUESTINFO* pmsg = (TMSG_SUBQUESTINFO*)pMsg;
			CCharacterView* pView = (CCharacterView*)GetView(0);
			pView->UpdateSubQuestData( pmsg );
		}
		break;
	case MP_RMTOOL_CHARACTER_CHANGE_LOGINPOINT_ACK:
		{
			TMSG_DWORD2* pmsg = (TMSG_DWORD2*)pMsg;
			CCharacterView* pView = (CCharacterView*)GetView(0);
			pView->ChangeLoginPoint( pmsg->dwData2 );
		}
		break;
	case MP_RMTOOL_CHARACTER_CHANGE_LOGINPOINT_NACK:
		{
		}
		break;
	case MP_RMTOOL_CHARACTERINFO_RAREITEMOPTIONINFO:
		{
			TMSG_RAREITEMOPTIONINFO* pmsg = (TMSG_RAREITEMOPTIONINFO*)pMsg;
			SetCharacterRareItemOptionInfo( pmsg->wCount, pmsg->RareOptionInfo, pmsg->bEnd );			
		}
		break;
	case MP_RMTOOL_CHARACTER_USINGSHOPITEMINFO_SYN:
		{
			TMSG_USINGSHOPITEMINFO* pmsg = (TMSG_USINGSHOPITEMINFO*)pMsg;
			CCharacterView* pView = (CCharacterView*)GetView(0);
			pView->UpdateUsingShopItemInfo( pmsg );
		}
		break;
	case MP_RMTOOL_CHARACTER_MAINPETLIST_ACK:
		{
			TMSG_MAINPETLIST* pmsg = (TMSG_MAINPETLIST*)pMsg;
			CCharacterView* pView = (CCharacterView*)GetView(0);
			pView->UpdateMainPetListData( pmsg );
		}
		break;
	}
}

void CCharacterDoc::OpenCharacterInfo(DWORD dwServer, DWORD dwUserIdx, DWORD dwCharIdx, BOOL bReadOnly)
{
	m_dwUserIdx = dwUserIdx;
	m_dwCharIdx = dwCharIdx;
	m_bReadOnly = bReadOnly;

	// ĳ���� ���� ��û..
	TMSG_DWORD3 Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTERINFO_SYN;
	Msg.dwTemplateIdx = m_dwDocumentIdx;
	Msg.dwData1 = dwServer;			//���� ����
	Msg.dwData2 = dwUserIdx;		//���� �ε���
	Msg.dwData3 = dwCharIdx;		//ĳ���� �ε���

	NETWORK->Send(&Msg, sizeof(Msg));
}

//-------------------------------------------------------------------
// set fn
//-------------------------------------------------------------------
void CCharacterDoc::SetCharacterBaseInfo(BASEOBJECT_INFO* objInfo, CHARACTER_TOTALINFO* totalInfo, HERO_TOTALINFO* heroInfo)
{
	memcpy(&m_sBaseInfo, objInfo, sizeof(BASEOBJECT_INFO));
	memcpy(&m_sTotalInfo, totalInfo, sizeof(CHARACTER_TOTALINFO));
	memcpy(&m_sHTotalInfo, heroInfo, sizeof(HERO_TOTALINFO));

	SetLoadingFlag(eCL_BaseInfo);
}

void CCharacterDoc::SetCharacterInvenInfo( TMSG_CHARACTER_ITEMBASE* info )
{
	POSTYPE ItemPos = 0;
	for( DWORD i = 0; i < info->dwCount; ++i )
	{
		ItemPos = info->ItemInfo[i].Position;
		if( ItemPos >= TP_INVENTORY_START && ItemPos < TP_INVENTORY_END )
		{
			ItemPos -= TP_INVENTORY_START;
			if( m_sInven_org.Inventory[ItemPos].dwDBIdx == 0 )
				m_sInven_org.Inventory[ItemPos] = info->ItemInfo[i];
			else
				m_InvenInfoError.Add( &info->ItemInfo[i] );
		}
		else if( ItemPos >= TP_WEAR_START && ItemPos < TP_WEAR_END )
		{
			ItemPos -= TP_WEAR_START;
			if( m_sInven_org.WearedItem[ItemPos].dwDBIdx == 0 )
				m_sInven_org.WearedItem[ItemPos] = info->ItemInfo[i];
			else
				m_InvenInfoError.Add( &info->ItemInfo[i] );
		}
		else if( ItemPos >= TP_SHOPINVEN_START && ItemPos < TP_SHOPINVEN_END )
		{
			ItemPos -= TP_SHOPINVEN_START;
			if( m_sInven_org.ShopInventory[ItemPos].dwDBIdx == 0 )
				m_sInven_org.ShopInventory[ItemPos] = info->ItemInfo[i];
//			else
//				m_InvenInfoError.Add( &info->ItemInfo[i] );
		}
	}

	SetLoadingFlag(eCL_InvenInfo);
}

void CCharacterDoc::SetCharacterInvenInfo2(TMSG_CHARACTER_ITEMBASE* info)
{
	memcpy(&m_InvenInfoError, info, info->GetSize());
}

void CCharacterDoc::SetCharacterPyogukInfo(WORD wNum, MONEYTYPE money)
{
	m_sPyoguk_org.PyokukCount = wNum;
	m_sPyoguk_org.Money = money;

	if( wNum == 0 )	SetLoadingFlag(eCL_PyogukInfo | eCL_PyogukItemInfo);
	else			SetLoadingFlag(eCL_PyogukInfo);
}

void CCharacterDoc::SetCharacterPyogukItemInfo(WORD wCount, ITEMBASE* item, BOOL bEnd)
{
	WORD count = 0;
	for(WORD i=0;i<wCount;++i)
	{
		POSTYPE itempos = item[i].Position - TP_PYOGUK_START;

		if( m_sPyoguk_org.Pyokuk[itempos].dwDBIdx == 0 )
		{
			m_sPyoguk_org.Pyokuk[itempos].wIconIdx = item[i].wIconIdx;
			m_sPyoguk_org.Pyokuk[itempos].dwDBIdx = item[i].dwDBIdx;
			m_sPyoguk_org.Pyokuk[itempos].Position = item[i].Position;
			m_sPyoguk_org.Pyokuk[itempos].Durability = item[i].Durability;
			m_sPyoguk_org.Pyokuk[itempos].QuickPosition	= item[i].QuickPosition;
			m_sPyoguk_org.Pyokuk[itempos].RareIdx = item[i].RareIdx;
		}
		else
		{
			m_WareError.Pyokuk[count].wIconIdx = item[i].wIconIdx;
			m_WareError.Pyokuk[count].dwDBIdx = item[i].dwDBIdx;
			m_WareError.Pyokuk[count].Position = item[i].Position;
			m_WareError.Pyokuk[count].Durability = item[i].Durability;
			m_WareError.Pyokuk[count].QuickPosition	= item[i].QuickPosition;
			m_WareError.Pyokuk[count].RareIdx = item[i].RareIdx;

			++count;
			m_WareError.PyokukCount = count;
		}
	}

	if( bEnd )
		SetLoadingFlag(eCL_PyogukItemInfo);
}

void CCharacterDoc::SetCharacterMugongInfo(MUGONG_TOTALINFO* info)
{
	memcpy(&m_sMugong_org, info, sizeof(MUGONG_TOTALINFO));

	SetLoadingFlag(eCL_MugongInfo);
}

void CCharacterDoc::SetCharacterMugongInfoError( TMSG_CHARACTER_MUGONGINFO2* pinfo )
{
	memcpy(&m_MugongError, pinfo, sizeof(TMSG_CHARACTER_MUGONGINFO2));

	SetLoadingFlag(eCL_MugongInfo);
}

void CCharacterDoc::SetCharacterAbillityInfo(ABILITY_TOTALINFO* info)
{
	memcpy(&m_sAbility_org, info, sizeof(ABILITY_TOTALINFO));

	SetLoadingFlag(eCL_AbillityInfo);
}

void CCharacterDoc::SetCharacterItemOptionInfo(WORD wCount, ITEM_OPTION_INFO* info, BOOL bEnd)
{
	ITEM_OPTION_INFO* pOption = NULL;
	ITEM_OPTION_INFO* pOption2 = NULL;
	for(int i=0;i<wCount;++i)
	{
		pOption = new ITEM_OPTION_INFO;
		pOption2 = new ITEM_OPTION_INFO;
		memcpy(pOption, &info[i], sizeof(ITEM_OPTION_INFO));
		memcpy(pOption2, &info[i], sizeof(ITEM_OPTION_INFO));
		m_ItemOptionInfo_org.Add(pOption, pOption->dwOptionIdx);
		m_ItemOptionInfo.Add(pOption2, pOption2->dwOptionIdx);
	}
	if( bEnd )
		SetLoadingFlag(eCL_ItemOptionInfo);
}

void CCharacterDoc::SetCharacterShopWareItemInfo( TMSG_CHARACTER_ITEMBASE* info )
{
	for( DWORD i = 0; i < info->dwCount; ++i )
	{
		m_sShopWare_org.Item[i] = info->ItemInfo[i];
		m_sShopWare_org.Item[i].Position = (POSTYPE)(i + TP_SHOPITEM_START);
	}

	SetLoadingFlag( eCL_ShopWareItemInfo );
}

void CCharacterDoc::SetCharacterRareItemOptionInfo( WORD wCount, ITEM_RARE_OPTION_INFO* info, BOOL bEnd )
{
	ITEM_RARE_OPTION_INFO* pRareOption = NULL;
	ITEM_RARE_OPTION_INFO* pRareOption2 = NULL;
	for( int i = 0; i < wCount; ++i )
	{
		pRareOption = new ITEM_RARE_OPTION_INFO;
		pRareOption2 = new ITEM_RARE_OPTION_INFO;
		memcpy( pRareOption, &info[i], sizeof(ITEM_RARE_OPTION_INFO) );
		memcpy( pRareOption2, &info[i], sizeof(ITEM_RARE_OPTION_INFO) );
		m_ItemRareOptionInfo_org.Add( pRareOption, pRareOption->dwRareOptionIdx );
		m_ItemRareOptionInfo.Add( pRareOption2, pRareOption2->dwRareOptionIdx );		
	}

	if( bEnd )
		SetLoadingFlag( eCL_ItemRareOptionInfo );
}

//-------------------------------------------------------------------
BYTE CCharacterDoc::GetCharacterAbillityLevel(BYTE kind, WORD Index)
{
	BYTE Level;
	char* pLevels;

	pLevels = m_sAbility.AbilityDataArray[kind];

	WORD n = Index - (ABILITY_STARTINDEX_BATTLE + ABILITY_STARTINDEX_INTERVAL*kind);
	ASSERT(n<MAX_ABILITY_NUM_PER_GROUP);

	Level = GetAbilityLevel_WithDBValue(pLevels[n]);

	return Level;
}

void CCharacterDoc::SetAbilityLevel(BYTE kind, WORD Index, char level)
{
	char* pLevels;
	pLevels = m_sAbility.AbilityDataArray[kind];

	WORD n = Index - (ABILITY_STARTINDEX_BATTLE + ABILITY_STARTINDEX_INTERVAL*kind);
	ASSERT(n<MAX_ABILITY_NUM_PER_GROUP);

	pLevels[n] = level;
}

ITEM_OPTION_INFO* CCharacterDoc::GetItemOptionInfo(DWORD dwOptionIdx)
{
	return m_ItemOptionInfo.GetData(dwOptionIdx);
}

ITEM_OPTION_INFO* CCharacterDoc::GetItemOptionInfoOrigin(DWORD dwOptionIdx)
{
	return m_ItemOptionInfo_org.GetData(dwOptionIdx);
}

ITEM_RARE_OPTION_INFO* CCharacterDoc::GetRareItemOptionInfo( DWORD dwRareIdx )
{
	return m_ItemRareOptionInfo.GetData(dwRareIdx);
}

DWORD CCharacterDoc::CreateLocalOptionIndex()
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

DWORD CCharacterDoc::AddLocalOptionInfo( ITEM_OPTION_INFO* pLocalOption, BOOL bNew )
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

void CCharacterDoc::DelLocalOptionInfo(DWORD dwIndex)
{
	ITEM_OPTION_INFO* pOption = m_ItemOptionInfo.GetData(dwIndex);

	if( pOption )
	{
		delete pOption;

		m_ItemOptionInfo.Remove(dwIndex);
		m_OptionKeyGenerator.ReleaseIndex(dwIndex);
	}
}

void CCharacterDoc::ChangeLoginPoint( DWORD dwLoginPoint )
{
	TMSG_NAME_DWORD Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_CHANGE_LOGINPOINT_SYN;
	Msg.dwTemplateIdx = GetDocumentIdx();
	Msg.dwChrIdx = m_sBaseInfo.dwObjectID;
	strcpy( Msg.sName, m_sBaseInfo.ObjectName );
	Msg.dwData = dwLoginPoint;
	
	NETWORK->Send( &Msg, sizeof(Msg) );
}
