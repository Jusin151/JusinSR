#include "HellBoss_IdleState.h"
#include "HellBoss.h"
#include "Pattern_EyeBlast.h"
#include "Pattern_Shoot.h"
#include "HellBoss_AttackState.h"
#include "HellBoss_WalkState.h" // �߰� �ʿ�

void CHellBoss_IdleState::Enter(CHellBoss* pBoss)
{
	pBoss->Set_Animation("1_Idle");
}

void CHellBoss_IdleState::Update(CHellBoss* pBoss, float fDeltaTime)
{
	if (!pBoss->HasTarget()) return;

	_float3 vToPlayer = pBoss->Get_PlayerPos() - pBoss->Get_Pos();
	float fDist = D3DXVec3Length(&vToPlayer);

	if (fDist < 8.f) // 
	{
	
		int randIndex = rand() % 2;
		if (randIndex == 0)
			pBoss->Set_AttackPattern(new CPattern_EyeBlast());
		else
			pBoss->Set_AttackPattern(new CPattern_Shoot());

		pBoss->Change_State(new CHellBoss_AttackState());
		return;
	}
	else if (fDist < 40.f) //�÷��̾� �߳� �Ÿ� ����
	{
		
		pBoss->Change_State(new CHellBoss_WalkState());
		return;
	}

	pBoss->Set_Animation("1_Idle");
}

void CHellBoss_IdleState::Exit(CHellBoss* pBoss)
{

}
