// MHMap.h: interface for the CMHMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MHMAP_H__444BAED6_A7A5_4CB6_B1BF_027999B116FE__INCLUDED_)
#define AFX_MHMAP_H__444BAED6_A7A5_4CB6_B1BF_027999B116FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\Engine\EngineMap.h"
#include ".\Engine\EngineSky.h"
#include ".\Engine\EngineLight.h"
#include ".\Engine\EngineObject.h"
#include "EngineCloud.h"

class CTileManager;
class CMHFile;

#define MAP		USINGTON(CMHMap)

struct MAPDESC
{
	MAPDESC()
	{
		DefaultSight = 8000;
		Fov = 45;

		fogdesc.bEnable = TRUE;
		fogdesc.dwColor = 0xaaaaaaaa;
		fogdesc.fDensity = 0.5f;
		fogdesc.fStart = 20000;
		fogdesc.fEnd = 80000;
		BGMSoundNum = 0;
		
		strcpy(MapFileName,"new_gae_002.map");
		strcpy(TileFileName,"");
		strcpy(SkyMod,"SkyBox.MOD");
		strcpy(SkyAnm,"SkyBox.ANM");
		
		strcpy(SunObject,"bg_login.mp3");

		bSun = 0;
		bSky = 1;

		Ambient = 0xaaaaaaaa;
		
		SunPos.x = 800;
		SunPos.y = 1300;
		SunPos.z = 200;		
		
		backColor = 0x00000000;

		SunDistance = 7000;

		bFixHeight = FALSE;

		CloudNum = 0;
		CloudHMin = 1500;
		CloudHMax = 2000;
		strcpy(CloudListFile,"CloudList.txt");

		
		strcpy(CameraFilter,"");
		CameraFilterDist = 20;


		//trustpak 2005/04/15
		SkyOffset.x = 0.0f;
		SkyOffset.y = 0.0f;
		SkyOffset.z = 0.0f;
		///
	}
	BOOL bFixHeight;
	float FixHeight;
	VECTOR3 SunPos;

	DWORD Ambient;

	float DefaultSight;
	float Fov;
	
	FOG_DESC fogdesc;
	
	char MapFileName[64];
	char TileFileName[64];
	
	BOOL bSky;
	char SkyMod[64];
	char SkyAnm[64];

	SNDIDX BGMSoundNum;
	float BGMVolume;
	
	BOOL bSun;
	char SunObject[64];

	DWORD backColor;

	float SunDistance;

	char CloudListFile[64];
	DWORD CloudNum;
	int CloudHMin;
	int CloudHMax;

	char CameraFilter[64];
	float CameraFilterDist;
	
	//trustpak 2005/04/16
	VECTOR3 SkyOffset;

	///
};

class CMHMap  
{
	BOOL m_bIsInited;
	MAPTYPE m_MapNum;
	VECTOR3 m_ShadowPivotPos;

	MAPDESC* mapDesc;
	CEngineMap map;
	CEngineSky* sky;
	CTileManager* m_pTileManager;
	CEngineObject m_SunObject; 
	CEngineObject m_CameraFilterObject; 

	CEngineLight m_Shadowlight;
	
	CYHHashTable<CEngineCloud> m_CloudTable;

////
	BOOL	m_bVillage;
	VECTOR3 m_SunPosForShadow;
	
		
public:

	BOOL LoadMapDesc(MAPTYPE MapNum);
	void ApplyMapDesc();

	CMHMap();
	~CMHMap();
	DWORD GetTileWidth();
	MAPDESC* GetMapDesc()	{	return mapDesc;	}

	void LoadNpcForQuestDialog() ;

	void LoadStaticNpc(MAPTYPE MapNum);
	void LoadPreData(MAPTYPE MapNum);
	BOOL IsInited()	{	return m_bIsInited;	}
	void InitMap(MAPTYPE MapNum);

	void Release();
	
	void Restore();
	void Invalidate();

	void Process(DWORD tick);
	
	BOOL CollisonCheck(VECTOR3* pStart,VECTOR3* pEnd,VECTOR3* pRtCollisonPos,CObject* pObj);		// �浹�ϸ� TRUE��ȯ�ϰ� pRtCollisonPos�� ���ο� Ÿ�� ����

	void SetShadowPivotPos(VECTOR3* pPivotPos);
	
	CTileManager* GetTileManager()		{	return m_pTileManager;	}
	MAPTYPE GetMapNum()	{	return m_MapNum;	}
	

	void SetMapAmbient( DWORD dwAmbient ) { map.SetAmbientColor(dwAmbient); }
	

	BOOL IsInTile(int x, int y, MAPTYPE MapNum,CObject* pObject);
	BOOL CollisionTilePos(int x, int y, MAPTYPE MapNum,CObject* pObject);
	BOOL CollisionTileCell(int x, int y, MAPTYPE MapNum,CObject* pObject);

	BOOL CollisionLine(VECTOR3* pStart,VECTOR3* pEnd, VECTOR3 * Target, MAPTYPE MapNum,CObject* pObject);
	BOOL NonCollisionLine(VECTOR3* pStart, VECTOR3* pEnd, VECTOR3* Target, MAPTYPE MapNum, CObject* pObject);

	////
	BOOL IsVillage() { return m_bVillage; }
	void SetVillage( BOOL bVillage ) { m_bVillage = bVillage; }

	// quest npc
	DWORD	m_dwQuestNpcId;
	void	AddQuestNpc( QUESTNPCINFO* pInfo );
};

EXTERNGLOBALTON(CMHMap)

#endif // !defined(AFX_MHMAP_H__444BAED6_A7A5_4CB6_B1BF_027999B116FE__INCLUDED_)
