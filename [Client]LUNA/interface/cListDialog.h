#pragma once


#include "cDialog.h"
#include "cListItem.h"

enum {LD_TOP, LD_MIDDLE, LD_DOWN};

// 061209 LYW --- Add enum for list dialog's style.
enum {	LDS_NORMAL,
LDS_SERVERLIST, 
LDS_FONTSHADOW,		// ����Ʈ ���̾�α׿� �߰��Ǵ� ��Ʈ�� ������ �� �� �ְ�, �׸��ڰ� ����� �ؽ�Ʈ�� ����Ѵ�.
LDS_NOTICE_QUEST,	// ����Ʈ ������ ���� ��Ÿ���� �߰��Ѵ�.
} ;
// 070816 LYW --- Add enum for list dialog's extend or reduction.
enum {
	LD_EXTEND = 0,
	LD_REDUCTION,
} ;

class cGuageBar;
class cIcon;

struct sGAMENOTIFY_MSG;


class cListDialog : public cDialog, public cListItem
{
public:
	cListDialog();
	virtual ~cListDialog();

	virtual void Init(LONG x, LONG y, WORD wid, cImage * topImage, WORD topHei, cImage * middleImage, WORD middleHei, cImage * downImage, WORD downHei, cImage * overImage, BYTE num, LONG ID);
	virtual void InitList(WORD wMaxLine, RECT * textClippingRect);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void Add(cWindow * btn);
	virtual void Render();

	void RenderNormalList() ;// �⺻ ������� �����ϴ� �Լ�.
	void RenderServerList() ;
	void RenderShadowList() ;
	void RenderQuestNotice() ;
	virtual void SetActive( BOOL );
	BOOL IsMaxLineOver();

	BOOL Resize(BYTE middleNum);
	WORD GetHeight(BYTE i)
	{
		if(i == LD_TOP) return m_topHeight;
		if(i == LD_MIDDLE) return m_middleHeight;
		if(i == LD_DOWN) return m_downHeight;
		return 0;
	}
	BYTE GetMiddleImageNum(){ return m_middleNum; }
	void SetMinMaxMiddleNum(BYTE min, BYTE max) { m_minMiddleNum = min; m_maxMiddleNum = max; }
	void SetOverImageScale(float x, float y) { m_OverImageScale.x = x; m_OverImageScale.y = y; }

	// 070529 LYW --- Add function to return min num.
	BYTE GetMinMiddleNum() { return m_minMiddleNum ; }
	BYTE GetMaxMiddleNum() { return m_maxMiddleNum ; }

	void OnUpwardItem();
	void OnDownwardItem();
	int PtIdxInRow(LONG x, LONG y);

	virtual void AddItem( const char * text, DWORD color, int line=-1);

	// 080225 LUJ, ������ �Բ� ����Ʈ �������� �߰��Ѵ�
	typedef std::list< std::string >	ToolTipTextList;
	void AddItem( const char* text, DWORD color, const ToolTipTextList& );

	// 080225 LUJ, ������ �����ϸ鼭 ������ �����Ѵ�
	virtual void RemoveItem( const char* );
	virtual void RemoveItem( DWORD index );	

	void AddItemWithFont(char* str, DWORD color, int fontIdx, int line=-1) ;
	void AddNoticeItem(sGAMENOTIFY_MSG* pGameNotifyMsg) ;

	void SetExtendReduction(int nSetMode) ;										// ����Ʈ ���̾�α׸� Ȯ��/��� �ϴ� �Լ�.(����Ʈ �˸��� ���̾�α׿��� ���)

	void onSize(BOOL fSize);
	//	virtual void ListMouseCheck( LONG x, LONG y, DWORD we );
	void ListMouseCheck( CMouse * mouseInfo );

	int GetCurSelectedRowIdx();
	void SetCurSelectedRowIdx(int Idx);
	void ResetGuageBarPos();
	void SetLineHeight( LONG height );

	void SetAutoScroll( BOOL bAuto ) { m_bAutoScroll = bAuto; }
	int GetClickedRowIdx()	const		{ return m_ClickSelected ; }
	void SetClickedRowIdx( int row )	{ m_ClickSelected = row; }

	// 071128 ����. ���õ� ������ ���ڿ� ��ȯ
	const char* GetClickedItem();

	int GetSelectRowIdx()		{ return mSelectedRowIdx;	}
	//int GetPressedRowIdx()		{ return mPressedRowIndex; }
	int GetTopListItemIdx()		{ return m_lTopListItemIdx;	}
	void SetTopListItemIdx(int Idx);	

	cPtrList* GetListItem()		{ return &m_ListItem; }

	BOOL PtInWindow(LONG x, LONG y);

	void SetShowSelect(BOOL val);
	// 061209 LYW --- Setting to show scroll part.
	void SetShowScroll( BOOL val ) { m_bShowScroll = val ; }
	// 061209 LYW --- Setting to disable scroll part.
	void SetDisableScroll() ;
	// 061209 LYW --- Setting style.
	void SetStyle( BYTE style ) { m_bStyle = style ; m_lLineHeight += 10 ; }
	// 061209 LYW --- Setting align text.
	void SetAlign( int align ) { m_nAlign = align ; }
	// 061225 LYW --- Add function to return upward button.
	cButton* GetUpwardBtn() { return m_pUpwardBtn ; }
	cButton* GetDownwardBtn() { return m_pDownwardBtn ; }
	// 061225 LYW --- Add unction to return guagebar.
	cGuageBar* GetGuageBar() { return m_pGuageBar ; }

	// 070620 LYW --- cListDialog : Add function to setting color for column of this dialog.
	void SetColumnColor(DWORD dwColor) { m_dwColumnColor = dwColor ; }
	void SetSelectColumnColor(DWORD dwColor) { m_dwSelectColumnColor = dwColor ; }

	// 070709 ����, ����Ʈ ��ġ�� ����(����0~1)
	void SetPosition( float ratio );
	void SetTextRect( const RECT& );

	// �ڵ����� �ٹٲ����ش�
	void AddItemAutoLine( const char*, DWORD color );

	// 071017 LYW --- cListDialog : Add function to setting and return font index to render shadow.
	void Set_ShadowFontIndex(WORD wIndex) { m_wShadowFont = wIndex ; }
	WORD Get_ShadowFontIndex() { return m_wShadowFont ; }

	//  071018 LYW --- cListDialog : Add function to setting and return shadow distance.
	void Set_ShadowDistance(int nDistance) { m_nShadowDistance = nDistance ; }
	int  Get_ShadowDistance() { return m_nShadowDistance ; }

	// 071128 ����
	virtual void RemoveAll();


protected:

	LONG m_lLineHeight;
	LONG m_lLineNum;
	LONG m_lTopListItemIdx;
	cImage m_TopImage;		WORD m_topHeight;
	cImage m_MiddleImage;	WORD m_middleHeight;
	cImage m_DownImage;		WORD m_downHeight;

	BYTE m_middleNum;
	BYTE m_minMiddleNum;
	BYTE m_maxMiddleNum;
	RECT m_textRelRect;						// only left,top,bottom field is used

	cButton * m_pUpwardBtn;
	cButton * m_pDownwardBtn;
	cButton * m_pLengthLongBtn;
	cButton * m_pLengthShortBtn;

	cGuageBar * m_pGuageBar;

	// ���콺 Ŭ���� ���� ó��: ���콺 Ŀ���� ��ġ�� �� ��ȣ
	int		mSelectedRowIdx;

	//int		mPressedRowIndex;

	int		m_ClickSelected;

	cImage			m_OverImage;
	cImageScale		m_OverImageScale;
	float			m_fGap;

	BOOL			m_bAutoScroll;
	BOOL			m_bShowSelect;

	// 061209 LYW --- Add boolean variable wether show scroll part or not.
	BOOL			m_bShowScroll ;

	// 061209 LYW --- Add new style for list dialog.
	BYTE			m_bStyle ;
	int				m_nAlign;

	// 070620 LYW --- cListDialog : Add columncolor for list dialog.
	DWORD			m_dwColumnColor ;
	DWORD			m_dwSelectColumnColor ;

	// list bar
	//cImage * m_pBarImage;
	//VECTOR2	m_barListPos;
	//cSize	m_barListSize;
	//int		m_interval;
	//BOOL	m_fBarDrag;
	//LONG	m_dragGabY;
	RECT			mTextRect;

	cPtrList		m_NoticeList ;

	// 071017 LYW --- cListDialog : Add font index to render shadow.
	WORD			m_wShadowFont ;

	// 071018 LYW --- cListDialog : Add shadow distance to use render shadowfont.
	int				m_nShadowDistance ;

	// 080225 LUJ, ������ �������� �����Ѵ�. Ű: ����Ʈ ������ �ε���
	struct ToolTip
	{
		std::string	mText;
		cIcon*		mIcon;
	};
	typedef std::list< ToolTip >	ToolTipList;
	ToolTipList						mToolTipList;

	// 080225 LUJ, �������� ����
	cIcon*	mToolTipIcon;
};