#ifndef __CLISTDIALOG_EX__
#define __CLISTDIALOG_EX__

#include "./interface/cListDialog.h"										// ����Ʈ ���̾�α� ����� �ҷ��´�.

class cListDialogEx : public cListDialog									// ����Ʈ ���̾�α� Ŭ������ ��ӹ��� ����Ʈ ���̾�α� Ȯ�� Ŭ����.
{
public:
	cListDialogEx();														// ������ �Լ�.
	~cListDialogEx();														// �Ҹ��� �Լ�.

	virtual void ListMouseCheck( LONG x, LONG y, DWORD we );				// ����Ʈ ���콺 üũ �Լ�.
	virtual void Render();													// ���� �Լ�.
};

#endif //__CLISTDIALOG_EX__