#include "stdafx.h"
#include ".\familyjoindialog.h"
#include "ObjectManager.h"
#include "ChatManager.h"
//#include "FamilyManager.h"

#include "WindowIdEnum.h"

CFamilyJoinDialog::CFamilyJoinDialog(void)
{
}

CFamilyJoinDialog::~CFamilyJoinDialog(void)
{
}

void CFamilyJoinDialog::Linking()
{

}
	
void CFamilyJoinDialog::OnActionEvent(LONG lId, void* p, DWORD we)
{
	switch(lId)
	{
	case JO_MEMBERBTN:
		{
			// 070801 ����, ���� �йи� �����ϸ� �ּ�ó��

			//CObject * targetObj = OBJECTMGR->GetSelectedObject();
			//if(targetObj)
			//{
			//	if(targetObj->GetObjectKind()  == eObjectKind_Player)
			//	{
			//		if(((CPlayer*)targetObj)->GetFamilyIdx())
			//		{
			//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(38));
			//			return;
			//		}
			//		else
			//			FAMILYMGR->AddMemberSyn(targetObj->GetID());
			//	}
			//}
		}
		break;
	case JO_STUDENTBTN:
		{
			// 070801 ����, ���� �йи� �����ϸ� �ּ�ó��

			//if(FAMILYMGR->GetLevel() < FAMILY_5LEVEL)
			//{
			//	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1368));
			//	return;
			//}

			//CObject * targetObj = OBJECTMGR->GetSelectedObject();
			//if(targetObj)
			//{
			//	if(targetObj->GetObjectKind()  == eObjectKind_Player)
			//	{
			//		if(((CPlayer*)targetObj)->GetFamilyIdx())
			//		{
			//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(38));
			//			//return;
			//		}
			//		else
			//			FAMILYMGR->AddStudentSyn(targetObj->GetID(), ((CPlayer*)targetObj)->GetLevel());
			//	}
			//}
		}
		break;
	case JO_CANCELBTN:		//��ȹ�ڿ��� ���ؼ� �⺻ CANCLE�� �ٲ���.
		{
			//SetActive(FALSE);
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	SetActive(FALSE);

}