// RarenessManager.cpp: implementation of the RarenessManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RarenessManager.h"
#include "MHFile.h"
#include "ItemManager.h"
#include "..\[CC]Header\GameResourceStruct.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(RarenessManager)

RarenessManager::RarenessManager()
{
	Init();
}

RarenessManager::~RarenessManager()
{
	Release();
}

void RarenessManager::Init()
{
	m_RareItemInfoTable.Initialize(64);
}

void RarenessManager::Release()
{
	ReleaseRareItemInfo();
}

bool RarenessManager::LoadRareItemOptionInfo()
{
	CMHFile fp;

	char szBuf[256] = {0,};
	char line[512];

	char FileName[256];

	sprintf(FileName, "System/Resource/Item_RareOptionInfo.bin");
	if( !fp.Init( FileName, "rb" ) )
		return FALSE;

	DWORD dwItemKindIdx = 0;
	while( !fp.IsEOF() )
	{
		if(dwItemKindIdx == RareItemKindMAX)
			break;

		fp.GetString(szBuf);

		if( szBuf[0] == '@' )
		{
			fp.GetLineX(line, 512);
			continue;
		}
		
		if( szBuf[0] == '#' )
		{
			m_RereItemInfo[dwItemKindIdx].str_min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].Str = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].dex_min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].Dex = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].vit_min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].Vit = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].wis_min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].Wis = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].Life_Min = fp.GetDword();
			m_RereItemInfo[dwItemKindIdx].Life = fp.GetDword();
			m_RereItemInfo[dwItemKindIdx].mana_min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].Mana = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].PhyAttack_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].PhyAttack = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].PhyDefense_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].PhyDefense = fp.GetWord();
			for(int i = ATTR_FIRE; i <= ATTR_MAX; ++i )
			{
			m_RereItemInfo[dwItemKindIdx].AttrAttack_Min.SetElement_Val( i, fp.GetFloat() );
			m_RereItemInfo[dwItemKindIdx].AttrAttack.SetElement_Val( i, fp.GetFloat() );
			}
			for(int j = ATTR_FIRE; j <= ATTR_MAX; ++j )
			{
			m_RereItemInfo[dwItemKindIdx].AttrRegist_Min.SetElement_Val( j, fp.GetFloat() );
			m_RereItemInfo[dwItemKindIdx].AttrRegist.SetElement_Val( j, fp.GetFloat() );
			}

			dwItemKindIdx++;
		}

	}

	return TRUE;
}

bool RarenessManager::LoadRareItemInfo()
{
	CMHFile fp;

	char szBuf[256] = {0,};
	char line[512];

	char FileName[256];

	sprintf(FileName, "System/Resource/Item_RareItemInfo.bin");
	if( !fp.Init(FileName,"rb") ) return FALSE;

	while( !fp.IsEOF() )
	{
		fp.GetString(szBuf);

		if( szBuf[0] == '@' )
		{
			fp.GetLineX(line,512);
			continue;
		}

//		strupr(szBuf);

		if( szBuf[0] == '*' )
		{
			sRareItemInfo* pRareItemInfo = new sRareItemInfo;
			pRareItemInfo->ItemIdx = fp.GetDword();
			pRareItemInfo->RareProb = fp.GetDword();

			ASSERT(!m_RareItemInfoTable.GetData(pRareItemInfo->ItemIdx));
			m_RareItemInfoTable.Add(pRareItemInfo, pRareItemInfo->ItemIdx);
		}
	}

	return TRUE;
}

bool RarenessManager::LoadRareValueProbList()
{
	CMHFile fp;

	char FileName[256];
	char szBuf[256] = {0,};

	sprintf(FileName, "System/Resource/Item_RareStatSetRate.bin");
	if(!fp.Init(FileName,"rb")) return FALSE;

	fp.GetLine(szBuf,256); //@

	DWORD Index = 1;
	while( !fp.IsEOF() )
	{
		if(Index == RARE_VALUE_PROB_LIST_MAX) break;
		
		Index	= fp.GetDword();	//1~100
		m_dwRareValueProb[Index - 1] = fp.GetDword();
	}

	return TRUE;
}

void RarenessManager::ReleaseRareItemInfo()
{
	sRareItemInfo* pList = NULL;
	m_RareItemInfoTable.SetPositionHead();
	while( pList = m_RareItemInfoTable.GetData() )
	{
		delete pList;
		pList = NULL;
	}
	m_RareItemInfoTable.RemoveAll();
}

bool RarenessManager::GetRare( DWORD ObtainItemIdx, ITEM_RARE_OPTION_INFO* pRareOptionInfo, BOOL bRare )
{
	if(ObtainItemIdx > BOUNDARY_INDEX_FOR_SHOPITEM) return FALSE;	//WeaponType �� �� �����۰� ���̾��� �ִ�.

	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( ObtainItemIdx );
	if( !pInfo ) return FALSE; //��ϵ� �������� �ƴϸ� ����

//	eITEM_KINDBIT eItemKind = GetItemKind( ObtainItemIdx );	//!���� Ȯ��
//	if( !(eItemKind & eEQUIP_ITEM) ) return FALSE;	//������ �������� �ƴϸ� ����

	DWORD ItemIdx = ObtainItemIdx;
	//�ش� �������� ���� ���� Ȯ���� ������ ��.
/*	if( eItemKind == eEQUIP_ITEM_WEAPON 
		|| eItemKind == eEQUIP_ITEM_DRESS
		|| eItemKind == eEQUIP_ITEM_HAT
		|| eItemKind == eEQUIP_ITEM_SHOES )
	{
		ItemIdx /= 10;
		ItemIdx *= 10;
	}	//�Ǽ��縮�� ������ +���� �����۵��� ����Ʈ �ε����� �ϳ��� ����. ���� ����.
*/
	sRareItemInfo* RareInfo = m_RareItemInfoTable.GetData(ItemIdx);

	if( !RareInfo ) return FALSE;
//!!!+
//	if( (rand()%10000) * 10000 + (rand()%10000 + 1) < RareInfo->RareProb )

	BOOL baa = ( (DWORD)((rand()%10000) * 10000 + (rand()%10000 + 1)) <= (RareInfo->RareProb) );
/*	if( baa || bRare )	//�����!!!
	{
		DWORD RareItemKind = 0;	//��� �迭�� ���ڷ� ���δ�.
		if( eItemKind == eEQUIP_ITEM_WEAPON )
		{
			RareItemKind = pInfo->WeaponType - 1;
		}
		else if( eItemKind == eEQUIP_ITEM_DRESS )			RareItemKind = eRI_DRESS;
		else if( eItemKind == eEQUIP_ITEM_HAT )				RareItemKind = eRI_HAT;
		else if( eItemKind == eEQUIP_ITEM_SHOES )			RareItemKind = eRI_SHOES;
		else if( eItemKind == eEQUIP_ITEM_RING )			RareItemKind = eRI_RING;
		else if( eItemKind == eEQUIP_ITEM_CAPE )			RareItemKind = eRI_CAPE;
		else if( eItemKind == eEQUIP_ITEM_NECKLACE )		RareItemKind = eRI_NECKLACE;
		else if( eItemKind == eEQUIP_ITEM_ARMLET )			RareItemKind = eRI_ARMLET;
		else if( eItemKind == eEQUIP_ITEM_BELT )			RareItemKind = eRI_BELT;

		if( m_RereItemInfo[RareItemKind].Str || m_RereItemInfo[RareItemKind].Dex || m_RereItemInfo[RareItemKind].Vit || m_RereItemInfo[RareItemKind].Wis )	//���� ��ű� ����
		{
			switch( rand()%StatesKindMAX )
			{
			case eRI_Str:
				pRareOptionInfo->Str = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].str_min, (DWORD)m_RereItemInfo[RareItemKind].Str );
				break;
			case eRI_Dex:
				pRareOptionInfo->Dex = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].dex_min, (DWORD)m_RereItemInfo[RareItemKind].Dex );
				break;
			case eRI_Vit:
				pRareOptionInfo->Vit = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].vit_min, (DWORD)m_RereItemInfo[RareItemKind].Vit );
				break;
			case eRI_Wis:
				pRareOptionInfo->Wis = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].wis_min, (DWORD)m_RereItemInfo[RareItemKind].Wis );
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		else
		{	//!!!SW050928 �� �̷��� ����?
//			ASSERTMSG(0, "RareItem : Not All RareOption_StatesSet on Script");

			pRareOptionInfo->Str = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].str_min, (DWORD)m_RereItemInfo[RareItemKind].Str );
			pRareOptionInfo->Dex = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].dex_min, (DWORD)m_RereItemInfo[RareItemKind].Dex );
			pRareOptionInfo->Vit = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].vit_min, (DWORD)m_RereItemInfo[RareItemKind].Vit );
			pRareOptionInfo->Wis = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].wis_min, (DWORD)m_RereItemInfo[RareItemKind].Wis );
		}

		if( eItemKind == eEQUIP_ITEM_DRESS || eItemKind == eEQUIP_ITEM_HAT || eItemKind == eEQUIP_ITEM_SHOES )	//���� �� ����
		{
			pRareOptionInfo->Life = GetRareItemValue( m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life );
			pRareOptionInfo->Mana = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].mana_min, (DWORD)m_RereItemInfo[RareItemKind].Mana );
		}
		else if( m_RereItemInfo[RareItemKind].Life && m_RereItemInfo[RareItemKind].Mana )	//���� ���� ����
		{
			switch( rand()%ValueKindMAX )
			{
			case eRI_LIFE:
				pRareOptionInfo->Life = GetRareItemValue( m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life );
				break;
			case eRI_MANA:
				pRareOptionInfo->Mana = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].mana_min, (DWORD)m_RereItemInfo[RareItemKind].Mana );
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		else
		{
//			ASSERTMSG(0, "RareItem : Not All RareOption_AddValue on Script");

			pRareOptionInfo->Life = GetRareItemValue( m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life );
			pRareOptionInfo->Mana = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].mana_min, (DWORD)m_RereItemInfo[RareItemKind].Mana );
		}
		
		pRareOptionInfo->PhyAttack = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].PhyAttack_Min, (DWORD)m_RereItemInfo[RareItemKind].PhyAttack );
		pRareOptionInfo->PhyDefense = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].PhyDefense_Min, (DWORD)m_RereItemInfo[RareItemKind].PhyDefense );

		WORD i = rand()%ATTR_MAX + 1;
		
		if(rand()%2)	//���� or ���
		{
			pRareOptionInfo->AttrAttack.SetElement_Val( i, (float)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].AttrAttack_Min.GetElement_Val(i)
				, (DWORD)(m_RereItemInfo[RareItemKind].AttrAttack.GetElement_Val(i))) / 100.f );
		}
		else
		{
			pRareOptionInfo->AttrRegist.SetElement_Val( i, (float)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].AttrRegist_Min.GetElement_Val(i)
				, (DWORD)(m_RereItemInfo[RareItemKind].AttrRegist.GetElement_Val(i))) / 100.f );
		}

		return TRUE;
	}
	else
*/		return FALSE;
}

DWORD RarenessManager::GetRareItemValue(DWORD RndMin, DWORD RndMax)
{
	if( !RndMin || !RndMax ) return 0; //�ִ� �ּ� ���� �ϳ��� 0 �̸� ����

	if( RndMin == RndMax ) return RndMax;	//���� ��ġ�� �ٴ� �ɼ��� ��ũ��Ʈ �󿡼� �ּ�/�ִ� �� ���� ����.

	ASSERTMSG(RndMax>RndMin, "Check RareOptionInfo File!");

	DWORD RndValue = ((rand()%10000) * 10000 + (rand()%10000 + 1));

	DWORD Index = 0;
	while(RndValue < m_dwRareValueProb[Index])
	{
		Index++;
	}

	//!�ӽ�
	DWORD XRndValue = (DWORD)((RndMax - RndMin) * Index / RARE_VALUE_PROB_LIST_MAX ) + RndMin;
	return XRndValue;
	//return (DWORD)((RndMax - RndMin) * Index / RARE_VALUE_PROB_LIST_MAX ) + RndMin;
}
