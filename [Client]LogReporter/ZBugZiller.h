#pragma once

#include <string>
#include <afxinet.h>

using namespace std;

class ZBugZiller
{
public:
	ZBugZiller();
	~ZBugZiller();
	void Release();


	void SetFTPInfo(const string & serverAddress, const int & portNumber = 21);
	void SetAccount (const string & strUserID, const string & strUserPassword);
	void ConnectFTP();
	void Upload(char * strlocalFile, char * strRemoteFile);
	
private:
	string		m_userID;
	string		m_userPassword;
	string		m_strServerAddress;
	int			m_iPortNo;

private:		// MFC classes
	CInternetSession *		m_pSession;
	CFtpConnection *		m_pFtpConnection;
};