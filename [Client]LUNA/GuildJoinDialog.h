/**********************************************************************

�̸�: GuildJoinDialog
�ۼ�: 2007/08/09 16:55:58, �̿���

����:	������ � �з��� �ʴ����� �����Ѵ�
                                                                     
***********************************************************************/

#pragma once


#include "interface\cdialog.h"


class CGuildJoinDialog :	public cDialog
{
public:
	CGuildJoinDialog(void);
	virtual ~CGuildJoinDialog(void);

	void Linking();
	void OnActionEvent(LONG lId, void* p, DWORD we);
	void SetEnableStudent( BOOL );
	void SetEnableMember( BOOL );
};
