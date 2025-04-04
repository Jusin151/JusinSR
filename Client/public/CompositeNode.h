#pragma once
#include <Client_Defines.h>
#include "Node.h"
// �߻�
class CCompositeNode : public INode // Select , Sequence �� ���
{
public:
    virtual ~CCompositeNode() { Release(); }
public:
    void AddChild(INode* child) { m_vecChild.push_back(child); }
protected:
    void Release()
    {
        for (auto& node : m_vecChild)
        {
            Safe_Delete<INode*>(node);
        }
        m_vecChild.clear();
    }
protected:
    vector<INode*> m_vecChild;
};

class CSequenceNode : public CCompositeNode // ��� �����ϴ� �ϳ��� FAIL�̸� ���� ����
{
public:
    NodeStatus Execute() override
    {
        for (auto& child : m_vecChild)
        {
            NodeStatus status = child->Execute();
            if (status == NodeStatus::FAIL) // �ϳ��� ���и� ��� ����
            {
                return NodeStatus::FAIL;
            }
            if (status == NodeStatus::RUNNING) // ���� ���� ��尡 ������ RUNNING ��ȯ
            {
                return NodeStatus::RUNNING;
            }
        }
        return NodeStatus::SUCCESS; // ��� ��尡 �����ϸ� SUCCESS ��ȯ
    }
};

class CSelectorNode : public CCompositeNode  // ������ ��尡 ������ ��ȯ�ؼ� ���� ��Ŵ
{
public:
    NodeStatus Execute() override
    {
        for (auto& child : m_vecChild)
        {
            NodeStatus status = child->Execute();
            if (status == NodeStatus::SUCCESS) // �ϳ��� �����ϸ� ��� ���� ��ȯ
            {
                return NodeStatus::SUCCESS;
            }
            if (status == NodeStatus::RUNNING) // ���� ���̸� RUNNING ��ȯ
            {
                return NodeStatus::RUNNING;
            }
        }
        return NodeStatus::FAIL; // ��� ��尡 �����ϸ� FAIL ��ȯ
    }
};