#pragma once
#include "interface\cdialog.h"

//�����ʴ뼱��â. ���Ŀ� or ���ϻ�
class CFamilyJoinDialog :	public cDialog
{
public:
	CFamilyJoinDialog(void);
	virtual ~CFamilyJoinDialog(void);

	void Linking();
	void OnActionEvent(LONG lId, void* p, DWORD we);

};
