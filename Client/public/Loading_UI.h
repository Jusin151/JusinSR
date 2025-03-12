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
	CVIBuffer_Rect* m_Loading_pVIBufferCom{};  // UI�� �簢���̹Ƿ� Rect ���� ���
	UI_Child_Desc m_Loading_INFO{};

private:
	HRESULT Ready_Components();


public:
	static CLoading_UI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

};


END
