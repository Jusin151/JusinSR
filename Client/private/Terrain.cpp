#include "Terrain.h"
#include "GameInstance.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject { pGraphic_Device }
{

}

CTerrain::CTerrain(const CTerrain& Prototype)
    : CGameObject { Prototype }
{
}

HRESULT CTerrain::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

    return S_OK;
}

void CTerrain::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CTerrain::Update(_float fTimeDelta)
{
	if (GetKeyState(VK_UP) & 0x8000)
	{
		m_pTransformCom->Go_Straight(0.016f);
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		m_pTransformCom->Go_Backward(0.016f);
	}
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransformCom->Go_Left(0.016f);
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->Go_Right(0.016f);
	}

}

void CTerrain::Late_Update(_float fTimeDelta)
{	
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CTerrain::Render()
{

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	/* 정점을 그린다. */
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	

    return S_OK;
}

HRESULT CTerrain::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f)};

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;



	return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain* pInstance = new CTerrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrain::Clone(void* pArg)
{	
	CTerrain* pInstance = new CTerrain(*this);


	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);	
	Safe_Release(m_pTextureCom);
}
