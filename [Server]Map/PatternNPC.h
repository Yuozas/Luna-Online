// PatternNPC.h: interface for the CPatternNPC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATTERNNPC_H__A68DADAB_6284_4150_816D_DEF6ACF7D7A1__INCLUDED_)
#define AFX_PATTERNNPC_H__A68DADAB_6284_4150_816D_DEF6ACF7D7A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMonster;

enum NPCIDX
{
	eParakho_L = 1, eParakho,eParakho_S,
	eHucpyobom_L,eHucpyobom,eHucpyobom_S,
	eCoyente_L, eCoyente, eCoyente_S, 
	eHyelgwiso_L, eHyelgwiso, eHyelgwiso_S, 
	eGorilra_L, eGorilra, eGorilra_S, 
	eNokrimdo_L, eNokrimdo, eNokrimdo_S, 
	eGom_L, eGom, eGom_S, 
	eTochawo_L, eTochawo, eTochawo_S, 
	eYeti_L, eYeti, eYeti_S, 
	eGongGwi_L,eGongGwi,eGongGwi_S,
	eHorangie_L, eHorangie, eHorangie_S, 
	eDongyounginja_L, eDongyounginja, eDongyounginja_S, 
	eYewa_L, eYewa, eYewa_S, 
	eSanso_L, eSanso, eSanso_S, 
	eAlyou_L, eAlyou, eAlyou_S, 
	eSagalru_L, eSagalru, eSagalru_S, 
	ePyobom_L, ePyobom, ePyobom_S, 
	eYekgwi_L, eYekgwi, eYekgwi_S, 
	eImugi_L, eImugi, eImugi_S, 
	eSamurai_L,eSamurai,eSamurai_S,
	eBunwun_L, eBunwun, eBunwun_S, 
	eMokbigwi_L, eMokbigwi, eMokbigwi_S, 
	eSuckgein_L, eSuckgein, eSuckgein_S, 
	eHu_L, eHu, eHu_S, 
	eInmeundanrang_L, eInmeundanrang, eInmeundanrang_S, 
	eNokrimchejuPe_L, eNokrimchejuPe, eNokrimchejuPe_S, 
	eMujiki_L, eMujiki, eMujiki_S, 
	eBarki_L, eBarki, eBarki_S, 
	eHwan_L, eHwan, eHwan_S, 
	eYoumyou_L,eYoumyou,eYoumyou_S,
	eSansin_L, eSansin, eSansin_S, 
	eSagaik_L, eSagaik, eSagaik_S, 
	eBuHey_L, eBuHey, eBuHey_S, 
	eHeulkyojangro_L, eHeulkyojangro, eHeulkyojangro_S, 
	eKangsi_L, eKangsi, eKangsi_S, 
	eHungchun_L, eHungchun, eHungchun_S, 
	eTa_L, eTa, eTa_S, 
	eSanjewangPeac_L, eSanjewangPeac, eSanjewangPeac_S, 
	eBackho_L, eBackho, eBackho_S, 
	eKumong_L,eKumong,eKumong_S,
	eWomawang_L, eWomawang, eWomawang_S, 
	eJujak_L, eJujak, eJujak_S, 
	eHyunmu_L, eHyunmu, eHyunmu_S, 
	eKirin_L, eKirin, eKirin_S, 
	eYoung_L,eYoung,eYoung_S
};

#define PATTERNNPC_OBJ CPatternNPC::GetInstance()

class CPatternNPC  
{
//	DWORD m_dwCurSearchTime;
//	DWORD m_dwTmpSearchTime;
//	DWORD m_dwLastSearchTime;
//	BOOL m_bFirstSearch;
public:
	GETINSTANCE(CPatternNPC)
	CPatternNPC();
	virtual ~CPatternNPC();

	//////////////////////////////////////////////////////////////////////////
	BOOL MonsterAreaCheck(CMonster* pMonster);
	void MonsterPatternManager(CMonster* pMonster, BYTE StateFlag);		// ��ȭ,���, ���� ������ ����
	void NpcPatternManager(CNpc* pNpc, BYTE StateFlag);
	BYTE MonsterSearchTarget(CMonster* pMonster);
	BYTE NpcSearchTarget(CNpc* pNpc);
	//////////////////////////////////////////////////////////////////////////
	void PeacePattern(CMonster* pMonster);			// Npc�� ��ȭ���� ����
	void AngerPattern(CMonster* pMonster);			// Npc�� ������� ����
	void AttackPattern(CMonster* pMonster);		// Npc�� �������� ����
	//////////////////////////////////////////////////////////////////////////
	void DefaultMonsterPeacePattern(CMonster* pMonster);	// �⺻ ��ȭ����
	void DefaultMonsterAngerPattern(CMonster* pMonster);	// �⺻ �������
	void DefaultMonsterAttackPattern(CMonster* pMonster);// �⺻ ��������
	void DefaultNpcPeacePattern(CNpc* pNpc);	// �⺻ ��ȭ����
	void DefaultNpcAngerPattern(CNpc* pNpc);	// �⺻ �������
	void DefaultNpcAttackPattern(CNpc* pNpc);// �⺻ ��������
	//////////////////////////////////////////////////////////////////////////
	void ParakhoPeace(CMonster* pMonster);		// �Ķ�ȣ
	void ParakhoAnger(CMonster* pMonster);
	void ParakhoAttack(CMonster* pMonster);

	void HucpyobomPeace(CMonster* pMonster);		// ��ǥ��
	void HucpyobomAnger(CMonster* pMonster);
	void HucpyobomAttack(CMonster* pMonster);

	void CoyentePeace(CMonster* pMonster);		// �ھ���
	void CoyenteAnger(CMonster* pMonster);
	void CoyenteAttack(CMonster* pMonster);

	void HyelgwisoPeace(CMonster* pMonster);		// ���ͼ�
	void HyelgwisoAnger(CMonster* pMonster);
	void HyelgwisoAttack(CMonster* pMonster);

	void GorilraPeace(CMonster* pMonster);		// ����
	void GorilraAnger(CMonster* pMonster);
	void GorilraAttack(CMonster* pMonster);

	void NokrimdoPeace(CMonster* pMonster);		// �츲��
	void NokrimdoAnger(CMonster* pMonster);
	void NokrimdoAttack(CMonster* pMonster);

	void GomPeace(CMonster* pMonster);			// ��
	void GomAnger(CMonster* pMonster);
	void GomAttack(CMonster* pMonster);

	void TochawoPeace(CMonster* pMonster);		// ������
	void TochawoAnger(CMonster* pMonster);
	void TochawoAttack(CMonster* pMonster);

	void YetiPeace(CMonster* pMonster);			// ��Ƽ
	void YetiAnger(CMonster* pMonster);
	void YetiAttack(CMonster* pMonster);

	void GongGwiPeace(CMonster* pMonster);		// ����
	void GongGwiAnger(CMonster* pMonster);
	void GongGwiAttack(CMonster* pMonster);

	void HorangiePeace(CMonster* pMonster);		// ȣ����
	void HorangieAnger(CMonster* pMonster);
	void HorangieAttack(CMonster* pMonster);

	void DongyounginjaPeace(CMonster* pMonster);	// ��������
	void DongyounginjaAnger(CMonster* pMonster);
	void DongyounginjaAttack(CMonster* pMonster);

	void YewaPeace(CMonster* pMonster);			// ����
	void YewaAnger(CMonster* pMonster);
	void YewaAttack(CMonster* pMonster);

	void SansoPeace(CMonster* pMonster);			// ���
	void SansoAnger(CMonster* pMonster);
	void SansoAttack(CMonster* pMonster);

	void AlyouPeace(CMonster* pMonster);			// ����
	void AlyouAnger(CMonster* pMonster);
	void AlyouAttack(CMonster* pMonster);

	void SagalruPeace(CMonster* pMonster);		// �簥��
	void SagalruAnger(CMonster* pMonster);
	void SagalruAttack(CMonster* pMonster);

	void PyobomPeace(CMonster* pMonster);			// ǥ��
	void PyobomAnger(CMonster* pMonster);
	void PyobomAttack(CMonster* pMonster);

	void YekgwiPeace(CMonster* pMonster);			// ����
	void YekgwiAnger(CMonster* pMonster);
	void YekgwiAttack(CMonster* pMonster);

	void ImugiPeace(CMonster* pMonster);			// �̹���
	void ImugiAnger(CMonster* pMonster);
	void ImugiAttack(CMonster* pMonster);

	void SamuraiPeace(CMonster* pMonster);		// �繫����
	void SamuraiAnger(CMonster* pMonster);
	void SamuraiAttack(CMonster* pMonster);

	void BunwunPeace(CMonster* pMonster);			// �п�
	void BunwunAnger(CMonster* pMonster);
	void BunwunAttack(CMonster* pMonster);

	void MokbigwiPeace(CMonster* pMonster);		// ����
	void MokbigwiAnger(CMonster* pMonster);
	void MokbigwiAttack(CMonster* pMonster);

	void SuckgeinPeace(CMonster* pMonster);		// ������
	void SuckgeinAnger(CMonster* pMonster);
	void SuckgeinAttack(CMonster* pMonster);

	void HuPeace(CMonster* pMonster);				// ��
	void HuAnger(CMonster* pMonster);
	void HuAttack(CMonster* pMonster);

	void InmeundanrangPeace(CMonster* pMonster);	// �θ�ܶ�
	void InmeundanrangAnger(CMonster* pMonster);
	void InmeundanrangAttack(CMonster* pMonster);

	void NokrimchejuPeace(CMonster* pMonster);	// �츲ä��
	void NokrimchejuAnger(CMonster* pMonster);
	void NokrimchejuAttack(CMonster* pMonster);

	void MujikiPeace(CMonster* pMonster);			// ������
	void MujikiAnger(CMonster* pMonster);
	void MujikiAttack(CMonster* pMonster);

	void BarkiPeace(CMonster* pMonster);			// ����
	void BarkiAnger(CMonster* pMonster);
	void BarkiAttack(CMonster* pMonster);

	void HwanPeace(CMonster* pMonster);			// ȯ
	void HwanAnger(CMonster* pMonster);
	void HwanAttack(CMonster* pMonster);

	void YoumyouPeace(CMonster* pMonster);		// ����
	void YoumyouAnger(CMonster* pMonster);
	void YoumyouAttack(CMonster* pMonster);

	void SansinPeace(CMonster* pMonster);			// ���
	void SansinAnger(CMonster* pMonster);
	void SansinAttack(CMonster* pMonster);
	
	void SagaikPeace(CMonster* pMonster);			// �簡��
	void SagaikAnger(CMonster* pMonster);
	void SagaikAttack(CMonster* pMonster);

	void BuHeyPeace(CMonster* pMonster);			// ����
	void BuHeyAnger(CMonster* pMonster);
	void BuHeyAttack(CMonster* pMonster);

	void HeulkyojangroPeace(CMonster* pMonster);	// �������
	void HeulkyojangroAnger(CMonster* pMonster);
	void HeulkyojangroAttack(CMonster* pMonster);

	void KangsiPeace(CMonster* pMonster);			// ����
	void KangsiAnger(CMonster* pMonster);
	void KangsiAttack(CMonster* pMonster);

	void HungchunPeace(CMonster* pMonster);		// ��õ
	void HungchunAnger(CMonster* pMonster);
	void HungchunAttack(CMonster* pMonster);

	void TaPeace(CMonster* pMonster);				// Ÿ
	void TaAnger(CMonster* pMonster);
	void TaAttack(CMonster* pMonster);

	void SanjewangPeace(CMonster* pMonster);		// ������
	void SanjewangAnger(CMonster* pMonster);
	void SanjewangAttack(CMonster* pMonster);

	void BackhoPeace(CMonster* pMonster);			// ��ȣ
	void BackhoAnger(CMonster* pMonster);
	void BackhoAttack(CMonster* pMonster);

	void KumongPeace(CMonster* pMonster);			// ����
	void KumongAnger(CMonster* pMonster);
	void KumongAttack(CMonster* pMonster);

	void WomawangPeace(CMonster* pMonster);		// �츶��
	void WomawangAnger(CMonster* pMonster);
	void WomawangAttack(CMonster* pMonster);

	void JujakPeace(CMonster* pMonster);			// ����
	void JujakAnger(CMonster* pMonster);
	void JujakAttack(CMonster* pMonster);

	void HyunmuPeace(CMonster* pMonster);			// ����
	void HyunmuAnger(CMonster* pMonster);
	void HyunmuAttack(CMonster* pMonster);

	void KirinPeace(CMonster* pMonster);			// �⸰
	void KirinAnger(CMonster* pMonster);
	void KirinAttack(CMonster* pMonster);

	void YoungPeace(CMonster* pMonster);			// ��
	void YoungAnger(CMonster* pMonster);
	void YoungAttack(CMonster* pMonster);
};

#endif // !defined(AFX_PATTERNNPC_H__A68DADAB_6284_4150_816D_DEF6ACF7D7A1__INCLUDED_)





















