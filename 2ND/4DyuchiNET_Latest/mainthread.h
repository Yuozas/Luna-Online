#pragma once

#include "../4DyuchiNET_Common/typedef.h"


BOOL StartMainThread(DWORD dwFlag);
void EndMainThread();	// ���� ������ ����

HANDLE	GetMainThreadMsgEvent();	// ���ν����忡 �޽��� ������ �뺸�� �̺�Ʈ �ڵ� ����
HANDLE	GetMainThreadUserDefineEvent(DWORD index);	// ����� ���� �̺�Ʈ �ڵ� ����

void SetMainThreadUserDefineEventFunc(DWORD index,EVENTCALLBACK pFunc);	// ����� ���� �̺�Ʈ�� �߻������� ����� �Լ� ����
																		// Modified by chan78 at 2001/10/17
void SetMainThreadUserDefineEventNum(DWORD num);	// ����� ���� �̺�Ʈ ���� ����

void BreakMainThread();	// ���� ������ �Ͻ�����
void ResumeMainThread();// ���� ������ �簡��
void ProcessPreConnectedItem();	// accept,connect�� �� pre-connection���� ó��
void SetOnInitialFunc(OnIntialFunc pFunc);




