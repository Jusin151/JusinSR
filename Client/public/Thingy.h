#pragma once

#include "Monster_Base.h"

class CThingy : public CMonster_Base
{
public:
	
private:
	CThingy(LPDIRECT3DDEVICE9 pGraphic_Device);
	CThingy(const CThingy& Prototype);
	virtual ~CThingy() = default;

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


	void Shooting(_float fTimeDelta);
	void Melee_Attack(_float fTimeDelta);

	// �� ���¿� �°� �������� ������
	void Select_Frame(_float fTimeDelta);

private:
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	// �ؽ�ó �߰� 
	HRESULT Ready_Components();


private:
	_bool	m_bMorph = { false };

public:
	static CThingy* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

