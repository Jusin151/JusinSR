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


class CUI_AmmoDisplay final : public CUI_Base
{
private:
	CUI_AmmoDisplay(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI_AmmoDisplay(const CUI_AmmoDisplay& Prototype);
	virtual ~CUI_AmmoDisplay() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	CTexture* m_Ammo_pTextureCom{};
	CTransform* m_Ammo_pTransformCom{};
	CVIBuffer_Rect* m_Ammo_pVIBufferCom{};  // UI�� �簢���̹Ƿ� Rect ���� ���
	UI_Child_Desc m_Ammo_INFO{};

private:
	HRESULT Ready_Components();


public:
	static CUI_AmmoDisplay* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	_float m_fArmor{};  // ü��
};


END
