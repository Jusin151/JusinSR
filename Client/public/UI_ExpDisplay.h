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


class CUI_ExpDisplay final : public CUI_Base
{
private:
	CUI_ExpDisplay(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI_ExpDisplay(const CUI_ExpDisplay& Prototype);
	virtual ~CUI_ExpDisplay() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	CTexture* m_EXP_pTextureCom{};
	CTransform* m_EXP_pTransformCom{};
	CVIBuffer_Rect* m_EXP_pVIBufferCom{};  // UI는 사각형이므로 Rect 버퍼 사용
	UI_Child_Desc m_EXP_INFO{};

private:
	HRESULT Ready_Components();


public:
	static CUI_ExpDisplay* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	_float m_fExp{};  // 체력
};


END