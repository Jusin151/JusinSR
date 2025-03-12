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

// 맨 뒤의 전기
class CBackGround_3 final : public CUI_Base
{
private:
	CBackGround_3(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBackGround_3(const CBackGround_3& Prototype);
	virtual ~CBackGround_3() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	CTexture* m_Back3_pTextureCom{};
	CTransform* m_Back3_pTransformCom{};
	CVIBuffer_Rect* m_Back3_pVIBufferCom{};  // UI는 사각형이므로 Rect 버퍼 사용
	UI_Child_Desc m_BackGround3_INFO{};

private:
	HRESULT Ready_Components();


public:
	static CBackGround_3* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

};


END
