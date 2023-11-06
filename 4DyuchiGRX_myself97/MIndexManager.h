/*
	MIndexManager
	
	�ð� ���� Release �ѹ濡 �ϵ��� ���ľ� �ҵ�. 
*/
#if !defined(AFX_MINDEXMANAGER_H__E9D7E92E_CD14_44FB_8C09_7EAF6B0A2F5E__INCLUDED_)
#define AFX_MINDEXMANAGER_H__E9D7E92E_CD14_44FB_8C09_7EAF6B0A2F5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4dyuchigrx_common/stdafx.h"

struct	MIndexNode		// ���� ��ũ�帮��Ʈ��...
{
//	int				iPrev;		// �ε��� ��ũ. -_-;
//	int				iNext;
	short				iPrev;		// �ε��� ��ũ. -_-;
	short				iNext;
	int iIndex;
};

class MIndexManager  
{
public:
	MIndexManager();
	~MIndexManager();

	void Init(int iMaxIndexCount);		// �ʱ�ȭ, iMaxIndexCount�� �ִ� �ε����� �����̴�.

	int GetIndex(void);					// ����ִ� �ε��� �ϳ��� ��´�. �ִ��� ������ ���Ƽ�,
	BOOL ReleaseIndex(int iIndex);		// �ε��� �ϳ��� ��ȯ�Ѵ�.

	DWORD ReleaseLastCreated(void);			// ���� �������� ������ �ε��� �ѳ��� Release �ϰ� �׳��� �ε����� ����.

	DWORD	GetCurrentIndex( int** ppIndexArray);		// ���� ��ϵǾ��ִ� �ε��� ���۸� �����Ѵ�. ���ϰ��� ����. get �Ҷ����� �����Ѵ�.

	void Delete(void);

	void	ReleaseAllIndex(void);			// �Ҵ�� ��� �ε����� �������Ѵ�.
protected:
	int				miEmptyIndexEntry;		// ����ִ� �ε����� ü�� �ε���.
	int				miUsedIndexEntry;			// ���Ǵ� �ε����� ü�� �ε���.

	MIndexNode*		mpBuffer;			// ����. ���ﶧ �ѹ��̸� �ȴ�.

	int				miMaxIndexCount;		// �ִ� �ε��� ����. 0���� �̰�-1����.
	int				miUsedIndexCount;	// ���ǰ� �ִ� �ε��� ����.

	int*			mpIndexArrayBuff;		// GetCurrentIndex ���� ���� �ܺ� ��¿� ����.

};

#endif // !defined(AFX_MINDEXMANAGER_H__E9D7E92E_CD14_44FB_8C09_7EAF6B0A2F5E__INCLUDED_)
