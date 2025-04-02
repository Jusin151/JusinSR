#pragma once
#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CTransform;
//class CTexture;
class CVIBuffer;
class CMaterial;
END

BEGIN(Client)
class CMiniMap final : public CGameObject
{
public:
    struct VERTEX
    {
        D3DXVECTOR3 position; // 3D ��ǥ
        D3DCOLOR color;       // ����

        static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
    };

    struct GridCoord
    {
        int row;
        int col;
    };
private:
    CMiniMap(LPDIRECT3DDEVICE9 pGraphic_Device);
    CMiniMap(const CMiniMap& Prototype);
    virtual ~CMiniMap() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Update(_float fTimeDelta) override;
    virtual void Late_Update(_float fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    HRESULT SetUp_RenderState();
    HRESULT Release_RenderState();
    HRESULT Ready_Components();

private:
    void CalculateMapSize();
    _float2 ConvertToMiniMapPos(_float3 vPos);
    void UpdateGridMap();
    void RenderGridMap();
    void InitializeGridMap();
    CMiniMap::GridCoord WorldToGrid(_float3 worldPos);
    _bool IsValidGridCoord(const GridCoord& coord);
    // �׸��� �Լ�
    void DrawBoxOnMiniMap(D3DXVECTOR2 pos, D3DCOLOR color, float size);
    void DrawRectOnMiniMap(float x, float y, float width, float height, D3DCOLOR color);
    void RenderStructureOnMiniMap(CGameObject* pObject);
    _float GetIconSizeForObject(CGameObject* pObject);
    void RenderPlayerOnMiniMap();
    void RenderTestSquare();
private:
    list<class CDoor*> m_DoorList;
    list<class CStructure*> m_StructureList;
    class CPlayer* m_pPlayer = { nullptr };
    LPDIRECT3DTEXTURE9 m_pMiniMapTexture = {nullptr};  // �̴ϸ� ���� Ÿ��
    LPDIRECT3DSURFACE9 m_pMiniMapSurface = {nullptr};
    LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer = { nullptr };
    // ��������Ʈ ��ü �߰�
    LPD3DXSPRITE m_pSprite = nullptr;
    // �̴ϸ� ��� ��ġ �� ũ��
    RECT m_MiniMapRect;

    _float m_MapWidth{0.f};
    _float m_MapHeight{0.f};
    _float m_MapMinX{0.f };
    _float m_MapMinZ{ 0.f };
    _float4x4 m_OldProjMatrix{};
    _float4x4 m_OldViewMatrix{};
   
    float m_GridCellSize = 2.0f;    // �׸��� �� ũ��
    int m_GridRows = 0;             // �׸��� �� ��
    int m_GridColumns = 0;          // �׸��� �� ��

    vector<vector<int>> m_GridMap;

public:
    static CMiniMap* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
    virtual json Serialize() override;
    virtual void Deserialize(const json& j) override;
};
END