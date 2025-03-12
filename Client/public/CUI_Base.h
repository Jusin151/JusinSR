#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"


BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;  // UI는 사각형으로 할듯
END

BEGIN(Client)

class CUI_Base : public CGameObject
{

public:
    typedef struct tagUI_Parent
    {
        _float4x4 ProjMatrix;
        _float4x4 WorldMatrix;
        _float4x4 ViewMatrix;
        _float2 vPos;
        _float2 vSize;
        _float fAlpha;
    }UI_Parent_Desc;

    typedef struct tagUI_Child
    {
        _float2 vPos;
        _float2 vSize;
        _float fAlpha;
    }UI_Child_Desc;

protected:
    CUI_Base(LPDIRECT3DDEVICE9 pGraphic_Device);
    CUI_Base(const CUI_Base& Prototype);
    virtual ~CUI_Base() = default;

public:
    virtual HRESULT Initialize_Prototype() = 0;
    virtual HRESULT Initialize(void* pArg) = 0;
    virtual void Update(_float fTimeDelta);
    virtual HRESULT Render() = 0;
    virtual void Free();

protected:
    HRESULT Ready_Components();

protected:
    UI_Parent_Desc m_UIBase_INFO{};
    _float2 m_vPosition = { 0.f, 0.f };  // UI 화면 좌표
    _float2 m_vSize = { 100.f, 50.f };  // UI 크기
    _float  m_fAlpha = 1.0f;            // 투명도?

    CTexture* m_pTextureCom = nullptr;
    CTransform* m_pTransformCom = nullptr;
    CVIBuffer_Rect* m_pVIBufferCom = nullptr;

    _float2  m_vRelativePosition = {};

public:
    CUI_Base* Parent = nullptr;
    unordered_map<std::wstring, CUI_Base*> Childs;

  
    void Set_Parent(CUI_Base* pParent)
    {
        Parent = pParent;
        m_vRelativePosition = 
        { m_vPosition.x - pParent->Get_Position().x,
          m_vPosition.y - pParent->Get_Position().y
        };
        pParent->Childs.insert({ GetLayerID(), this });
    }
    void Set_Position(_float2 vPos) 
    {
        m_vPosition = vPos;
        if (m_pTransformCom)
        {
            m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vPos.x, vPos.y, 0.f));
        }
       // Update_ChildPositions();  // 부모 위치 변경하면 자식 위치도 바꾸게 할 예정(근데 하기 싫엉)
    }
    void Set_Size(_float2 vSize)
    {
        m_vSize = vSize;
        if (m_pTransformCom)
        {
            m_pTransformCom->Set_Scale(vSize.x, vSize.y, 1.f);
        }
    }

    void Set_RelativePosition(const _float2& vRelativePos)
    {
        m_vRelativePosition = vRelativePos;
        Update_ChildPositions();
    }

    void Update_ChildPositions()
    {
        for (auto& pair : Childs)
        {
            CUI_Base* child = pair.second;
            _float2 newPos = { m_vPosition.x + child->m_vRelativePosition.x,
                                m_vPosition.y + child->m_vRelativePosition.y };
            child->Set_Position(newPos);
        }
    }

    void Set_Alpha(_float fAlpha) { m_fAlpha = fAlpha; }

    _float2 Get_Position() const { return m_vPosition; }
    _float2 Get_Size() const { return m_vSize; }
    _float Get_Alpha() const { return m_fAlpha; }

    //명훈이형이 말한 Layer ID 가져오는 함수 (추후 ID로 접근??)
    virtual wstring GetLayerID() { return L"Default"; }
};

END
