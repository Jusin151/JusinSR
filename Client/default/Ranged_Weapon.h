#pragma once

#include "Client_Defines.h"

#include "Transform.h"
#include "Weapon_Base.h"


BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;  // UI�� �簢������ �ҵ�
END

BEGIN(Client)

class CRanged_Weapon  : public CWeapon_Base
{

public:
    struct RangedWeapon_Info
    {
        int MaxAmmo;         // �ִ� ź�� ��
        int CurrentAmmo;     // ���� ź��
        float ReloadTime;    // ������ �ð�
        float BulletSpeed;   // źȯ �ӵ�
        float BulletSpread;  // ź ���� ����
    };

protected:
    CRanged_Weapon(LPDIRECT3DDEVICE9 pGraphic_Device);
    CRanged_Weapon(const CRanged_Weapon& Prototype);
    virtual ~CRanged_Weapon() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Update(_float fTimeDelta);
    virtual void Update(_float fTimeDelta);
    virtual void Late_Update(_float fTimeDelta);
    virtual HRESULT Render()override;
    virtual HRESULT Ready_Components();
    void Free();


    // CWeapon_Base��(��) ���� ��ӵ�
    void Attack() override;

    CGameObject* Clone(void* pArg) override;

};

END
