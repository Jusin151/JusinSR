#pragma once
#include "Shop.h"

BEGIN(Client)
class CPointShop : public CShop
{
private:
    CPointShop(LPDIRECT3DDEVICE9 pGraphic_Device);
    CPointShop(const CPointShop& Prototype);
    virtual ~CPointShop() = default;

public:
    // �⺻ ��� �Լ�
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Update(_float fTimeDelta)override;
    virtual void Update(_float fTimeDelta)override;
    virtual void Late_Update(_float fTimeDelta)override;
    virtual HRESULT Render()override;
    // Shop �������̽� ����
    virtual HRESULT Open_Shop() override;
    virtual HRESULT Close_Shop() override;
    virtual HRESULT Purchase_Item(const _uint iItemID, const _uint iCount = 1) override;
    virtual HRESULT Sell_Item(const _uint iItemID, const _uint iCount = 1) override;
    virtual void Refresh_Shop_Items() override;
    virtual _bool Can_Purchase(_uint iItemID, _uint iCount = 1) override;

protected:
    virtual HRESULT Ready_ShopItems() override;

public:
    static CPointShop* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;

private:
};
END