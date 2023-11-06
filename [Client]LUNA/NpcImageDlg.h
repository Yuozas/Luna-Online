#pragma once
#include ".\interface\cdialog.h"									// ���̾�α� �������̽� Ŭ���� ��� �ҷ�����.
#include "cImageSelf.h"												// �̹��� ���� Ŭ���� ��� �ҷ�����.

#include "cStatic.h"												// ����ƽ Ŭ���� ��� �ҷ�����.

enum NPC_EMOTION													// NPC ��� �̳� �ڵ�.
{
	EMOTION_NORMAL,													// �⺻ ���.
	EMOTION_HAPPY,													// �ູ ���.
	EMOTION_SURPRISE,												// ��� ���.
	EMOTION_UNHAPPY,												// ���� ���.
	EMOTION_ANGRY,													// ȭ�� ���.

	EMOTION_MAX,													// ��� �ִ��ġ.
};

struct NPC_IMAGE													// NPC �̹��� ����ü.
{
	WORD		idx;												// �ε���.
	cImageSelf	image[5];											// �̹��� �迭.
};

class cNpcImageDlg : public cDialog									// �⺻ ���̾�α׸� ��ӹ��� NPC �̹��� ���̾�α� Ŭ����.
{
	WORD	mNpcIdx;												// NPC �ε���.
	BYTE	mEmotion;												// ��ǹ�ȣ.
	DWORD	COLOR;													// ���� ����.
	DWORD	COLOR2;													// ���� ����.

	VECTOR2	SCALE;													// ������ ����.
	VECTOR2	POS;													// ��ġ ����.
	VECTOR2 BACK_SCALE;												// ��� ������ ����.
	VECTOR2 BACK_POS;												// ��� ��ġ ����.

	//cImage	mImageTop;												// ž �̹���.
	cImage	mImageBack;												// ��� �̹���.
	//cImage	mImageBottom;											// ���� �̹���.

	NPC_IMAGE* mpCurImage;											// NPC �̹��� ������.

	cStatic* m_pTopImage ;											// ž�̹��� ����ƽ.
	cStatic* m_pBottomImage ;										// ���� �̹��� ����ƽ.

	int m_nDispWidth ;												// ���÷��� ���� ������.
	int m_nDispHeight ;												// ���÷��� ���� ������.
public:
	cNpcImageDlg(void);												// ������ �Լ�.
	virtual ~cNpcImageDlg(void);									// �Ҹ��� �Լ�.

	void Linking();													// ��ũ �Լ�.

	void LoadingImage(WORD indx) ;									// �̹��� �ε� �Լ�.

	void SetActive( BOOL val );										// Ȱ��, ��Ȱ��ȭ �Լ�.

	void SetNpc( WORD NpcUniqueIdx );								// NPC ���� �Լ�.

	void SetEmotion( BYTE emotion ) { mEmotion = emotion; }			// ��� ���� �Լ�.

	virtual void Render();											// ���� �Լ�.

	void SetDisplayWH() ;											// ���÷��� ���μ��� ����� �����ϴ� �Լ�.
};










//#pragma once
//#include ".\interface\cdialog.h"									// ���̾�α� �������̽� Ŭ���� ��� �ҷ�����.
//#include "cImageSelf.h"												// �̹��� ���� Ŭ���� ��� �ҷ�����.
//
//#include "cStatic.h"												// ����ƽ Ŭ���� ��� �ҷ�����.
//
//enum NPC_EMOTION													// NPC ��� �̳� �ڵ�.
//{
//	EMOTION_NORMAL,													// �⺻ ���.
//	EMOTION_HAPPY,													// �ູ ���.
//	EMOTION_SURPRISE,												// ��� ���.
//	EMOTION_UNHAPPY,												// ���� ���.
//	EMOTION_ANGRY,													// ȭ�� ���.
//
//	EMOTION_MAX,													// ��� �ִ��ġ.
//};
//
//struct NPC_IMAGE													// NPC �̹��� ����ü.
//{
//	WORD		idx;												// �ε���.
//	cImageSelf	image[5];											// �̹��� �迭.
//};
//
//class cNpcImageDlg : public cDialog									// �⺻ ���̾�α׸� ��ӹ��� NPC �̹��� ���̾�α� Ŭ����.
//{
//	WORD	mNpcIdx;												// NPC �ε���.
//	BYTE	mEmotion;												// ��ǹ�ȣ.
//	DWORD	COLOR;													// ���� ����.
//
//	VECTOR2	SCALE;													// ������ ����.
//	VECTOR2	POS;													// ��ġ ����.
//	VECTOR2 BACK_SCALE;												// ��� ������ ����.
//	VECTOR2 BACK_POS;												// ��� ��ġ ����.
//
//	cImage	mImageTop;												// ž �̹���.
//	cImage	mImageBack;												// ��� �̹���.
//	cImage	mImageBottom;											// ���� �̹���.
//
//	NPC_IMAGE* mpCurImage;											// NPC �̹��� ������.
//
//	// 070507 LYW --- NpcImageDlg : Delete image table.
//	//CYHHashTable< NPC_IMAGE > mImageTable;
//
//	// 070507 LYW --- NpcImageDlg : Delete notice part.
//	/*
//	// 070328 LYW --- NpcImageDlg : Add static control for notice.
//	cStatic*	m_pNotice ;
//
//	// 070328 LYW --- NpcImageDlg : Add member variable for notice.
//	BOOL		m_bActivedNotice ;
//
//	// 070329 LYW --- NpcImageDlg : Add member variables for alpha.
//	int			m_nAlphaValue ;
//	int			m_nContinueCount ;
//	*/
//
//	cStatic* m_pTopImage ;											// ž�̹��� ����ƽ.
//	cStatic* m_pBottomImage ;										// ���� �̹��� ����ƽ.
//
//public:
//	cNpcImageDlg(void);												// ������ �Լ�.
//	virtual ~cNpcImageDlg(void);									// �Ҹ��� �Լ�.
//
//	// 070507 LYW --- NpcImageDlg : Delete function to linking image dlg.
//	void Linking();
//	// 070508 LYW --- NpcImageDlg : Add function to loading image.
//	void LoadingImage(WORD indx) ;
//	void SetActive( BOOL val );
//	void SetNpc( WORD NpcUniqueIdx );
//	void SetEmotion( BYTE emotion ) { mEmotion = emotion; }
//	virtual void Render();
//
//	// 070328 LYW --- NpcImageDlg : Add function to initialize notice.
//	void InitializeNotice( char* msgStr ) ;
//};
