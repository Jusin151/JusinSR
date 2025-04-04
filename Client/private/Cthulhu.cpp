#include "Cthulhu.h"
#include <Player.h>


CCthulhu::CCthulhu(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster_Base(pGraphic_Device), m_pBehaviorTree(nullptr)
{
}

CCthulhu::CCthulhu(const CCthulhu& Prototype)
	: CMonster_Base(Prototype), m_pBehaviorTree(nullptr)
{
}


HRESULT CCthulhu::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCthulhu::Initialize(void* pArg)
{

	if (FAILED(Ready_Components()))
		return E_FAIL;

	// �����̺�� Ʈ�� �ʱ�ȭ
	m_pBehaviorTree = new CBehaviorTree();
	if (!m_pBehaviorTree)
		return E_FAIL;

	m_pBehaviorTree->Initialize();
	return S_OK;
}
HRESULT CCthulhu::Ready_Components()
{
	// ������Ʈ �ʱ�ȭ �ڵ�
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Cthulhu"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), (CComponent**)&m_pColliderCom)))
		return E_FAIL;
	return S_OK;
}
HRESULT CCthulhu::On_Collision(CCollisionObject* other)
{
	return S_OK;
}


void CCthulhu::Select_Pattern(_float fTimeDelta)
{// ���� ���� ���� �߰�
	// ���� ���, �����̺�� Ʈ������ Ư�� ������ �����ϴ� ������ �ۼ��� �� �ֽ��ϴ�.
}
void CCthulhu::Priority_Update(_float fTimeDelta)
{
	Billboarding(fTimeDelta);
	// �����̺�� Ʈ�� ����
	if (m_pBehaviorTree)
		m_pBehaviorTree->Run();
}

void CCthulhu::Update(_float fTimeDelta)
{

	Select_Pattern(fTimeDelta);
}

void CCthulhu::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}
HRESULT CCthulhu::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	return S_OK;
}

HRESULT CCthulhu::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	return S_OK;
}

HRESULT CCthulhu::Render()
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

	if (g_bDebugCollider)
	{
		m_pColliderCom->Render();
	}

	Release_RenderState();

	return S_OK;
}


void CCthulhu::Billboarding(_float fTimeDelta)
{
	if (!m_pTarget) return;


	CTransform* pPlayerTransform = static_cast<CPlayer*>(m_pTarget)->Get_TransForm();

	_float3 vPlayerLook = pPlayerTransform->Get_State(CTransform::STATE_LOOK);

	vPlayerLook.y = 0.f;
	vPlayerLook.Normalize();

	_float3 vShopLook = -vPlayerLook;


	_float3 vUp = _float3(0.0f, 1.0f, 0.0f);
	_float3 vRight = vUp.Cross(vShopLook);
	vRight.Normalize();

	_float3 vNewUp = vShopLook.Cross(vRight);
	vNewUp.Normalize();

	_float3 vScale = m_pTransformCom->Compute_Scaled();

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vNewUp * vScale.y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vShopLook * vScale.z);
}

CCthulhu* CCthulhu::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCthulhu* pInstance = new CCthulhu(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to create CCthulhu instance");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CCthulhu::Clone(void* pArg)
{
	CCthulhu* pClone = new CCthulhu(*this);
	if (FAILED(pClone->Initialize(pArg)))
	{
		MSG_BOX("Failed to clone CCthulhu instance");
		Safe_Release(pClone);
	}
	return pClone;
}

void CCthulhu::Free()
{
	__super::Free();
	// �����̺�� Ʈ�� ����
	Safe_Delete(m_pBehaviorTree);
}


