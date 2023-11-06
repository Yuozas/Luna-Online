#pragma once

#include ".\Interface\cDialog.h"
#include ".\interface\cButton.h"
#include ".\interface\cGuagen.h"
#include ".\interface\cStatic.h"
#include ".\interface\cPushupButton.h"
#include "VirtualItem.h"
#include "cImageSelf.h"
#include "Item.h"
#include "../hseos/Farm/Animal.h"

class cButton;
class cIconDialog;
class CObjectGuagen;

class CAnimalDialog :
	public cDialog
{
	struct ImageData
	{
		cImageSelf*	pImage;
		VECTOR2*	pImageRect;
		VECTOR3		vDir;
		DWORD		dwCreateTime;
		float		fAlpha;

		void Clear() {pImage=NULL; pImageRect=NULL; dwCreateTime=0; fAlpha=0.0f;}
		void SetImage(cImageSelf* image, VECTOR2* imagerect, VECTOR3 dir) {
			pImage=image; pImageRect=imagerect; vDir=dir; dwCreateTime=gCurTime; fAlpha=1.0f;
		}
	};

	CAnimal*			m_pAnimal;
	// ���� ���� �ѹ�
	cStatic*			m_pSlotNum;
	// ���� �̸�
	cStatic*			m_pName;
	// ���� ������ ������
	CObjectGuagen*		m_pContentmentGuage;
	// ���� ������ ��ġ
	cStatic*			m_pContentment;
	// ���� HP ������
	CObjectGuagen*		m_pHPGuage;
	// ���� HP ��ġ
	cStatic*			m_pHP;
	// ���� ���ɵ� ������
	CObjectGuagen*		m_pInterestGuage;
	// ���� ���ɵ� ��ġ
	cStatic*			m_pInterest;
	// ���� �̹���
	cStatic*			m_pImage;
	// ��Ȯ�ϱ� ��ư
	cPushupButton*		m_pGetItemBtn;

	BOOL				m_bLockGetItemBtn;

	ImageData			m_CurImage;

	//cImageSelf			m_ImageAnimal[CAnimal::ANIMAL_KIND_MAX];
	//VECTOR2				m_ImageAnimalVT;
	static int*			m_nImg;

public:

	CAnimalDialog(void);
	virtual ~CAnimalDialog(void);

	void Linking();
	virtual void SetActive(BOOL val);
	virtual void Render();

	virtual BOOL FakeMoveIcon( LONG x, LONG y, cIcon* );

	VOID SetAnimal(CAnimal* pAnimal);
	VOID SetSlotNum(char* pText);
	VOID SetName(char* pText);
	VOID SetContentment(char* pText);
	VOID SetInterest(char* pText);
	VOID SetHP(char* pText);

	VOID SetContentmentValue(GUAGEVAL val, DWORD estTime);
	VOID SetInterestValue(GUAGEVAL val, DWORD estTime);
	VOID SetHPValue(GUAGEVAL val, DWORD estTime);

	VOID RefreshAnimalInfo();

	VOID SetAnimalImgInfo(int nState, int nAnimalKind, int nAnimalImg);
	static int		GetImgNum(int nAnimalKind)	{ return m_nImg[nAnimalKind]; }

	virtual void OnActionEvent(LONG id, void* p, DWORD event) ;

	cPushupButton*	GetItemBtn()	{	return m_pGetItemBtn;	}
};
