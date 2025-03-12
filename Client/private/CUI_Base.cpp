#include "CUI_Base.h"

#include "GameInstance.h"


CUI_Base::CUI_Base(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{pGraphic_Device}
{
}

CUI_Base::CUI_Base(const CUI_Base& Prototype)
 : CGameObject(Prototype),
	m_pTextureCom(Prototype.m_pTextureCom),
	m_pTransformCom(Prototype.m_pTransformCom),
	m_pVIBufferCom(Prototype.m_pVIBufferCom)
{
}

void CUI_Base::Update(_float fTimeDelta)
{

}

void CUI_Base::Free()
{
	__super::Free();
}

HRESULT CUI_Base::Ready_Components()
{
	return S_OK;
}

