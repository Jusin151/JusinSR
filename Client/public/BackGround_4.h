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


class CBackGround_4 final : public CUI_Base
{
private:
	CBackGround_4(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBackGround_4(const CBackGround_4& Prototype);
	virtual ~CBackGround_4() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	CTexture* m_Back4_pTextureCom{};
	CTransform* m_Back4_pTransformCom{};
	CVIBuffer_Rect* m_Back4_pVIBufferCom{};  // UI는 사각형이므로 Rect 버퍼 사용
	UI_Child_Desc m_BackGround4_INFO{};

private:
	HRESULT Ready_Components();


public:
	static CBackGround_4* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

};


END
