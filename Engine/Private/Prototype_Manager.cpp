#include "Prototype_Manager.h"

#include "GameObject.h"

CPrototype_Manager::CPrototype_Manager()
{
}

HRESULT CPrototype_Manager::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	m_vecPrototypes.resize(iNumLevels);

    return S_OK;
}

HRESULT CPrototype_Manager::Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype)
{
	if (m_vecPrototypes.empty() ||
		iPrototypeLevelIndex >= m_iNumLevels ||
		nullptr != Find_Prototype(iPrototypeLevelIndex, strPrototypeTag))
		return E_FAIL;

	m_vecPrototypes[iPrototypeLevelIndex].emplace(strPrototypeTag, pPrototype);

	return S_OK;
}

CBase* CPrototype_Manager::Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	CBase* pPrototype = Find_Prototype(iPrototypeLevelIndex, strPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	if (ePrototypeType == PROTOTYPE::TYPE_GAMEOBJECT)
		return dynamic_cast<CGameObject*>(pPrototype)->Clone(pArg);
	else
		// return dynamic_cast<CComponent*>(pPrototype)->Clone(pArg);
		return nullptr;	
}

CBase* CPrototype_Manager::Find_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag)
{
	auto	iter = m_vecPrototypes[iLevelIndex].find(strPrototypeTag);

	return iter == m_vecPrototypes[iLevelIndex].end() ? nullptr : iter->second;
}

CPrototype_Manager* CPrototype_Manager::Create(_uint iNumLevels)
{
	CPrototype_Manager* pInstance = new CPrototype_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("Failed to Created : CPrototype_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CPrototype_Manager::Free()
{
    __super::Free();

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_vecPrototypes[i])
			Safe_Release(Pair.second);
		m_vecPrototypes[i].clear();
	}
	m_vecPrototypes.clear();
}
