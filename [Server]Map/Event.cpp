#include "StdAfx.h"
#include ".\Event.h"

CEvent::CEvent( CEvent::Type type ) :
mType( type )
{}

// 080616 LUJ, �ٸ� ������Ʈ���� ���ظ� ���� �� �ߵ��Ǵ� �̺�Ʈ
CGiveDamageEvent::CGiveDamageEvent( CObject* target, const RESULTINFO& result ) :
CEvent( TypeGiveDamage ),
mResult( result ),
mTarget( target )
{}

// 080616 LUJ, �ٸ� ������Ʈ�� ���ظ� �޾��� �� �ߵ��Ǵ� �̺�Ʈ
CTakeDamageEvent::CTakeDamageEvent( CObject* attacker, const RESULTINFO& result ) :
CEvent( TypeTakeDamage ),
mResult( result ),
mAttacker( attacker )
{}

// 080616 LUJ, �ٸ� ������Ʈ���� �ǻ�� �� �߻��ϴ� �̺�Ʈ
// 080708 LUJ, �����ڸ� ���ڷ� �޵��� ��
CDieEvent::CDieEvent( CObject* killer, CObject* victim ) :
CEvent( TypeDie ),
mKiller( killer ),
mVictim( victim )
{}

// 080616 LUJ, �ٸ� ������Ʈ�� ������ �� �߻��ϴ� �̺�Ʈ
// 080708 LUJ, CDieEvent�� ���� ������ ������ ��
CKillEvent::CKillEvent( CObject* killer, CObject* victim ) :
CDieEvent( killer, victim )
{
	mType = TypeKill;
}