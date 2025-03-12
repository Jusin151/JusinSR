#include "UI_ExpDisplay.h"
#include "GameInstance.h"
#include "CUI_Manager.h"

CUI_ExpDisplay::CUI_ExpDisplay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI_Base(pGraphic_Device)
{
}

CUI_ExpDisplay::CUI_ExpDisplay(const CUI_ExpDisplay& Prototype)
	: CUI_Base(Prototype),
	m_EXP_pTextureCom(Prototype.m_EXP_pTextureCom),
	m_EXP_pTransformCom(Prototype.m_EXP_pTransformCom),
	m_EXP_pVIBufferCom(Prototype.m_EXP_pVIBufferCom),
	m_EXP_INFO{ Prototype.m_EXP_INFO }
{
}

HRESULT CUI_ExpDisplay::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_ExpDisplay::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))

		return E_FAIL;

	if (pArg != nullptr)
	{
		m_EXP_INFO = *reinterpret_cast<UI_Child_Desc*>(pArg);
		m_EXP_INFO.vPos += CUI_Manager::GetInstance()->GetParent_Pos();
		Set_Position(m_EXP_INFO.vPos);
		Set_Size(m_EXP_INFO.vSize);
	}
	else
		return E_FAIL;


	m_EXP_pTransformCom->Set_Scale(m_EXP_INFO.vSize.x, m_EXP_INFO.vSize.y, 1.f);
	m_EXP_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_EXP_INFO.vPos.x, m_EXP_INFO.vPos.y, 0.f));
	return S_OK;
}

void CUI_ExpDisplay::Priority_Update(_float fTimeDelta)
{
}

void CUI_ExpDisplay::Update(_float fTimeDelta)
{
}

void CUI_ExpDisplay::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CUI_ExpDisplay::Render()
{
	D3DXMATRIX matOldView, matOldProj;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matOldProj);

	D3DXMATRIX matView;
	D3DXMatrixIdentity(&matView);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProj;
	D3DXMatrixOrthoLH(&matProj, g_iWinSizeX, g_iWinSizeY, 0.f, 1.f);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &matProj);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	if (FAILED(m_EXP_pTransformCom->Bind_Resource()))
		return E_FAIL;
	if (FAILED(m_EXP_pTextureCom->Bind_Resource(0)))
		return E_FAIL;
	if (FAILED(m_EXP_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_EXP_pVIBufferCom->Render()))
		return E_FAIL;


	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &matOldProj);

	return S_OK;
}

HRESULT CUI_ExpDisplay::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_EXPDisplay"),
		TEXT("Com_Texture_EXP"), reinterpret_cast<CComponent**>(&m_EXP_pTextureCom))))
 		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_EXPDisplay"),
		TEXT("Com_VIBuffer_EXP"), reinterpret_cast<CComponent**>(&m_EXP_pVIBufferCom))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC tDesc{ 10.f,D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Transform_EXPDisplay"),
		TEXT("Com_Transform_EXP"), reinterpret_cast<CComponent**>(&m_EXP_pTransformCom), &tDesc)))
		return E_FAIL;

	return S_OK;
}

CUI_ExpDisplay* CUI_ExpDisplay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_ExpDisplay* pInstance = new CUI_ExpDisplay(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("경험치바 UI 원본 생성 실패 ");
		Safe_Release(pInstance);
	}


	return pInstance;
}

CGameObject* CUI_ExpDisplay::Clone(void* pArg)
{
	CUI_ExpDisplay* pInstace = new CUI_ExpDisplay(*this);

	if (FAILED(pInstace->Initialize(pArg)))
	{
		MSG_BOX("경험치바 UI 복제 실패");
		Safe_Release(pInstace);
	}

	return pInstace;
}

void CUI_ExpDisplay::Free()
{
	__super::Free();

	Safe_Release(m_EXP_pTextureCom);
	Safe_Release(m_EXP_pTransformCom);
	Safe_Release(m_EXP_pVIBufferCom);
}
