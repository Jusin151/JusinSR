#include "Pattern_EyeBlast.h"
#include "HellBoss.h"

CPattern_EyeBlast::CPattern_EyeBlast()
{
    m_bStarted = false;
    m_fAccTime = 0.f;
    m_fDuration = 1.5f; //  ���
}

void CPattern_EyeBlast::Execute(CHellBoss* pBoss, float fDeltaTime)
{
    if (!m_bStarted)
    {
        m_bStarted = true;
        m_fAccTime = 0.f;

        
        pBoss->Set_Animation("3_EyeBlast");

        // ���� ��� 
        //  pBoss->Play_Sound("����� ���.wav ��������");

        //  ����Ʈ ���� 
        // pBoss->Fire_EyeBeam(); ������ �����
    }

    //  �ð�
    m_fAccTime += fDeltaTime;

    if (m_fAccTime >= m_fDuration)
    {
        m_bStarted = false;
        m_fAccTime = 0.f;

        // ��(������ Idle ���·� ���ư��� �� ����)
        // pBoss->Change_State(new CHellBoss_IdleState());
    }
}
