#ifndef _FARMMANAGE_H
#define _FARMMANAGE_H

#include "./Interface/cDialog.h"
#include "../hseos/Farm/SHFarm.h"
#include "cImage.h"

class cStatic;
class CObjectGuagen;
class cPushupButton;

class CSHFarmManageGardenDlg : public cDialog
{
public:
	const static int	MAX_CROP_NUM	= 15;

protected:
	static int*			m_nImg;

	cPushupButton*		m_pcsCrop[MAX_CROP_NUM];

	cImage				m_csMineSign;
	BOOL				m_bMine[MAX_CROP_NUM];

public:
	CSHFarmManageGardenDlg() ;
	~CSHFarmManageGardenDlg() ;

	static void		SetCropImgInfo(int nState, int nCropKind, int nCropImg);
	static int		GetImgNum(int nCropKind)	{ return m_nImg[nCropKind-1]; }
	static void		Release();

	void			SetCropImg(int nCropIndex, int nCropKind, DWORD nCropOwner);
	cPushupButton*	GetCrop(int nCropIndex)		{ return m_pcsCrop[nCropIndex]; }

	void		 	Linking();
	virtual void 	SetActive( BOOL val );
	virtual void 	Render();

	virtual DWORD ActionEvent(CMouse* mouseInfo) ;
};


class CSHFarmManageDlg : public cDialog
{
protected:
	cStatic*				m_pcsMapName;			
	cStatic*				m_pcsFarmName;			
	cStatic*				m_pcsCropNum;			
	cStatic*				m_pcsCropName;			
	cStatic*				m_pcsCropOwner; 			
	cStatic*				m_pcsCropGrowTime;		
	cStatic*				m_pcsCropGrowRemainTime1;
	cStatic*				m_pcsCropGrowRemainTime2;
	cStatic*				m_pcsCropGrowStep1;		
	cStatic*				m_pcsCropGrowStep2;		
	cStatic*				m_pcsCropLife; 			
	cStatic*				m_pcsCropImg;

	CObjectGuagen*			m_pcsCropGrowRemainTimeGauge;
	CObjectGuagen*			m_pcsCropGrowStepGauge;
	CObjectGuagen*			m_pcsCropLifeGauge;
	
	cPushupButton*			m_pcsGardenTabBtn;
	cPushupButton*			m_pcsAnimalCageTabBtn;


	CSHFarmManageGardenDlg*	m_pcsGardenDlg;

	int						m_nFarmZone;
	CSHFarm					m_csFarm;

	CSHCrop*				m_pcsSelCrop;

public:
	CSHFarmManageDlg() ;
	~CSHFarmManageDlg() ;

	void		 	Linking();
	virtual void 	SetActive( BOOL val );
	virtual void 	Render();
	void			OnActionEvent( LONG lId, void* p, DWORD we );

	char*			GetFarmMapName(int nFarmZone);

	void			SetFarm(int nFarmZone, int nFarmID);
	void			Clear();
	CSHCrop*		GetCrop(int nCropIndex)				{ return m_csFarm.GetGarden(0)->GetCrop(nCropIndex); }
	CSHFarm*		GetFarm()							{ return &m_csFarm; }

	void			SetCropImg(int nCropIndex, int nCropKind, DWORD nCropOwner)	{ m_pcsGardenDlg->SetCropImg(nCropIndex, nCropKind, nCropOwner); }
	void			SetCropViewInfo(int nCropindex, BOOL bUpdate = FALSE);
};



#endif