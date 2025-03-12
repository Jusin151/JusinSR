#include "CUI_Menu.h"

CUI_Menu::CUI_Menu(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI_Base(pGraphic_Device)
{
}

CUI_Menu::CUI_Menu(const CUI_Menu& Prototype)
	: CUI_Base(Prototype)
{
}

HRESULT CUI_Menu::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CUI_Menu::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void CUI_Menu::Priority_Update(_float fTimeDelta)
{
}

void CUI_Menu::Update(_float fTimeDelta)
{
}

void CUI_Menu::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_Menu::Render()
{
	return E_NOTIMPL;
}

HRESULT CUI_Menu::Ready_Components()
{
	return E_NOTIMPL;
}

CUI_Menu* CUI_Menu::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	return nullptr;
}

CGameObject* CUI_Menu::Clone(void* pArg)
{
	return nullptr;
}

void CUI_Menu::Free()
{
}

void CUI_Menu::OpenMenu()
{
}

void CUI_Menu::CloseMenu()
{
}
