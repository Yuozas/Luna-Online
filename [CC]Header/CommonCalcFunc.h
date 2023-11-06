#ifndef _COMMONCALCFUNC_H_
#define _COMMONCALCFUNC_H_

struct ITEM_OPTION;
struct ITEM_INFO;

DWORD CalcRangeAttackPower(WORD dex,WORD Weapon);
DWORD CalcMeleeAttackPower(WORD str,WORD Weapon);

BOOL CheckRandom(float SeedVal,LEVELTYPE OperatorLevel,LEVELTYPE TargetLevel);
WORD GetPercent(float SeedVal,LEVELTYPE OperatorLevel,LEVELTYPE TargetLevel);

//SW060906 신규펫(스탯반올림)
//int	GetIntegerRound(int IntVal, int Cipher);
DWORD GetStatRound(DWORD StatVal);
DWORD GetAddStatFromRound(DWORD StatVal);

// 070507 LYW --- CommonCalcFunc : Add function to return round.
float GetFloatRound( float val ) ;
float GetFloatInt( float val ) ;


// 080221 LUJ, 주어진 자리 수만큼 반올림 시킴
float Round( float value, int precision );

// 080320 LUJ, 추가 인챈트 수치
// 080421 LUJ, 복잡한 수식 처리를 단순화하고 새로운 공식을 적용함
DWORD GetBonusEnchantValue( const ITEM_INFO&, const ITEM_OPTION&, DWORD itemValue );

#ifdef _MAPSERVER_
BOOL CheckSpecialAttack( float SeedVal, CObject* pOperator, CObject* pTarget );
#endif

#endif
