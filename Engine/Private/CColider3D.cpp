#include "CColider3D.h"


CCollider3D::CCollider3D(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent { pGraphic_Device }
{
}

CCollider3D::CCollider3D(const CCollider3D& Prototype)
	: CComponent{ Prototype }
{
}

void CCollider3D::Free()
{
	__super::Free();
}
