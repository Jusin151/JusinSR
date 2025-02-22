#include "GameInstance.h"

#include "Level_Manager.h"
#include "Graphic_Device.h"

IMPLEMENT_SINGLETON(CGameInstance);

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc)
{
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;


	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	m_pLevel_Manager->Update(fTimeDelta);
}

HRESULT CGameInstance::Draw()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Render_Begin();



	m_pLevel_Manager->Render();

	m_pGraphic_Device->Render_End();

	return S_OK;
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	/* Ư�� ������ �ڿ��� �����Ѵ�. */
	/* Ư�� ������ ��ü�� �����Ѵ�. */
	/* Ư�� ������ �������� �����Ѵ�. */
}

#pragma region LEVEL_MANAGER

HRESULT CGameInstance::Change_Level(_uint iLevelIndex, CLevel* pNewLevel)
{
	return m_pLevel_Manager->Change_Level(iLevelIndex, pNewLevel);
}

#pragma endregion

void CGameInstance::Free()
{
	__super::Free();

	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pLevel_Manager);
}
