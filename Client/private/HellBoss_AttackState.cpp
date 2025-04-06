#include "HellBoss_AttackState.h"
#include "HellBoss.h"
#include "HellBoss_IdleState.h"

void CHellBoss_AttackState::Enter(CHellBoss* pBoss)
{
	// ������ �ִϸ��̼� �ȿ� ���ϸ��� �����ϰ� �ִٸ�, ���� �ʿ��� Set_Animation �� ���� ����
	// �Ǵ� ���⼭ �⺻ �ִϸ� �������൵ ��
}

void CHellBoss_AttackState::Update(CHellBoss* pBoss, float fDeltaTime)
{

	pBoss->Use_Attack(fDeltaTime);

	auto pPattern = pBoss->Get_AttackPattern();
	if (pPattern && pPattern->Is_Finished())
	{
	
		delete pPattern;
		pBoss->Set_AttackPattern(nullptr);

		pBoss->Change_State(new CHellBoss_IdleState()); 
	}
}

void CHellBoss_AttackState::Exit(CHellBoss* pBoss)
{
	
}
