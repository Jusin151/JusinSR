#pragma once

#include "Monster_Base.h"
#include "Weapon_Headers.h"

class CStaff_Bullet : public CMonster_Base 
{
public:
	


private:
	CStaff_Bullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CStaff_Bullet(const CStaff_Bullet& Prototype);
	virtual ~CStaff_Bullet() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	// �������� �÷��̾� �������� �ٶ󺸵��� ��
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;


public:
	virtual HRESULT On_Collision(_float fTimeDelta) override;
	// �ϴ� �Ѿư��� ���ϸ�
	virtual void Select_Pattern(_float fTimeDelta) override;

	void Attack_Melee();

private:
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	// �ؽ�ó �߰� 
	HRESULT Ready_Components();


private:
	// ���� �� ���� �ݶ��̴�
	CCollider_Cube* m_pAttackCollider = { nullptr };

public:
	static CStaff_Bullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	virtual void Free();
private:
	CTransform* m_Player_Transform = { nullptr };
	_float3 Player_RIght{};
	_float3 Player_Up{};
	_float3 Player_Look{};
	_float3 Player_Pos{};
private:
	_float m_fSpeed = 10.0f; // źȯ �ӵ�
	_float m_fElapsedTime = 0.0f; // �ִϸ��̼� ������ �ð�
	_int m_iCurrentFrame = 0; // ���� �ִϸ��̼� ������
};

