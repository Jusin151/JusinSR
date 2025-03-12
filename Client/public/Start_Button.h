#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "CUI_Base.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;  // UI�� �簢������
END

BEGIN(Client)

//  ���� ��ư
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
	CTexture* m_StartButton_pTextureCom_Second{}; // �̹��� ��ü��
	CTransform* m_StartButton_pTransformCom{};
	CVIBuffer_Rect* m_StartButton_pVIBufferCom{};  // UI�� �簢���̹Ƿ� Rect ���� ���
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
