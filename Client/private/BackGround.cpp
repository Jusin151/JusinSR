#include "BackGround.h"
#include "GameInstance.h"
#include "CUI_Manager.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI_Base(pGraphic_Device)
{
}

CBackGround::CBackGround(const CBackGround& Prototype)
	: CUI_Base(Prototype),
	m_Back_pTextureCom(Prototype.m_Back_pTextureCom),
	m_Back_pTransformCom(Prototype.m_Back_pTransformCom),
	m_Back_pVIBufferCom(Prototype.m_Back_pVIBufferCom),
	m_BackGround_INFO{ Prototype.m_BackGround_INFO }
{
}

HRESULT CBackGround::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

HRESULT CBackGround::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_BackGround_INFO.vSize = { 1085.f,720.f };
	m_BackGround_INFO.fAlpha = 1.0f;
	m_BackGround_INFO.vPos = { -192.f,-90.f };
	m_BackGround_INFO.ProjMatrix = {};
	m_BackGround_INFO.ViewMatrix = {};
	m_BackGround_INFO.WorldMatrix = {};

	Set_Position(m_BackGround_INFO.vPos);
	Set_Size(m_BackGround_INFO.vSize);
	CUI_Manager::GetInstance()->AddUI(L"LOGO", this);


	m_Back_pTransformCom->Set_Scale(m_BackGround_INFO.vSize.x, m_BackGround_INFO.vSize.y, 1.f);
	m_Back_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_BackGround_INFO.vPos.x, m_BackGround_INFO.vPos.y, 0.f));
	return S_OK;
}

void CBackGround::Priority_Update(_float fTimeDelta)
{
}

void CBackGround::Update(_float fTimeDelta)
{

}

void CBackGround::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CBackGround::Render()
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


	if (FAILED(m_Back_pTransformCom->Bind_Resource()))
		return E_FAIL;
	if (FAILED(m_Back_pTextureCom->Bind_Resource(0)))
		return E_FAIL;
	if (FAILED(m_Back_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_Back_pVIBufferCom->Render()))
		return E_FAIL;


	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &matOldProj);

	return S_OK;
}

HRESULT CBackGround::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		TEXT("Com_Texture_Back"), reinterpret_cast<CComponent**>(&m_Back_pTextureCom))))
   		return E_FAIL;


 	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect_BackGround"),
		TEXT("Com_VIBuffer_Back"), reinterpret_cast<CComponent**>(&m_Back_pVIBufferCom))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC tDesc{ 10.f,D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Transform_BackGround"),
		TEXT("Com_Transform_Back"), reinterpret_cast<CComponent**>(&m_Back_pTransformCom), &tDesc)))
		return E_FAIL;



	return S_OK;
}

CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBackGround* pInstance = new CBackGround(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("첫번째백그라운드원본 생성 실패 ");
		Safe_Release(pInstance);
	}


	return pInstance;
}

CGameObject* CBackGround::Clone(void* pArg)
{
	CBackGround* pInstace = new CBackGround(*this);

	if (FAILED(pInstace->Initialize(pArg)))
	{
		MSG_BOX("첫번째백그라운드 복제 실패");
		Safe_Release(pInstace);
	}

	return pInstace;
}

void CBackGround::Free()
{
	__super::Free();

	Safe_Release(m_Back_pTextureCom);
	Safe_Release(m_Back_pTransformCom);
	Safe_Release(m_Back_pVIBufferCom);
}
