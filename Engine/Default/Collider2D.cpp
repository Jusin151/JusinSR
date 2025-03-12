#include "Collider2D.h"


CCollider2D::CCollider2D(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CComponent { pGraphic_Device }
{
}

CCollider2D::CCollider2D(const CCollider2D& Prototype) :
	CComponent{ Prototype }
{

}

HRESULT CCollider2D::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollider2D::Initialize(void* pArg)
{
	return S_OK;
}

void CCollider2D::Priority_Update(_float fTimeDelta)
{
}

void CCollider2D::Update(_float fTimeDelta)
{
}

void CCollider2D::Late_Update(_float fTimeDelta)
{
}

HRESULT CCollider2D::Render()
{
	return S_OK;
}

void CCollider2D::Free()
{
	__super::Free();
}
