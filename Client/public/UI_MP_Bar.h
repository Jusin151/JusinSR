﻿#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "CUI_Base.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;  // UI는 사각형으로
END

BEGIN(Client)


class CUI_MP_Bar final : public CUI_Base
{
private:
	CUI_MP_Bar(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI_MP_Bar(const CUI_MP_Bar& Prototype);
	virtual ~CUI_MP_Bar() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	void Update_Mp_Bar();
	virtual HRESULT Render()override;

private:
	CTexture* m_MP_pTextureCom{};
	CTransform* m_MP_pTransformCom{};
	CVIBuffer_Rect* m_MP_pVIBufferCom{};  // UI는 사각형이므로 Rect 버퍼 사용
	UI_Child_Desc m_MP_INFO{};

private:
	HRESULT Ready_Components();


public:
	static CUI_MP_Bar* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	_float m_fArmor{};  // 체력
	_float m_fMp{};  // ü��
};


END
