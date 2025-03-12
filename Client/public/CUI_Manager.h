#include "Client_Defines.h"
#include "GameObject.h"


class CUI_Manager
{
public:
    static CUI_Manager* GetInstance()
    {
        static CUI_Manager instance;
        return &instance;
    }

private:
    CUI_Manager() = default;


    CUI_Manager(const CUI_Manager&) = delete;
    CUI_Manager& operator=(const CUI_Manager&) = delete;
    CUI_Manager(CUI_Manager&&) = delete;
    CUI_Manager& operator=(CUI_Manager&&) = delete;

public:


    void AddUI(const wstring& tag, CUI_Base* pUI)
    {
        if (m_UIMap.find(tag) != m_UIMap.end())
        {
            return;
        }
        m_UIMap[tag] = pUI;
    }

    CUI_Base* GetUI(const wstring& tag)
    {
        auto it = m_UIMap.find(tag);
        if (it != m_UIMap.end())
        {
            return it->second;
        }
        return nullptr;
    }

    CUI_Base* GetParentUI()
    {
        CUI_Base* pUI = GetUI(L"Parent");
        if (pUI)
        {
            return pUI->Parent;
        }
        return nullptr;
    }

    _float2 GetParent_Pos()
    {
        CUI_Base* pUI = GetUI(L"Parent");
        if (pUI)
        {
            return pUI->Get_Position();
        }
        return _float2(99999, 99999);
    }

    unordered_map<wstring, CUI_Base*> m_UIMap;

};


