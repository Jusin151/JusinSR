#pragma once

#include "Client_Defines.h"
#include "CUI_Base.h"
#include <vector>
#include <unordered_map>
#include <string>

BEGIN(Client)

class CItemBase;

class CItem_Manager 
{
public:
    static CItem_Manager* GetInstance()
    {
        static CItem_Manager instance;
        return &instance;
    }

private:
    CItem_Manager() = default;

    CItem_Manager(const CItem_Manager&) = delete;
    CItem_Manager& operator=(const CItem_Manager&) = delete;
    CItem_Manager(CItem_Manager&&) = delete;
    CItem_Manager& operator=(CItem_Manager&&) = delete;

public:


    // ������ �߰�, ����, ��ȸ ���
    void AddItem(CItemBase* pItem) 
    {

    }
    void RemoveItem(int iItemID)
    {

    }
    CItemBase* GetItem(int iItemID)
    {

    }
    const vector<CItemBase*>& GetAllItems() const { return m_Items; }



private:
    vector<CItemBase*> m_Items;
    unordered_map<int, CItemBase*> m_ItemMap; // ������ ID�� Ű�� ����

    // �κ��丮 UI ���� ������ (��: ���� ��ġ, ��� �̹��� ��)�� �߰��� �� ����.
};
END
