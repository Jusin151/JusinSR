#include "Prototype_Manager.h"

#include "GameObject.h"
#include "Component.h"

CPrototype_Manager::CPrototype_Manager()
{
}

HRESULT CPrototype_Manager::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	m_pPrototypes = new /*map<const _wstring, class CBase*>*/PROTOTYPES[iNumLevels];	

    return S_OK;
}

HRESULT CPrototype_Manager::Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype)
{

	if (nullptr == m_pPrototypes ||
		iPrototypeLevelIndex >= m_iNumLevels ||
		nullptr != Find_Prototype(iPrototypeLevelIndex, strPrototypeTag))
		return E_FAIL;

	m_pPrototypes[iPrototypeLevelIndex].emplace(strPrototypeTag, pPrototype);

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
		return dynamic_cast<CComponent*>(pPrototype)->Clone(pArg);
		return nullptr;	
}

void CPrototype_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& Pair : m_pPrototypes[iLevelIndex])	
		Safe_Release(Pair.second);

	m_pPrototypes[iLevelIndex].clear();	
}

CBase* CPrototype_Manager::Find_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag)
{
	auto	iter = m_pPrototypes[iLevelIndex].find(strPrototypeTag);
	if (iter == m_pPrototypes[iLevelIndex].end())
		return nullptr;

	return iter->second;
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
		for (auto& Pair : m_pPrototypes[i])
			Safe_Release(Pair.second);
		m_pPrototypes[i].clear();
	}

	Safe_Delete_Array(m_pPrototypes);


}
