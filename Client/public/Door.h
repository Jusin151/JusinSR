#pragma once
#include "Client_Defines.h"
#include "CollisionObject.h"

BEGIN(Engine)
class CTransform;
class CCollider;
class CTexture;
class CVIBuffer;
class CMaterial;
END

BEGIN(Client)
class CDoor : public CCollisionObject
{
public:
    enum class DOOR_STATE : uint8_t
    {
        CLOSED,     // ���� ����
        OPENING,    // ������ ��
        OPEN,       // ���� ����
        CLOSING     // ������ ��
    };

    enum class DOOR_TYPE : uint8_t
    {
        NORMAL,     // �Ϲ� �� (�ٷ� ����)
        KEY         // ���谡 �ʿ��� ��
    };

    typedef struct tagDoorDesc : public OBJECT_DESC
    {
        DOOR_TYPE eType;          // �� ����
        _float3 vPosition;        // �� ��ġ
        _float fOpenAngle;        // ���� ������ ����
        _wstring stKeyItemTag;    // �ʿ��� ���� ������ �±� (KEY Ÿ���� ���)
        bool bStartsActive;       // ���۽� Ȱ��ȭ ����
    }DOOR_DESC;

private:
    CDoor(LPDIRECT3DDEVICE9 pGraphic_Device);
    CDoor(const CDoor& Prototype);
    virtual ~CDoor() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Update(_float fTimeDelta) override;
    virtual void Late_Update(_float fTimeDelta) override;
    virtual HRESULT Render() override;
    virtual HRESULT On_Collision(CCollisionObject* other) override;

    // �� ����/�ݱ� �Լ�
    void Open_Door();
    void Close_Door();

    // �� ���� Ȯ��
    _bool IsOpen() const { return m_eDoorState == DOOR_STATE::OPEN; }
    _bool IsClosed() const { return m_eDoorState == DOOR_STATE::CLOSED; }
    _bool IsMoving() const { return m_eDoorState == DOOR_STATE::OPENING || m_eDoorState == DOOR_STATE::CLOSING; }

    // ���� Ȯ�� �Լ�
    _bool TryOpen(CGameObject* pPlayer);
    void SetKeyRequired(const _wstring& keyItemTag) { m_stKeyItemTag = keyItemTag; }

private:
    HRESULT SetUp_RenderState();
    HRESULT Release_RenderState();
    HRESULT Ready_Components();
    void UpdateDoorTransform();

private:
    CTransform* m_pTransformCom = { nullptr };
    CCollider* m_pColliderCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };
    CVIBuffer* m_pVIBufferCom = { nullptr };
    CMaterial* m_pMaterialCom = { nullptr };

    DOOR_STATE m_eDoorState = DOOR_STATE::CLOSED;
    DOOR_TYPE m_eDoorType = DOOR_TYPE::NORMAL;

    // �� �ִϸ��̼� ���� ����
    _float m_fDoorOpenAmount = 0.0f;    // 0.0f (����) ~ 1.0f (������ ����)
    _float m_fDoorOpenSpeed = 2.0f;     // �ʴ� ������ �ӵ�
    _float3 m_vRotationAxis = { 0.0f, 1.0f, 0.0f };  // ȸ����(�⺻���� Y��)
    _float m_fOpenAngle = 90.0f;        // ���� ������ ����(�⺻�� 90��)

    // ���� ���� ����
    _wstring m_stKeyItemTag = L"";      // �ʿ��� ���� ������ �±�

    // ���� ��ġ�� ȸ�� ����
    _float3 m_vOriginalRotation;
    _float3 m_vOpenRotation;

public:
    static CDoor* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
    virtual json Serialize() override;
    virtual void Deserialize(const json& j) override;
};
END