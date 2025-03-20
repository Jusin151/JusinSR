#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CCollider_Sphere;
class CCollider_Cube;
END

class CGameObject_Cube final : public CGameObject
{
private:
	CGameObject_Cube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject_Cube(const CGameObject& Prototype);
	virtual ~CGameObject_Cube() = default;

public:
	/* ������ ȣ���ϴ� �Լ�. �����ɸ� �� �ִ� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize_Prototype() override;
	/* �纻�� ȣ���ϴ� �Լ�. */
	virtual HRESULT Initialize(void* pArg) override;

	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Pre_Render();
	virtual HRESULT Render() override;
	virtual HRESULT Post_Render();
	virtual void Reset() {}; // ���Ŀ� ������Ʈ Ǯ���� SetActive�� true�� �� �� �⺻���� �ٽ� ������ ���� �Լ�

private:
	HRESULT Ready_Components();
	CTexture*		m_pTextureCom = { nullptr };
	CTransform*		m_pTransformCom = { nullptr };
	CVIBuffer_Cube* m_pVIBufferCom = { nullptr };

public:
	static CGameObject_Cube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject_Cube* Clone(void* pArg) override;
	virtual void Free() override;

	
};
