#include "HellBoss.h"
#include "HellBoss_AttackState.h"
#include "HellBoss_IdleState.h"
#include "HellBoss_Bullet.h"
#include "GameInstance.h" 

void CHellBoss_AttackState::Enter(CHellBoss* pBoss)
{
    // ���� �ִϸ��̼� ����
    pBoss->Set_Animation("4_Shoot");

    // �÷��̾� ���� ���� ���
    _float3 vDir = pBoss->Get_PlayerPos() - pBoss->Get_Pos();
    D3DXVec3Normalize(&vDir, &vDir);

    // �Ѿ� ����
    //CHellBoss_Bullet* pBullet = CHellBoss_Bullet::Create(pBoss->Get_Graphic_Device());
    //if (pBullet)
    //{
    //    // �ʱ�ȭ
    //    pBullet->Initialize(nullptr);

    //    // ��ġ ����
    //    pBullet->Get_Transform()->Set_State(CTransform::STATE_POSITION, pBoss->Get_Pos());

    //    // ���� ����
    //    pBullet->Set_Direction(vDir);

      
        if (FAILED(pBoss->Get_GameInstance()->Add_GameObject(LEVEL_HONG, TEXT("Prototype_GameObject_HellBoss_Bullet"),
            LEVEL_HONG, TEXT("Layer_HellBoss_Bullet"))))
        {
            MSG_BOX("HellBoss_Bullet ���� ����");
        }

    //
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
