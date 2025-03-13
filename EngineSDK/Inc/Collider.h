#pragma once

#include "Component.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCollider abstract : public CComponent
{
protected:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:

	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	// ������Ʈ���� �����ؼ� �Ŵ����� �ѱ��
	_float3 Get_State(CTransform::TRANSFORMSTATE eState) const {
		return *reinterpret_cast<const _float3*>(&m_WorldMatrix.m[eState][0]);
	}

	void Set_State(CTransform::TRANSFORMSTATE eState, const _float3& vState) {
		memcpy(&m_WorldMatrix.m[eState][0], &vState, sizeof vState);
	}


	COLLIDERGROUP	Get_Type()
	{
		return m_eType;
	}
	void	Set_Other_Type(COLLIDERGROUP eOtherType)
	{
		m_eOtherType = eOtherType;
	}

	COLLIDERGROUP	Get_Other_Type()
	{
		return m_eOtherType;
	}

protected:
	_float4x4				m_WorldMatrix = {};
	// ť���� ���� �����
	_float3					m_fPos = {};

	// �ڱ� Ÿ�� ���� �ϰ�
	// �浹�ϸ� �ڱ� Ÿ���� ��뿡�� ����, ��� Ÿ���� ������ ����
	// �� �� object���� ��� Ÿ�� ���� üũ
	COLLIDERGROUP			m_eType = { CG_END };
	COLLIDERGROUP			m_eOtherType = { CG_END };
public:

	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;

	
};

END

