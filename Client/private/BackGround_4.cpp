#include "BackGround_4.h"
#include "GameInstance.h"
#include "CUI_Manager.h"

CBackGround_4::CBackGround_4(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI_Base(pGraphic_Device)
{
}

CBackGround_4::CBackGround_4(const CBackGround_4& Prototype)
	: CUI_Base(Prototype),
	m_Back4_pTextureCom(Prototype.m_Back4_pTextureCom),
	m_Back4_pTransformCom(Prototype.m_Back4_pTransformCom),
	m_Back4_pVIBufferCom(Prototype.m_Back4_pVIBufferCom),
	m_BackGround4_INFO{ Prototype.m_BackGround4_INFO }
{
}

HRESULT CBackGround_4::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBackGround_4::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_BackGround4_INFO.vSize = { 1569.f,600.f };
	m_BackGround4_INFO.fAlpha = 1.0f;
	m_BackGround4_INFO.vPos = { 0.f,-90.f };

	Set_Position(m_BackGround4_INFO.vPos);
	Set_Size(m_BackGround4_INFO.vSize);
	CUI_Manager::GetInstance()->AddUI(L"LOGO_4", this);

	m_Back4_pTransformCom->Set_Scale(m_BackGround4_INFO.vSize.x, m_BackGround4_INFO.vSize.y, 1.f);
	m_Back4_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_BackGround4_INFO.vPos.x, m_BackGround4_INFO.vPos.y, 0.f));
	return S_OK;
}

void CBackGround_4::Priority_Update(_float fTimeDelta)
{
}

void CBackGround_4::Update(_float fTimeDelta)
{
	const float moveSpeed = 50.0f; // 초당
	const float moveDistance = 1569.0f; // 이미지 크기만큼
	static float accumulatedDistance = 0.0f; 

	accumulatedDistance += moveSpeed * fTimeDelta;

	if (accumulatedDistance >= moveDistance)
	{
		accumulatedDistance = 0.0f;
	}

	// 새로운 위치 계산
	m_BackGround4_INFO.vPos.x = 0.f - accumulatedDistance;
	m_BackGround4_INFO.vPos.y = -90.f;

	m_Back4_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_BackGround4_INFO.vPos.x, m_BackGround4_INFO.vPos.y, 0.f));
}

void CBackGround_4::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CBackGround_4::Render()
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

	// 첫 번째 이미지 렌더링
	if (FAILED(m_Back4_pTransformCom->Bind_Resource()))
		return E_FAIL;
	if (FAILED(m_Back4_pTextureCom->Bind_Resource(0)))
		return E_FAIL;
	if (FAILED(m_Back4_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_Back4_pVIBufferCom->Render()))
		return E_FAIL;

	// 두 번째 이미지 렌더링 (첫 번째 이미지의 오른쪽에!!)
	m_Back4_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_BackGround4_INFO.vPos.x + 1569.f, m_BackGround4_INFO.vPos.y, 0.f));
	if (FAILED(m_Back4_pTransformCom->Bind_Resource()))
		return E_FAIL;
	if (FAILED(m_Back4_pTextureCom->Bind_Resource(0)))
		return E_FAIL;
	if (FAILED(m_Back4_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_Back4_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &matOldProj);

	return S_OK;
}

HRESULT CBackGround_4::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround_4"),
		TEXT("Com_Texture_Back_2"), reinterpret_cast<CComponent**>(&m_Back4_pTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect_BackGround_4"),
		TEXT("Com_VIBuffer_Back_2"), reinterpret_cast<CComponent**>(&m_Back4_pVIBufferCom))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC tDesc{ 10.f,D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Transform_BackGround_4"),
		TEXT("Com_Transform_Back_2"), reinterpret_cast<CComponent**>(&m_Back4_pTransformCom), &tDesc)))
		return E_FAIL;

	return S_OK;
}

CBackGround_4* CBackGround_4::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBackGround_4* pInstance = new CBackGround_4(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("네번째백그라운드 원본 생성 실패 ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBackGround_4::Clone(void* pArg)
{
	CBackGround_4* pInstace = new CBackGround_4(*this);

	if (FAILED(pInstace->Initialize(pArg)))
	{
		MSG_BOX("네번째백그라운드 복제 실패");
		Safe_Release(pInstace);
	}

	return pInstace;
}

void CBackGround_4::Free()
{
	__super::Free();

	Safe_Release(m_Back4_pTextureCom);
	Safe_Release(m_Back4_pTransformCom);
	Safe_Release(m_Back4_pVIBufferCom);
}
