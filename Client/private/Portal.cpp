#include "Portal.h"
#include <Collider_Sphere.h>
#include <GameInstance.h>
#include "Player.h"
#include "CollisionObject.h"
#include "Level_Loading.h"

CPortal::CPortal(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject{pGraphic_Device}
{
}

CPortal::CPortal(const CPortal& Prototype)
    : CGameObject (Prototype),
    m_pTextureCom{ Prototype.m_pTextureCom },
     m_pTransformCom(Prototype.m_pTransformCom),
    m_pVIBufferCom(Prototype.m_pVIBufferCom)
{
}

HRESULT CPortal::Initialize_Prototype()
{
   
    return S_OK;
}

HRESULT CPortal::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pPlayer = m_pGameInstance->Find_Object(LEVEL_GAMEPLAY, TEXT("Layer_Player"));

    m_pPlayer = static_cast<CPlayer*>(m_pPlayer);
    if (m_pPlayer == nullptr)
        return E_FAIL;
    else
        Safe_AddRef(m_pPlayer);


    //m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(-4.3f, 0.5f, 8.2f));

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(12.3f, 0.5f, 2.5f));

    m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);

    m_bPortal_On = false;

    return S_OK;
}

void CPortal::Priority_Update(_float fTimeDelta)
{
}

void CPortal::Update(_float fTimeDelta)
{
    LookAtPlayer(fTimeDelta);
    m_pGameInstance->Add_Collider(CG_SHOP, m_pColliderCom);

    m_fFrameTime += fTimeDelta;

    if (m_fFrameTime >= 0.1f) 
    {
        m_fFrameTime = 0.f;
        ++m_iCurrentFrame;
        if (m_iCurrentFrame > 7)
            m_iCurrentFrame = 0;
    }

    if (SUCCEEDED(On_Collision()))
    {
        
      /*
            if (GetKeyState('E') & 0x8000)
            {
                if (FAILED(m_pGameInstance->Change_Level(LEVEL_GAMEPLAY,
                    CLevel_Loading::Create(m_pGraphic_Device, LEVEL_EDITOR))))
                    return;
            }*/

     

    }



}


void CPortal::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
        return;

    m_pGameInstance->Add_RenderGroup(CRenderer::RG_COLLIDER, this);


}


HRESULT CPortal::SetUp_RenderState()
{

    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // ���� ���� ���غ��� ũ�� �ȼ� ������
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200); // ���ذ� ���� (0~255)

    return S_OK;
}

HRESULT CPortal::Release_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    return S_OK;
}


HRESULT CPortal::Render()
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




HRESULT CPortal::On_Collision()
{
    // �÷��̾��� ��ġ�� ������ ��ġ�� ������
    CTransform* pPlayerTransform = static_cast<CTransform*>(m_pPlayer->Get_Component(L"Com_Transform"));
    if (nullptr == pPlayerTransform)
        return E_FAIL;

    _float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
    _float3 vShopPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    /*_float3 asd = vPlayerPos - vShopPos;

    float fDistanceSq = asd.LengthSq();*/

    // �÷��̾�� ���� ���� �Ÿ��� 25 ���� (��, 5 ���� ����)�� ���� �浹�� ����
    const float fThresholdSq = 1.f; // 5^2
    _float tmpDist = _float3::Distance(vPlayerPos, vShopPos);
    if (tmpDist <= fThresholdSq)
    {
        return S_OK;

        /* bPlayer_Check = true;
          m_bIsOpen = true;*/
    }


    //switch (m_pColliderCom->Get_Other_Type())
    //{
    //case CG_PLAYER:
    //    // �÷��̾�� �浹�� ��� ��ȣ �ۿ� ó�� (��: ���� ����)
    //    if (!m_bIsOpen)
    //    {
    //        Open_Shop();
    //        // �ʿ信 ���� m_bIsOpen = true;�� ���� ������ �� ����.
    //    }
    //    break;
    //default:
    //    break;
    //}
    return E_FAIL;
}

void CPortal::LookAtPlayer(_float fTimeDelta)
{
    if (!m_pPlayer)
        return;

    CTransform* pPlayerTransform = static_cast<CPlayer*>(m_pPlayer)->Get_TransForm();

    // �÷��̾��� look ���� �������� (�÷��̾ �ٶ󺸴� ����)
    _float3 vPlayerLook = pPlayerTransform->Get_State(CTransform::STATE_LOOK);

    vPlayerLook.y = 0.0f;
    vPlayerLook.Normalize();

    _float3 vShopLook = -vPlayerLook;  // ���� ���� ����

    // ���� ������ ����Ͽ� Ʈ������ ����
    _float3 vUp = _float3(0.0f, 1.0f, 0.0f);  // ���� �� ����
    _float3 vRight = vUp.Cross(vShopLook);
    vRight.Normalize();

    // ���� ������ �����ϱ� ���� �� ���� ����
    _float3 vNewUp = vShopLook.Cross(vRight);
    vNewUp.Normalize();

    // ������ ȸ�� ��� ����
    m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
    m_pTransformCom->Set_State(CTransform::STATE_UP, vNewUp);
    m_pTransformCom->Set_State(CTransform::STATE_LOOK, vShopLook);

    m_pTransformCom->Set_Scale(1.7f, 2.f, 2.f);
}

HRESULT CPortal::Ready_Components()
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
     if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY,TEXT("Prototype_Component_Texture_Portal"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
         return E_FAIL;

    /* Collider Component */
    CCollider::COL_DESC	ColliderDesc = {};

    //ColliderDesc.pOwner = (this);
    // �̰ɷ� �ݶ��̴� ũ�� ����
     ColliderDesc.fScale = { 1.f,1.f,1.f };
    // ������Ʈ�� ������� �Ÿ� ����
    ColliderDesc.fLocalPos = { 0.f, 0.f, 0.0f };

    /* For.Com_Collider_Sphere */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;


    return S_OK;
}

CPortal* CPortal::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{

    CPortal* pInstance = new CPortal(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CPortal");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPortal::Clone(void* pArg)
{
    CPortal* pInstance = new CPortal(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CPortal");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPortal::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pColliderCom);
    Safe_Release(m_pPlayer);

}
