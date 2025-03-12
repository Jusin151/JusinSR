#include "Loading_UI.h"
#include "GameInstance.h"
#include "CUI_Manager.h"

CLoading_UI::CLoading_UI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI_Base(pGraphic_Device)
{
}

CLoading_UI::CLoading_UI(const CLoading_UI& Prototype)
	: CUI_Base(Prototype),
	m_Loading_pTextureCom(Prototype.m_Loading_pTextureCom),
	m_Loading_pTransformCom(Prototype.m_Loading_pTransformCom),
	m_Loading_pVIBufferCom(Prototype.m_Loading_pVIBufferCom),
	m_Loading_INFO{ Prototype.m_Loading_INFO }
{
}

HRESULT CLoading_UI::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

HRESULT CLoading_UI::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_Loading_INFO.vSize = { 1280.f,720.f };
	m_Loading_INFO.fAlpha = 1.0f;
	m_Loading_INFO.vPos = { 0.f,0.f };


	Set_Position(m_Loading_INFO.vPos);
	Set_Size(m_Loading_INFO.vSize);
	CUI_Manager::GetInstance()->AddUI(L"Loading", this);


	m_Loading_pTransformCom->Set_Scale(m_Loading_INFO.vSize.x, m_Loading_INFO.vSize.y, 1.f);
	m_Loading_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_Loading_INFO.vPos.x, m_Loading_INFO.vPos.y, 0.f));
	return S_OK;
}

void CLoading_UI::Priority_Update(_float fTimeDelta)
{
}

void CLoading_UI::Update(_float fTimeDelta)
{
}

void CLoading_UI::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CLoading_UI::Render()
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


	if (FAILED(m_Loading_pTransformCom->Bind_Resource()))
		return E_FAIL;
	if (FAILED(m_Loading_pTextureCom->Bind_Resource(0)))
		return E_FAIL;
	if (FAILED(m_Loading_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_Loading_pVIBufferCom->Render()))
		return E_FAIL;


	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &matOldProj);

	return S_OK;
}

HRESULT CLoading_UI::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Texture_Loading_UI"),
		TEXT("Com_Texture_Loading"), reinterpret_cast<CComponent**>(&m_Loading_pTextureCom))))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_VIBuffer_Loading_UI"),
		TEXT("Com_VIBuffer_Loading"), reinterpret_cast<CComponent**>(&m_Loading_pVIBufferCom))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC tDesc{ 10.f,D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Transform_Loading_UI"),
		TEXT("Com_Transform_Loading"), reinterpret_cast<CComponent**>(&m_Loading_pTransformCom), &tDesc)))
		return E_FAIL;



	return S_OK;
}

CLoading_UI* CLoading_UI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoading_UI* pInstance = new CLoading_UI(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("로딩메뉴 원본 생성 실패 ");
		Safe_Release(pInstance);
	}


	return pInstance;
}

CGameObject* CLoading_UI::Clone(void* pArg)
{
	CLoading_UI* pInstace = new CLoading_UI(*this);

	if (FAILED(pInstace->Initialize(pArg)))
	{
		MSG_BOX("로딩메뉴 복제 실패");
		Safe_Release(pInstace);
	}

	return pInstace;
}

void CLoading_UI::Free()
{
	__super::Free();

	Safe_Release(m_Loading_pTextureCom);
	Safe_Release(m_Loading_pTransformCom);
	Safe_Release(m_Loading_pVIBufferCom);
}
