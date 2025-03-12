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

//  시작 버튼
class CStart_Button final : public CUI_Base
{
private:
	CStart_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
	CStart_Button(const CStart_Button& Prototype);
	virtual ~CStart_Button() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	CTexture* m_StartButton_pTextureCom{};
	CTexture* m_StartButton_pTextureCom_Second{}; // 이미지 교체용
	CTransform* m_StartButton_pTransformCom{};
	CVIBuffer_Rect* m_StartButton_pVIBufferCom{};  // UI는 사각형이므로 Rect 버퍼 사용
	UI_Child_Desc m_StartButton_INFO{};

private:
	HRESULT Ready_Components();
	bool m_bIsMouseOver = {};

public:
	static CStart_Button* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

};


END
