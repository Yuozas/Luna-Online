// FamilyInviteDialog.h: interface for the CFamilyInviteDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FAMILYINVITEDIALOG_H__170A81FF_38F2_4063_8AB5_7498C5E96FB7__INCLUDED_)
#define AFX_FAMILYINVITEDIALOG_H__170A81FF_38F2_4063_8AB5_7498C5E96FB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"

class cTextArea;
class CFamilyInviteDialog : public cDialog  
{
	cTextArea * m_pInviteMsg;
public:
	CFamilyInviteDialog();
	virtual ~CFamilyInviteDialog();
	
	void Linking();
	//void SetInfo(char* FamilyName, char* MasterName);
	//SW060713 ¹®ÇÏ»ý
	void SetInfo(char* FamilyName, char* MasterName, int FlgKind);

};

#endif // !defined(AFX_FAMILYINVITEDIALOG_H__170A81FF_38F2_4063_8AB5_7498C5E96FB7__INCLUDED_)
