#pragma once
#include "Client_Defines.h"
#include "CollisionObject.h"
#include "Serializable.h"

BEGIN(Engine)
class CTransform;
class CCollider;
class CTexture;
END

BEGIN(Client)
class CTrigger : public CCollisionObject
{
public:
    enum class TRIGGER_TYPE : uint8_t
    {
        BUTTON,           // ���� �� �ִ� ��ư
        INTERACTION     // ��ȣ�ۿ�
    };

    typedef struct tagTriggerDesc : public OBJECT_DESC
    {
        TRIGGER_TYPE eType;
        _float3 vPosition;         // Ʈ���� ��ġ
        _float fActivationRange;   // ���� Ʈ������ Ȱ��ȭ ����
        _wstring stTargetTag;        // ������ �޴� ��ü�� �±�
        bool bStartsActive;        // ���۽� Ȱ��ȭ ����
    }TRIGGER_DESC;

private:
    CTrigger(LPDIRECT3DDEVICE9 pGraphic_Device);
    CTrigger(const CTrigger& Prototype);
    virtual ~CTrigger() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Update(_float fTimeDelta) override;
    virtual void Late_Update(_float fTimeDelta) override;
    virtual HRESULT Render() override;
    virtual HRESULT On_Collision(CCollisionObject* other) override;

    void Activate();
    void Deactivate();

    void AddTargetObject(CGameObject* pTarget);

    // Ư�� Ʈ���� ������ �����ϱ� ���� �������̵�
    void OnTrigger_Activated();
    void OnTrigger_Deactivated();

private:
    CTransform* m_pTransformCom = { nullptr };
    CCollider* m_pColliderCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };

    TRIGGER_TYPE m_eTriggerType = TRIGGER_TYPE::BUTTON;
    bool m_bWasTriggered = false;

    CGameObject* m_TargetObject = { nullptr };

private:
    HRESULT Ready_Components();

public:
    static CTrigger* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
    virtual json Serialize() override;
    virtual void Deserialize(const json& j) override;
};
END