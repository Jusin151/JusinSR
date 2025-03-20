#pragma once

#include "Monster_Base.h"

class CCrocman : public CMonster_Base
{
private:
	CCrocman(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCrocman(const CCrocman& Prototype);
	virtual ~CCrocman() = default;

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
	// ���� �Ÿ� ���� �Ѿư��ų�, ���� �Ÿ� �̳��� �ݶ��̴� �����ؼ� �����ϵ���
	virtual void Select_Pattern(_float fTimeDelta) override;

	void Chasing(_float fTimeDelta);
	void Attack_Melee(_float fTimeDelta);

	// �� ���¿� �°� �������� ������
	void Select_Frame(_float fTimeDelta);

private:
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	// �ؽ�ó �߰� 
	HRESULT Ready_Components();


private:
	// ���� �� ���� �ݶ��̴�
	CCollider_Cube* m_pAttackCollider = { nullptr };

public:
	static CCrocman* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

