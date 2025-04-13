#pragma once
#include "Deco_Base.h"

class CTree : public CDeco_Base
{
private:
	CTree(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTree(const CTree& Prototype);
	virtual ~CTree() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
private:
	// texturecom�� �� monster���� �߰�
	HRESULT Ready_Components();

public:
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;

public:
	virtual HRESULT Render()override;
	virtual HRESULT On_Collision(CCollisionObject* other);



private:
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	virtual void Select_State()override;

private:


public:
	static CTree* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	virtual void Free();


};

