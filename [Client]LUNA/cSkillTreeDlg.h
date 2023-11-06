#pragma once
#include "./Interface/cTabDialog.h"

class cWindow;
class cStatic;

class cSkillTreeDlg :
	public cTabDialog
{
	cStatic* mpSkillPoint;

	// 080203 LYW --- �ൿ�� ���õ� ������ �ε��� ������ �߰�.
	CIndexGenerator m_IdxGenerator ;

public:
	cSkillTreeDlg(void);
	virtual ~cSkillTreeDlg(void);

	void Linking();

	void Render();
	void Add( cWindow *window );

	void SetSkillPoint();
	virtual void SetActive(BOOL val);

	// 080203 LYW --- �ε��� ������ ��ȯ �Լ� �߰�.
	CIndexGenerator* GetIndexGenerator() { return &m_IdxGenerator ; }
};
