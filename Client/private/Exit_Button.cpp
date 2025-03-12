#include "Exit_Button.h"
#include "GameInstance.h"
#include "CUI_Manager.h"

CExit_Button::CExit_Button(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI_Base(pGraphic_Device)
{
}

CExit_Button::CExit_Button(const CExit_Button& Prototype)
	: CUI_Base(Prototype),
	m_ExitButton_pTextureCom(Prototype.m_ExitButton_pTextureCom),
	m_ExitButton_pTransformCom(Prototype.m_ExitButton_pTransformCom),
	m_ExitButton_pVIBufferCom(Prototype.m_ExitButton_pVIBufferCom),
	m_ExitButton_INFO{ Prototype.m_ExitButton_INFO }
{
}

HRESULT CExit_Button::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

HRESULT CExit_Button::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_ExitButton_INFO.vSize = { 296.f,32.f };
	m_ExitButton_INFO.fAlpha = 1.0f;
	m_ExitButton_INFO.vPos = { 485.f,-64.f };


	Set_Position(m_ExitButton_INFO.vPos);
	Set_Size(m_ExitButton_INFO.vSize);
	CUI_Manager::GetInstance()->AddUI(L"Start_Button", this);


	m_ExitButton_pTransformCom->Set_Scale(m_ExitButton_INFO.vSize.x, m_ExitButton_INFO.vSize.y, 1.f);
	m_ExitButton_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_ExitButton_INFO.vPos.x, m_ExitButton_INFO.vPos.y, 0.f));
	return S_OK;
}

void CExit_Button::Priority_Update(_float fTimeDelta)
{
}

void CExit_Button::Update(_float fTimeDelta)
{
}

void CExit_Button::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CExit_Button::Render()
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


	if (FAILED(m_ExitButton_pTransformCom->Bind_Resource()))
		return E_FAIL;
	if (FAILED(m_ExitButton_pTextureCom->Bind_Resource(0)))
		return E_FAIL;
	if (FAILED(m_ExitButton_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_ExitButton_pVIBufferCom->Render()))
		return E_FAIL;


	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &matOldProj);

	return S_OK;
}

HRESULT CExit_Button::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Menu_ExitButton"),
		TEXT("Com_Texture_ExitButton"), reinterpret_cast<CComponent**>(&m_ExitButton_pTextureCom))))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Menu_ExitButton"),
		TEXT("Com_VIBuffer_ExitButton"), reinterpret_cast<CComponent**>(&m_ExitButton_pVIBufferCom))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC tDesc{ 10.f,D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Transform_Menu_ExitButton"),
		TEXT("Com_Transform_ExitButton"), reinterpret_cast<CComponent**>(&m_ExitButton_pTransformCom), &tDesc)))
		return E_FAIL;



	return S_OK;
}

CExit_Button* CExit_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CExit_Button* pInstance = new CExit_Button(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("종료버튼 원본 생성 실패 ");
		Safe_Release(pInstance);
	}


	return pInstance;
}

CGameObject* CExit_Button::Clone(void* pArg)
{
	CExit_Button* pInstace = new CExit_Button(*this);

	if (FAILED(pInstace->Initialize(pArg)))
	{
		MSG_BOX("종료버튼 복제 실패");
		Safe_Release(pInstace);
	}

	return pInstace;
}

void CExit_Button::Free()
{
	__super::Free();

	Safe_Release(m_ExitButton_pTextureCom);
	Safe_Release(m_ExitButton_pTransformCom);
	Safe_Release(m_ExitButton_pVIBufferCom);
	Safe_Release(m_StartButton_pTextureCom_Second);
}
