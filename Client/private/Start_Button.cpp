#include "Start_Button.h"
#include "GameInstance.h"
#include "CUI_Manager.h"

CStart_Button::CStart_Button(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI_Base(pGraphic_Device)
{
}

CStart_Button::CStart_Button(const CStart_Button& Prototype)
	: CUI_Base(Prototype),
	m_StartButton_pTextureCom(Prototype.m_StartButton_pTextureCom),
	m_StartButton_pTransformCom(Prototype.m_StartButton_pTransformCom),
	m_StartButton_pTextureCom_Second{ Prototype.m_StartButton_pTextureCom_Second },
	m_StartButton_pVIBufferCom(Prototype.m_StartButton_pVIBufferCom),
	m_StartButton_INFO{ Prototype.m_StartButton_INFO }
{
}

HRESULT CStart_Button::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

HRESULT CStart_Button::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_StartButton_INFO.vSize = { 296.f,32.f };
	m_StartButton_INFO.fAlpha = 1.0f;
	m_StartButton_INFO.vPos = { 485.f,64.f };


	Set_Position(m_StartButton_INFO.vPos);
	Set_Size(m_StartButton_INFO.vSize);
	CUI_Manager::GetInstance()->AddUI(L"Start_Button", this);


	m_StartButton_pTransformCom->Set_Scale(m_StartButton_INFO.vSize.x, m_StartButton_INFO.vSize.y, 1.f);
	m_StartButton_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_StartButton_INFO.vPos.x, m_StartButton_INFO.vPos.y, 0.f));
	return S_OK;
}

void CStart_Button::Priority_Update(_float fTimeDelta)
{
}
void CStart_Button::Update(_float fTimeDelta)
{
	
}

void CStart_Button::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CStart_Button::Render()
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

	if (FAILED(m_StartButton_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (m_bIsMouseOver)
	{
		if (FAILED(m_StartButton_pTextureCom_Second->Bind_Resource(0)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_StartButton_pTextureCom->Bind_Resource(0)))
			return E_FAIL;
	}

	if (FAILED(m_StartButton_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_StartButton_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &matOldProj);

	return S_OK;
}


HRESULT CStart_Button::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Menu_StartButton"),
		TEXT("Com_Texture_StartButton"), reinterpret_cast<CComponent**>(&m_StartButton_pTextureCom))))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Menu_StartButton"),
		TEXT("Com_VIBuffer_StartButton"), reinterpret_cast<CComponent**>(&m_StartButton_pVIBufferCom))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC tDesc{ 10.f,D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Transform_Menu_StartButton"),
		TEXT("Com_Transform_StartButton"), reinterpret_cast<CComponent**>(&m_StartButton_pTransformCom), &tDesc)))
		return E_FAIL;

	CTransform::TRANSFORM_DESC tDesc_Select{ 10.f,D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Menu_StartButton_Select"),
		TEXT("Com_Transform_StartButton_Select"), reinterpret_cast<CComponent**>(&m_StartButton_pTextureCom_Second), &tDesc_Select)))
		return E_FAIL;


	return S_OK;
}

CStart_Button* CStart_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CStart_Button* pInstance = new CStart_Button(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("시작버튼 원본 생성 실패 ");
		Safe_Release(pInstance);
	}


	return pInstance;
}

CGameObject* CStart_Button::Clone(void* pArg)
{
	CStart_Button* pInstace = new CStart_Button(*this);

	if (FAILED(pInstace->Initialize(pArg)))
	{
		MSG_BOX("시작버튼 복제 실패");
		Safe_Release(pInstace);
	}

	return pInstace;
}

void CStart_Button::Free()
{
	__super::Free();

	Safe_Release(m_StartButton_pTextureCom);
	Safe_Release(m_StartButton_pTransformCom);
	Safe_Release(m_StartButton_pVIBufferCom);
	Safe_Release(m_StartButton_pTextureCom_Second);
}
