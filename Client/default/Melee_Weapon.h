#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"
#include "Weapon_Base.h"


BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;  // UI�� �簢������ �ҵ�
END

BEGIN(Client)

class CMelee_Weapon  : public CWeapon_Base
{

public:
    struct MeleeWeapon_Info
    {
        float KnockbackForce;  // Ÿ�� �� �˹� (0�̸� �˹� ����)
        int ComboCount;        // �޺� ���� ���� Ƚ��
        float ComboDelay;      // �޺� ���� �� ������
    };
protected:
    CMelee_Weapon(LPDIRECT3DDEVICE9 pGraphic_Device);
    CMelee_Weapon(const CMelee_Weapon& Prototype);
    virtual ~CMelee_Weapon() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Update(_float fTimeDelta);
    virtual void Update(_float fTimeDelta);
    virtual void Late_Update(_float fTimeDelta);
    virtual HRESULT Render()override;
    virtual HRESULT Ready_Components();
    void Free();



    CGameObject* Clone(void* pArg) override;


    // CWeapon_Base��(��) ���� ��ӵ�
    void Attack() override;

};

END
