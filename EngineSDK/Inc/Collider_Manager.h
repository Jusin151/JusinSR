#pragma once

#include "Base.h"
#include <queue>

BEGIN(Engine)

class CCollider;
class CCollider_Handler;

class  CCollider_Manager final : public CBase
{
public:

	


private:
	CCollider_Manager();
	virtual ~CCollider_Manager() = default;


public:

	// �߰�. gameinstance�� ����ؼ� ������Ʈ���� �߰�. collider���� �ϸ� ������Ʈ�� �����Ǹ� �� �׷���?
	HRESULT Add_Collider(COLLIDERGROUP eGroup, CCollider* Collider);


	// update �Ŀ� ��ϵ� ��� collider�� ����
	void Clear();

	// enum���� ���ϴ� Ÿ�Ը� �浹 ó�� �ϵ���
	void Update_Collison();

	// �Ÿ��θ�
	void Collison_Sphere_To_Sphere( list<CCollider*> src, list<CCollider*> dst);
	// SAT �Ἥ
	void Collison_Cube_To_Cube(list<CCollider*> src, list<CCollider*> dst);

	// ���� �Ÿ��� ������ �պ��� ������ �Ǵ�
	bool Calc_Sphere_To_Sphere(CCollider* src, CCollider* dst);

	// ���� �浹 üũ ������
	bool Calc_Cube_To_Cube(CCollider* src, CCollider* dst);

	// ������ ������.
	bool Calc_Cube_To_Sphere(CCollider* src, CCollider* dst);

private:

	vector<list<CCollider*>> m_pColliders{ static_cast<size_t>(CG_END) };



	//// �� ����Ҷ� �־�δ� �뵵��, key�� dir, value�� depth
	//map<_float3, _float> m_Axes;
	//// �Ÿ� ���� ������� �־��ֱ�, �浹 �� �̵���ų�� ���
	//priority_queue<_float, vector<_float>, greater<_float>> m_Depths;
	

public:
	static CCollider_Manager* Create();
	virtual void Free() override;
};

END
