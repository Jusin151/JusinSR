#include "BackGround_3.h"
#include "GameInstance.h"
#include "CUI_Manager.h"

CBackGround_3::CBackGround_3(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI_Base(pGraphic_Device)
{
}

CBackGround_3::CBackGround_3(const CBackGround_3& Prototype)
	: CUI_Base(Prototype),
	m_Back3_pTextureCom(Prototype.m_Back3_pTextureCom),
	m_Back3_pTransformCom(Prototype.m_Back3_pTransformCom),
	m_Back3_pVIBufferCom(Prototype.m_Back3_pVIBufferCom),
	m_BackGround3_INFO{ Prototype.m_BackGround3_INFO }
{
}

HRESULT CBackGround_3::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

HRESULT CBackGround_3::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	// 디폴트 2048에 763
	m_BackGround3_INFO.vSize = { 2048.f,763.f };
	m_BackGround3_INFO.fAlpha = 1.0f;
	m_BackGround3_INFO.vPos = { 0.f,0.f };


	Set_Position(m_BackGround3_INFO.vPos);
	Set_Size(m_BackGround3_INFO.vSize);
	CUI_Manager::GetInstance()->AddUI(L"LOGO_3", this);


	m_Back3_pTransformCom->Set_Scale(m_BackGround3_INFO.vSize.x, m_BackGround3_INFO.vSize.y, 1.f);
	m_Back3_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_BackGround3_INFO.vPos.x, m_BackGround3_INFO.vPos.y, 0.f));
	return S_OK;
}

void CBackGround_3::Priority_Update(_float fTimeDelta)
{
}

void CBackGround_3::Update(_float fTimeDelta)
{
	const float moveSpeed = 20.0f; // 초당 
	const float moveDistance = 2048.0f; // 이미지크기만큼
	static float accumulatedDistance = 0.0f; // 누적이동거리 저장


	accumulatedDistance += moveSpeed * fTimeDelta;

	if (accumulatedDistance >= moveDistance)
	{
		accumulatedDistance = 0.0f;
	}

	// 새로운 위치 계산
	m_BackGround3_INFO.vPos.x = 0.f - accumulatedDistance;
	m_BackGround3_INFO.vPos.y = 0.f;


	m_Back3_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_BackGround3_INFO.vPos.x, m_BackGround3_INFO.vPos.y, 0.f));
}

void CBackGround_3::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}



HRESULT CBackGround_3::Render()
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
	if (FAILED(m_Back3_pTransformCom->Bind_Resource()))
		return E_FAIL;
	if (FAILED(m_Back3_pTextureCom->Bind_Resource(0)))
		return E_FAIL;
	if (FAILED(m_Back3_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_Back3_pVIBufferCom->Render()))
		return E_FAIL;

	// 두 번째 이미지 렌더링 (첫 번째 이미지의 오른쪽에 했음!)
	m_Back3_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_BackGround3_INFO.vPos.x + 2048.f, m_BackGround3_INFO.vPos.y, 0.f));
	if (FAILED(m_Back3_pTransformCom->Bind_Resource()))
		return E_FAIL;
	if (FAILED(m_Back3_pTextureCom->Bind_Resource(0)))
		return E_FAIL;
	if (FAILED(m_Back3_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_Back3_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &matOldProj);

	return S_OK;
}


HRESULT CBackGround_3::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround_3"),
		TEXT("Com_Texture_Back_2"), reinterpret_cast<CComponent**>(&m_Back3_pTextureCom))))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect_BackGround_3"),
		TEXT("Com_VIBuffer_Back_2"), reinterpret_cast<CComponent**>(&m_Back3_pVIBufferCom))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC tDesc{ 10.f,D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Transform_BackGround_3"),
		TEXT("Com_Transform_Back_2"), reinterpret_cast<CComponent**>(&m_Back3_pTransformCom), &tDesc)))
		return E_FAIL;



	return S_OK;
}

CBackGround_3* CBackGround_3::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBackGround_3* pInstance = new CBackGround_3(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("세번째백그라운드 원본 생성 실패 ");
		Safe_Release(pInstance);
	}


	return pInstance;
}

CGameObject* CBackGround_3::Clone(void* pArg)
{
	CBackGround_3* pInstace = new CBackGround_3(*this);

	if (FAILED(pInstace->Initialize(pArg)))
	{
		MSG_BOX("세번째백그라운드 복제 실패");
		Safe_Release(pInstace);
	}

	return pInstace;
}

void CBackGround_3::Free()
{
	__super::Free();

	Safe_Release(m_Back3_pTextureCom);
	Safe_Release(m_Back3_pTransformCom);
	Safe_Release(m_Back3_pVIBufferCom);
}
