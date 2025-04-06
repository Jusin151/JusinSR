#include "Patter_Morph.h"
#include "HellBoss.h"

CPatter_Morph::CPatter_Morph()
{
    m_bStarted = false;
    m_fAccTime = 0.f;
    m_fDuration = 1.5f; //  ���
}

void CPatter_Morph::Execute(CHellBoss* pBoss, float fDeltaTime)
{
    if (!m_bStarted)
    {
        m_bStarted = true;
        m_fAccTime = 0.f;

        
        pBoss->Set_Animation("5_Morph");

        // ���� ��� 
        //  pBoss->Play_Sound("��ȭ ���.wav ��������");

        //  ����Ʈ ���� 
        // pBoss->Fire_Morph(); ������ 
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

void CPatter_Morph::Update(CHellBoss* pBoss, _float fDeltaTime)
{
}
