

//  ���� �ΰ� �ʱ�ȭ.
//  ���� �̸��� �ΰ��� �ָ� �⺻���� log.txt��..
//int LogInit( char* FileName);

//int LogExit(void);
#define LogEnable

int LogInit(void);

int Log( char* Sentence);

int LogPrintf( char* szFormat, ...);

