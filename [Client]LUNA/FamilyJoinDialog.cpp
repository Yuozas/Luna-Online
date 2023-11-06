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
			// 070801 웅주, 기존 패밀리 제거하며 주석처리

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
			// 070801 웅주, 기존 패밀리 제거하며 주석처리

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
	case JO_CANCELBTN:		//기획자에게 말해서 기본 CANCLE로 바꾸자.
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