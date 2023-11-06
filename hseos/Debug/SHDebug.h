/*********************************************************************

	 ����		: SHDebug.h
	 �ۼ���		: hseos
	 �ۼ���		: 2007/04/13

	 ���ϼ���	: ����� Ŭ������ ���

 *********************************************************************/

#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
class CSHDebug
{
public:
	//----------------------------------------------------------------------------------------------------------------
	typedef enum							// ����� ���� ����
	{
		DEBUG_INFO_KIND_FARM_OWN = 0,		// ..���� ����
		DEBUG_INFO_KIND_FARM_GARDEN,		// ..�Թ� ����
		DEBUG_INFO_KIND_RESIDENT_REGIST,	// ..�ֹε�� ����
		DEBUG_INFO_KIND_FAMILY,				// ..�йи� ����
		DEBUG_INFO_KIND_MAX_NUM,
	} DEBUG_INFO_KIND;

private:
	//----------------------------------------------------------------------------------------------------------------
	static BOOL					m_bShowDebugInfo[DEBUG_INFO_KIND_MAX_NUM];
	static int					m_nValue[5][DEBUG_INFO_KIND_MAX_NUM];

	static int					m_nExceptionProcGrade;
	static UINT					m_nErrorCode;					

public:
	CSHDebug();
	~CSHDebug();

	//----------------------------------------------------------------------------------------------------------------
	//							�ʱ�ȭ
	static VOID					Init();
	//							����
	static VOID					MainLoop();
	//							�׸���
	static VOID					Render();

	//----------------------------------------------------------------------------------------------------------------
	//							����� ���� ǥ�� ���
	static VOID					ToggleShowDebugInfo(DEBUG_INFO_KIND nKind, int nValue1 = NULL, int nValue2 = NULL, int nValue3 = NULL)
								{
									m_bShowDebugInfo[nKind] = !m_bShowDebugInfo[nKind];
									if (m_bShowDebugInfo[nKind] == FALSE && (nValue1 != NULL || nValue2 != NULL))
										m_bShowDebugInfo[nKind] = TRUE;
									m_nValue[nKind][0] = nValue1;
									m_nValue[nKind][1] = nValue2;
									m_nValue[nKind][2] = nValue3;
								}
	//							����� ���� ǥ�� ���
	static BOOL					GetShowDebugInfo(DEBUG_INFO_KIND nKind)
								{
									return m_bShowDebugInfo[nKind];
								}
	//							�ؽ�Ʈ �Է� �м�/ó��
	static VOID					ParseTextInput(char* pszText, char* pCmd);
	//							���� ����� ���� ǥ��
	static VOID					RenderDebugInfoFarm();


	static void SetExceptionProcGrade(int nGrade)		{ m_nExceptionProcGrade = nGrade; }
	static int  GetExceptionProcGrade()					{ return m_nExceptionProcGrade; }
	static void LogExceptionError(char* pszTxt);

	static void SetErrorCode(int nCode)					{ m_nErrorCode = nCode; }
	static void RenderErrorCode();
};