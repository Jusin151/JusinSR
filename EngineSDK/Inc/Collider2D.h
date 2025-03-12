#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider2D abstract : public CComponent
{
	typedef struct tagCollider2D
	{
		CBase* pParent;
		_float2 vPosition;
	}COLLIDER2D_DESC;
private:
	CCollider2D(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider2D(const CCollider2D& Prototype);
	virtual ~CCollider2D() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
private:
	CBase* m_pParent = {};
	_float2 m_vPosition = {};
public:
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	virtual CBase* OnCollision2DEnter(void * pArg) = 0;
	virtual CBase* OnCollision2DExit(void* pArg) = 0;
	virtual void OnCollision(void* pArg) = 0;
public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END