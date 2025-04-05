#include "Pattern_Shoot.h"
#include "HellBoss.h"
#include "HellBoss_IdleState.h" 

CPattern_Shoot::CPattern_Shoot()
{
    m_bStarted = false;
    m_fAccTime = 0.f;
    m_fDuration = 1.5f; //  ���
}

void CPattern_Shoot::Execute(CHellBoss* pBoss, float fDeltaTime)
{
    if (!m_bStarted)
    {
        m_bStarted = true;
        m_fAccTime = 0.f;

        pBoss->Set_Animation("4_Shoot");
    }


    m_fAccTime += fDeltaTime;

    //  �ִϸ��̼��� �������� 
    if (pBoss->Get_AnimationFinished())
    {
        m_bStarted = false;
        m_fAccTime = 0.f;


        pBoss->Change_State(new CHellBoss_IdleState());
    }
}
