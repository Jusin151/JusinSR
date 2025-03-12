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



class CUI_Dialogue final : public CUI_Base
{
private:
	CUI_Dialogue(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI_Dialogue(const CUI_Dialogue& Prototype);
	virtual ~CUI_Dialogue() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();


public:
	static CUI_Dialogue* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

    void Show_Dialogue(const wstring& strText) // ��� ǥ��
    {
		m_strText = strText;
    }
private:
	wstring m_strText{};  // ���
};



END
