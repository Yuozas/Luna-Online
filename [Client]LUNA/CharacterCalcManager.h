#pragma once


#define CHARCALCMGR USINGTON(CCharacterCalcManager)


class CHero;

struct SetScript;
struct ITEM_INFO;


class CCharacterCalcManager  
{	
public:	
//	//MAKESINGLETON(CCharacterCalcManager)
	CCharacterCalcManager();
	virtual ~CCharacterCalcManager();
	
	void NetworkMsgParse(BYTE Protocol,void* pMsg);

	// 070206 LYW --- Add functions to process network msg.
public :
	void Char_Life_Ack( void* pMsg ) ;
	void Char_Mana_Ack( void* pMsg ) ;
	void Char_Exppoint_Ack( void* pMsg ) ;
	void Char_MaxLife_Notify( void* pMsg ) ;
	void Char_MaxMana_Notify( void* pMsg ) ;
	void Char_PlayerLevelup_Notify( void* pMsg ) ;
	void Char_Level_Notify( void* pMsg ) ;
	void Char_LevelupPoint_Notify( void* pMsg ) ;
	void Char_Str_Notify( void* pMsg ) ;
	void Char_Dex_Notify( void* pMsg ) ;
	void Char_Vit_Notify( void* pMsg ) ;
	void Char_Wis_Notify( void* pMsg ) ;

	// 070411 LYW --- CharacterCalcManager : Add network message of Intelligence part.
	void Char_Int_Notify( void* pMsg ) ;

	void Char_Fame_Notify( void* pMsg ) ;
	void Char_BadFame_Notify( void* pMsg ) ;
	void Char_BadFame_Ack( void* pMsg ) ;
	void Char_BadFame_Nack( void* pMsg ) ;
	void Char_BadFame_Changed( void* pMsg ) ;
	void Char_PlayTime_Ack( void* pMsg ) ;
	void Char_PlayTime_Nack( void* pMsg ) ;
	void Char_ExitStart_Ack( void* pMsg ) ;
	void Char_ExitStart_Nack( void* pMsg ) ;
	void Char_Exit_Nack( void* pMsg ) ;
	void Char_YouAreDied( void* pMsg ) ;
	void Char_PointNinus_Ack( void* pMsg ) ;
	void Char_PointMinus_Nack( void* pMsg ) ;
	void Char_Mussang_Info( void* pMsg ) ;
	void Char_Mussang_Ack( void* pMsg ) ;
	void Char_Mussang_End( void* pMsg ) ;

	// 070412 LYW --- CharacterCalcManager : Add function to change stage of character.
	void Char_Stage_Notify( void* pMsg ) ;
	
	// 070416 LYW --- CharacterCalcManager : Add functions to notify class update.
	int Notify_SecondJob( BYTE jobType, BYTE byIndex ) ;
	int Notify_ThirdJob( BYTE jobType, BYTE byIndex ) ;
	int Notify_FourthJob( BYTE jobType, BYTE byIndex ) ;
	int Notify_FifthJob( BYTE jobType, BYTE byIndex ) ;
	int Notify_SixthJob( BYTE jobType, BYTE byIndex ) ;

	// desc_hseos_���͹���01
	// S ���͹��� �߰� added by hseos 2007.05.23
	void Char_Monstermeter_KillMonster(void* pMsg);
	void Char_Monstermeter_PlayTime(void* pMsg);
	// E ���͹��� �߰� added by hseos 2007.05.23


	void Char_Hide_Notify( void* pMsg );
	void Char_Detect_Notify( void* pMsg );
	// 070725 ����: StatsCalcManager�� ����� ����
public:
	void Initialize( CHero* );
	//void CalcItemStats( CHero* );
	void CalcCharStats( CHero* );

	void AddItem			( CHero*, const ITEMBASE& );
	void RemoveItem			( CHero*, const ITEMBASE& );
	void AddSetItemStats	( CHero*, const SetScript&, int setItemSize );
	void RemoveSetItemStats	( CHero*, const SetScript&, int setItemSize );


	// 080124 LUJ, ��ġ ���� �����Ѵ�
private:	
	void AddStat	( const ITEM_INFO&,		PlayerStat& );

	// 080319 LUJ, �߰� ��æƮ ��ġ�� �ο���
	void AddStat	( const ITEM_INFO&,		const ITEM_OPTION&,	PlayerStat& );
	void AddStat	( const PlayerStat&,	PlayerStat& );
	void RemoveStat	( const ITEM_INFO&,		PlayerStat& );

	// 080319 LUJ, �߰� ��æƮ ��ġ�� ������
	void RemoveStat	( const ITEM_INFO&,		const ITEM_OPTION&,	PlayerStat& );
	void RemoveStat	( const PlayerStat&,	PlayerStat& );
};


EXTERNGLOBALTON(CCharacterCalcManager)
