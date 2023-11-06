// KyungGongInfo.cpp: implementation of the CKyungGongInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KyungGongInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKyungGongInfo::CKyungGongInfo()
{

}

CKyungGongInfo::~CKyungGongInfo()
{

}

void CKyungGongInfo::Init(CMHFile* pFile)
{	
	m_Info.KyungGongIdx = pFile->GetWord();		//���� index
//	pFile->GetString(m_Info.KyungGongName);		//����̸�
	SafeStrCpy( m_Info.KyungGongName, pFile->GetString(), MAX_NAME_LENGTH+1 );
	m_Info.NeedMana = pFile->GetWord();		//�Ҹ� ����
	m_Info.MoveType = pFile->GetWord();			//�̵� Ÿ��
	m_Info.Speed = pFile->GetFloat();			//�̵� �ӵ�
	m_Info.ChangeTime = pFile->GetWord();		//��� ��� ��ȯ �ð�
	m_Info.StartEffect = FindEffectNum(pFile->GetString());		//��� ��� ����
	m_Info.IngEffect = FindEffectNum(pFile->GetString());			//��� ���� ����
	m_Info.EndEffect = FindEffectNum(pFile->GetString());			//��� �� ����
}