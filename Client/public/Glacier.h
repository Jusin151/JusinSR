#pragma once

#include "Monster_Base.h"

class CGlacier : public CMonster_Base
{
public:
	enum HPSTATE {HP_MAX, HP_HURT, HP_VERYHURT};

private:
	CGlacier(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGlacier(const CGlacier& Prototype);
	virtual ~CGlacier() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	// �������� �÷��̾� �������� �ٶ󺸵��� ��
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

	virtual void Deserialize(const json& j) override;

public:
	virtual HRESULT On_Collision(CCollisionObject* other) override;
	// ���� �Ÿ� ���� �Ѿư��ų�, ���� �Ÿ� �̳��� �ݶ��̴� �����ؼ� �����ϵ���
	virtual void Select_Pattern(_float fTimeDelta) override;

	// hp Ȯ���ϸ鼭 HPSTATE ����
	void Select_HpState();
	// ���� �������� �÷��̾ ���ʿ� �ִ��� �����ʿ� �ִ��� Ȯ��. 0�̸� look, 0 �ʰ��ϸ� ������, 0 �̸��̸� ����
	_float Check_Direction();

	void Shooting(_float fTimeDelta);

	// �� ���¿� �°� �������� ������
	void Select_Frame(_float fTimeDelta);

private:
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	// �ؽ�ó �߰� 
	HRESULT Ready_Components();


private:
	HPSTATE m_eHpState = { HP_MAX };
	_uint m_iMaxHp = {0};
	_bool m_bRight = { false };

public:
	static CGlacier* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

