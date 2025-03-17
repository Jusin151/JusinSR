#pragma once
#include "Base.h"
#include "Layer.h"
#include "GameObject.h"
#include "Object_Manager.h"
#include "GameInstance.h"

BEGIN(Engine)
class CEditor final : public CBase
{
public: 
    CEditor() : m_pGameInstance(CGameInstance::Get_Instance())
    {
        Safe_AddRef(m_pGameInstance);
    }
    ~CEditor()
    {
        Safe_Release(m_pGameInstance);
    }

public:
    // ������Ʈ ���
    void RegisterObject(_uint iLevelID)
    {
        auto& layerMap = m_pGameInstance->m_pObject_Manager->m_pLayers[iLevelID];

        for (auto& layer : layerMap)
        {
            auto& objList = layer.second->m_GameObjects;
            for (auto it = objList.begin();it!=objList.end();++it)
            {
                m_LevelObjects[iLevelID].push_back(static_cast<ISerializable*>(*it));
            }
        }
    }

    // ������ ����
    _bool SaveLevel(_uint ilevelID, const _wstring& filepath) 
    {
        RegisterObject(ilevelID);
        json j;
        j["level"] = ilevelID;
        j["objects"] = json::array();

        for (auto* obj : m_LevelObjects[ilevelID]) 
        {
            j["objects"].push_back(obj->Serialize());
        }

        ofstream file(filepath);
        if (!file.is_open()) return false;

        file << j.dump(4);
        file.close();
        return true;
    }

    //// Ư�� ���� �ҷ�����
    //bool LoadLevel(int levelID, const char* filepath,
    //    function<void(const json&)> objectCreator) 
    //{
    //    ifstream file(filepath);
    //    if (!file.is_open()) return false;

    //    json j;
    //    file >> j;
    //    file.close();

    //    if (j["level"].get<int>() != levelID) return false;

    //    for (auto& objData : j["objects"])
    //    {
    //        // �ݹ��� ���� ������Ʈ ���� ������ �ܺη� ����
    //        objectCreator(objData);
    //    }

    //    return true;
    //}

    void RenderUI() 
    {
        ImGui::Begin("Editor");

        static char filepath[256] = "../Save/LEVEL.json";
        static int currentLevel = 1;

        ImGui::InputText("File Path", filepath, sizeof(filepath));
        ImGui::InputInt("Level ID", &currentLevel);
        if (ImGui::Button("Save Level")) 
        {
            if (SaveLevel(currentLevel, ISerializable::Utf8ToWide(filepath))) 
            {
                int a = 0;
            }
        }

        ImGui::End();
    }

private:
    map<_uint, vector<ISerializable*>> m_LevelObjects; // ������ ������Ʈ
    CGameInstance* m_pGameInstance = { nullptr };
};
END
