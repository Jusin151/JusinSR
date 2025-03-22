#include "Shop.h"
#include <Collider_Sphere.h>
#include <GameInstance.h>
CShop::CShop(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

CShop::CShop(const CShop& Prototype)
    : CGameObject(Prototype)
{
}

HRESULT CShop::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CShop::Initialize(void* pArg) // �ڽĿ��� ������ __Super:: �� �θ���
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    // ���� �⺻ ����
    m_bIsOpen = false;

    return S_OK;
}

void CShop::Priority_Update(_float fTimeDelta)
{
}

void CShop::Update(_float fTimeDelta)
{
}

void CShop::Late_Update(_float fTimeDelta)
{

}

HRESULT CShop::Render()
{

    return S_OK;
}

HRESULT CShop::On_Collision()
{
    if (nullptr == m_pColliderCom)
        return E_FAIL;

    // �ȹٲ�� �浹 ���Ͼ
    if (m_pColliderCom->Get_Other_Type() == CG_END)
        return S_OK;

    switch (m_pColliderCom->Get_Other_Type())
    {
    case CG_PLAYER:
        // ��ȣ �ۿ��� ���ȴٸ�
        // m_bIsOpen = true;
        break;
    default:
        break;
    }

    m_pColliderCom->Set_Other_Type(CG_END);

    return S_OK;
}

HRESULT CShop::SetUp_RenderState()
{
    if (nullptr == m_pGraphic_Device)
        return E_FAIL;

    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    // ���� ���� Ȱ��ȭ
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    // Z ���� ����
    m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

    return S_OK;
}

HRESULT CShop::Release_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}

HRESULT CShop::Ready_Components()
{
    /* Transform Component */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
        return E_FAIL;

    /* VIBuffer Component */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    ///* Texture Component */
    //if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Shop"),
    //    TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
    //    return E_FAIL;

    /* Collider Component */
    CCollider::COL_DESC	ColliderDesc = {};
    ColliderDesc.pOwner = this;
    // �̰ɷ� �ݶ��̴� ũ�� ����
    ColliderDesc.fScale = m_pTransformCom->Compute_Scaled();
    ColliderDesc.eType = CG_SHOP;
    // ������Ʈ�� ������� �Ÿ� ����
    ColliderDesc.fLocalPos = { 0.f, 0.f, 0.0f };


    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &ColliderDesc)))
        return E_FAIL;

    return S_OK;
}

void CShop::Free()
{
    __super::Free();

    // ���� ������ ����

    // ������Ʈ ����
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pColliderCom);
}
