#include "CUI_WeaponIcon.h"

CUI_WeaponIcon::CUI_WeaponIcon(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI_Base(pGraphic_Device)
{
}

CUI_WeaponIcon::CUI_WeaponIcon(const CUI_WeaponIcon& Prototype)
	: CUI_Base(Prototype)
{
}

HRESULT CUI_WeaponIcon::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CUI_WeaponIcon::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void CUI_WeaponIcon::Priority_Update(_float fTimeDelta)
{
}

void CUI_WeaponIcon::Update(_float fTimeDelta)
{
}

void CUI_WeaponIcon::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_WeaponIcon::Render()
{
	return E_NOTIMPL;
}

HRESULT CUI_WeaponIcon::Ready_Components()
{
	return E_NOTIMPL;
}

CUI_WeaponIcon* CUI_WeaponIcon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	return nullptr;
}

CGameObject* CUI_WeaponIcon::Clone(void* pArg)
{
	return nullptr;
}

void CUI_WeaponIcon::Free()
{
}
