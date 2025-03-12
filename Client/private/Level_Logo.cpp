#include "Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "CUI_Base.h"

CLevel_Logo::CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{


}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(Ready_Layer_BackGround()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Menu()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Logo::Update(_float fTimeDelta)
{
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		if (FAILED(m_pGameInstance->Change_Level(LEVEL_LOADING,
			CLevel_Loading::Create(m_pGraphic_Device, LEVEL_GAMEPLAY))))
			return;
	}

}

HRESULT CLevel_Logo::Render()
{
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround()
{
	 // 앞에 숫자는 렌더 순서임
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround_3"),
		LEVEL_LOGO, TEXT("01_Layer_BackGround_3"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround_4"),
		LEVEL_LOGO, TEXT("02_Layer_BackGround_4"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		LEVEL_LOGO, TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround_2"),
		LEVEL_LOGO, TEXT("Layer_BackGround_2"))))
		return E_FAIL;





	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Menu()
{

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Prototype_GameObject_Menu"),
		LEVEL_LOGO, TEXT("Layer_Menu"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Prototype_GameObject_Menu_StartButton"),
		LEVEL_LOGO, TEXT("Layer_Menu_StartButton"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Prototype_GameObject_Menu_ExitButton"),
		LEVEL_LOGO, TEXT("Layer_Menu_ExitButton"))))
		return E_FAIL;


	return S_OK;
}

CLevel_Logo* CLevel_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Logo* pInstance = new CLevel_Logo(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Logo::Free()
{
	__super::Free();

}
