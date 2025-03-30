#include "Door.h"
#include "GameInstance.h"
#include "Collider_Sphere.h"
#include "Collider_Cube.h"
#include "Player.h"  


CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCollisionObject(pGraphic_Device)
{
}

CDoor::CDoor(const CDoor& Prototype)
	:CCollisionObject(Prototype)
{
}

HRESULT CDoor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDoor::Initialize(void* pArg)
{
	INIT_PARENT(pArg)

		if (pArg != nullptr)
		{
			DOOR_DESC* pDesc = (DOOR_DESC*)pArg;
			m_eDoorType = pDesc->eType;
			m_eDoorColor = pDesc->eColor;
			m_fSlideDistance = pDesc->fSlideDistance;
			m_stKeyItemTag = pDesc->stKeyItemTag;
			m_bIsActive = pDesc->bStartsActive;
		}

	if (FAILED(Ready_Components()))
		return E_FAIL;

	// �ʱ� ��ġ ����
	m_vOriginalPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// ������ ���� ��ġ ��� (�Ʒ��� �����̵�)
	m_vSlidePosition = m_vOriginalPosition;
	m_vSlidePosition.y -= m_fSlideDistance;  // Y�� ���� �ϰ�
	return S_OK;
}

void CDoor::Update(_float fTimeDelta)
{
	// �� �ִϸ��̼� ������Ʈ
	if (m_eDoorState == DOOR_STATE::OPENING)
	{
		m_fDoorOpenAmount += m_fDoorOpenSpeed * fTimeDelta;
		if (m_fDoorOpenAmount >= 1.0f)
		{
			m_fDoorOpenAmount = 1.0f;
			m_eDoorState = DOOR_STATE::OPEN;
		}
		UpdateDoorTransform();
	}
	else if (m_eDoorState == DOOR_STATE::CLOSING)
	{
		m_fDoorOpenAmount -= m_fDoorOpenSpeed * fTimeDelta;
		if (m_fDoorOpenAmount <= 0.0f)
		{
			m_fDoorOpenAmount = 0.0f;
			m_eDoorState = DOOR_STATE::CLOSED;
		}
		UpdateDoorTransform();
	}

	// �浹 ó��
	if (m_pTransformCom && m_pColliderCom)
	{
		m_pColliderCom->Set_WorldMat(m_pTransformCom->Get_WorldMat());
		m_pColliderCom->Update_Collider(TEXT("Com_Transform"), m_pTransformCom->Compute_Scaled());
	}

	m_pGameInstance->Add_Collider(CG_DOOR, m_pColliderCom);
}

void CDoor::Late_Update(_float fTimeDelta)
{
	// ������ �׷쿡 �߰�
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CDoor::Render()
{
	if (FAILED(m_pMaterialCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	SetUp_RenderState();

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	Release_RenderState();

	return S_OK;
}

HRESULT CDoor::SetUp_RenderState()
{
	// ���� ���� ����
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);

	return S_OK;
}

HRESULT CDoor::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	return S_OK;
}

HRESULT CDoor::On_Collision(CCollisionObject* other)
{
	// ��Ȱ�� ���¿����� �浹 ó������ ����
	if (!m_bIsActive)
		return S_OK;

	// �÷��̾����� Ȯ��
	if (other->Get_Type() != CG_PLAYER)
		return S_OK;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		TryOpen(other);
	}
 	return S_OK;
}

void CDoor::Open_Door()
{
	if (m_eDoorState == DOOR_STATE::CLOSED || m_eDoorState == DOOR_STATE::CLOSING)
	{
		m_eDoorState = DOOR_STATE::OPENING;
	}
}

void CDoor::Close_Door()
{
	if (m_eDoorState == DOOR_STATE::OPEN || m_eDoorState == DOOR_STATE::OPENING)
	{
		m_eDoorState = DOOR_STATE::CLOSING;
	}
}

bool CDoor::TryOpen(CCollisionObject* pPlayer)
{
	// ���谡 �ʿ��� ���� ���
	if (m_eDoorType == DOOR_TYPE::KEY)
	{
		// �÷��̾��� �κ��丮���� ���� Ȯ��
		CPlayer* pPlayerObj = dynamic_cast<CPlayer*>(pPlayer);
		if (pPlayerObj)
		{
			bool hasKey = false;

			// ���� ���� Ű Ȯ��
			switch (m_eDoorColor)
			{
			case DOOR_COLOR::BLUE:
				break;
			case DOOR_COLOR::RED:
				break;
			case DOOR_COLOR::YELLOW:

				break;
				break;
			case DOOR_COLOR::NORMAL:
				// �Ϲ� ���� Ư�� Ű Ȯ�� ���� ����
				hasKey = true;
				break;
			}

			//// Ŀ���� Ű �±װ� �ִ� ���
			//if (!m_stKeyItemTag.empty())
			//{
			//	hasKey = pPlayerObj->HasItem(m_stKeyItemTag);
			//}

			if (hasKey)
			{
				Open_Door();
				return true;
			}
			else
			{
				//"���谡 �ʿ��մϴ�"
				return false;
			}
		}
		return false;
	}
	else
	{
		// �Ϲ� ���� �ٷ� ����
		Open_Door();
		return true;
	}
}

void CDoor::UpdateDoorTransform()
{
	// ���� ���� ������ ���� ȸ�� ����
	_float3 currentPosition;

	// ���� ����: original + t * (slide - original)
	currentPosition.x = m_vOriginalPosition.x + m_fDoorOpenAmount * (m_vSlidePosition.x - m_vOriginalPosition.x);
	currentPosition.y = m_vOriginalPosition.y + m_fDoorOpenAmount * (m_vSlidePosition.y - m_vOriginalPosition.y);
	currentPosition.z = m_vOriginalPosition.z + m_fDoorOpenAmount * (m_vSlidePosition.z - m_vOriginalPosition.z);

	// �� ��ġ ����
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, currentPosition);
}

HRESULT CDoor::Ready_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC TransformDesc{ 1.0f, D3DXToRadian(90.0f) };

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider_Cube::COL_CUBE_DESC ColliderDesc = {};
	ColliderDesc.pOwner = this;
	ColliderDesc.fScale = { 1.0f, 1.0f, 1.f }; 
	ColliderDesc.fLocalPos = { 0.0f, 0.0f, 0.0f };
	m_eType = CG_DOOR;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_TexturedCube",
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Material */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Material"),
		TEXT("Com_Material"), reinterpret_cast<CComponent**>(&m_pMaterialCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (!m_tObjDesc.stProtTextureTag.empty())
	{
		if (FAILED(__super::Add_Component(m_tObjDesc.iProtoLevel, m_tObjDesc.stProtTextureTag,
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}

	return S_OK;
}

CDoor* CDoor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDoor* pInstance = new CDoor(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDoor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDoor::Clone(void* pArg)
{
	CDoor* pInstance = new CDoor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDoor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDoor::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pMaterialCom);
}

json CDoor::Serialize()
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

	// �� ���� ������ ����ȭ
	j["door_type"] = static_cast<int>(m_eDoorType);
	j["door_color"] = static_cast<int>(m_eDoorColor);
	j["door_state"] = static_cast<int>(m_eDoorState);
	j["slide_distance"] = m_fSlideDistance;
	j["open_speed"] = m_fDoorOpenSpeed;

	// ���� �±� ����
	if (!m_stKeyItemTag.empty())
	{
		j["key_item_tag"] = ISerializable::WideToUtf8(m_stKeyItemTag);
	}

	return j;
}

void CDoor::Deserialize(const json& j)
{
	SET_TRANSFORM(j, m_pTransformCom);

	if (j.contains("door_type"))
		m_eDoorType = static_cast<DOOR_TYPE>(j["door_type"].get<int>());

	if (j.contains("door_color"))
		m_eDoorColor = static_cast<DOOR_COLOR>(j["door_color"].get<int>());

	if (j.contains("door_state"))
		m_eDoorState = static_cast<DOOR_STATE>(j["door_state"].get<int>());

	if (j.contains("slide_distance"))
		m_fSlideDistance = j["slide_distance"].get<float>();

	if (j.contains("open_speed"))
		m_fDoorOpenSpeed = j["open_speed"].get<float>();

	if (j.contains("is_active"))
		m_bIsActive = j["is_active"].get<bool>();

	if (j.contains("key_item_tag"))
		m_stKeyItemTag = ISerializable::Utf8ToWide(j["key_item_tag"].get<std::string>());

	// ���� ��ġ ����
	m_vOriginalPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// ���� �����̵� ��ġ ���
	m_vSlidePosition = m_vOriginalPosition;
	m_vSlidePosition.y -= m_fSlideDistance;

	// ���� ���¿� ���� �� ��ġ ����
	if (m_eDoorState == DOOR_STATE::OPEN)
	{
		m_fDoorOpenAmount = 1.0f;
		UpdateDoorTransform();
	}
	else if (m_eDoorState == DOOR_STATE::CLOSED)
	{
		m_fDoorOpenAmount = 0.0f;
		UpdateDoorTransform();
	}
}
