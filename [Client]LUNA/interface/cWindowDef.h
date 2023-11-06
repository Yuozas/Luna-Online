#ifndef _cWINDOWDEF_H_
#define _cWINDOWDEF_H_

//#include "..\Engine\GraphicEngine.h"
//#include "../4DyuchiGRX_common/IGeometry.h"

#define MAX_TOOLTIPSTR_SIZE		256		//KES. 빡빡하다.. 
#define MAX_LISTITEM_SIZE		256		//128-->256
#define MAX_TEXT_SIZE			128
#define MAX_COMBOTEXT_SIZE		128
#define MAX_REPORTTEXT_SIZE		128
#define MAX_CHECKBOXTEXT_SIZE	128
#define TOOLTIP_DELAY_TIME		100
#define LINE_HEIGHT				15
#define DEFAULT_CELLSIZE		35
#define DEFAULT_ICONSIZE		DEFAULT_CELLSIZE
#define DEFAULT_CELLBORDER		2
#define MAX_COMMONDLGID_NUM		1000


//Icon Color
#define ICONCLR_USABLE				RGB_HALF( 255, 255, 255 )
#define ICONCLR_DISABLE				RGB_HALF( 255, 100, 100 )
#define ICONCLR_DISABLE_BYZEROSUNG	RGB_HALF( 160, 160, 160 )

#define ICONALPHA_USABLE				255
#define ICONALPHA_DISABLE				224
#define ICONALPHA_DISABLE_BYZEROSUNG	192

#define ICONTEXT_USABLE				RGB_HALF( 255, 255, 255 )
#define ICONTEXT_DISABLE			RGB_HALF( 180, 180, 180 )
#define ICONTEXT_DISABLE_BYZEROSUNG	RGB_HALF( 180, 180, 180 )
//////////


//Tooltip Color
#define TOOLTIP_ALPHA		220

//툴팁 바탕
#define TTCLR_DEFAULT						RGB_HALF( 51,  81, 125 )
#define TTCLR_ITEM_CANEQUIP					RGB_HALF( 30,  30, 30 )
#define TTCLR_ITEM_CANNOTEQUIP				RGB_HALF( 30,  30, 30 ) //RGB_HALF( 51,  81, 125 )
#define TTCLR_SKILL_CANUSE					TTCLR_ITEM_CANEQUIP
#define TTCLR_SKILL_CANNOTUSE				TTCLR_ITEM_CANNOTEQUIP
#define TTCLR_SKILLBOOK_CANCONVERT			TTCLR_ITEM_CANEQUIP
#define TTCLR_SKILLBOOK_CANNOTCONVERT		TTCLR_ITEM_CANNOTEQUIP

//툴팁 글씨
#define TTTC_SKILLLEVEL			RGB_HALF( 112, 212, 255 )
#define TTTC_DEFAULT			RGB_HALF( 240, 240, 240 )
#define TTTC_SKILLNAME			TTTC_DEFAULT
#define TTTC_EXTRAATTR			RGB_HALF( 84, 143, 223 )		//+머머머(%)
#define TTTC_EXTRAATTRITEM		TTTC_EXTRAATTR					//검+1등 속성가진 아이템
#define TTTC_RAREITEM			RGB_HALF( 85, 252, 85 )		//!임시 레어 아이템 
#define TTTC_NUMBER				RGB_HALF( 84, 143, 223 )		//공격력 0~0
#define TTTC_LIMIT				RGB_HALF( 230, 20,  20 )		//레벨0부터사용
#define TTTC_FREELIMIT			TTTC_DEFAULT					//레벨제한 풀림
#define TTTC_SKILLBOOKKIND		RGB_HALF( 230, 232, 38 )		//검외공서
#define TTTC_SKILLKIND			RGB_HALF( 230, 232, 38 )		//외공, 내공등
#define TTTC_SKILLEXP			RGB_HALF( 230, 232, 38 )		//무공경험치
#define TTTC_SKILLNEEDMANA		RGB_HALF( 230, 232, 38 )		//필요마나
#define TTTC_SKILLATTRKIND		TTTC_EXTRAATTR					//무공송석종류(화속성)
#define TTTC_EXTRAITEM			TTTC_DEFAULT
#define TTTC_BUYPRICE			RGB_HALF( 164, 213, 45 )
#define TTTC_SELLPRICE			TTTC_BUYPRICE
#define	TTTC_DESCRIPTION		RGB_HALF( 200, 215, 170 )
#define TTTC_MACROKEY			RGB_HALF( 250, 250, 10 )
#define TTTC_QUESTITEM			RGB(0, 200, 255)

#define TTTC_ITEM_NAME			RGB_HALF(255, 255, 255)
#define TTTC_ITEM_KIND			RGB_HALF(193, 126, 39)
#define TTTC_ITEM_LIMIT			RGB_HALF(163, 163, 163)
#define TTTC_ITEM_DATA			RGB_HALF(204, 192, 92)
#define TTTC_ITEM_VALUE			RGB_HALF(0, 255, 0)	

#define TTTC_SHOPITEMOPTION		RGB(255, 128, 128)

//공성전 이름바탕
#define TTCLR_CASTLEGUILD		RGB_HALF( 237,  226, 52 )
#define TTCLR_SIEGEGUILD		RGB_HALF( 77,  192, 37 )
#define TTCLR_ENERMY			RGB_HALF( 248,  50, 50 )

//////////////////////////////////////////////////////////////
// 06. 06. 2차 전직 - 이영준
// 무공 변환
#define TTTC_TRANSSKILLNAME	RGB_HALF( 204, 146, 248 )
#define TTTC_TRANSSKILLOPTION	TTTC_EXTRAATTR
//////////////////////////////////////////////////////////////

/////////////



//KES INTERFACE 030922
enum TEXT_ALIGN
{
	TXT_DEFAULT	= 0,
	TXT_LEFT	= 1,
	TXT_RIGHT	= 2,
	TXT_CENTER	= 4,
	TXT_TOP		= 128,
	TXT_BOTTOM	= 256,
	TXT_MIDDLE	= 512,
};




enum WINDOW_EVENT 
{ 
	WE_NULL=0, WE_CLOSEWINDOW=1, WE_TOPWINDOW = 2, WE_CHANGETEXT=4, 
	WE_RETURN=8, WE_PUSHUP=16, WE_PUSHDOWN=32, 
	WE_BTNCLICK=64, 
	WE_SPINBTNUP=128, WE_SPINBTNDOWN=256, WE_RBTNCLICK=512, WE_LBTNCLICK=1024, 
	WE_COMBOBOXSELECT=2048,WE_ROWCLICK=4096,WE_CELLSELECT=8192,WE_CHECKED=16384,WE_NOTCHECKED=32768,
	WE_LBTNDBLCLICK=65536,WE_RBTNDBLCLICK=131072,WE_DESTROY=262144,WE_SETFOCUSON=524288,
	WE_MOUSEOVER=1048576, WE_ACTIVEWINDOW = 2097152, WE_ROWDBLCLICK = 4194304,
};

enum WINDOW_TYPE 
{ 
	WT_NULL=0, WT_WINDOW=1, WT_EDITBOX = 2, WT_LISTBOX = 3, WT_BUTTON = 4, WT_STATIC = 5, 
	WT_SPIN = 6, WT_LISTDIALOG=7, WT_PUSHUPBUTTON=8, WT_GRIDDIALOG=9, 
	WT_LIST = 11,
	WT_COMBOBOX = 12,
	WT_LISTCTRL = 13,
	WT_GUAGEBAR = 14,
	WT_GUAGE = 15,
	WT_ANI = 16,
	WT_TEXTAREA = 17,
	WT_CHECKBOX = 18,
	WT_GUAGEN = 19,
	WT_GUAGENE = 20,
	WT_COMBOBOXEX = 21,
	WT_LISTCTRLEX = 22,
	WT_BROWSER = 23,	//---BROWSER

	WT_DIALOG = 64,
	/*dlg custom window type*/
	WT_CHARINFODIALOG = 65,
	WT_MAINDIALOG = 67,
	WT_CHATDIALOG = 68,
	WT_MONEYDIALOG = 69,
	WT_PARTYDLG = 70,
	WT_PARTYMINDLG = 71,
	WT_CHARGUAGEDIALOG = 72,
//	WT_MUNPADLG = 73,
//	WT_MUNPACREATEDLG = 74,
	WT_MUNPAMARKDLG = 75,
	WT_STORAGEDLG,
	WT_NOTEDLG,
	WT_FRIENDDLG,
	WT_MININOTEDLG,
	WT_MINIFRIENDDLG,
	WT_MONSTERGUAGEDLG,
	WT_NPCSCRIPDIALOG,
	WT_CHARMAKEDIALOG, 
	WT_HELPDIALOG,
	WT_HELPERSPEECHDIALOG,
	WT_MSGBOX,
	WT_WANTEDDIALOG,
	WT_WANTNPCDIALOG,
	WT_WANTREGISTDIALOG,
	WT_JOURNALDIALOG,
//	WT_DIVIDEBOX,
	WT_PKLOOTINGDLG,
	WT_MPMISSIONDLG,

	WT_PARTYCREATEDLG,
	WT_PARTYINVITEDLG,
	WT_MPGUAGEDLG,
	WT_GUILDCREATEDLG,
	WT_GUILDDLG,
	WT_GUILDINVITEDLG,
	WT_GUILDMARKDLG,
	WT_GUILDLEVELUPDLG,
	WT_GUILDNICKNAMEDLG,
	WT_GUILDRANKDLG,
	WT_GUILDUNIONCREATEDLG,	

	WT_ITEMSHOPDIALOG,
	WT_ITEMSHOPINVEN,
	WT_SKPOINTDIALOG,
	WT_SKPOINTNOTIFYDIALOG,

	WT_MOVEDIALOG,
	WT_SHOUT_DLG,
	WT_SHOUTCHAT_DLG,
	WT_CHASE_DLG,
	WT_CHASEINPUT_DLG,
	WT_NAMECHANGE_DLG,
	WT_NAMECHANGENOTIFY_DLG,
	
	WT_GTREGIST_DLG,
	WT_GTREGISTCANCEL_DLG,
	WT_GTSTANDING_DLG,
	WT_GTBATTLELIST_DLG,
	WT_GTSCOREINFO_DLG,
	
	WT_ICON = 128,
	/*ICON custom window type*/
	WT_SKILL, WT_JINBUB,
	// 070216 LYW --- cWindowDef : Add window type for conduct.
	WT_CONDUCT,

	WT_ITEM, WT_QUICKITEM, WT_STALLITEM,
	WT_EXCHANGEITEM,
	WT_DEALITEM,
	WT_VIRTUALITEM,
	WT_ITEMSHOW,
	WT_MENUICON,



	WT_ICONDIALOG = 256,
	WT_SKILLDIALOG=257,
	WT_WEAREDDIALOG=258,
	WT_QUESTDIALOG=259,
	WT_QUESTQUICKVIEWDIALOG=260,
	//WT_GAMENOTIFYDIALOG=261, 
	



	WT_ICONGRIDDIALOG = 512, 
	WT_STREETSTALLDIALOG = 513,
	WT_QUICKDIALOG = 514,
//	WT_MUNPAWAREICONGRIDDIALOG = 515,
	WT_STREETBUYSTALLDIALOG = 516,
	

	WT_TABDIALOG = 1024,
	WT_INVENTORYDIALOG = 1025,
	WT_DEALDIALOG = 1026,
//	WT_MUNPAWAREHOUSEDLG = 1029,
	WT_QUESTTOTALDIALOG  = 1030,
	WT_GUILDWAREHOUSEDLG = 1031,
	
	// guildfieldwar
	WT_GUILDFIELDWAR_DECLARE_DLG = 2048,
	WT_GUILDFIELDWAR_RESULT_DLG = 2049,
	WT_GUILDFIELDWAR_INFO_DLG = 2050,
	WT_GUILDWAR_INFO_DLG = 2051,	
	
	// partywar
	WT_PARTYWAR_DLG = 2052,
	
	// SeigeWar
	WT_SEIGEWAR_TIMEREG_DLG = 2053,
	WT_SEIGEWAR_PROTECTREG_DLG = 2054,
	WT_SEIGEWAR_INFO_DLG = 2055,
	
	WT_ITEM_SEAL_DLG = 2057,	

	WT_SEIGEWAR_PROFIT_DLG = 2058,	
	WT_SIEGEWAR_ENGRAVE_DLG = 2059,
	WT_SIEGEWAR_TIME_DLG = 2060,

	//
	WT_ITEM_CHANGEJOB_DLG = 2061,
	WT_SW_STARTTIME_DLG = 2062,
	WT_ITEM_REINFORCERESET_DLG = 2063,
	//WT_ITEM_RARECREATE_DLG = 2064,

		//ScreenShot
	WT_SCREENSHOT_DLG = 2065,

	WT_SKILLTREE_DLG,
	WT_JOBSKILL_DLG,

	// 061219 LYW --- Add window type for main system dialog.
	WT_MAINSYSTEM_DLG, 

	// 070402 LYW --- Add window type for chatting.
	WT_CHATTING,

	WT_FAMILYCREATEDLG,
	WT_FAMILYDLG,
	WT_FAMILYINVITEDLG,
	WT_FAMILYMARKDLG,
	WT_FAMILYLEVELUPDLG,
	WT_FAMILYNICKNAMEDLG,
	WT_FAMILYRANKDLG,
	WT_FAMILYUNIONCREATEDLG,	
	WT_FAMILYWAREHOUSEDLG,

	WT_MATCHPARTNERLISTDLG,
	WT_MATCHCHATDLG,
	WT_MATCHINFODLG,
	WT_MATCHRECORDDLG,

	WT_MATCH_IDENTIFICATIONDLG,
	WT_MATCH_FAVOR_ICONDLG,

	WT_GUILDWAREHOUSERANKDLG,
	WT_GUILDREVIVALDLG,

	// 080414 KTH
	WT_PETSTORAGEDLG,

	WT_PET_STATE_DLG,
	WT_PET_WEARED_DLG,
	WT_PET_INVEN_DLG,
};

typedef void (*cbDivideFUNC)( LONG lId, void *p, DWORD param1, void * param2, void * param3 );
typedef void (*cbFUNC)(LONG lId, void *p, DWORD we);
typedef void (*cbWindowProcess)();
typedef void (*cbEditFUNC)( LONG lId, void *p, DWORD we );

typedef struct
{
	DWORD x;
	DWORD y;
	DWORD w;
	DWORD h;
}cPoint;

typedef struct
{
	BYTE r;
	BYTE g;
	BYTE b;
}RGBCOLOR;
typedef struct
{
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE a;
}RGBACOLOR;

typedef struct
{
	DWORD x;
	DWORD y;
}cCoord, WINPOS;

typedef struct
{
	DWORD w;
	DWORD h;
}cSize;

typedef struct _ITEM
{
	DWORD rgb;
	char string[MAX_LISTITEM_SIZE];

	int line;
	int nFontIdx ;
	int nAlpha ;
}ITEM;

typedef RECT		cImageRect;
typedef VECTOR2		cImageSize;
typedef VECTOR2		cImageScale;

struct LINKITEM : public ITEM
{
	LINKITEM()
	{
		dwType = 0;
		dwOverRGB = RGB(0,255,255);
		rgb = RGB(255,255,255);
		wLine = 0;
		NextItem = NULL;
	}

	~LINKITEM()
	{
		SAFE_DELETE(NextItem);
	}
	DWORD	dwType;
	DWORD	dwOverRGB;
	WORD	wLine;
	void*	NextItem; // 연결 고리가 있는경우
};

struct OPTIONITEM : public ITEM
{
	OPTIONITEM()
	{
		rgb = RGB(255,255,255);
		nExtData = -1;
	}

	WORD	wComboType;
	DWORD	dwData;
	int		nExtData;
};

struct LINEITEM : public ITEM
{
	LINEITEM()
	{
		line = -1;
		rgb = RGB(255,255,255);		
	}

	WORD	line;
};

#endif //_cWINDOWDEF_H_
