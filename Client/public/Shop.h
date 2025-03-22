#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CCollider_Sphere;
END

BEGIN(Client)
class CShop : public CGameObject
{
protected:
	CShop(LPDIRECT3DDEVICE9 pGraphic_Device);
	CShop(const CShop& Prototype);
	virtual ~CShop() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

	// ���� ���� ���
	virtual HRESULT Open_Shop() = 0;  //  ���� ����
	virtual HRESULT Close_Shop() = 0;  //  ���� �ݱ�
	virtual HRESULT Purchase_Item(const _uint iItemID, const _uint iCount = 1) = 0;  // ������ ����
	virtual HRESULT Sell_Item(const _uint iItemID, const _uint iCount = 1) = 0;  // ������ �Ǹ�

	virtual void Refresh_Shop_Items() = 0;  // ���� ������ ���ΰ�ħ
	virtual _bool Can_Purchase(_uint iItemID, _uint iCount = 1) = 0;  // ���� ���� ���� Ȯ��


	HRESULT On_Collision();
protected:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CCollider_Sphere* m_pColliderCom = { nullptr };
	_bool m_bIsOpen = { false };

protected:
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	HRESULT Ready_Components();

	// �������� �ٸ� �� �ִ� ������ �ʱ�ȭ �Լ�
	virtual HRESULT Ready_ShopItems() = 0;
public:
	////static CShop* Create(LPDIRECT3DDEVICE9 pGraphic_Device); �ڽĿ��� ����
	//virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
END

