#pragma once

#include "Monster_Base.h"

class CHellhound : public CMonster_Base
{
public:

private:
	CHellhound(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHellhound(const CHellhound& Prototype);
	virtual ~CHellhound() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	// �������� �÷��̾� �������� �ٶ󺸵��� ��
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	virtual HRESULT On_Collision(CCollisionObject* other) override;
	// ���� �Ÿ� ���� �Ѿư��ų�, ���� �Ÿ� �̳��� �ݶ��̴� �����ؼ� �����ϵ���
	virtual void Select_Pattern(_float fTimeDelta) override;

	// hp Ȯ���ϸ鼭 HPSTATE ����
	void Check_Hp();

	void Melee_Attack(_float fTimeDelta);

	// �� ���¿� �°� �������� ������
	void Select_Frame(_float fTimeDelta);

private:
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	// �ؽ�ó �߰� 
	HRESULT Ready_Components();


private:
	
	_uint m_iMaxHp = { 0 };
	_bool m_bDamaged = { false };

public:
	static CHellhound* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

