#include "Trigger.h"
#include "GameInstance.h"
#include "Collider_Sphere.h"
#include "Collider_Cube.h"

CTrigger::CTrigger(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCollisionObject{ pGraphic_Device }
{
}

CTrigger::CTrigger(const CTrigger& Prototype)
	:CCollisionObject{ Prototype }
{
}

HRESULT CTrigger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTrigger::Initialize(void* pArg)
{
	INIT_PARENT(pArg)

		if (FAILED(Ready_Components()))
			return E_FAIL;

	if (pArg != nullptr)
	{
		TRIGGER_DESC* pDesc = (TRIGGER_DESC*)pArg;
		m_eTriggerType = pDesc->eType;
		m_bIsActive = pDesc->bStartsActive;
	}

	return S_OK;
}

void CTrigger::Update(_float fTimeDelta)
{
	if (m_pTransformCom && m_pColliderCom)
	{
		m_pColliderCom->Set_WorldMat(m_pTransformCom->Get_WorldMat());

		// �ݶ��̴� Ʈ������ ������Ʈ
		m_pColliderCom->Update_Collider(TEXT("Com_Transform"), m_pTransformCom->Compute_Scaled());
	}

	// ���� �ν��Ͻ��� �浹 Ȯ�� �ý��ۿ� �ݶ��̴� �߰�
	m_pGameInstance->Add_Collider(CG_TRIGGER, m_pColliderCom);
}

void CTrigger::Late_Update(_float fTimeDelta)
{
	// ����� ��忡���� ������ �׷쿡 �߰�
	if (g_bDebugCollider)
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CTrigger::Render()
{
	// ����� ��忡���� Ʈ���� �ݶ��̴� ������
	if (g_bDebugCollider && m_pColliderCom)
	{
		m_pColliderCom->Render();
	}

	return S_OK;
}

HRESULT CTrigger::On_Collision(CCollisionObject* other)
{
	// ��Ȱ�� ���¿����� �浹 ó������ ����
	if (!m_bIsActive)
		return S_OK;

	// �÷��̾����� Ȯ�� (CG_PLAYER�� ��)
	if (other->Get_Type() != CG_PLAYER)
		return S_OK;

	// Ʈ���� Ÿ�Կ� ���� ó��
	switch (m_eTriggerType)
	{
	case TRIGGER_TYPE::BUTTON:
	{
		// ��ư�� �浹 �� �ٷ� Ȱ��ȭ
		if (!m_bWasTriggered)
		{
			m_bWasTriggered = true;
			OnTrigger_Activated();
		}
		break;
	}
	case TRIGGER_TYPE::INTERACTION:
	{

		// Ű �Է� ������ ������ �Է� �ý��۰� ���� �ʿ�
		break;
	}
	}

	return S_OK;
}

void CTrigger::Activate()
{
	if (!m_bIsActive)
	{
		m_bIsActive = true;
		OnTrigger_Activated();
	}
}

void CTrigger::Deactivate()
{
	if (m_bIsActive)
	{
		m_bIsActive = false;
		m_bWasTriggered = false;
		OnTrigger_Deactivated();
	}
}

void CTrigger::AddTargetObject(CGameObject* pTarget)
{
	if (pTarget != nullptr)
		m_TargetObject = pTarget;
	Safe_AddRef(m_TargetObject);
}

void CTrigger::OnTrigger_Activated()
{

}

void CTrigger::OnTrigger_Deactivated()
{

}

HRESULT CTrigger::Ready_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC TransformDesc{ 1.0f, D3DXToRadian(90.0f) };

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;


	CCollider_Cube::COL_CUBE_DESC ColliderDesc = {};
	ColliderDesc.pOwner = this;
	ColliderDesc.fScale = { 1.0f, 1.0f, 1.0f };
	ColliderDesc.fLocalPos = { 0.0f, 0.0f, 0.0f };

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;


	if (!m_tObjDesc.stProtTextureTag.empty())
	{
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(m_tObjDesc.iProtoLevel, m_tObjDesc.stProtTextureTag,
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}

	return S_OK;
}

CTrigger* CTrigger::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrigger* pInstance = new CTrigger(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTrigger::Clone(void* pArg)
{
	CTrigger* pInstance = new CTrigger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrigger::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_TargetObject);
}

json CTrigger::Serialize()
{
	json j = __super::Serialize();
	// Ʈ������ ������ ����ȭ
	auto pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	auto scale = m_pTransformCom->Compute_Scaled();
	auto angle = m_pTransformCom->Get_EulerAngles();

	j["position"] = {
	RoundToDecimalPlaces(pos.x, 2),
	RoundToDecimalPlaces(pos.y, 2),
	RoundToDecimalPlaces(pos.z, 2)
	};
	j["rotation"] = {
		RoundToDecimalPlaces(angle.x, 2),
		RoundToDecimalPlaces(angle.y, 2),
		RoundToDecimalPlaces(angle.z, 2)
	};
	j["scale"] = {
		RoundToDecimalPlaces(scale.x, 2),
		RoundToDecimalPlaces(scale.y, 2),
		RoundToDecimalPlaces(scale.z, 2)
	};

	j["trigger_type"] = static_cast<int>(m_eTriggerType);

	if (m_TargetObject)
	{
		j["target_tags"] = ISerializable::WideToUtf8(m_TargetObject->Get_Tag());
	}

	return j;
}

void CTrigger::Deserialize(const json& j)
{
	SET_TRANSFORM(j, m_pTransformCom);

	if (j.contains("trigger_type"))
		m_eTriggerType = static_cast<TRIGGER_TYPE>(j["trigger_type"].get<int>());

	if (j.contains("is_active"))
		m_bIsActive = j["is_active"].get<bool>();

}