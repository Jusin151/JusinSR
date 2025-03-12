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

// 검은색 괴물
class CBackGround_2 final : public CUI_Base
{
private:
	CBackGround_2(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBackGround_2(const CBackGround_2& Prototype);
	virtual ~CBackGround_2() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	CTexture* m_Back2_pTextureCom{};
	CTransform* m_Back2_pTransformCom{};
	CVIBuffer_Rect* m_Back2_pVIBufferCom{};  // UI는 사각형이므로 Rect 버퍼 사용
	UI_Child_Desc m_BackGround2_INFO{};

private:
	HRESULT Ready_Components();


public:
	static CBackGround_2* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

};


END
