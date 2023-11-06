#include "stdafx.h"
#include "CommonCalcFunc.h"
// 080324 LUJ, 에이전트와 디스트리뷰트에서 참조되지 않도록 전처리기 처리함

#if ! _AGENTSERVER && ! _DISTRIBUTESERVER
#include "CommonStruct.h"
#include "GameResourceStruct.h"
#endif


#ifdef _MAPSERVER_
#include "Object.h"
#endif

DWORD CalcRangeAttackPower(WORD dex,WORD Weapon)
{
	double dW = Weapon;
	double dS = dex;

	return (DWORD)((dW * ((dS+200)/200)*((dS+1000)/500)+dS)*0.74 + min(dS-12,25));
}


DWORD CalcMeleeAttackPower(WORD str,WORD Weapon)
{
	double dW = Weapon;
	double dS = str;

	return (DWORD)((dW * ((dS+200)/200)*((dS+1000)/500)+dS)*0.74 + min(dS-12,25));
}

WORD GetPercent(float SeedVal,LEVELTYPE OperatorLevel,LEVELTYPE TargetLevel)
{	
	//SeedVal+{(ÀÚ½ÅÀÇ ·¹º§ - »ó´ë ·¹º§)/2.5*0.01}
	int LevelGap = OperatorLevel - TargetLevel;
	float fRate = SeedVal + LevelGap*0.025f;
	if(fRate <= 0)
		return 0;

	return (WORD)(fRate * 100);
}

BOOL CheckRandom(float SeedVal,LEVELTYPE OperatorLevel,LEVELTYPE TargetLevel)
{
	WORD Percent = GetPercent(SeedVal,OperatorLevel,TargetLevel);
	WORD randVal = rand()%100;
	if(randVal <= Percent)
		return TRUE;
	else
		return FALSE;
}

/*
//임의의 정수와 반올림 자리수를 받아 반올림 값 리턴.
//ex> 234를 둘째 자리에서 반올림 => GetIntegerRound( 234, 1 );
//ex> -12를 첫째 자리에서 반올림 => GetIntegerRound( -12, 0 );
int	GetIntegerRound(int IntVal, int Cipher)
{
	//소수점이하는 반올림 하지 않는다.
	if(Cipher<0)	return 0;

	double RoundCipher = pow(10, Cipher);

	double RoundVal = IntVal;

	RoundVal += RoundCipher * 5;

	RoundVal /= RoundCipher * 10;

	RoundVal *= RoundCipher * 10;

	return int(RoundVal);
}
*/



DWORD GetStatRound( DWORD StatVal )
{
	DWORD RoundVal = StatVal;

	RoundVal += 5;

	RoundVal /= 10;

	RoundVal *= 10;

	if( RoundVal > StatVal )
		return RoundVal;
	else
		return StatVal;
}

// 070507 LYW --- CommonCalcFunc : Add function to return round.
float GetFloatRound( float val )
{
	float checkVal = val * 10 ;
	DWORD dwCheckVal = (DWORD)checkVal ;

	if( dwCheckVal%10 >= 5 )
	{
		return (float)((dwCheckVal += 10)/10) ;
	}
	else 
	{
		return (float)(dwCheckVal/10) ;
	}
}

float GetFloatInt( float val )
{
	float checkVal = val * 10 ;

	WORD wCheckVal = (WORD)checkVal ;

	return (float)wCheckVal/10 ;
}

DWORD GetAddStatFromRound( DWORD StatVal )
{
	/*
	WORD RoundVal = StatVal;

	RoundVal += 5;

	RoundVal /= 10;

	RoundVal *= 10;

	if( RoundVal > StatVal )
		return (RoundVal - StatVal);
	else
		return 0;
*/
	StatVal %= 10;

	if( StatVal < 5 )
		return 0;
	else
		return 10 - StatVal;
}

#ifdef _MAPSERVER_
BOOL CheckSpecialAttack( float SeedVal, CObject* pOperator, CObject* pTarget )
{
	if( SeedVal > 0.f )
	{
		LEVELTYPE OperLevel = pOperator->GetLevel();
		LEVELTYPE TargetLevel = pTarget->GetLevel();
		float fSAR = 0.f;
		if( OperLevel < TargetLevel )
		{
			if( TargetLevel - OperLevel < 10  )
				fSAR = SeedVal * ( 1.f - (float)( TargetLevel - OperLevel ) / 10.f );
			else
				fSAR = 0.f;
		}
		else
		{
			fSAR = SeedVal * ( 1.f + (float)( OperLevel - TargetLevel ) / 5.f );
		}
		
		if( pTarget->GetObjectKind() & eObjectKind_Monster )
		{
			fSAR *= 3.f;
		}
		
		float fRand = (float)(rand()%100) / 100.f;
		
		if( fSAR < fRand )
			return FALSE;
		
	}
	else
	{
		return FALSE;
	}
	
	return TRUE;
}
#endif


// 080221 LUJ, 주어진 자리 수만큼 반올림 시킴
float Round( float value, int precision )
{
	const double base = pow( 10.0f, precision );

	return float( floor( value * base + 0.5f ) / base );
}


// 080324 LUJ, 에이전트와 디스트리뷰트에서 참조되지 않도록 전처리기 처리함

#if ! _AGENTSERVER && ! _DISTRIBUTESERVER
// 080320 LUJ, 추가 인챈트 수치를 반환함


// 080421 LUJ, 복잡한 수식 처리를 단순화하고 새로운 공식을 적용함
DWORD GetBonusEnchantValue( const ITEM_INFO& info, const ITEM_OPTION& option, DWORD itemValue )
{
	// 080320 LUJ,	실수 오차를 없애기 위해 반올림한 값을 사용해야 한다. 공식 작성: 송가람


	//
	//				A								B													C
	//				(인챈트 수치 * 인챈트 레벨 ) + ( 아이템능력 * 인챈트레벨 * 인챈트레벨 * 0.0017 ) + ( 아이템등급 * 인챈트레벨 * 아이템레벨 * 0.011 )
	
	// 080421 LUJ, A값은 이미 옵션에 더해져 있다. A에 곱셈/나눗셈이 있을 경우 적용할 수 없음을 유의
	const float a = 0;
	const float b = float( itemValue ) * option.mEnchant.mLevel * option.mEnchant.mLevel * 0.0017f;
	const float c = info.kind * option.mEnchant.mLevel * info.LimitLevel * 0.011f;

	// 080421 LUJ, 반올림한 값을 반환한다
	return DWORD( max( 0, floor( 0.5f + a + b + c ) ) );
}

#endif
