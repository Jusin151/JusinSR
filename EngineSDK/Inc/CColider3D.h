#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider3D abstract : public CComponent
{
	typedef struct tagCollider3D
	{
		CBase* pParent;
		_float3 vPosition;
	}COLLIDER3D_DESC;
protected:
	CCollider3D(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider3D(const CCollider3D& Prototype);
	virtual ~CCollider3D() = default;
public:
	virtual HRESULT Initialize_Prototype() = 0;
	virtual HRESULT Initialize(void* pArg) = 0;
protected:
	CBase* m_pParent = {};
	_float3 m_vPosition;
public:
	virtual void Priority_Update(_float fTimeDelta) = 0;
	virtual void Update(_float fTimeDelta) = 0;
	virtual void Late_Update(_float fTimeDelta) = 0;
	virtual HRESULT Render() = 0;
	virtual CBase* OnCollision3DEnter(void* pArg) = 0;
	virtual CBase* OnCollision3DExit(void* pArg) = 0;
	virtual void OnCollision(void* pArg) = 0;

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END

