﻿#include "Structure.h"
#include "GameInstance.h"
#include "Collider_Sphere.h"
#include "Collider_Cube.h"

CStructure::CStructure(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CStructure::CStructure(const CStructure& Prototype)
	:CGameObject{ Prototype }
	,m_tStructure_Desc(Prototype.m_tStructure_Desc)
{
}

HRESULT CStructure::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStructure::Initialize(void* pArg)
{
	STRUCTURE_DESC* tStructureDesc = static_cast<STRUCTURE_DESC*>(pArg);

	if (tStructureDesc)
	{
		m_tStructure_Desc = *tStructureDesc;

		if (m_tStructure_Desc.stVIBuffer)
			m_strVIBuffer = m_tStructure_Desc.stVIBuffer;

		if (m_tStructure_Desc.stCollProtoTag)
			m_strCollProtoTag = m_tStructure_Desc.stCollProtoTag;

		if (m_tStructure_Desc.stTextureTag)
			m_strTextureTag = m_tStructure_Desc.stTextureTag;

		// 기존 텍스처 경로는 이미 wstring이므로 그대로 복사
		m_tStructure_Desc.stTexturePath = m_tStructure_Desc.stTexturePath;

		// 복사된 wstring의 c_str() 포인터를 할당
		m_tStructure_Desc.stVIBuffer = m_strVIBuffer.c_str();
		m_tStructure_Desc.stCollProtoTag = m_strCollProtoTag.c_str();
		m_tStructure_Desc.stTextureTag = m_strTextureTag.c_str();
	}
	if (FAILED(Ready_Components()))
		return E_FAIL;

	
	m_bIsCubeCollider = (dynamic_cast<CCollider_Cube*>(m_pColliderCom) != nullptr);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tStructure_Desc.vPos);
	return S_OK;
}

void CStructure::Priority_Update(_float fTimeDelta)
{
}

void CStructure::Update(_float fTimeDelta)
{
	m_pColliderCom->Set_WorldMat(m_pTransformCom->Get_WorldMat());

	if (m_bIsCubeCollider)
	{
		(m_pColliderCom)->Update_Collider();
	}
	m_pGameInstance->Add_Collider(CG_STRUCTURE, m_pColliderCom);
}

void CStructure::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CStructure::Render()
{
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

HRESULT CStructure::On_Collision(_float fTimeDelta)
{
	return E_NOTIMPL;
}

HRESULT CStructure::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	/*m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);*/

	return S_OK;
}

HRESULT CStructure::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

HRESULT CStructure::Ready_Components()
{
	_wstring str = m_tStructure_Desc.stTextureTag;
	if (FAILED(m_pGameInstance->Find_Prototype(str)))
	{
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
			m_tStructure_Desc.stTextureTag,
			CTexture::Create(m_pGraphic_Device,
				m_tStructure_Desc.stTexturePath.c_str(), 1))))
			return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_tStructure_Desc.stTextureTag,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, m_tStructure_Desc.stVIBuffer,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;
	CCollider_Cube::COL_CUBE_DESC	ColliderDesc = {};
	ColliderDesc.eType = CG_STRUCTURE;
	ColliderDesc.pOwner = this;
	// 이걸로 콜라이더 크기 설정
	ColliderDesc.fScale = { 1.f, 1.f, 1.f };
	// 오브젝트와 상대적인 거리 설정
	ColliderDesc.fLocalPos = { 0.f, 0.f, 0.f };

	if (FAILED(__super::Add_Component(LEVEL_STATIC, m_tStructure_Desc.stCollProtoTag,
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CStructure* CStructure::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CStructure* pInstance = new CStructure(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CStructure");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStructure::Clone(void* pArg)
{
	CStructure* pInstance = new CStructure(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CStructure");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStructure::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
}

json CStructure::Serialize()
{
	json j = __super::Serialize();
	//j["texture_tag"] = WideToUtf8(m_tStructure_Desc.stTextureTag);
	//j["texture_path"] = WideToUtf8(m_tStructure_Desc.stTexturePath.c_str());
	//j["vibuffer"] = WideToUtf8(m_tStructure_Desc.stVIBuffer);
	//j["collider_tag"] = WideToUtf8(m_tStructure_Desc.stCollProtoTag);

	j["texture_tag"] = WideToUtf8(m_strTextureTag);
	j["texture_path"] = WideToUtf8(m_tStructure_Desc.stTexturePath); // 이미 wstring
	j["vibuffer"] = WideToUtf8(m_strVIBuffer);
	j["collider_tag"] = WideToUtf8(m_strCollProtoTag);


	j["is_cube_collider"] = m_bIsCubeCollider;

	auto pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	auto scale = m_pTransformCom->Compute_Scaled();
	auto angle = m_pTransformCom->Get_EulerAngles();
	j["position"] = { pos.x, pos.y, pos.z };
	j["rotation"] = { angle.x, angle.y, angle.z };
	j["scale"] = { scale.x, scale.y, scale.z };

	return j;
}

void CStructure::Deserialize(const json& j)
{
	if (j.contains("position")) 
	{
		auto pos = j["position"];
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			_float3(pos[0], pos[1], pos[2]));
	}
}
