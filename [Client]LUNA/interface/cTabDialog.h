// cTabDialog.h: interface for the cTabDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _cTABDIALOG_H_
#define _cTABDIALOG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cDialog.h"


// 071210 LYW --- cTabDialog : 
// ������ �� ���̾�α״� ���� �� ��ŭ ���� �߰��� �ϵ�,�߰��Ǵ� ���� ��� ��������. 
// ������ ��쿡 ����, ���� ���� ���̰�, ���� ���� ������ �ʴ� ���·� �����ؾ� �ϴ� ��찡 �ִ�.
// ( �� : �κ��丮 �⺻ 2ĭ, �κ� Ȯ�� ������ ���� Ȯ�� )
// �ʿ信 ���� �� ���̾�α׸� �ʱ�ȭ �� ��, ������ �� ��ŭ��, 
// ���� ���̴� ���·� ó���ϱ� ���� �� ���̾�α��� ��Ÿ���� �߰��Ѵ�.
enum TABDIALOG_STYPE
{
	e_Normal = 0,		// ��� ��Ÿ����, ������ �� ���̾�α� ����̴�.
	e_VisibleOption,	// ���̱� �ɼ��� �ִ� ��Ÿ����, ��ũ��Ʈ�� #TAB_STYLE�� ��Ÿ�� ��ȣ�� ���� ���� �����Ѵ�.
						// #TAB_STYLE 1 2 : ���̱� ���� �ɼ��� �ִ� ���̸�, ���� 2���� Ȱ��ȭ �� ���·� �����Ѵ�.
	e_Max_Style,
} ;


class cPushupButton;

class cTabDialog : public cDialog  
{
public:
	cTabDialog();
	virtual ~cTabDialog();
	
	void InitTab(BYTE tabNum);
	virtual void Render();
	virtual void RenderTabComponent();
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void SetAbsXY(LONG x, LONG y);
	virtual void SetActive(BOOL val);
	virtual void SetAlpha(BYTE al);
	virtual void SetOptionAlpha( DWORD dwAlpha);

	void AddTabBtn(BYTE idx, cPushupButton * btn);
	void AddTabSheet(BYTE idx, cWindow * sheet);
	
	cPushupButton * GetTabBtn(BYTE idx);
	cWindow * GetTabSheet(BYTE idx);

	BYTE GetCurTabNum() { return m_bSelTabNum; }
	BYTE GetTabNum(){ return m_bTabNum; }
	
	virtual void SetDisable( BOOL val );
	virtual void SelectTab(BYTE idx);
	
	virtual cWindow * GetWindowForID(LONG id);

	// 071210 LYW --- cTabDialog : �� ���̾�α��� ��Ÿ���� ���� �� ��ȯ�ϴ� �Լ� �߰�.
	void SetTabStyle(BYTE byStyle) ;
	BYTE GetTabStyle() { return m_byTabStyle ; }

	// 071210 LYW --- cTabDialog : ������ �� ī��Ʈ�� ��� ������ ���� �� ��ȯ�ϴ� �Լ� �߰�.
	void SetVisibleTabCount(BYTE byCount) ;
	BYTE GetVisibleTabCount() { return m_byVisibleTabCount ; }

	// 071210 LYW --- cTabDialog : ������ ���� ���̰� �ϴ� �Լ� �߰�.
	void ShowTab(BYTE byTabNum, BOOL bShow) ;

protected:
	BYTE curIdx1;
	BYTE curIdx2;
	BYTE m_bTabNum;
	BYTE m_bSelTabNum;
	cPushupButton** m_ppPushupTabBtn;
	cWindow** m_ppWindowTabSheet;
//	DWORD	m_BtnPushstartTime;
//	DWORD	m_BtnPushDelayTime;

	// 071210 LYW --- cTabDialog : �� ���̾�α��� ��Ÿ���� ��� ������ �߰��Ѵ�.
	BYTE m_byTabStyle ;
	// 071210 LYW --- cTabDialog : ���� �� �� ī��Ʈ�� ��� ������ �߰��Ѵ�.
	BYTE m_byVisibleTabCount ;
};

#endif // _cTABDIALOG_H_
