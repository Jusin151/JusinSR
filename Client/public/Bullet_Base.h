#pragma once

#include "Monster_Base.h"
#include "Weapon_Headers.h"

class CBullet_Base : public CCollisionObject
{


protected:
	CBullet_Base(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBullet_Base(const CBullet_Base& Prototype);
	virtual ~CBullet_Base() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	// �������� �÷��̾� �������� �ٶ󺸵��� ��
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;
protected:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CCollider_Cube* m_pColliderCom = { nullptr };


public:
	virtual HRESULT On_Collision(CCollisionObject* other)override;

	void Attack_Melee();

private:
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	// �ؽ�ó �߰� 
	HRESULT Ready_Components();


protected:
	// ���� �� ���� �ݶ��̴�
	CCollider_Cube* m_pAttackCollider = { nullptr };

public:
	static CBullet_Base* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	virtual void Free();
protected:
	CTransform* m_Player_Transform = { nullptr };
	_float3 Player_RIght{};
	_float3 Player_Up{};
	_float3 Player_Look{};
	_float3 Player_Pos{};
protected:
	_float m_fSpeed = 10.0f; // źȯ �ӵ�
	_float m_fElapsedTime = 0.0f; // �ִϸ��̼� ������ �ð�
	_int m_iCurrentFrame = 0; // ���� �ִϸ��̼� ������
};

