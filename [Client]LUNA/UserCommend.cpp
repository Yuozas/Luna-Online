//
//060727 - by wonju : UserCommendParser
//
#include "stdafx.h"
#include "UserCommend.h"
#include "GameResourceManager.h"
#include "GuildManager.h"

CUserCommendParser::CUserCommendParser()
{
}

CUserCommendParser::~CUserCommendParser()
{
}

BOOL CUserCommendParser::Apply(char* str)
{
	//���ҽ� �޴����� ��ϵ� �׼Ǹ�ϰ� ���Ͽ�
	//���� ���̸� �����Ѵ�.
	for(unsigned int i = 0; i < GAMERESRCMNGR->GetUserCommendList().size(); ++i)
	{
		int nResult = (GAMERESRCMNGR->GetUserCommendList())[i].compare(str);
		
		if(nResult == 0)
		{
			switch(i) //���� �´� ������ �Ѵ�.
			{
			case 0:
				{
					//GUILDMGR->GetTotalHuntedMonsterCountFromMapServer();
				}
				break;
			case 1:
				break;
			default:break;
			}

			return TRUE;
		}
	}
	
	return FALSE;
}

