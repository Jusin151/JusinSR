#pragma once

#include "Monster_Base.h"

class CHarpoonguy : public CMonster_Base
{
private:
	CHarpoonguy(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHarpoonguy(const CHarpoonguy& Prototype);
	virtual ~CHarpoonguy() = default;

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
	virtual HRESULT On_Collision(_float fTimeDelta) override;
	// ���� �Ÿ� ���� �Ѿư��ų�, ���� �Ÿ� �̳��� �ݶ��̴� �����ؼ� �����ϵ���
	virtual void Select_Pattern(_float fTimeDelta) override;

	void Chasing(_float fTimeDelta);
	void Shooting(_float fTimeDelta);

	// �� ���¿� �°� �������� ������
	void Select_Frame(_float fTimeDelta);

private:
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	// �ؽ�ó �߰� 
	HRESULT Ready_Components();


private:

public:
	static CHarpoonguy* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

