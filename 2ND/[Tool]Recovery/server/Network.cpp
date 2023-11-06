#include "stdafx.h"
#include "Network.h"
#include "MSSystem.h"
#include "..\..\[CC]ServerModule\network_guid.h"


GLOBALTON(CNetwork)

CNetwork::CNetwork()
{
	CoInitialize(NULL);
	m_pINet = NULL;

	ZeroMemory( &mAddress, sizeof( mAddress ) );
}

CNetwork::~CNetwork()
{
	CoUninitialize();
}
void CNetwork::Release()
{
	if(m_pINet)
	{
		m_pINet->Release();
		m_pINet = NULL;
	}
}


bool CNetwork::Init(DESC_NETWORK * desc)
{
	HRESULT hr = CoCreateInstance( CLSID_4DyuchiNET, NULL, CLSCTX_INPROC_SERVER, IID_4DyuchiNET, (void**)&m_pINet);
	if (FAILED(hr))
		return false;
	if(!m_pINet->CreateNetwork( desc, 0, 0, 0 ) )
		return false;

	return true;
}


bool CNetwork::StartServer()
{
	char hostName[ MAX_PATH ];
	char address[ MAX_PATH ];
	char text[ MAX_PATH ];

	gethostname( hostName, sizeof( hostName ) );
	HOSTENT FAR* lphostent = ::gethostbyname( hostName );
	
	// �Ǽ����� ������ ����� ���� ���� ��Ʈ��ũ�� ������ �ִ�. ù��° ������ IP�� ����, �ι�° ������ ����
	// �ܺ� ������ ���� ���̴�. ���� �ι�° �ּҸ� �׻� ����ؾ��Ѵ�.

	for( int i = 0; ; ++i )
	{
		const in_addr* ip = ( in_addr* )lphostent->h_addr_list[ i ];

		if( ! ip )
		{
			break;
		}

		StringCopySafe( text, inet_ntoa( *ip ), sizeof( text ) );

		const char* token = strtok( text, ". " );

		if( ! strcmp( token, "192" ) )
		{
			continue;
		}

		StringCopySafe( address, inet_ntoa( *ip ), sizeof( address ) );
	}

	const WORD	port = 23900;

	if(! m_pINet->StartServerWithUserSide( address, port ))
	{
		MessageBox( 0, "Sever cannot start", "", MB_ICONERROR | MB_OK );
		return false;
	}
	
	// ���� ����
	{
		StringCopySafe( mAddress.mIp, address, sizeof( address ) );
		mAddress.mPort	= port;
	}

	return true;
}


void CNetwork::Send(DWORD connectionIndex, const MSGROOT& message, DWORD size)
{
	m_pINet->SendToUser(connectionIndex, ( char* )&message, size, FLAG_SEND_NOT_ENCRYPTION);
}


void CNetwork::GetUserAddress(DWORD dwConnectionIndex, char* ip, WORD * port)
{
	m_pINet->GetUserAddress(dwConnectionIndex, ip, port);
}


void CNetwork::DisconnectUser(DWORD connectionIndex )
{
	m_pINet->CompulsiveDisconnectUser( connectionIndex );
}


const CNetwork::Address& CNetwork::GetAddress() const
{
	return mAddress;
}