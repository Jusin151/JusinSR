#pragma once
#include "Base.h"

BEGIN(Engine)
class CGameObject;
class CPool_Manager final : public CBase
{
private:
	CPool_Manager();
	virtual ~CPool_Manager() = default;

public:
	class CGameObject* Acquire_Object(_uint iPrototypeLevelIndex, const _wstring& strLayerTag);

	// Ǯ�� ��ü ��ȯ�ϱ�
	HRESULT Return_Object(_uint iPrototypeLevelIndex, const _wstring& strLayerTag, CGameObject* pGameObject);

	// ��� ������ �ִ���, � �̸����� �־�����, ���߿� �ҷ��� �� ����� �±�, � ������, ��������� ����
	HRESULT Reserve_Pool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strLayerTag,_uint iCount,void* pArg = nullptr);
	void Clear(_uint iLevelIndex);
private:
	class CGameInstance* m_pGameInstance = { nullptr }; // ������Ÿ�Կ��� �������� ����
	unordered_map<_uint,map<_wstring, queue<CGameObject*>>> m_Pools; // ����, �ش� ������ tag�� ���� ������Ʈ��
	unordered_map<_wstring, pair<_wstring,void*>> m_InitArgMap;  // ���̾� �±� -> (������Ÿ�� �±�, �ʱ�ȭ ����)

public:
	static CPool_Manager* Create();
	virtual void Free() override;
};

END