#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "CUI_Base.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;  // UI는 사각형으로
END

BEGIN(Client)

class CUI_Default_Panel final : public CUI_Base
{
private:
	CUI_Default_Panel(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI_Default_Panel(const CUI_Default_Panel& Prototype);
	virtual ~CUI_Default_Panel() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CTexture* m_Default_pTextureCom{};
	CTransform* m_Default_pTransformCom{};
	CVIBuffer_Rect* m_Default_pVIBufferCom{};  // UI는 사각형이므로 Rect 버퍼 사용
	UI_Parent_Desc m_UI_INFO{};
protected:
	bool m_bIsUI{};
private:
	HRESULT Ready_Components();


public:
	static CUI_Default_Panel* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) ;
	virtual void Free();

};
END