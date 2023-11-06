// cJackpotDialog.cpp: implementation of the cJackpotDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "cJackpotDialog.h"
//#include "WindowIDEnum.h"
//#include "cScriptManager.h"
//#include "cJackpotManager.h"
//#include "MHTimeManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//LONG NumImgRelpos[] =
//{
//	149, 22,
//	133, 22,
//	116, 22,
//	95, 22,
//	79, 22,
//	62, 22,
//	41, 22,
//	25, 22,
//	8, 22,
//};

//LONG NumImgRelpos[] =
//{
//	149, 22,
//	133, 22,
//	116, 22,
//	95, 22,
//	79, 22,
//	62, 22,
//	41, 22,
//	25, 22,
//	8, 22,
//};

//cJackpotDialog::cJackpotDialog()
//{
//	m_pBtnClose = NULL;
//	m_dwTotalMoney = 999;
//}
//
//cJackpotDialog::~cJackpotDialog()
//{
//	Release();
//}
//
//void cJackpotDialog::InitNumImage()
//{
//	cImage* pImg;
//	for( int n = 0; n < NUM_COUNT; ++n )
//	{
//		pImg = new cImage;
//		SCRIPTMGR->GetImage(n, pImg, PFT_JACKPOTPATH);
//		m_stNumImage[n].pImage = pImg;
//		m_stNumImage[n].dwW = NUMIMAGE_W;
//		m_stNumImage[n].dwH = NUMIMAGE_H;
//	}
//}
//
//void cJackpotDialog::ReleaseNumImage()
//{
//	for( int n = 0; n < NUM_COUNT; ++n )
//	{
//		if(m_stNumImage[n].pImage)
//		{
//			delete m_stNumImage[n].pImage;
//			m_stNumImage[n].pImage = NULL;
//		}
//	}
//}
//
//void cJackpotDialog::Linking()
//{
//	Init();
//	InitNumImage();	
//
//	m_pBtnClose		= (cButton*)GetWindowForID( CMI_CLOSEBTN );
//}
//
//void cJackpotDialog::SetNumImagePos()
//{
//	float fDialPosX = this->GetAbsX();
//	float fDialPosY = this->GetAbsY();
//
//	for( int i = 0; i < CIPHER_NUM; ++i )
//	{
//		m_vPos[i].x = fDialPosX + (float)NumImgRelpos[i*2];
//		m_vPos[i].y = fDialPosY + (float)NumImgRelpos[i*2+1];
//	}
//}
//
//void cJackpotDialog::ConvertCipherNum()
//{// ���ڸ� ���ڸ� ���Ѵ�. Get Number per Each Cipher
//	// ���� ��ȭ�� ���� �ִϸ� �Ѵ�. Do Animations When Number Changers
//
//	int n = 0;	// n�ڸ���
//	DWORD d = 1;	// ����
//
//	DWORD money = m_dwTotalMoney;
//
//	if( !money ) return;
//	while( money / d > 0 )
//	{//	�ڸ��� ���ϱ� n
//		d *= 10;
//		n++;
//	}
//
//	ASSERT(n<CIPHER_NUM);
//	d /= 10;
//
//	m_dwMaxCipher = n;	// Need in ChangingAni
//
//	for( int i = 1; i <= n; ++i )
//	{// ū�ڸ� ����
//		m_stCipherNum[n-i].dwNumber = money / d;
//		m_stCipherNum[n-i].bIsAni = TRUE;
//		money = m_dwTotalMoney % d;
//		d /= 10;
//	}
//
//	while( n < CIPHER_NUM )
//	{
//		m_stCipherNum[n].dwNumber = DEFAULT_IMAGE;
//		m_stCipherNum[n].bIsAni = FALSE;
//		n++;
//	}
//}
//
//bool cJackpotDialog::IsNumChanged()					// ���� ��ȭ üũ	// Check Num charnges
//{
//	if(m_dwOldTotalMoney != m_dwTotalMoney)
//	{
//		if( m_dwTotalMoney < m_dwOldTotalMoney )
//		{
//			m_bDoSequenceAni = FALSE;
//		}
//		else m_bDoSequenceAni = TRUE;
//		
//		m_dwTempMoney = m_dwOldTotalMoney;	// For SequenceAni
//
//		m_dwOldTotalMoney = m_dwTotalMoney;	// For CompareChanges
//		return TRUE;
//	}
//	else
//		return FALSE;
//}
//
//void cJackpotDialog::DoAni()				// ���� ��ȯ �ִ�	// Ticktime control
//{
//	if(!m_bIsAnimationing) return;
//
//	DWORD curtime = gCurTime;
//
//	if( curtime - m_dwNumChangeTime > NUM_CHANGE_TIMELENGTH )
//	{
//		for( int i = 0; i < CIPHER_NUM; ++i )
//		{
//			if( m_stCipherNum[i].bIsAni )
//			{
//				m_stCipherNum[i].dwNumber++;
//				if(m_stCipherNum[i].dwNumber > 9)
//				{
//					m_stCipherNum[i].dwNumber = 0;
//				}
//			}
//		}
//
////		m_dwNumChangeTime = curtime;
//	}
//
//	if( curtime - m_dwAniStartTime < BASIC_ANI_TIMELENGTH ) return;
//
//	if( curtime - m_dwIntervalAniTime > BETWEEN_ANI_TIMELENGTH )
//	{
//		if(m_stCipherNum[m_dwCipherCount].bIsAni)
//		{
//			m_stCipherNum[m_dwCipherCount].dwNumber = m_stCipherNum[m_dwCipherCount].dwRealCipherNum;
//			m_stCipherNum[m_dwCipherCount].bIsAni = FALSE;
//		}
//
//		if(m_dwCipherCount == m_dwMaxCipher)
//		{
//			m_bIsAnimationing = FALSE;
//			return;
//		}
//
//		m_dwIntervalAniTime = curtime;
//		m_dwCipherCount++;
//
//	}
//}
//
//void cJackpotDialog::DoSequenceAni()
//{
//	if(!m_bIsAnimationing)
//	{
//		m_dwTotalMoney = m_dwOldTotalMoney;	// When TotalMoney Decrease // ���پ��� �׳� �����.(KES PM�� ����)
//		return;
//	}
//
//	DWORD durTime =  gCurTime - m_dwAniStartTime;
//	
//	DWORD durMoney =  durTime / NUM_CHANGE_TIMELENGTH * MONEY_PER_MON;
//
//	m_dwTotalMoney = m_dwTempMoney + durMoney;
//
//	if( m_dwTotalMoney >= m_dwOldTotalMoney )
//	{
//		m_dwTotalMoney = m_dwOldTotalMoney;
//		m_bIsAnimationing = FALSE;
//	}
//}
//
//
//void cJackpotDialog::Render()
//{//
//	if(!m_bActive) return;
//
//	cDialog::RenderWindow();
//	cDialog::RenderComponent();
//
//	// ���� �̹��� ��� // ���, �̹�����ȣ
//	for( int i = 0; i < CIPHER_NUM; ++i )
//	{
//		if(m_stCipherNum[i].dwNumber == DEFAULT_IMAGE)
//			continue;
//		m_stNumImage[m_stCipherNum[i].dwNumber].pImage->RenderSprite( NULL, NULL, 0.0f, &m_vPos[i], 0xffffffff );
//	}
//
//	
//}
//
//void cJackpotDialog::Process()
//{// JACKPOTMGR�� ���� TotalMoney ����
//	m_dwTotalMoney = JACKPOTMGR->GetJPTotalMoney();
//	
//	SetNumImagePos();
///*
//	// route for DoAni
////	if(IsNumChanged())
////	{
////		ConvertCipherNum();
////		InitForAni();
////	}
////
////	DoAni();
//*/
//
//	// route for DoSequenceAni
//	if(IsNumChanged())
//	{
//		InitForSequenceAni();
//	}
//
//	DoSequenceAni();
//	
//	ConvertCipherNum();
//}
//
//void cJackpotDialog::InitForAni()
//{
//	//���� ���ڸ� �����صΰ� ��� ���� �ִϸ� �Ұ� ���·� �д�.
//	for( int i = 0; i < CIPHER_NUM; ++i )
//	{
//		m_stCipherNum[i].dwRealCipherNum = m_stCipherNum[i].dwNumber;
//	}
//	// ��ü �ִ� ����
//	m_bIsAnimationing = TRUE;
//	// �ִ� ī���� �ʱ�ȭ
//	m_dwCipherCount = 0;
//	// �ִ� ���� �ð�
//	m_dwAniStartTime = gCurTime;
//}
//
//void cJackpotDialog::InitForSequenceAni()
//{
//	if(!m_bDoSequenceAni) return;
//
//	m_bIsAnimationing = TRUE;
//
//	m_dwAniStartTime = gCurTime;
//	//NUM_CHANGE_TIMELENGTH
//}
//
//void cJackpotDialog::Init()
//{
//	memset(m_vPos, 0, sizeof(VECTOR2)*CIPHER_NUM);		// ����â �ڸ��� ��ġ
//
//	m_pBtnClose = NULL;			// �ݱ� ��ư
//
//	m_dwTotalMoney	= 0;			// �ѱݾ�
//	m_dwOldTotalMoney	= 0;
//	m_dwTempMoney	= 0;
//
//	m_dwAniStartTime	= 0;
//	m_dwNumChangeTime	= 0;
//	m_dwIntervalAniTime	= 0;	
//	m_dwMaxCipher	= 0;
//	m_dwCipherCount	= 0;
//	m_bIsAnimationing	= FALSE;
//	m_bDoSequenceAni	= FALSE;
//
//	ConvertCipherNum();
//}
//
//void cJackpotDialog::Release()
//{
//	ReleaseNumImage();
//}
