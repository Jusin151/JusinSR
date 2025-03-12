#include "UI_Default_Panel.h"
#include "GameInstance.h"
#include "CUI_Manager.h"

CUI_Default_Panel::CUI_Default_Panel(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI_Base(pGraphic_Device)
{
}


CUI_Default_Panel::CUI_Default_Panel(const CUI_Default_Panel& Prototype)
	: CUI_Base(Prototype),
	m_Default_pTextureCom(Prototype.m_Default_pTextureCom),
  m_Default_pTransformCom(Prototype.m_Default_pTransformCom),
  m_Default_pVIBufferCom(Prototype.m_Default_pVIBufferCom),
	m_UI_INFO{ Prototype.m_UI_INFO }
{
	
}

HRESULT CUI_Default_Panel::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;
	return S_OK;
}
HRESULT CUI_Default_Panel::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;
    
	if (pArg != nullptr)
	{
		m_UI_INFO = *reinterpret_cast<UI_Parent_Desc*>(pArg);
		Set_Parent(this);
		Set_Position(m_UI_INFO.vPos);
		Set_Size(m_UI_INFO.vSize);
		CUI_Manager::GetInstance()->AddUI(L"Parent",this);
	}
	else
		return E_FAIL;

	m_Default_pTransformCom->Set_Scale(m_UI_INFO.vSize.x, m_UI_INFO.vSize.y, 1.f);
	m_Default_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_UI_INFO.vPos.x, m_UI_INFO.vPos.y, 0.f));

	return S_OK;
}

void CUI_Default_Panel::Priority_Update(_float fTimeDelta){}
void CUI_Default_Panel::Update(_float fTimeDelta){}

void CUI_Default_Panel::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))return;
}
HRESULT CUI_Default_Panel::Render()
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

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (FAILED(m_Default_pTransformCom->Bind_Resource()))
		return E_FAIL;
	if (FAILED(m_Default_pTextureCom->Bind_Resource(0)))
		return E_FAIL;
	if (FAILED(m_Default_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_Default_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &matOldProj);

	return S_OK;
}
 



HRESULT CUI_Default_Panel::Ready_Components()
 {
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Default_PlayerUI"),
		TEXT("Com_Texture_Default"), reinterpret_cast<CComponent**>(&m_Default_pTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_PlayerUI"),
		TEXT("Com_VIBuffer_Default"), reinterpret_cast<CComponent**>(&m_Default_pVIBufferCom))))
		return E_FAIL;

 	CTransform::TRANSFORM_DESC tDesc{ 10.f,D3DXToRadian(90.f) };

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Transform_Default"),
		TEXT("Com_Transform_Default"),reinterpret_cast<CComponent**>(&m_Default_pTransformCom),&tDesc)))
 		return E_FAIL;

	return S_OK;
}

CUI_Default_Panel* CUI_Default_Panel::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Default_Panel* pInstance = new CUI_Default_Panel(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CUI_Default_Panel 원본 생성 실패 ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Default_Panel::Clone(void* pArg)
{
	CUI_Default_Panel* pInstace = new CUI_Default_Panel(*this);
	if (FAILED(pInstace->Initialize(pArg)))
	{
		MSG_BOX("CUI_Default_Panel 복제 실패");
		Safe_Release(pInstace);
	}
	return pInstace;
}

void CUI_Default_Panel::Free()
{
	__super::Free();
	Safe_Release(m_Default_pTextureCom);
	Safe_Release(m_Default_pVIBufferCom);
	Safe_Release(m_Default_pTransformCom);

}
