#include "MiniMap.h"
#include "Player.h"
#include <Transform.h>
#include <StructureManager.h>

CMiniMap::CMiniMap(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{
}

CMiniMap::CMiniMap(const CMiniMap& Prototype)
    : CGameObject(Prototype),
    m_pMiniMapSurface(Prototype.m_pMiniMapSurface),
    m_pMiniMapTexture(Prototype.m_pMiniMapTexture)
{
    Safe_AddRef(m_pMiniMapSurface);
    Safe_AddRef(m_pMiniMapTexture);
}

HRESULT CMiniMap::Initialize_Prototype()
{
  
    return S_OK;
}

HRESULT CMiniMap::Initialize(void* pArg)
{  // �̴ϸ� �ؽ�ó ����
    m_pGraphic_Device->CreateTexture(256, 256, 1, D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
        &m_pMiniMapTexture, NULL);
    m_pMiniMapTexture->GetSurfaceLevel(0, &m_pMiniMapSurface);

    // ��������Ʈ ��ü ����
    D3DXCreateSprite(m_pGraphic_Device, &m_pSprite);

    // �̴ϸ� ��� ��ġ �� ũ�� ����
    m_MiniMapRect = { 10, 10, 10 + 150, 10 + 150 }; // �»�� ��ġ(10,10)�� 150x150 ũ��� ǥ��
    m_pGraphic_Device->CreateVertexBuffer(
        4 * sizeof(VERTEX), // ���� 4��
        0,
        VERTEX::FVF,
        D3DPOOL_MANAGED,
        &m_pVertexBuffer,
        NULL
    );
    m_DoorList =  CStructureManager::Get_Instance()->Get_Doors();
    m_StructureList =  CStructureManager::Get_Instance()->Get_Structures();


    m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Find_Object(LEVEL_STATIC, L"Layer_Player"));
    if (!m_pPlayer)
    {
        m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Find_Object(LEVEL_GAMEPLAY, L"Layer_Player"));
    }

    if (!m_pPlayer)
    {
        return E_FAIL;
    }
    CalculateMapSize();
    return S_OK;
}

void CMiniMap::Update(_float fTimeDelta)
{
}

void CMiniMap::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CMiniMap::Render()
{
    // 1. ���� ���� Ÿ�� ����
    LPDIRECT3DSURFACE9 pBackBuffer = nullptr;
    m_pGraphic_Device->GetRenderTarget(0, &pBackBuffer);

    // 2. ���� Ÿ���� �̴ϸ� �ؽ�ó�� ����
    m_pGraphic_Device->SetRenderTarget(0, m_pMiniMapSurface);

    //// 3. �̴ϸ� �ؽ�ó Ŭ����
    m_pGraphic_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

    // 4. �̴ϸ� ���� �׸���
    SetUp_RenderState();

    // ���� ���������� ĥ�ϱ�
    for (const auto& structure : m_StructureList)
    {
        D3DXVECTOR2 miniMapPos = ConvertToMiniMapPos(dynamic_cast<CTransform*>(structure->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION));
        DrawBoxOnMiniMap(miniMapPos, D3DCOLOR_XRGB(0, 0, 0)); // ������ ��
    }

    // ���� ȸ������ ĥ�ϱ�
    for (const auto& door : m_DoorList)
    {
        D3DXVECTOR2 miniMapPos = ConvertToMiniMapPos(dynamic_cast<CTransform*>(door->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION));
        DrawBoxOnMiniMap(miniMapPos, D3DCOLOR_XRGB(150, 150, 150)); // ȸ�� ��
    }

    // �÷��̾� ��ġ ǥ��
    D3DXVECTOR2 playerMiniMapPos = ConvertToMiniMapPos(dynamic_cast<CTransform*>(m_pPlayer->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION));
    DrawBoxOnMiniMap(playerMiniMapPos, D3DCOLOR_XRGB(0, 255, 0)); // �ʷϻ� �÷��̾�

    Release_RenderState();

    // 5. ���� ���� Ÿ������ ����
    m_pGraphic_Device->SetRenderTarget(0, pBackBuffer);
    Safe_Release(pBackBuffer);

    // 6. �̴ϸ� �ؽ�ó�� ȭ�� �»�ܿ� �׸���
    if (m_pSprite && m_pMiniMapTexture)
    {
        // ��������Ʈ ����
        m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

        // �̴ϸ� �ؽ�ó�� ȭ�� �»�ܿ� �׸���
        D3DXVECTOR3 pos(100.0f, 100.0f, 0.0f); // �»�� ��ġ (10, 10)

        // �ؽ�ó ũ�� ���� (������)
        D3DXMATRIX matScale;
        D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.0f); // �ؽ�ó ũ�⸦ 60%�� ����

        D3DXMATRIX matTranslation;
        D3DXMatrixTranslation(&matTranslation, pos.x, pos.y, 0.0f);

        D3DXMATRIX matFinal = matScale * matTranslation;
        m_pSprite->SetTransform(&matFinal);

        m_pSprite->Draw(m_pMiniMapTexture, NULL, NULL, NULL, D3DCOLOR_ARGB(200, 255, 255, 255)); // �ణ �����ϰ�(���İ� 200)

        // ��������Ʈ ����
        m_pSprite->End();
    }

    return S_OK;
}

HRESULT CMiniMap::SetUp_RenderState()
{
    m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);
    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_OldViewMatrix); // ���� �� ��� ����

    // ���� ���� ����
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE); // ���� ��Ȱ��ȭ

    // �� ��� ���� (�߿�!)
    D3DXMATRIX matView;
    D3DXMatrixIdentity(&matView); // ���� ��ķ� ����
    D3DXVECTOR3 eye(0.0f, 0.0f, -10.0f);
    D3DXVECTOR3 at(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
    D3DXMatrixLookAtLH(&matView, &eye, &at, &up);
    m_pGraphic_Device->SetTransform(D3DTS_VIEW, &matView);

    // ���� ���� ��� ����
    D3DXMATRIX matOrtho;
    D3DXMatrixOrthoLH(&matOrtho, 256.0f, 256.0f, 0.1f, 1000.0f); // �̴ϸ� ũ�⿡ �°� ����
    m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &matOrtho);
    return S_OK;
}

HRESULT CMiniMap::Release_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE); // ���� ���� ����
    m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);
    m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_OldViewMatrix); // �� ��� ����
    return S_OK;
}

HRESULT CMiniMap::Ready_Components()
{
    return E_NOTIMPL;
}

void CMiniMap::CalculateMapSize()
{
    float minX = FLT_MAX, minZ = FLT_MAX;
    float maxX = FLT_MIN, maxZ = FLT_MIN;

    // ���� ���� ��ġ�� ���� Ȯ��
    for (const auto& structure : m_StructureList)
    {
        _float3 pos = dynamic_cast<CTransform*>(structure->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
        minX = min(minX, pos.x);
        maxX = max(maxX, pos.x);
        minZ = min(minZ, pos.z);
        maxZ = max(maxZ, pos.z);
    }

    for (const auto& door : m_DoorList)
    {
        _float3 pos = dynamic_cast<CTransform*>(door->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
        minX = min(minX, pos.x);
        maxX = max(maxX, pos.x);
        minZ = min(minZ, pos.z);
        maxZ = max(maxZ, pos.z);
    }

    m_MapWidth = maxX - minX;
    m_MapHeight = maxZ - minZ;

    // �ּ� ��ǥ ���� (��ǥ ��ȯ�� ���)
    m_MapMinX = minX;
    m_MapMinZ = minZ;
}

_float2 CMiniMap::ConvertToMiniMapPos(_float3 vPos)
{
    // ���� ��ǥ -> �̴ϸ� ��ǥ ��ȯ
    float scaleX = 256.0f / m_MapWidth; // �̴ϸ��� �ʺ� ������ �ʺ�� ����
    float scaleY = 256.0f / m_MapHeight; // �̴ϸ��� ���̸� ������ ���̷� ����

    float miniMapPosX = (vPos.x - m_MapMinX) * scaleX; // ���� ��ǥ�� �̴ϸ� ��ǥ�� ��ȯ
    float miniMapPosY = (vPos.z - m_MapMinZ) * scaleY; // ���� ��ǥ�� �̴ϸ� ��ǥ�� ��ȯ

    return _float2(miniMapPosX, miniMapPosY);
}

void CMiniMap::DrawBoxOnMiniMap(D3DXVECTOR2 pos,D3DCOLOR color)
{
    VERTEX* pVertices;
    m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

    float size = 2.0f; // �ڽ� ũ��

    pVertices[0] = { D3DXVECTOR3(pos.x - size, pos.y - size, 0), color };
    pVertices[1] = { D3DXVECTOR3(pos.x + size, pos.y - size, 0), color };
    pVertices[2] = { D3DXVECTOR3(pos.x + size, pos.y + size, 0), color };
    pVertices[3] = { D3DXVECTOR3(pos.x - size, pos.y + size, 0), color };

    m_pVertexBuffer->Unlock();

    // TRIANGLELIST���� TRIANGLEFAN���� ����
    m_pGraphic_Device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
}

json CMiniMap::Serialize()
{
    return json();
}

void CMiniMap::Deserialize(const json& j)
{
}

CMiniMap* CMiniMap::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CMiniMap* pInstance = new CMiniMap(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CMiniMap");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMiniMap::Clone(void* pArg)
{
    CMiniMap* pInstance = new CMiniMap(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CMiniMap");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMiniMap::Free()
{
    __super::Free();
    Safe_Release(m_pMiniMapSurface);
    Safe_Release(m_pMiniMapTexture);
    Safe_Release(m_pVertexBuffer);
    Safe_Release(m_pSprite); // ��������Ʈ ���� �߰�
    //Safe_Release(m_pTransformCom);
    //Safe_Release(m_pMaterialCom);
}