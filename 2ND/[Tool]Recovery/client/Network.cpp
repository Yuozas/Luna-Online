#include "stdafx.h"
#include "Network.h"
#include "client.h"


CNetwork::CNetwork() :
mInterface( 0 ),
mModule( 0 )
{
	//ZeroMemory( mConnectionIndex, sizeof( mConnectionIndex ) );
	//ZeroMemory( m_CheckSum, sizeof( m_CheckSum ) );

	LoadScript();
}


CNetwork::~CNetwork()
{
	Release();
}


void CNetwork::Release()
{
	AllDisconnect();

	if( mInterface )
	{
		mInterface->Release();
		mInterface = 0;
	}	
	
	if( mModule )
	{
		FreeLibrary( mModule );
		mModule = 0;
	}
}



static HWND mHandle;;

BOOL CNetwork::Initialise( HWND handle )
{
	Release();

	if( ! handle )
	{
		return FALSE;
	}

	mModule = LoadLibrary( "Network.dll" );

	DllGetClassObject_BaseNetwork	pNetFunc = (DllGetClassObject_BaseNetwork)GetProcAddress(mModule,"DllGetClassObject");

	if( ! pNetFunc )
	{
		MessageBox( 0, "Network.dll�� ã�� ���߽��ϴ�. �糪�� ��ġ�� �������� �� ���α׷��� �����Ű����", "", MB_ICONQUESTION | MB_OK );
		return FALSE;
	}

	HRESULT hr = pNetFunc(CLSID_SC_BASENETWORK_DLL, IID_SC_BASENETWORK_DLL, (void**)&mInterface);

	if( hr != S_OK )
	{
		MessageBox( 0, "Network.dll�� ã�� ���߽��ϴ�. �糪�� ��ġ�� �������� �� ���α׷��� �����Ű����", "", MB_ICONQUESTION | MB_OK );
		return FALSE;
	}

	if( ! mInterface )
	{
		MessageBox( 0, "COM �������̽� ���ε��� �����߽��ϴ�. �����ڿ��� �����Ͻʽÿ�", "", MB_ICONERROR | MB_OK );
		return FALSE;
	}

	{
		DESC_BASENETWORK desc	= { 0 };
		desc.wSockEventWinMsgID	= WM_SOCKEVENTMSG;	
		desc.OnConnect			= CNetwork::OnConnect;
		desc.OnDisconnect		= CNetwork::OnDisconnect;
		desc.ReceivedMsg		= CNetwork::OnReceive;

		mInterface->InitNetwork( handle, MAX_SERVERSET, &desc );
	}

	mHandle = handle;

	return TRUE;
}


BOOL CNetwork::Connect()
{	
	ASSERT( mInterface );

	std::set< DWORD > unableSet;

	// ���� �Ұ����� ������ ��������
	for( ServerMap::const_iterator it = mServerMap.begin(); mServerMap.end() != it; ++it )
	{
		const DWORD		serverIndex	= it->first;
		const Server&	server		= it->second;

		char ip[ MAX_PATH ];

		_tcscpy( ip, server.mIP );

		if( ! mInterface->ConnectToServer( ip, server.mPort ) )
		{
			unableSet.insert( serverIndex );
		}
	}

	// ���� �Ұ����� ���� ������� �������� �õ�������Ѵ�. ���� �Ұ����� ������ ���� �õ��� ���,
	// ���� ������ ������ ������ �ǳ� �������� ������ �� ���� ���� �ȴ�. ��Ʈ��ũ ��� ���� �ڵ忡 ������ �ִ� ������ �����ȴ�.
	for( ServerMap::const_iterator it = mServerMap.begin(); mServerMap.end() != it; ++it )
	{
		const DWORD		serverIndex	= it->first;
		const Server&	server		= it->second;

		if( unableSet.end() != unableSet.find( serverIndex ) )
		{
			continue;
		}

		char ip[ MAX_PATH ];

		_tcscpy( ip, server.mIP );

		const DWORD		connectionIndex		= mInterface->ConnectToServer( ip, server.mPort );

		if( connectionIndex )
		{
			mConnectionMap[ serverIndex ]	= connectionIndex;
		}
		else
		{
			// �̹� ���� �Ұ����� ������ �����´µ��� ���� ������ ��� ó�� �Ұ����ϴ�.
			ASSERT( 0 );
			return 0;
		}
	}

	return 0 < mConnectionMap.size();
}


void CNetwork::Disconnect( DWORD serverIndex )
{
	ConnectionMap::iterator it = mConnectionMap.find( serverIndex );

	if( mConnectionMap.end() != it )
	{
		const DWORD connectionIndex = it->second;

		mInterface->CompulsiveDisconnect( connectionIndex );

		mConnectionMap.erase( it );
	}

	/*
	if( mConnectionIndex[serverset] )
	{
		mInterface->CompulsiveDisconnect( mConnectionIndex[serverset] );
		mConnectionIndex[serverset] = 0;		
	}
	*/
}

void CNetwork::AllDisconnect()
{
	for( ConnectionMap::const_iterator it = mConnectionMap.begin(); mConnectionMap.end() != it; ++it )
	{
		const DWORD connectionIndex	= it->second;

		mInterface->CompulsiveDisconnect( connectionIndex );
	}

	mConnectionMap.clear();

	/*
	if( mInterface )
	{
		for( int i = 0; i < MAX_SERVERSET; ++i )
		{
			Disconnect( i );
		}
	}
	*/
}

void CNetwork::Send( DWORD serverIndex, const MSGROOT& message, size_t size )
{
	ConnectionMap::const_iterator it = mConnectionMap.find( serverIndex );

	if( mConnectionMap.end() != it )
	{
		const DWORD connectionIndex = it->second;

		mInterface->Send( connectionIndex, ( char* )( &message ), DWORD( size ) );
	}

	/*
	if( mConnectionIndex[serverset] )
	{
		mInterface->Send( mConnectionIndex[serverset], (char*)pMsg, size );
	}
	*/
}

void CNetwork::SendAll( const MSGROOT& message, size_t size )
{
	for( ConnectionMap::const_iterator it = mConnectionMap.begin(); mConnectionMap.end() != it; ++it )
	{
		const DWORD serverIndex	= it->first;

		Send( serverIndex, message, size );
	}

	/*
	for( int i = 0; i < MAX_SERVERSET; ++i )
	{
		Send( i, pMsg, MsgLen );
	}
	*/
}


void CNetwork::OnConnect( DWORD dwConIndex )
{}


void CNetwork::OnDisconnect( DWORD dwConIndex )
{
	CNetwork& network = CNetwork::GetInstance();

	ConnectionMap& connectionMap = network.mConnectionMap;

	for( ConnectionMap::iterator it = connectionMap.begin(); connectionMap.end() != it; ++it )
	{
		const DWORD connectionIndex = it->second;

		if( connectionIndex == dwConIndex )
		{
			connectionMap.erase( it );

			// TODO: any action for user

			break;
		}
	}

	if( connectionMap.empty() )
	{
		MessageBox( 0, "��� ������ ���� ����Ǿ����ϴ�", "", MB_ICONSTOP | MB_OK );

		AfxGetApp()->GetMainWnd()->SendMessage( WM_CLOSE );
	}

	/*
	for( int i = 0; i < sizeof( network.mConnectionIndex ) / sizeof( DWORD ); ++i )
	{
		DWORD& connectionIndex = network.mConnectionIndex[i];

		if( connectionIndex == dwConIndex )
		{
			connectionIndex = 0;

			// CMainFrame* window = ( CMainFrame* )AfxGetApp()->GetMainWnd();

			// TODO: proceed disconnection event
			break;
		}
	}
	*/
}

void CNetwork::OnReceive( DWORD connectionIndex, char* pMsg, DWORD size )
{
	const MSGROOT*	message		= ( MSGROOT* )pMsg;
	BOOL			isReceive	= FALSE;

	CNetwork&				network			= CNetwork::GetInstance();
	const ConnectionMap&	connectionMap	= network.mConnectionMap;
	DWORD					serverIndex		= 0;

	for( ConnectionMap::const_iterator it = connectionMap.begin(); connectionMap.end() != it; ++it )
	{
		if( it->second == connectionIndex )
		{
			isReceive	= TRUE;
			serverIndex	= it->first;

			break;
		}
	}

	/*
	for( POSITION position = network.mConnection.GetStartPosition(); position; )
	{
		DWORD serverIndexInMap;
		DWORD connectionIndexInMap;

		network.mConnection.GetNextAssoc( position, serverIndexInMap, connectionIndexInMap );

		if( connectionIndex == connectionIndexInMap )
		{
			isReceive = TRUE;
			break;
		}
	}
	*/
	
	if( ! isReceive )
	{
		return;
	}

	CclientApp* application = ( CclientApp* )AfxGetApp();
	ASSERT( application );

	application->Parse( serverIndex, message );	
}


CNetwork& CNetwork::GetInstance()
{
	static CNetwork instance;

	return instance;
}


void CNetwork::LoadScript()
{
	const CString name( "RecoveryClient.cfg" );

	CStdioFile file;

	if( ! file.Open( name, CFile::modeRead  ) )
	{
		CString textFileIsNotFound;
		textFileIsNotFound.LoadString( IDS_STRING307 );

		CString text;
		text.Format( textFileIsNotFound, name );

		MessageBox( 0, text, "", MB_ICONERROR | MB_OK );
		return;
	}

	enum
	{
		eModeServer,
		eModeNone,
	}
	mode = eModeNone;

	CString line;

	DWORD serverIndex = 0;

	while( file.ReadString( line ) )
	{
		int i = 0;

		const CString	seperator( "\"\n= " );
		CString			token = line.Tokenize( seperator, i );

		if( token.IsEmpty() )
		{
			continue;
		}
		else if( "[server]" == token  )
		{
			mode = eModeServer;
			continue;
		}
		else if( "//" == token.Left( 2 ) )
		{
			continue;
		}

		switch( mode )
		{
		case eModeServer:
			{
				const WORD recoveryServerPort = 23900;

				Server& data = mServerMap[ ++serverIndex ];

				data.mIP	= token;
				//data.mPort	= atoi( line.Tokenize( seperator, i ) );
				data.mPort	= recoveryServerPort;
				data.mName	= line.Tokenize( seperator, i );

				do 
				{
					token = line.Tokenize( seperator, i );

					data.mName += ' ' + token;
				}
				while( ! token.IsEmpty() );

				break;
			}
		}
	}
}


DWORD CNetwork::GetServerIndex( const CString& name ) const
{
	for( ServerMap::const_iterator it = mServerMap.begin(); mServerMap.end() != it; ++it )
	{
		const Server& data = it->second;

		if( name == data.mName )
		{
			return it->first;
		}
	}

	return 0;
}


const CString& CNetwork::GetServerName( DWORD serverIndex ) const
{
	ServerMap::const_iterator it = mServerMap.find( serverIndex );

	if( mServerMap.end() == it )
	{
		static const CString emptyString;

		return emptyString;
	}

	return it->second.mName;
}