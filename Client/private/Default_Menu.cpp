#include "Default_Menu.h"
#include "GameInstance.h"
#include "CUI_Manager.h"

CDefault_Menu::CDefault_Menu(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI_Base(pGraphic_Device)
{
}

CDefault_Menu::CDefault_Menu(const CDefault_Menu& Prototype)
	: CUI_Base(Prototype),
	m_BackMenu_pTextureCom(Prototype.m_BackMenu_pTextureCom),
	m_BackMenu_pTransformCom(Prototype.m_BackMenu_pTransformCom),
	m_BackMenu_pVIBufferCom(Prototype.m_BackMenu_pVIBufferCom),
	m_BackMenu_INFO{ Prototype.m_BackMenu_INFO }
{
}

HRESULT CDefault_Menu::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

HRESULT CDefault_Menu::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_BackMenu_INFO.vSize = { 310.f,720.f };
	m_BackMenu_INFO.fAlpha = 1.0f;
	m_BackMenu_INFO.vPos = { 484.5f,0.f };


	Set_Position(m_BackMenu_INFO.vPos);
	Set_Size(m_BackMenu_INFO.vSize);
	CUI_Manager::GetInstance()->AddUI(L"Default_Menu", this);


	m_BackMenu_pTransformCom->Set_Scale(m_BackMenu_INFO.vSize.x, m_BackMenu_INFO.vSize.y, 1.f);
	m_BackMenu_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_BackMenu_INFO.vPos.x, m_BackMenu_INFO.vPos.y, 0.f));
	return S_OK;
}

void CDefault_Menu::Priority_Update(_float fTimeDelta)
{
}

void CDefault_Menu::Update(_float fTimeDelta)
{
}

void CDefault_Menu::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CDefault_Menu::Render()
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


	if (FAILED(m_BackMenu_pTransformCom->Bind_Resource()))
		return E_FAIL;
	if (FAILED(m_BackMenu_pTextureCom->Bind_Resource(0)))
		return E_FAIL;
	if (FAILED(m_BackMenu_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_BackMenu_pVIBufferCom->Render()))
		return E_FAIL;


	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &matOldProj);

	return S_OK;
}

HRESULT CDefault_Menu::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Menu"),
		TEXT("Com_Texture_Back_5"), reinterpret_cast<CComponent**>(&m_BackMenu_pTextureCom))))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect_Menu"),
		TEXT("Com_VIBuffer_Back_5"), reinterpret_cast<CComponent**>(&m_BackMenu_pVIBufferCom))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC tDesc{ 10.f,D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Transform_Menu"),
		TEXT("Com_Transform_Back_5"), reinterpret_cast<CComponent**>(&m_BackMenu_pTransformCom), &tDesc)))
		return E_FAIL;



	return S_OK;
}

CDefault_Menu* CDefault_Menu::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDefault_Menu* pInstance = new CDefault_Menu(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("로고메뉴 원본 생성 실패 ");
		Safe_Release(pInstance);
	}


	return pInstance;
}

CGameObject* CDefault_Menu::Clone(void* pArg)
{
	CDefault_Menu* pInstace = new CDefault_Menu(*this);

	if (FAILED(pInstace->Initialize(pArg)))
	{
		MSG_BOX("로고메뉴 복제 실패");
		Safe_Release(pInstace);
	}

	return pInstace;
}

void CDefault_Menu::Free()
{
	__super::Free();

	Safe_Release(m_BackMenu_pTextureCom);
	Safe_Release(m_BackMenu_pTransformCom);
	Safe_Release(m_BackMenu_pVIBufferCom);
}
