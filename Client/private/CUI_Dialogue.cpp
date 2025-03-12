#include "CUI_Dialogue.h"

CUI_Dialogue::CUI_Dialogue(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI_Base(pGraphic_Device)
{
}

CUI_Dialogue::CUI_Dialogue(const CUI_Dialogue& Prototype)
	: CUI_Base(Prototype)
{
}

HRESULT CUI_Dialogue::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CUI_Dialogue::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void CUI_Dialogue::Priority_Update(_float fTimeDelta)
{
}

void CUI_Dialogue::Update(_float fTimeDelta)
{
}

void CUI_Dialogue::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_Dialogue::Render()
{
	return E_NOTIMPL;
}

HRESULT CUI_Dialogue::Ready_Components()
{
	return E_NOTIMPL;
}

CUI_Dialogue* CUI_Dialogue::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	return nullptr;
}

CGameObject* CUI_Dialogue::Clone(void* pArg)
{
	return nullptr;
}

void CUI_Dialogue::Free()
{
}
