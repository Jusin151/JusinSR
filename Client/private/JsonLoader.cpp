#include "JsonLoader.h"
#include <Player.h>
#include <Terrain.h>
#include <TestMonster.h>
#include <Structure.h>
#include <Camera_Free.h>
#include <Camera_FirstPerson.h>
#include "UI_Headers.h" 
#include "GamePlay_Button.h"

HRESULT CJsonLoader::Load_Prototypes(CGameInstance* pGameInstance, LPDIRECT3DDEVICE9 pGraphic_Device,const _wstring& filePath)
{
    // JSON ���� �ε�
    ifstream file(filePath);
    if (!file.is_open()) 
    {
        MSG_BOX("������Ÿ�� JSON ������ ã�� �� �����ϴ�.");
        return E_FAIL;
    }

    json j;
    file >> j;

    // �ؽ�ó ������Ÿ�� �ε�
    if (j.contains("textures")) 
    {
        for (const auto& texture : j["textures"])
        {
            _wstring tag = ISerializable::Utf8ToWide(texture["tag"]);
            _wstring path = ISerializable::Utf8ToWide(texture["path"]);
            LEVEL level = static_cast<LEVEL>(texture["level"].get<_uint>());
            _uint count = texture["count"];
            if (FAILED(pGameInstance->Add_Prototype(level, tag,
                CTexture::Create(pGraphic_Device, path.c_str(), count))))
                return E_FAIL;
        }
    }

    // ���� ������Ʈ ������Ÿ�� �ε�
    if (j.contains("gameObjects")) 
    {
        for (const auto& obj : j["gameObjects"])
        {
            _wstring tag = ISerializable::Utf8ToWide(obj["tag"]);
            LEVEL level = static_cast<LEVEL>(obj["level"].get<_uint>());
            string className = obj["class"];
            CBase* pGameObject = Create_Object_ByClassName(className, pGraphic_Device);
            if (!pGameObject)
                continue; 

            if (FAILED(pGameInstance->Add_Prototype(level, tag, pGameObject)))
                return E_FAIL;
        }
    }

    // ���� ������Ÿ�� �ε�
    if (j.contains("buffers")) 
    {
        for (const auto& buffer : j["buffers"])
        {
            _wstring tag = ISerializable::Utf8ToWide(buffer["tag"]);
            LEVEL level = static_cast<LEVEL>(buffer["level"].get<_uint>());
            string className = buffer["class"];

            // ���� ���� ���� �߰�
            CBase* pBuffer = nullptr;

            if (className == "CVIBuffer_Terrain") 
            {
                _uint width = buffer["width"].get<_uint>();
                _uint height = buffer["height"].get<_uint>();
                pBuffer = CVIBuffer_Terrain::Create(pGraphic_Device, width, height);
            }
            // �ٸ� Ÿ���� ���۰� �ִٸ� ���⿡ �߰�

            if (!pBuffer)
                continue;

            if (FAILED(pGameInstance->Add_Prototype(level, tag, pBuffer)))
                return E_FAIL;
        }
    }

    return S_OK;
}

HRESULT CJsonLoader::Load_Level(CGameInstance* pGameInstance, LPDIRECT3DDEVICE9 pGraphic_Device, const _wstring& jsonFilePath, LEVEL eLevel)
{
    ifstream file(jsonFilePath);
    if (!file.is_open())
    {
        MSG_BOX("���� JSON ������ ã�� �� �����ϴ�.");
        return E_FAIL;
    }

    json j;
    file >> j;

    // �� ���̾� ó��
    for (const auto& [layerName, layerObjects] : j["layers"].items())
    {
        _wstring wLayerName = ISerializable::Utf8ToWide(layerName);

        // ���̾ ��ü ó��
        for (const auto& objData : layerObjects)
        {
            // FromPool ��ü���� Ȯ��
            bool fromPool = false;
            if (objData.contains("FromPool"))
            {
                fromPool = objData["FromPool"].get<bool>();
            }

            // ���̾ �´� ������Ÿ�� �±� ��������
            _wstring prototypeTag = Get_Prototype_For_Layer(wLayerName);

            if (prototypeTag.empty())
                continue; // ������ ������Ÿ���� ������ �ǳʶ�

            // ��ü ���� ��� ����
            if (fromPool)
            {
                // Ǯ���� ��ü ����
                if (FAILED(pGameInstance->Reserve_Pool(eLevel, prototypeTag.c_str(), wLayerName.c_str(), 1)))
                    continue;

                // ��ü ����
                if (FAILED(pGameInstance->Add_GameObject_FromPool(eLevel, eLevel, wLayerName.c_str())))
                    continue;

                // ������ ��ü ��������
                CGameObject* pGameObject = pGameInstance->Find_Last_Object(eLevel, wLayerName);
                if (!pGameObject)
                    continue;

                // JSON �����ͷ� ��ü �ʱ�ȭ
                pGameObject->Deserialize(objData);
            }
            else
            {
                pGameInstance->Add_GameObject(eLevel, prototypeTag, eLevel, wLayerName);
                CGameObject* pGameObject = pGameInstance->Find_Last_Object(eLevel, wLayerName);
                if (!pGameObject) continue;
                pGameObject->Deserialize(objData);
            }
        }
    }

    return S_OK;

}

CBase* CJsonLoader::Create_Object_ByClassName(const string& className, LPDIRECT3DDEVICE9 pGraphic_Device)
{
    // �⺻ ���� ������Ʈ
    if (className == "CPlayer")
        return CPlayer::Create(pGraphic_Device);
    else if (className == "CTestMonster")
        return CTestMonster::Create(pGraphic_Device);
    else if (className == "CTerrain")
        return CTerrain::Create(pGraphic_Device);
    else if (className == "CStructure")
        return CStructure::Create(pGraphic_Device);
    // ī�޶�
    else if (className == "CCamera_Free")
        return CCamera_Free::Create(pGraphic_Device);
    else if (className == "CCamera_FirstPerson")
        return CCamera_FirstPerson::Create(pGraphic_Device);
    // UI ������Ʈ
    else if (className == "CUI_Default_Panel")
        return CUI_Default_Panel::Create(pGraphic_Device);
    else if (className == "CUI_Left_Display")
        return CUI_Left_Display::Create(pGraphic_Device);
    else if (className == "CUI_Player_Icon")
        return CUI_Player_Icon::Create(pGraphic_Device);
    else if (className == "CUI_HP_Bar")
        return CUI_HP_Bar::Create(pGraphic_Device);
    else if (className == "CUI_MP_Bar")
        return CUI_MP_Bar::Create(pGraphic_Device);
    else if (className == "CUI_Mid_Display")
        return CUI_Mid_Display::Create(pGraphic_Device);
    else if (className == "CUI_Right_Display")
        return CUI_Right_Display::Create(pGraphic_Device);
    else if (className == "CUI_Bullet_Bar")
        return CUI_Bullet_Bar::Create(pGraphic_Device);
    else if (className == "CUI_Menu")
        return CUI_Menu::Create(pGraphic_Device);
    else if (className == "CGamePlay_Button")
        return CGamePlay_Button::Create(pGraphic_Device);
    else if (className == "CUI_Spell_Shop")
        return CUI_Spell_Shop::Create(pGraphic_Device);

    // ã�� ���� Ŭ���� �̸��� ���� ó��
    wstring wClassName = ISerializable::Utf8ToWide(className);
    wstring errorMsg = L"�� �� ���� Ŭ���� �̸�: " + wClassName;
    OutputDebugString(errorMsg.c_str());

    return nullptr;
}

_wstring CJsonLoader::Get_Prototype_For_Layer(const _wstring& layerName)
{
    if (layerName == L"Layer_Player")
        return L"Prototype_GameObject_Player";
    else if (layerName == L"Layer_Monster")
        return L"Prototype_GameObject_TestMonster";
    else if (layerName == L"Layer_BackGround")
        return L"Prototype_GameObject_Terrain";
    else if (layerName == L"Layer_Camera")
        return L"Prototype_GameObject_Camera_FirstPerson";
    else if (layerName == L"Layer_Default_PlayerUI")
        return L"Prototype_GameObject_Default_PlayerUI";

    return L"";
}
