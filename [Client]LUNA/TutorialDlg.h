#pragma once
//=================================================================================================
//	FILE		: TutorialDlg.h
//	PURPOSE		: Window interface to render speech and NPC image of TUTORIAL.
//	PROGRAMMER	: Yongs Lee
//	DATE		: October 16, 2007
//	ADDITION	:
//	ATTENTION	:
//=================================================================================================





//=========================================================================
//		GM�� ���� ���� �� �� ���� �޽����� ����ϱ� ���� ���� �޽��� ����
//=========================================================================
#ifdef _GMTOOL_

#define TDERR_MSG1		"Invalid tutorial pointer"							// ���ڷ� �Ѿ�� Ʃ�丮�� ������ ����.
#define TDERR_MSG2		"Failed to receive subtutorial info : %d"			// ���ڷ� �Ѿ�� Ʃ�丮�� ������ ����.
#define TDERR_MSG3		"Failed to receive msg info : %d"					// �޽����� �޴µ� ����.
#define TDERR_MSG4		"There is no message : %d"							// �޽����� ����.
#define TDERR_MSG5		"Failed to receive msg line : %d msg - %d line"		// �� �� �޽����� �� �� ������ �޴µ� ����.
#define TDERR_MSG6		"Invalid current sub tutorial info"					// ���� ���� ����Ʈ ���� ����.
#define TDERR_MSG7		"Failed to receive npc file name - npc%d emotion%d" // npc �̹��� �ޱ� ����.
#define TDERR_MSG8		"Failed to load sprite of npc - npc%d emotion%d"	// npc�� �̹��� ��������Ʈ �ε� ����.
#define TDERR_MSG9		"Invalid check condition of npc talk : %d npc"		// �߸��� npc ��ȭ ���� üũ��� ����.
#define TDERR_MSG10		"Invalid sound index : %d - %d - %d"				// ��� Ʃ�丮�� ��� ���� Ʃ�丮���� ���� �ε��� ����.

#endif 





//=========================================================================
//		�ʿ��� ��������� �ҷ��´�.
//=========================================================================
#include "./Interface/cDialog.h"											// ���̾�α� �������̽��� �����Ѵ�.
#include "cImageSelf.h"





//=========================================================================
//		�ʼ� ���� ��Ʈ.
//=========================================================================
#define TUTORIAL_FONTIDX	7												// Ʃ�丮�� ��¿� ��Ʈ �ε����� �����Ѵ�.
#define NPC_IMAGE_PATH		"./data/interface/2dimage/npciamge/"			// NPC �̹����� �ε��ϱ� ���� ��θ� ���� �Ѵ�.





//=========================================================================
//		�ʿ��� ��Ʈ�� �������̽��� �����Ѵ�.
//=========================================================================
class cButton ;																// ��ư Ŭ������ �����Ѵ�.
class cImageSelf ;															// �̹��� Ŭ������ �����Ѵ�.
class cListDialog ;															// ����Ʈ ���̾�α� Ŭ������ �����Ѵ�.
class cStatic ;																// ����ƽ Ŭ������ �����Ѵ�.

class cTutorial ;															// Ʃ�丮�� Ŭ������ �����Ѵ�.

struct SUB_TUTORIAL ;														// ���� Ʃ�丮�� ����ü�� �����Ѵ�.





//=========================================================================
//		Ʃ�丮�� ���̾�α� Ŭ���� ����
//=========================================================================
class cTutorialDlg : public cDialog											// Ʃ�丮�� ���̾�α� Ŭ����.
{
	cListDialog*		m_pSpeechList ;										// Ʃ�丮�� ��� ����Ʈ ���̾�α�.

	cButton*			m_pOkBtn ;											// Ȯ�� ��ư ������.
	cButton*			m_pPrevBtn ;										// ���� ��ư ������.
	cButton*			m_pNextBtn ;										// ���� ��ư ������.
	cButton*			m_pPassBtn ;										// �н� ��ư ������.

	cStatic*			m_pSpeechBack ;										// ��� ��� �̹��� ����ƽ ������.

	cTutorial*			m_pTutorial ;										// ��� Ʃ�丮�� ������.
	SUB_TUTORIAL*		m_pCurSubTutorial ;									// ���� ���� Ʃ�丮���� ������ ���� ������.

	BOOL				m_bShowTutorial ;									// Ʃ�丮���� ������ �� ���θ� �����ϴ� ����.

	int					m_nCurSubIndex ;									// ���� ���� Ʃ�丮�� �ε����� ��� ����.
	int					m_nTotalSubCount ;									// �� ���� Ʃ�丮���� ������ ��� ����.

	cImageSelf			m_NpcImage ;										// NPC �̹��� ����� ���� �̹��� ������.
	int					m_nImageAlpha ;										// NPC �̹����� ���� ���� ���� ����.
	DWORD				m_dwStartTime ;

	int					m_nDispWidth ;										// ��ũ�� ���� ������.
	int					m_nDispHeight ;										// ��ũ�� ���� ������.

	int					m_nBtnStartXpos ;									// ��ư ���� x��ǥ.
	int					m_nBtnStartYpos ;									// ��ư ���� y��ǥ.

	VECTOR2				m_vImagePos ;										// npc �̹��� ��� ��ġ ����.

	cPtrList			m_SpeechMsgList ;									// ��� ���� �޽��� ����Ʈ.

public:
	cTutorialDlg(void);														// ������ �Լ�.
	virtual ~cTutorialDlg(void);											// �Ҹ��� �Լ�.

	void Linking() ;														// ���̾�α� ���� ��Ʈ�ѵ��� ��ũ�ϴ� �Լ�.

	virtual void Render() ;													// ���� �Լ�.

	//virtual DWORD ActionEvent(CMouse* mouseInfo) ;							// ���콺 �̺�Ʈ ó���� �� �Լ�.
	virtual void OnActionEvent(LONG lId, void* p, DWORD we) ;				// ���� �� ��Ʈ�� �̺�Ʈ�� ó�� �� �Լ�.

	void Set_TutorialInfo(cTutorial* pTutorial) ;							// Ʃ�丮�� ������ �����ϴ� �Լ�.
	void Start_Tutorial() ;													// Ʃ�丮�� �����Լ�.
	//void Start_Tutorial(cTutorial* pTutorial) ;								// Ʃ�丮�� �����Լ�.

	void LoadNpcImage() ;													// ncp �̹����� �ε��ϴ� �Լ�.

	void Prepare_Tutorial() ;												// Ʃ�丮�� ������ �̸� �غ��ϴ� �Լ�.

	void ResetSpeechMsgList() ;												// ��� ����Ʈ�� �ٽ� �����ϴ� �Լ�.

	void DeActiveAllBtn() ;													// ��� ��ư�� ��Ȱ��ȭ �ϴ� �Լ�.
	void ActiveBtns() ;														// ��Ȳ�� �´� ��ư�� Ȱ��ȭ �ϴ� �Լ�.

	BOOL IsSameEndNpc(DWORD dwNpcIdx) ;										// Ʃ�丮�� ���� ���ǰ� ���� npc ���� üũ�ϴ� �Լ�.

	BOOL IsHavePrevSubTutorial() ;											// ���� ���� Ʃ�丮���� �ִ��� ���θ� üũ�ϴ� �Լ�.
	BOOL IsHaveNextSubTutorial() ;											// ���� ���� Ʃ�丮���� �ִ��� ���θ� üũ�ϴ� �Լ�.
	void StartNextSubTutorial() ;											// ���� ���� Ʃ�丮���� �����ϴ� �Լ�.

	void UpdateTutorial() ;													// Ʃ�丮���� �Ϸ��ϰ� ������Ʈ �ϴ� �Լ�.

	SUB_TUTORIAL* GetCurSubTutorial() ;										// ���� ���� Ʃ�丮���� ��ȯ�ϴ� �Լ�.

	cStatic* GetSpeechBack() { return m_pSpeechBack ; }						// ��� ����� ��ȯ�ϴ� �Լ�.
};
