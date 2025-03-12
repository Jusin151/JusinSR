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

// ������ �޴� ȭ��
class CExit_Button final : public CUI_Base
{
private:
	CExit_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
	CExit_Button(const CExit_Button& Prototype);
	virtual ~CExit_Button() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	CTexture* m_ExitButton_pTextureCom{};
	CTexture* m_StartButton_pTextureCom_Second{}; // �̹��� ��ü��
	CTransform* m_ExitButton_pTransformCom{};
	CVIBuffer_Rect* m_ExitButton_pVIBufferCom{};  // UI�� �簢���̹Ƿ� Rect ���� ���
	UI_Child_Desc m_ExitButton_INFO{};

private:
	HRESULT Ready_Components();


public:
	static CExit_Button* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

};


END
