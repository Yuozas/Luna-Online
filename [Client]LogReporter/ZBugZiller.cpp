#include "StdAfx.h"
#include ".\zbugziller.h"
#include <IO.H>

ZBugZiller::ZBugZiller()
{
	m_userID = "";
	m_userPassword = "";
	m_strServerAddress = "";
	m_iPortNo = 21;

	m_pSession = NULL;
	m_pFtpConnection = NULL;
}
void ZBugZiller::SetFTPInfo(const string & serverAddress, const int & portNumber)
{
	m_strServerAddress = serverAddress;
	m_iPortNo = portNumber;
}
void ZBugZiller::SetAccount (const string & strUserID, const string & strUserPassword)
{
	m_userID = strUserID;
	m_userPassword = strUserPassword;
}
void ZBugZiller::ConnectFTP()
{
	m_pSession = new CInternetSession();
	try
	{
		m_pFtpConnection = m_pSession->GetFtpConnection(m_strServerAddress.c_str(), m_userID.c_str(), m_userPassword.c_str(), m_iPortNo, TRUE);
	}
	catch ( ... )
	{
		Release();
//		MessageBox(HWND_DESKTOP, "[¿À·ù] : ¼­¹ö¿¡ Á¢¼Ó ÇÒ ¼ö ¾ø½À´Ï´Ù.", "¹¬Çâ¿Â¶óÀÎ", MB_OK);
		return;
	}
}
void ZBugZiller::Release()
{
	if ( m_pFtpConnection != NULL )
	{
		m_pFtpConnection->Close();
		delete m_pFtpConnection;
		m_pFtpConnection =NULL;
	}

	if ( m_pSession != NULL )
	{
		m_pSession->Close();
		delete m_pSession;
		m_pSession =NULL;
	}
}
ZBugZiller::~ZBugZiller(void)
{
	Release();
}

void ZBugZiller::Upload(char * strlocalFile, char * strRemoteFile)
{
	if(!m_pFtpConnection || !m_pSession)
		return;

//	CFileStatus aStatus;
//if ( CFile::GetStatus(strlocalFile, aStatus) == TRUE )
//	{
		/*
		CTime creationTime = aStatus.m_ctime;
				CString strTime = "[";
		        strTime += strKey.c_str();
				strTime += "]";
				strTime += creationTime.Format("(%Y-%m-%d)%H-%M-%S");
		*/
		//CString strTime;
		if( _access(strlocalFile, 0) != -1)
		{
			try
			{
				m_pFtpConnection->PutFile(strlocalFile, strRemoteFile, FTP_TRANSFER_TYPE_ASCII);
			}
			catch ( ... )
			{
				Release();
//				MessageBox(HWND_DESKTOP, "[¿À·ù] : ¼­¹ö¿¡ ·Î±×¸¦ Àü´Þ ÇÒ ¼ö ¾ø½À´Ï´Ù.", "¹¬Çâ¿Â¶óÀÎ", MB_OK);
				return;
			}

			while(1)
			{
				if(DeleteFile(strlocalFile) == TRUE)
					break;
				Sleep(200);
			}
		}

//	}
}

