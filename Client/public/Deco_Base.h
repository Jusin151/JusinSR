#pragma once
class Deco_Base
{
};

#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "CollisionObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CMaterial;
class CTransform;
class CVIBuffer_Rect;
class CCollider_Sphere;
class CCollider_Cube;
END

BEGIN(Client)

class CDeco_Base abstract : public CCollisionObject
{
public:
	enum DECOSTATE { DS_IDLE , DS_HIT , DS_DEATH , DS_END  };

protected:
	CDeco_Base(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDeco_Base(const CDeco_Base& Prototype);
	virtual ~CDeco_Base() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
private:
	// texturecom�� �� monster���� �߰�
	HRESULT Ready_Components();

public:
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
public:
public:
	virtual HRESULT Render()override;
	virtual HRESULT On_Collision(CCollisionObject* other) { return S_OK; }
public:
	virtual void Select_State() PURE;
	void Look_Player();

	virtual json Serialize() override;

	CGameObject* Get_Target() { return m_pTarget; }

protected:

	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	// �ڽĿ��� �߰�
	CTexture* m_pTextureCom = { nullptr };
	CCollider_Cube* m_pColliderCom = { nullptr };

	// ���߿� �߰�
	CShader* m_pShaderCom = { nullptr };
	CMaterial* m_pMaterialCom = { nullptr };

protected:

	CGameObject* m_pTarget = { nullptr };

	// �ִϸ��̼� ������ ����
	_float m_fElapsedTime = { 0.f };
	_int m_iCurrentFrame = { 0 };

	// �̰ɷ� ���� ���� �����ϱ�
	_int m_iHitCount = { 0 };

	DECOSTATE m_eCurState = { DS_IDLE };
	
	_bool		m_bInit = { false };

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();
};


END