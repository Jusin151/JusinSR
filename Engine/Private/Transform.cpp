#include "Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{
}

CTransform::CTransform(const CTransform& Prototype)
	: CComponent(Prototype)
	, m_WorldMatrix{ Prototype.m_WorldMatrix }
{
}

HRESULT CTransform::Initialize_Prototype()
{
	D3DXMatrixIdentity(&m_WorldMatrix);

	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{

	if (nullptr != pArg)
	{
		TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(pArg);
		m_fSpeedPerSec = pDesc->fSpeedPerSec;
		m_fRotationPerSec = pDesc->fRotationPerSec;
	}

	return S_OK;
}

HRESULT CTransform::Bind_Resource()
{
	return m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);
}

_float3 CTransform::Compute_Scaled() const
{
	_float3		vRight = Get_State(STATE_RIGHT);
	_float3		vUp = Get_State(STATE_UP);
	_float3		vLook = Get_State(STATE_LOOK);

	return _float3(D3DXVec3Length(&vRight),
		D3DXVec3Length(&vUp),
		D3DXVec3Length(&vLook));
}

void CTransform::Scaling()
{
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vRight = Get_State(STATE_RIGHT);

	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vRight = Get_State(STATE_RIGHT);

	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Up(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vUp = Get_State(STATE_UP);
	vPosition += *D3DXVec3Normalize(&vUp, &vUp) * m_fSpeedPerSec * fTimeDelta;
	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Down(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vUp = Get_State(STATE_UP);
	vPosition -= *D3DXVec3Normalize(&vUp, &vUp) * m_fSpeedPerSec * fTimeDelta;
	Set_State(STATE_POSITION, vPosition);
}

void CTransform::LookAt(const _float3& vTargetPos)
{
	_float3		vScaled = Compute_Scaled(); //스케일값 저장해놓고

	_float3		vPosition = Get_State(STATE_POSITION); // 위치벡터 저장해놓고

	_float3		vLook = vTargetPos - vPosition; // 방향벡터만들어서 룩벡터에 싸악만들고

	_float3		vRight = {};

	_float3		vUpDir{ 0.f, 1.f, 0.f };	// 임의의 업벡터
	D3DXVec3Cross(&vRight, &vUpDir, &vLook); // 라이트 구하고

	_float3		vUp = {};
	D3DXVec3Cross(&vUp, &vLook, &vRight); // 룩이랑 라이트로 외적 때려서 Up 구하고

	Set_State(STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScaled.x);
	Set_State(STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScaled.y);
	Set_State(STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScaled.z);
}

void CTransform::Chase(const _float3& vTargetPos, _float fTimeDelta, _float fMinDistance)
{
	_float3		vPosition = Get_State(STATE_POSITION); // 스케일값 소실할 수도 있으니 저장

	_float3		vMoveDir = vTargetPos - vPosition; // 방햑벡터 만들고

	if (fMinDistance <= D3DXVec3Length(&vMoveDir)) // 이제 대충 따라오게 하는거 뭔말인지 알지
	{
		vPosition += *D3DXVec3Normalize(&vMoveDir, &vMoveDir) * m_fSpeedPerSec * fTimeDelta;
	}

	Set_State(STATE_POSITION, vPosition);
}


void CTransform::Turn(const _float3& vAxis, _float fTimeDelta)
{
	_float3			vRight = Get_State(STATE_RIGHT); // 라이트 저장
	_float3			vUp = Get_State(STATE_UP); // 업 저장
	_float3			vLook = Get_State(STATE_LOOK); // 룩 저장

	_float4x4		RotationMatrix; // 회전 행렬 싸악 만들어주고


	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_fRotationPerSec * fTimeDelta);

	// D3DXVec4Transform();
	// D3DXVec3TransformCoord();
	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Rotation(const _float3& vAxis, _float fRadian)
{
	_float3		vScaled = Compute_Scaled();

	_float3			vRight = _float3(1.f, 0.f, 0.f) * vScaled.x;
	_float3			vUp = _float3(0.f, 1.f, 0.f) * vScaled.y;
	_float3			vLook = _float3(0.f, 0.f, 1.f) * vScaled.z;

	_float4x4		RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	// D3DXVec4Transform();
	// D3DXVec3TransformCoord();
	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Set_Scale(_float x, _float y, _float z)
{
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * x);
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * y);
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * z);
}

CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform* pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();


}
