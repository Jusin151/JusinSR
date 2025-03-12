#include "BackGround_2.h"
#include "GameInstance.h"
#include "CUI_Manager.h"

CBackGround_2::CBackGround_2(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI_Base(pGraphic_Device)
{
}

CBackGround_2::CBackGround_2(const CBackGround_2& Prototype)
	: CUI_Base(Prototype),
	m_Back2_pTextureCom(Prototype.m_Back2_pTextureCom),
	m_Back2_pTransformCom(Prototype.m_Back2_pTransformCom),
	m_Back2_pVIBufferCom(Prototype.m_Back2_pVIBufferCom),
	m_BackGround2_INFO{ Prototype.m_BackGround2_INFO }
{
}

HRESULT CBackGround_2::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

HRESULT CBackGround_2::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_BackGround2_INFO.vSize = { 3000.f,643.f };
	m_BackGround2_INFO.fAlpha = 1.0f;
	m_BackGround2_INFO.vPos = { -300.f,-280.f };


		Set_Position(m_BackGround2_INFO.vPos);
		Set_Size(m_BackGround2_INFO.vSize);
		CUI_Manager::GetInstance()->AddUI(L"LOGO_2", this);


	m_Back2_pTransformCom->Set_Scale(m_BackGround2_INFO.vSize.x, m_BackGround2_INFO.vSize.y, 1.f);
	m_Back2_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_BackGround2_INFO.vPos.x, m_BackGround2_INFO.vPos.y, 0.f));
	return S_OK;
}

void CBackGround_2::Priority_Update(_float fTimeDelta)
{
}
void CBackGround_2::Update(_float fTimeDelta)
{
	const float moveSpeed = 100.0f; // 초당 100 단위 이동
	const float moveDistance = 2574.0f; // 이미지의 너비
	static float accumulatedDistance = 0.0f; // 누적 이동 


	accumulatedDistance += moveSpeed * fTimeDelta;


	if (accumulatedDistance >= moveDistance)
	{
		accumulatedDistance = 0.0f;
	}


	m_BackGround2_INFO.vPos.x = -300.f + accumulatedDistance;
	m_BackGround2_INFO.vPos.y = -280.f;

	// 위치 설정!!
	m_Back2_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_BackGround2_INFO.vPos.x, m_BackGround2_INFO.vPos.y, 0.f));
}


void CBackGround_2::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}




HRESULT CBackGround_2::Render()
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
	if (FAILED(m_Back2_pTransformCom->Bind_Resource()))
		return E_FAIL;
	if (FAILED(m_Back2_pTextureCom->Bind_Resource(0)))
		return E_FAIL;
	if (FAILED(m_Back2_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_Back2_pVIBufferCom->Render()))
		return E_FAIL;

	// 두 번째 이미지 렌더링 (첫 번째 이미지의 오른쪽에 할꺼임!)
	m_Back2_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_BackGround2_INFO.vPos.x - 2574.f, m_BackGround2_INFO.vPos.y, 0.f));
	if (FAILED(m_Back2_pTransformCom->Bind_Resource()))
		return E_FAIL;
	if (FAILED(m_Back2_pTextureCom->Bind_Resource(0)))
		return E_FAIL;
	if (FAILED(m_Back2_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_Back2_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &matOldProj);

	return S_OK;
}



HRESULT CBackGround_2::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround_2"),
		TEXT("Com_Texture_Back_2"), reinterpret_cast<CComponent**>(&m_Back2_pTextureCom))))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect_BackGround_2"),
		TEXT("Com_VIBuffer_Back_2"), reinterpret_cast<CComponent**>(&m_Back2_pVIBufferCom))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC tDesc{ 10.f,D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Transform_BackGround_2"),
		TEXT("Com_Transform_Back_2"), reinterpret_cast<CComponent**>(&m_Back2_pTransformCom), &tDesc)))
		return E_FAIL;



	return S_OK;
}

CBackGround_2* CBackGround_2::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBackGround_2* pInstance = new CBackGround_2(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("두번째백그라운드 원본 생성 실패 ");
		Safe_Release(pInstance);
	}


	return pInstance;
}

CGameObject* CBackGround_2::Clone(void* pArg)
{
	CBackGround_2* pInstace = new CBackGround_2(*this);

	if (FAILED(pInstace->Initialize(pArg)))
	{
		MSG_BOX("두번째백그라운드 복제 실패");
		Safe_Release(pInstace);
	}

	return pInstace;
}

void CBackGround_2::Free()
{
	__super::Free();

	Safe_Release(m_Back2_pTextureCom);
	Safe_Release(m_Back2_pTransformCom);
	Safe_Release(m_Back2_pVIBufferCom);
}
