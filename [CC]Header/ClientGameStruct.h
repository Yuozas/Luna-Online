#ifndef __CLIENTGAMESTRUCT_H__
#define __CLIENTGAMESTRUCT_H__

//------------------------------------------------------------------------------------
//
//	ClientGameStruct.h: Å¬¶óÀÌ¾ðÆ® °ÔÀÓ½Ã½ºÅÛ¿¡¼­¸¸ »ç¿ëÇÏ´Â struct Á¤ÀÇ
//
//	»ç¿ë ¿øÄ¢:
//	1. comment¸¦ ÇÑ´Ù
//	2. ÀÌ ÆÄÀÏ¿¡ »ç¿ëÇÏ±â¿¡ Àû´çÇÑÁö ´Ù½Ã ÇÑ¹ø »ý°¢ÇÑ´Ù
//
//------------------------------------------------------------------------------------
class CObject;
typedef CObject* LPOBJECT;

template<class T>
class ObjectArray {
public:
	DWORD nTempCount;	// ¼øÂ÷Á¢±ÙÀ» À§ÇÑ º¯¼ö
	DWORD nObjectNum;
	T pObjectArray[MAX_TARGET_NUM];
	void Init()	{	nObjectNum = 0;	}
	void AddObject(T Object)
	{
		pObjectArray[nObjectNum++] = Object;
	}
	void SetPositionHead()	{	 nTempCount = 0;	}
	T GetNextObject()		{
		if(nTempCount >= nObjectNum)
			return NULL;
		else
			return pObjectArray[nTempCount++];
	}
};

struct GameLoadingParam
{
	DWORD selectedCharNum;
	DWORD m_MapNum;
};

#endif //__CLIENTGAMESTRUCT_H__