#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CLight;
class CShader;
class CTexture;
class CMaterial;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CLamp final : public CGameObject
{
private:
	CLamp(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLamp(const CLamp& Prototype);
	virtual ~CLamp() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
private:
	HRESULT Ready_Components();
public:
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
public:
	virtual HRESULT Pre_Render();
	virtual HRESULT Render() override;
	virtual HRESULT Post_Render();
public:
	virtual void Reset() {}; // ���Ŀ� ������Ʈ Ǯ���� SetActive�� true�� �� �� �⺻���� �ٽ� ������ ���� �Լ�

private:
	CLight* m_pLightCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CMaterial* m_pMaterialCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

public:
	static CLamp* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CLamp* Clone(void* pArg) override;
	virtual void Free() override;
public:
	virtual json Serialize() override;
	virtual void Deserialize(const json& j) override;
};
END