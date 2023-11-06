/*
080515 LUJ, libcurl�� �̿��Ͽ� HTTP �������ݷ� ������ �ٿ�ε�޴´�
			���õ� �Լ�, ������ Ŭ���� �ȿ� ��Ű¡�Ǿ� �ִ�.
			
			�ϳ��� �����忡�� �����ϹǷ�, ������ ����� �������� �ʴ´�
*/
#pragma once
//090819 ONS ���÷�óUI�� ���ۼӵ��� �����ð��� ǥ��.
typedef void ( *PursuitFunction )( double percent, double remaintime, double down_speed );
typedef void CURL;

class CZipMemFile;

class CNetwork
{
public:
	static CNetwork& GetInstance();
	virtual ~CNetwork(void);

	static int WriteToMemory( char* data, size_t size, size_t nmemb, void* );
	static int WriteToDisk( char* data, size_t size, size_t nmemb, void* );
	static int Pursuit( LPVOID p, double doubleTotal, double doubleNow, double unsignedLongTotal, double unsignedLongNow );
	// 081201 LUJ, �ٿ�ε� ����� ��ȯ�ϵ��� �Ѵ�
	// 081202 LUJ, �ٿ�ε带 �޸𸮿� ���� �� �ֵ��� �Ѵ�
	static BOOL Download( const TCHAR* path, const TCHAR* fileName, PursuitFunction callBackFunctionPursuit, bool isMemoryDownload );
	// 080625 LUJ, DLL �ʱ�ȭ
	bool Initialize();
	void Release();

	CZipMemFile&	GetMemoryFile() const;
	CFile&			GetDiskFile() const;

private:
	CNetwork(void);

private:
	CURL*	mCurl;
	// 080625 LUJ, libcurl.dll�� ��ġ ���Ͽ� ���ԵǾ� ����. ���α׷��� ���� ���� �� dll�� ���ε��Ǿ� �־�
	//				���������� �� ������ �߻���. �̸� ���ϱ� ���� dll�� �������� �����Ͽ�, ���� Ǯ�� ����
	//				dll ����� �����ϱ����� �Ʒ� ���� �߰�
	HMODULE	mLibraryModule;

	PursuitFunction mCallBackFunctionPursuit;

};
