#include "Staff_Bullet.h"
#include "Monster_Base.h"
#include "VIBuffer_Rect.h"
#include "Texture.h"
#include "Collider_Cube.h"
#include "Transform.h"
#include "Player.h"
#include "GameInstance.h"

CStaff_Bullet::CStaff_Bullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Base(pGraphic_Device)
{
}

CStaff_Bullet::CStaff_Bullet(const CStaff_Bullet& Prototype)
	:CMonster_Base(Prototype)
{
}

HRESULT CStaff_Bullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStaff_Bullet::Initialize(void* pArg)
{
	
	if (FAILED(Ready_Components()))
		return E_FAIL;



	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.5f, 0.f));
	m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);

	

	return S_OK;
}

void CStaff_Bullet::Priority_Update(_float fTimeDelta)
{

}

void CStaff_Bullet::Update(_float fTimeDelta)
{
	// źȯ�� ������ �������� ��ġ ������Ʈ
	m_pTransformCom->Go_Straight(fTimeDelta * m_fSpeed);

	// �ִϸ��̼� ������ ������Ʈ
	m_fElapsedTime += fTimeDelta;
	if (m_fElapsedTime >= 0.1f) // �����Ӵ� 0.1��
	{
		m_fElapsedTime = 0.0f;
		m_iCurrentFrame = (m_iCurrentFrame + 1) % 7; // 0���� 6���� �ݺ�
	}
}


void CStaff_Bullet::Late_Update(_float fTimeDelta)
{
	
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}


HRESULT CStaff_Bullet::Render()
{
	if (FAILED(m_pTextureCom->Bind_Resource(m_iCurrentFrame)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	SetUp_RenderState();

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	Release_RenderState();

	return S_OK;
}

HRESULT CStaff_Bullet::On_Collision(_float fTimeDelta)
{
	if (nullptr == m_pColliderCom)
		return E_FAIL;

	// �ȹٲ�� �浹 ���Ͼ
	if (m_pColliderCom->Get_Other_Type() == CG_END)
		return S_OK;

	_float3 fMTV = m_pColliderCom->Get_MTV();
	_float3 fPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 temp = { 1.f, 0.f, 1.f };

	switch (m_pColliderCom->Get_Other_Type())
	{
	case CG_PLAYER:

		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);
		//m_pTransformCom->Go_Backward(fTimeDelta);
		break;

	case CG_WEAPON:



		temp += fPos;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, temp);

		break;
	default:
		break;
	}

	// �浹 ó�� �ϰ� �ٽ� type�� ����
	m_pColliderCom->Set_Other_Type(CG_END);

	return S_OK;
}

void CStaff_Bullet::Select_Pattern(_float fTimeDelta)
{
	
}

void CStaff_Bullet::Attack_Melee()
{
	m_pAttackCollider->Update_Collider(TEXT("Com_Transform"));

	m_pGameInstance->Add_Collider(CG_MONSTER_PROJECTILE, m_pAttackCollider);
}

HRESULT CStaff_Bullet::SetUp_RenderState()
{
	// �ϴ� �߰��غ���

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // ���� ���� ���غ��� ũ�� �ȼ� ������
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200); // ���ذ� ���� (0~255)

	return S_OK;
}

HRESULT CStaff_Bullet::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	return S_OK;
}

HRESULT CStaff_Bullet::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Weapon_Bullet"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC tDesc{ 10.f,D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &tDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBufferm"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	_float3 fPos = { 10.f, 0.5f, 10.f };

	/* For.Com_Collider */
	CCollider_Cube::COL_CUBE_DESC	ColliderDesc = {};
	ColliderDesc.eType = CG_MONSTER_PROJECTILE;
	ColliderDesc.pOwner = this;
	// �̰ɷ� �ݶ��̴� ũ�� ����
	ColliderDesc.fScale = { 1.f, 1.f, 1.f };
	// ������Ʈ�� ������� �Ÿ� ����
	ColliderDesc.fLocalPos = { 0.f, 0.5f, 0.f };


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider_Attack"), reinterpret_cast<CComponent**>(&m_pAttackCollider), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CStaff_Bullet* CStaff_Bullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CStaff_Bullet* pInstance = new CStaff_Bullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CStaff_Bullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStaff_Bullet::Clone(void* pArg)
{
	CStaff_Bullet* pInstance = new CStaff_Bullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CStaff_Bullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStaff_Bullet::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pAttackCollider);
}
