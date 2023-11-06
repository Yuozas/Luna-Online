// cJackpotDialog.h: interface for the cJackpotDialog class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_CJACKPOTDIALOG_H__8AEBC3BE_AB04_48D0_BB0E_AA0800D15728__INCLUDED_)
//#define AFX_CJACKPOTDIALOG_H__8AEBC3BE_AB04_48D0_BB0E_AA0800D15728__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//
//#include "./Interface/cDialog.h"
//
//#define NUMIMAGE_W 8
//#define NUMIMAGE_H 14
//
//#define BASIC_ANI_TIMELENGTH	2000
//#define BETWEEN_ANI_TIMELENGTH	500		//interval between Numbers
//#define NUM_CHANGE_TIMELENGTH	100		//interval
//
//class cButton;
//
//struct stNumImage
//{
//	cImage* pImage;
//	DWORD dwW,dwH;
//	stNumImage():pImage(NULL),dwW(0),dwH(0){}
//};
//
//struct stCipherNum
//{
//	stCipherNum():dwNumber(0),dwRealCipherNum(0),bIsAni(FALSE){}
//	DWORD dwNumber;// �ڸ��� ���� (��¿�)
//	DWORD dwRealCipherNum;// �ڸ��� ���� (������)
//	bool bIsAni;// ���� �ִ� ���� ������..(�ִϿ�)
//};
//
//class cJackpotDialog : public cDialog
//{
//	enum{DEFAULT_IMAGE = 99, NUM_0 = 0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_COUNT, CIPHER_NUM = 9};
//
//	stNumImage	m_stNumImage[NUM_COUNT];	// ���� �̹���
//	VECTOR2		m_vPos[CIPHER_NUM];		// ����â �ڸ��� ��ġ
//
//	cButton*	m_pBtnClose;			// �ݱ� ��ư
//
//	DWORD		m_dwTotalMoney;			// �ѱݾ�
//	DWORD		m_dwOldTotalMoney;			// ���� �ݾ�(��ȭ��)
//	DWORD		m_dwTempMoney;			// @����
//
//	stCipherNum m_stCipherNum[CIPHER_NUM];	// �� �ڸ��� ����� ����
//	DWORD		m_dwAniStartTime;		// ���� �ִ� ���� �ð�
//	DWORD		m_dwNumChangeTime;		// �ڸ� �ִ� ��ȯ ����	// ���� ���� ����
//	DWORD		m_dwIntervalAniTime;	// �ڸ��� �ִ� ��ȯ ���� // ���� �ڸ� �ִϰ� ������ ..���� ���� �ڸ� �ִ� ���� �ϴ� ��~
//	DWORD		m_dwMaxCipher;			// �ݾ� �� �ڸ��� (�ִϿ�)
//	DWORD		m_dwCipherCount;		// �ڸ��� (�ִϿ�)
//	bool		m_bIsAnimationing;			// ��ü �ִ� ���� ������..(�ִϿ�)
//	bool		m_bDoSequenceAni;			// ForSequenceAni
//
//public:
//	cJackpotDialog();
//	virtual ~cJackpotDialog();
//
//	void InitNumImage();				// ���� �̹��� ���
//	void ReleaseNumImage();				// ���� �̹��� ����
//	void Linking();						// ��ư ����
//	void SetNumImagePos();				// ���� �̹��� ��� ��ǥ ����	
//	void ConvertCipherNum();			// ������ ����
//	bool IsNumChanged();				// ���� ��ȭ üũ	// Check Num charnges
////	void DoNumChangeAni();				// ���� ��ȯ �ִ�	// Ticktime control
//	void DoAni();
//	void DoSequenceAni();
//	void Render();						// ���
//	void Process();
//
//	void InitForAni();
//	void InitForSequenceAni();
//	void Init();
//	void Release();
//
//};
//
//#endif // !defined(AFX_CJACKPOTDIALOG_H__8AEBC3BE_AB04_48D0_BB0E_AA0800D15728__INCLUDED_)
//
///*
// *	AGENT �κ��� MSG�� �޾� ������ ���� ������ ����Ѵ�
// *	
// */