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

// 오른쪽 메뉴 화면
class CLoading_UI final : public CUI_Base
{
private:
	CLoading_UI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLoading_UI(const CLoading_UI& Prototype);
	virtual ~CLoading_UI() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	CTexture* m_Loading_pTextureCom{};
	CTransform* m_Loading_pTransformCom{};
	CVIBuffer_Rect* m_Loading_pVIBufferCom{};  // UI는 사각형이므로 Rect 버퍼 사용
	UI_Child_Desc m_Loading_INFO{};

private:
	HRESULT Ready_Components();


public:
	static CLoading_UI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

};


END
