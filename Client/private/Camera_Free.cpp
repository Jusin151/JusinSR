#include "Camera_Free.h"

CCamera_Free::CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera { pGraphic_Device }
{
}

CCamera_Free::CCamera_Free(const CCamera_Free& Prototype)
	: CCamera{ Prototype }
{
}

HRESULT CCamera_Free::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	CAMERA_FREE_DESC	Desc{};

	Desc.vEye = _float3(0.f, 10.f, -10.f);
	Desc.vAt = _float3(0.f, 0.f, 0.f);
	Desc.fFov = D3DXToRadian(60.f);
	Desc.fNear = 0.1f;
	Desc.fFar = 300.f;
	Desc.fMouseSensor = 1.f;

	m_fMouseSensor = 0.1f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	//SetCursorPos(g_iWinSizeX / 2, g_iWinSizeY / 2);
	GetCursorPos(&m_ptOldMousePos);
	ScreenToClient(g_hWnd, &m_ptOldMousePos);

	return S_OK;
}

void CCamera_Free::Priority_Update(_float fTimeDelta)
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	/* 카메라의 움직임 제어 */
	if (GetKeyState('W') & 0x8000)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	if (GetKeyState('S') & 0x8000)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
	if (GetKeyState('A') & 0x8000)
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	if (GetKeyState('D') & 0x8000)
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	_int		iMouseMove = {};

	if (iMouseMove = ptMouse.x - m_ptOldMousePos.x)
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * iMouseMove * m_fMouseSensor);
	}

	if (iMouseMove = ptMouse.y - m_ptOldMousePos.y)
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * iMouseMove * m_fMouseSensor);
	}
	

	


	m_ptOldMousePos = ptMouse;

	__super::Update_VP_Matrices();
}

void CCamera_Free::Update(_float fTimeDelta)
{



}

void CCamera_Free::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

HRESULT CCamera_Free::Ready_Components()
{
	CTransform::TRANSFORM_DESC		TransformDesc{};

	TransformDesc.fSpeedPerSec = 30.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(180.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CCamera_Free* CCamera_Free::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Free* pInstance = new CCamera_Free(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();


}
