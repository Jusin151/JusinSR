#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"


BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;  // UI�� �簢������ �ҵ�
END

BEGIN(Client)

class CWeapon_Base abstract : public CGameObject
{
public:
    enum WEAPON_ID
    {
        Claymore
    };



    typedef struct tagWeapon_INFO
    {
        WEAPON_ID WeaponID;            // ���� ���� ID
        wstring Name;            // ���� �̸�
        _float2 vPos = {};  // UI ȭ�� ��ǥ
        _float2 vSize = {};  // UI ũ��
        _float Damage;            // �⺻ ���ݷ�
        _float AttackSpeed;       // ���� �ӵ�
        _float Range;             // ���� ���� (����/���Ÿ� ����)
        _float Cooldown;          // ���� ��Ÿ��
        _bool IsEquipped;         // ���� ���� ������ ����
        wstring TextureKey;      // ���� UI�� ���� �ؽ�ó Ű
 
    }Weapon_DESC;


 
protected:
    CWeapon_Base(LPDIRECT3DDEVICE9 pGraphic_Device);
    CWeapon_Base(const CWeapon_Base& Prototype);
    virtual ~CWeapon_Base() = default;

public:
    virtual HRESULT Initialize_Prototype()PURE;
    virtual HRESULT Initialize(void* pArg)PURE;
    virtual void Priority_Update(_float fTimeDelta)PURE;
    virtual void Update(_float fTimeDelta)PURE;
    virtual void Late_Update(_float fTimeDelta)PURE;
    virtual HRESULT Render()PURE;
    virtual HRESULT Ready_Components()PURE;
    virtual  void Free()PURE;
    virtual void Attack()PURE;
    CGameObject* Clone(void* pArg)PURE;

protected:
    CTexture* m_pTextureCom = nullptr;
    CVIBuffer_Rect* m_pVIBufferCom = nullptr;
    CTransform* m_pTransformCom = nullptr;

public:
    _bool  Get_MouseClick() const { return m_bIsMouseClick; }
    void   Set_MouseClick(_bool type) { m_bIsMouseClick = type; }
    virtual wstring GetLayerID() { return L"Default"; }

    Weapon_DESC m_Weapon_INFO;

protected:


    bool m_bIsMouseOver = {};
    bool m_bIsMouseClick = {};



};

END
