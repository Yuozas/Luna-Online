#ifndef _COMMONCALCFUNC_H_
#define _COMMONCALCFUNC_H_

struct ITEM_OPTION;
struct ITEM_INFO;

DWORD CalcRangeAttackPower(WORD dex,WORD Weapon);
DWORD CalcMeleeAttackPower(WORD str,WORD Weapon);

BOOL CheckRandom(float SeedVal,LEVELTYPE OperatorLevel,LEVELTYPE TargetLevel);
WORD GetPercent(float SeedVal,LEVELTYPE OperatorLevel,LEVELTYPE TargetLevel);

//SW060906 �ű���(���ȹݿø�)
//int	GetIntegerRound(int IntVal, int Cipher);
DWORD GetStatRound(DWORD StatVal);
DWORD GetAddStatFromRound(DWORD StatVal);

// 070507 LYW --- CommonCalcFunc : Add function to return round.
float GetFloatRound( float val ) ;
float GetFloatInt( float val ) ;


// 080221 LUJ, �־��� �ڸ� ����ŭ �ݿø� ��Ŵ
float Round( float value, int precision );

// 080320 LUJ, �߰� ��æƮ ��ġ
// 080421 LUJ, ������ ���� ó���� �ܼ�ȭ�ϰ� ���ο� ������ ������
DWORD GetBonusEnchantValue( const ITEM_INFO&, const ITEM_OPTION&, DWORD itemValue );

#ifdef _MAPSERVER_
BOOL CheckSpecialAttack( float SeedVal, CObject* pOperator, CObject* pTarget );
#endif

#endif
