//////////////////////////////////////////////////////////////////////
// class name : cWindow 
// Lastest update : 2002.10.29. by taiyo
//////////////////////////////////////////////////////////////////////

#ifndef _cWINDOW_H_
#define _cWINDOW_H_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cObject.h"
#include "cImage.h"
#include "cMultiLineText.h"
#include "cWindowDef.h"


// 070202 LYW --- Define max count of display mode.
#define MAX_DISP_MODE	3

class CMouse;
class CKeyboard;


void defaultWindowFunc(LONG lId, void * p, DWORD we);

class cWindow : public cObject
{
public:

	cWindow();
	virtual ~cWindow();

    // 070202 LYW --- Add function to save display position.
	void SaveDispPoint( VECTOR2 point1, VECTOR2 point2, VECTOR2 point3 ) ;
	
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	virtual void Render();
	void ToolTipRender();
	virtual DWORD ActionEvent( CMouse * mouseInfo );			//���콺��(keypressüũ�� KEYBOARD(�۷ι�)�̿�)
	virtual DWORD ActionKeyboardEvent( CKeyboard* keyInfo ) { return WE_NULL; }	//Ű���常

	virtual void Add( cWindow* ) = 0;

	BOOL PtInWindow(LONG x, LONG y);
	// 070209 LYW --- cWindow : Modified setting position part.
	/*
	virtual void SetAbsXY(LONG x, LONG y){ m_absPos.x=(float)x; m_absPos.y=(float)y; }
	void SetRelXY(LONG x, LONG y){ m_relPos.x=(float)x; m_relPos.y=(float)y; }
	void SetValidXY(LONG x, LONG y) { m_validPos.x = (float)x; m_validPos.y=(float)y; }
	*/
	virtual void SetAbsXY(LONG x, LONG y){ m_absPos.x=(float)x; m_absPos.y=(float)y; m_bIsMovedWnd = TRUE ; }
	void SetRelXY(LONG x, LONG y){ m_relPos.x=(float)x; m_relPos.y=(float)y; m_bIsMovedWnd = TRUE ; }
	void SetValidXY(LONG x, LONG y) { m_validPos.x = (float)x; m_validPos.y=(float)y; m_bIsMovedWnd = TRUE ; }

	
	virtual void SetAutoClose(BOOL val){}
	virtual BOOL IsAutoClose(){ return FALSE; }

	//void SetWH(LONG x, LONG y){ m_width=(WORD)x; m_height=(WORD)y; }
	void SetWH(WORD wWidth, WORD wHeight) { m_width = wWidth; m_height = wHeight ;}
	void SetFontIdx(WORD wIdx){ m_wFontIdx=wIdx; }
	void SetFocus(BOOL val){ m_bFocus=val; }
	void SetDepend(BOOL val){ m_bDepend=val; }
	void SetMovable(BOOL val){ m_bMovable=val; }
	void SetBasicImage(cImage * image){ if(image) { m_BasicImage = *image; m_pCurImage = &m_BasicImage; } }
//	void SetToolTip(char * msg, DWORD color=RGB_HALF(255,255,0), cImage * image=NULL, DWORD imgColor = 0x00ffffff );
	void SetToolTip(char * msg, DWORD color=RGB_HALF(255,255,0), cImage * image=NULL, DWORD imgColor = TTCLR_DEFAULT );

	// 070803 ����, �ٹٲ޵Ǵ� ���� �߰�
	void SetAutoLineToolTip( const char*, DWORD color, cImage*, DWORD lineSize );

	void AddToolTipLine( const char* msg, DWORD color= TTTC_DEFAULT );
	cImage * GetBasicImage() { return &m_BasicImage; }
	
	BOOL IsMovable(){ return m_bMovable; }
	BOOL IsDepend(){ return m_bDepend; }
	BOOL IsFocus(){ return m_bFocus; }
	WORD GetWidth(){ return m_width; } 
	WORD GetHeight(){ return m_height; }
	virtual void SetHeight(WORD hei) { m_height = hei; }
	float GetAbsX(){ return m_absPos.x;	}
	float GetAbsY(){ return m_absPos.y;	}
	void SetAbsX(LONG x){ m_absPos.x=(float)x;	}
	void SetAbsY(LONG y){ m_absPos.y=(float)y;	}
	float GetRelX(){ return m_relPos.x;	}
	float GetRelY(){ return m_relPos.y;	}
	virtual void SetAlpha(BYTE al){ m_alpha = al; }
	virtual void SetOptionAlpha(DWORD dwAlpha) { m_dwOptionAlpha = dwAlpha; }
	WORD GetFontIdx() { return m_wFontIdx; }

	void SetToolTipImageRGB( DWORD color ) { toolTip.SetImageRGB( color ); }
	void SetImageRGB(DWORD val) { m_dwImageRGB = val; }
	void SetcbFunc(cbFUNC Func=defaultWindowFunc){ if(Func) cbWindowFunc=Func; }
	void ExcuteCBFunc( DWORD we );

	BOOL IsAlwaysTop() { return m_bAlwaysTop; }
	void SetAlwaysTop( BOOL bAlwaysTop ) { m_bAlwaysTop = bAlwaysTop; }
	void SetTopOnActive( BOOL bTop ) { m_bSetTopOnActive = bTop; }
	void SetDestroy() { m_bDestroy = TRUE; }
//	virtual void SetTopWindow() {}

	// cIcon only used  //????? taiyo what;s this?
//	inline void SetFlag(DWORD flag)	{	m_dwFlag= flag;	}
//	inline DWORD GetFlag()	{	return m_dwFlag;	}
	void SetImageSrcRect(RECT * pRect){ CopyRect(&m_imgSrcRect, pRect); }
	void SetScale(cImageScale * scale)	{
											m_scale.x = scale->x;
											m_scale.y = scale->y;
										}
	// 070201 LYW --- Add function to return image scale.
	cImageScale* GetImageScale() { return &m_scale ; }

	// 070209 LYW --- cWindow : Add function to setting and return whether moved window or not.
	void SetMovedWnd( BOOL val ) { m_bIsMovedWnd = val ; }
	BOOL IsMovedWnd() { return m_bIsMovedWnd ; }
	
	virtual void SetFocusEdit(BOOL val) {}

	// desc_hseos_���͹���01
	// S ���͹��� �߰� added by hseos 2007.05.30
	//----------------------------------------------------------------------------------------------------------------
	//		�ڽ��� ���İ� ��뿩�� ���� (�θ� �������� ���İ��� ������ ���� ���)
	void	SetUseOwnAlpha(BOOL bUse)	{ m_bOwnAlpha = bUse; }
	//----------------------------------------------------------------------------------------------------------------
	//		�ڽ��� ���İ� ��뿩�� ��� (�θ� �������� ���İ��� ������ ���� ���)
	BOOL	GetUseOwnAlpha()			{ return m_bOwnAlpha; }
	// E ���͹��� �߰� added by hseos 2007.05.30

	// desc_hseos_�ػ�01
	// S �ػ� �߰� added by hseos 2007.06.12
	void	SetDeltaPos(int nIndex, VECTOR2 point);
	// E �ػ� �߰� added by hseos 2007.06.12

protected:
	void SetCurImage(cImage * image){ m_pCurImage = image; }			// for improving performance
	void GetVariablePostion(SIZE& standardResolution, POINT& standardPosition, SIZE& currentResolution);
protected:
	cbFUNC cbWindowFunc;
	// attribute
	WORD m_wFontIdx;
	BOOL m_bFocus;
	BOOL m_bMovable;
	BOOL m_bDepend;				// for protecting rendering within IconDialog
	BOOL m_bDestroy;
	
	//KES 030829 �߰�
	BOOL m_bAlwaysTop;
	BOOL m_bSetTopOnActive;
	/// tooltip related member
//	char m_strToolTipMsg[MAX_TOOLTIPSTR_SIZE];			// reserved
	cImage * m_pToolTipImage;			// reserved
	DWORD m_toolTipFgColor;
	DWORD m_dwViewCurTime;
	DWORD m_dwViewLastTime;
	LONG m_ttX, m_ttY;
	cMultiLineText toolTip;
	///

	cImage m_BasicImage;
	cImage * m_pCurImage;

	DWORD m_dwImageRGB;
	DWORD m_alpha;
	DWORD m_dwOptionAlpha;   //KES 030825 �ɼǿ��� ������ ��ü �����쿡 ����� ���İ�.
	// numerical value
public:
	VECTOR2 m_absPos;
	VECTOR2 m_relPos;
	RECT	m_imgSrcRect;
	// 070209 LYW --- cWindow : Add RECT for render part.
	RECT	m_renderRect ;
	RECT	m_renderShadowRect ;
	BOOL	m_bIsMovedWnd ;

	// 070202 LYW --- Add cPoint for display point.
	BOOL	m_bCheckDispPoint ;
	VECTOR2	m_vDispPoint[MAX_DISP_MODE] ;
private:
	VECTOR2 m_validPos;
protected:

	WORD m_width;
	WORD m_height;
	cImageScale		m_scale;

	// desc_hseos_���͹���01
	// S ���͹��� �߰� added by hseos 2007.05.30
	BOOL	m_bOwnAlpha;					// �ڽ��� ���İ� ��� ���� (�θ� �������� ���İ��� ������ ���� ���)
	// E ���͹��� �߰� added by hseos 2007.05.30

	// desc_hseos_�ػ�01
	// S �ػ� �߰� added by hseos 2007.06.12
	VECTOR2 m_vDeltaPos[2];
	// E �ػ� �߰� added by hseos 2007.06.12
};

#endif // _cWINDOW_H_
