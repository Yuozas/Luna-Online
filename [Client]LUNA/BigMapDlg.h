#pragma once


#include "Interface/cDialog.h"
#include "MiniMapHeroIcon.h"
#include "cImageSelf.h"

// desc_hseos_³óÀå½Ã½ºÅÛ_01
// S ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2007.10.31
class CSHFarm;
// E ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2007.10.31

class CBigMapDlg : public cDialog
{
protected:

	CMiniMapHeroIcon				m_HeroIcon;	
	CYHHashTable<CMiniMapIcon>		m_IconTable;
	CMemoryPoolTempl<CMiniMapIcon>	m_IconPool;
	CMiniMapIconImage				m_pIconImage[eBMICON_MAX];

	CMiniMapIcon*					m_pIconForToolTip;

	cImageSelf		m_MapImage;
	WORD			m_MapNum;

	cImage			m_TooltipImage;

	// 070209 LYW --- Add VECTOR2.
	VECTOR2 m_vScale ;

public:
	CBigMapDlg();
	virtual ~CBigMapDlg();

	virtual void SetActive( BOOL val );
	virtual void Render();
	void Linking();
	void LoadMinimapImageInfo( MAPTYPE MapNum );
	void InitBigMap( MAPTYPE MapNum );

	void Process();
	void InitIconImage();

	CMiniMapIcon* GetEmptyMiniMapIcon( WORD Kind,CObject* pObject );
	CMiniMapIcon* GetEmptyMiniMapIcon( WORD Kind,PARTY_MEMBER* pMember );
	// desc_hseos_³óÀå½Ã½ºÅÛ_01
	// S ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2007.10.31
	CMiniMapIcon* GetEmptyMiniMapIcon( WORD Kind, CSHFarm* pFarm );
	// E ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2007.10.31
	void ReleaseMinniMapIcon( CMiniMapIcon* pIcon );
	void SetPartyIconObjectPos(DWORD PlayerID, int posX, int posZ);
	void SetPartyIconTargetPos(DWORD PlayerID, int posX, int posZ);
	
	void AddHeroIcon( CObject* pObject );
	void AddStaticNpcIcon( CObject* pObject );
	void AddPartyMemberIcon( PARTY_MEMBER* pMember );
	// desc_hseos_³óÀå½Ã½ºÅÛ_01
	// S ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2007.10.31
	void AddFarmIcon(CSHFarm* pFarm);
	// E ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2007.10.31

	void AddIcon( WORD Kind, CObject* pObject );
	void AddIcon( WORD Kind, PARTY_MEMBER* pMember );
	// desc_hseos_³óÀå½Ã½ºÅÛ_01
	// S ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2007.10.31
	void AddIcon(WORD Kind, CSHFarm* pFarm);
	// E ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2007.10.31

	void RemoveIcon( DWORD ObjectID );
	virtual DWORD ActionEvent( CMouse* mouseInfo );

	BOOL CanActive();

	void ShowQuestMarkIcon( CObject* pTarget, int nKind );


private:
	SIZE m_mapSize;
};