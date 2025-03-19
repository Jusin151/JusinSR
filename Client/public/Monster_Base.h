#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CCollider_Sphere;
class CCollider_Cube;

END

BEGIN(Client)

class CMonster_Base abstract : public CGameObject
{
public:
	enum MONSTERSTATE { MS_IDLE, MS_WALK, MS_HIT, MS_ATTACK, MS_DEATH, MS_END };

protected:
	CMonster_Base(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMonster_Base(const CMonster_Base& Prototype);
	virtual ~CMonster_Base() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	// �������� �÷��̾� �������� �ٶ󺸵��� ��
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;
	virtual HRESULT On_Collision(_float fTimeDelta) PURE;
	virtual void Select_Pattern(_float fTimeDelta) PURE;
	void Look_Player();

	// texturecom�� �� monster���� �߰�
	HRESULT Ready_Components();

protected:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CCollider_Cube* m_pColliderCom = { nullptr };

	// �ϴ� ���ʹ� �÷��̾ Ÿ������ �ϵ���. �ٸ� �� Ÿ������ �ϴ� ���� ���߿� �ϵ���
	CGameObject*	m_pTarget = { nullptr };

	// �ִϸ��̼� ������ ����.. �� ������ �ð�, ���� ������ ��, �׸��� ���� �� �ؽ��� ������ �ڽĿ��� ����
	_float m_fElapsedTime = { 0.f };
	_int m_iCurrentFrame = {0};

	// ���� ���� ����, ���� ����
	MONSTERSTATE m_eCurState = { MS_IDLE };
	MONSTERSTATE m_ePreState = { MS_IDLE };


public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();
};


END