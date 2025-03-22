#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;  // UI�� �簢������ �ҵ�
END

BEGIN(Client)

class CEffect_Base abstract : public CGameObject
{
public:
    enum class EFFECT_TYPE
    {
        NONE, // �⺻��
        BULLET_HIT, // �Ѿ� ����
        SLASH, // ���� ����
    };
    struct EFFECT_DESC
    {
        _float2 vPos = {};
        _float2 vScale = {};
        _float fLifeTime = {};
        EFFECT_TYPE eType = EFFECT_TYPE::NONE;
    };

protected:
    EFFECT_TYPE m_eType = EFFECT_TYPE::NONE;

protected:
    CEffect_Base(LPDIRECT3DDEVICE9 pGraphic_Device);
    CEffect_Base(const CEffect_Base& Prototype);
    virtual ~CEffect_Base() = default;

public:
    virtual HRESULT Initialize_Prototype()PURE;
    virtual HRESULT Initialize(void* pArg)PURE;
    virtual void Priority_Update(_float fTimeDelta)PURE;
    virtual void Update(_float fTimeDelta)PURE;
    virtual void Late_Update(_float fTimeDelta)PURE;
    virtual HRESULT Render()override;
    virtual HRESULT Ready_Components();
    virtual  void Free();

protected:
    CTexture* m_pTextureCom = {};
    CVIBuffer_Rect* m_pVIBufferCom = {};
    CTransform* m_pTransformCom = {};
    EFFECT_DESC m_Weapon_Effect_INFO{};

protected: // ����Ʈ ����
    _float m_fLifeTime = {}; // �ִ� ���ʱ��� �����Ҳ���
    _float m_fCurrentTime = {}; // ���� �ð�
    _bool m_bDead = {}; // ������ Active Ȱ��ȭ
};
END
