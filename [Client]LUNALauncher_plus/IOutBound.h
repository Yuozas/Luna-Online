#pragma once


#define		PURPLE_OUTBOUND_INFORM					9
#define		PURPLE_OUTBOUND_DOWNLOAD_PROGRESS		18
#define		PURPLE_OUTBOUND_DOWNLOAD_SPEED			27
#define		PURPLE_OUTBOUND_DOWNLOAD_RESTTIME		31
#define		PURPLE_OUTBOUND_FILENAME				36
#define		PURPLE_OUTBOUND_PATCH_PROGRESS			45
#define		PURPLE_OUTBOUND_PATCH_FILE_NUMBER		54
#define		PURPLE_OUTBOUND_ENDING					63


typedef struct PurpleCDS
{
	TCHAR szString[160];
	DWORD dwIndex;
} PurpleCDS;

/**
 * 1. �ܺθ�� �⵿�� ��
 *		���÷��Ĵ� ���� ������ �������ڷ� �����Ѵ�.
 *		(1) HWND : SendMessage�� ���� ������ �ڵ�. 10������ ǥ�õȴ�.
 *		(2) argument : ��Ÿ �ܺθ���� �⵿�� �� �ʿ��� ������ �����Ѵ�.
 *
 * 2. ���÷���/�ܺθ�� ���� ����
 *		(1) ���÷��� => �ܺθ��� ����ϴ� ���� ������ (���ʿ� ���ڸ� �����ϱ⸸ �Ѵ�), 
 *			���Ŀ��� �ܺθ�� => ���÷����� ��Ÿ��� �Ѵ�. 
 *		(2) ��Ź���� WM_COPYDATA�� ����ϸ� �޽����� ������ ������ ������ ���¸� ���Ѵ�. 
 *			::SendMessage(HWND, WM_COPYDATA, HWND, PCOPYDATASTRUCT);
 *				a. HWND : ���÷��� ������ �ڵ��̴�. �� ���� �ܺθ���� ���� �⵿�� �� ���ڷ� ���޵ȴ�.
 *				b. WM_COPYDATA : ������ �޽���
 *				c. HWND : �޽����� ���Ϲ��� ������ �ڵ��� �����Ѵ�. (�� ���� �̷��� ���ؼ� �����Ѵ�.)
 *				d. PCOPYDATASTRUCT : WM_COPYDATA�� �� �� �����ϴ� ����ü�̴�. ������ MSDN ����.
 *		
 * 						typedef struct tagCOPYDATASTRUCT {
 * 							ULONG_PTR dwData;
 *							DWORD cbData;
 *							PVOID lpData;
 *						} COPYDATASTRUCT, *PCOPYDATASTRUCT; 
 *
 * 3. �ε���(dwData)�� ���� �ǹ�
 *		(1) PURPLE_OUTBOUND_INFORM : Inform ����ƽ�� ��Ʈ���� ǥ���Ѵ�.
 *			- szString : ����ƽ�� ǥ���� ��Ʈ��
 *			- dwIndex : N/A
 *		
 *		(2) PURPLE_OUTBOUND_DOWNLOAD_PROGRESS : �ٿ�ε� ���α׷����� �����Ѵ�.
 *			- szString : N/A
 *			- dwIndex : ���α׷��� (0���� 100����)
 *
 *		(3) PURPLE_OUTBOUND_DOWNLOAD_SPEED : �ٿ�ε� �ӵ��� ǥ���Ѵ�.
 *			- szString : N/A
 *			- dwIndex : �ٿ�ε� ���ǵ� (byte / millisecond)
 *
 *		(3.1) PURPLE_OUTBOUND_DOWNLOAD_RESTTIME : �����ð��� ǥ���Ѵ�. 
 *			- szString : N/A
 *			- dwIndex : �����ð� (millisecond)
 *
 *		(4) PURPLE_OUTBOUND_FILENAME : ���� ó���ϴ� ������ �̸��� ǥ���Ѵ�. 
 *			- szString : ó������ ���ϸ�Ī
 *			- dwIndex : N/A
 *
 *		(5) PURPLE_OUTBOUND_PATCH_PROGRESS : ��ġ ���α׷����� �����Ѵ�. ��ġ ���α׷����� ��ü��ġ������ ǥ���Ѵ�.
 *			- szString : N/A
 *			- dwIndex : ���α׷��� (0���� 100����)
 *
 *		(6) PURPLE_OUTBOUND_PATCH_FILE_NUMBER : ��ġ�� ��ü ���� ���� �� ������� ó���� ������ ������ ǥ���Ѵ�.
 *			- szString : N/A
 *			- dwIndex : HIWORD => ��ü���ϰ���, LOWORD => ������� ó���� ���� ����
 *
 *		(7) PURPLE_OUTBOUND_ENDING : �ܺθ���� ������� �����Ѵ�. 
 *			- szString : ������Ʈ��
 *			- dwIndex : �����ڵ�
 *
 *			a. �����ڵ� 0 : ��ġ�Ϸ� �� ���������. ���÷��Ĵ� �� �޽����� ������ ���ӽ��� stand by ���°� �ȴ�.
 *			b. �����ڵ� 1 : �ν��罯�带 �ٽ� �޾ƾ� ���� ���Ѵ�. szString�� �ٿ�޾ƾ� �ϴ� ���� URL�� ����Ѵ�.
 *			c. ��Ÿ �����ڵ� : �ϳ��� �����Ѵ�. (���� ���, dll �ε忡 �����Ͽ���. ��ġ���� �ٿ�ε忡 �����Ͽ��ٵ�..)
**/














