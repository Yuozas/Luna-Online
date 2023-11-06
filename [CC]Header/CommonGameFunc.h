#ifndef __COMMONGAMEFUNC_H__
#define __COMMONGAMEFUNC_H__

// ----------------------------------------------------------------------------------
// 게임에서 사용하는 공통 Global Function (both client and server)
// ----------------------------------------------------------------------------------


class CObject;

extern BOOL g_bAssertMsgBox;
extern char g_szHeroIDName[];

char* GetCurTimeToString();
DWORD GetCurTimeValue();
DWORD GetCurTime();
DWORD GetCurTimeToSecond();
void StringtimeToSTTIME(stTIME* Time, char* strTime);
void SetProtocol(MSGBASE* pMsg,BYTE bCategory,BYTE bProtocol);
void SendAssertMsg(char* pStrFileName,int Line,char* pMsg);
void WriteAssertMsg(char* pStrFileName,int Line,char* pMsg);
void CriticalAssertMsg(char* pStrFileName,int Line,char* pMsg);

WP_KIND GetWeaponType(DWORD skillIdx);
eITEMTABLE GetTableIdxPosition(WORD abs_pos);
eQUICKICON_KIND GetIconKind(DWORD wIdx);
//eITEM_KINDBIT GetItemKind(DWORD iconIdx);
//eSKILLITEM_KIND GetItemKindDetail(DWORD iconIdx);

POSTYPE ConvAbsPos2SkillPos(POSTYPE abspos);
void SetScriptCheckValue( ScriptCheckValue& );
DWORD GetMainTargetID(MAINTARGET* pMainTarget);
BOOL GetMainTargetPos(MAINTARGET* pMainTarget,VECTOR3* pRtPos, CObject ** ppObject);

BOOL IsSameDay(char* strDay,WORD Year,WORD Month,WORD Day);

void SetVector3(VECTOR3* vec,float x,float y,float z);
float CalcDistanceXZ(VECTOR3* v1,VECTOR3* v2);
void VRand(VECTOR3* pResult,VECTOR3* pOriginPos,VECTOR3* pRandom);
void TransToRelatedCoordinate(VECTOR3* pResult,VECTOR3* pOriginPos,float fAngleRadY);
void RotateVectorAxisX(VECTOR3* pResult,VECTOR3* pOriginVector,float fAngleRadX);
void RotateVectorAxisY(VECTOR3* pResult,VECTOR3* pOriginVector,float fAngleRadY);
void RotateVectorAxisZ(VECTOR3* pResult,VECTOR3* pOriginVector,float fAngleRadZ);
void RotateVector( VECTOR3* pResult, VECTOR3* pOriginVector, float x, float y, float z );
void RotateVector( VECTOR3* pResult, VECTOR3* pOriginVector, VECTOR3 vRot );

void BeelinePoint(VECTOR3 * origin, VECTOR3 * dir, float dist, VECTOR3 * result);
void AdjacentPointForOriginPoint(VECTOR3 * origin, VECTOR3 * target, float dist, VECTOR3 * result);

char* GetMapName(MAPTYPE mapnum);
//LPTSTR GetMapName( MAPTYPE );
#ifdef _CLIENT_
void AddComma( char* pBuf );
char* AddComma( DWORD dwMoney );
char* RemoveComma( char* str );
//MAPTYPE GetMapNumForName( LPCTSTR );
WORD GetMapNumForName( char* pStrName );
//BOOL CutString( char* pStr, int nCutLen );
#endif
//BOOL IsCharInString(char* pStr, char* pNoChar);
BOOL IsVillage();
void SafeStrCpy( char* pDest, const char* pSrc, int nDestBufSize );
void SafeStrCpyEmptyChange( char* pDest, const char* pSrc, int nDestBufSize );

BOOL CheckValidPosition(VECTOR3& pos);

float roughGetLength( float fDistX,float fDistY );

BOOL IsCharInString(char* pStr, char* pNoChar);
void LoadEffectFileTable(char* pListFileName);
int FindEffectNum(char* pFileName);
#define ASSERTVALID_POSITION(vec)	CheckValidPosition(vec)

void ERRORBSGBOX(char* str,...);

const char* GetDay(WORD nDay);
WORD GetDay(const char* strDay);

const char* GetWeather(WORD nWeatherState);
WORD GetWeather(const char* strWeather);
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif
BOOL IsEmptyLine(char* pszText);
int	 GetPastTime(DWORD nDate);
int	 GetPastTime(char* pszDate);
#endif //__COMMONGAMEFUNC_H__
