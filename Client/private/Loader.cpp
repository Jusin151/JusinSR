#include "Loader.h"

#include "GameInstance.h"
#include "Terrain.h"
#include "Camera_Free.h"
////////////////////////////// 레벨_로고
#include "BackGround.h"
#include  "BackGround_2.h"
#include  "BackGround_3.h"
#include  "BackGround_4.h"
#include  "Default_Menu.h"
#include  "Start_Button.h"
#include  "Exit_Button.h"
////////////////////////////// 레벨_게임플레이
#include "UI_Default_Panel.h"
#include "UI_HPDisplay.h"
#include "CUI_AmmoDisplay.h"
#include "UI_ExpDisplay.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device{ pGraphic_Device }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

_uint APIENTRY LoadingMain(void* pArg)
{
	/* 자원로딩한다. */
	CLoader* pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;

}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	HRESULT		hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Logo();
		break;

	case LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlay();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}
HRESULT CLoader::Loading_For_Logo()
{

 	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));

	Add_To_Logo_Textures();

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));


	Add_To_Logo_Buffer();

	Add_To_Logo_Transform();

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));

	Add_To_Logo_Prototype();

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}
HRESULT CLoader::Loading_For_GamePlay()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));

	Add_To_GamePlay_Textures();

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));


	Add_To_GamePlay_Buffer();

	Add_To_GamePlay_Transform();


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));

	Add_To_GamePlay_Prototype();
	//////////////////////////////////////


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Add_To_Logo_Prototype()
{

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround_2"),
		CBackGround_2::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround_3"),
		CBackGround_3::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround_4"),
		CBackGround_4::Create(m_pGraphic_Device))))
		return E_FAIL;

	Add_To_Logo_Menu_Prototype();

	return S_OK;
}

HRESULT CLoader::Add_To_Logo_Menu_Textures()
{

	// 검은색 메뉴바
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Menu"),
		CTexture::Create(m_pGraphic_Device, TEXT("../../Resources/Textures/UI/Logo/SR_Real.png"), 1))))
		return E_FAIL;

	// 메뉴바 안의 시작버튼
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Menu_StartButton"),
		CTexture::Create(m_pGraphic_Device, TEXT("../../Resources/Textures/UI/Logo/SR_Start.png"), 1))))
		return E_FAIL;

	// 메뉴바 안의 시작버튼 선택시
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Menu_StartButton_Select"),
		CTexture::Create(m_pGraphic_Device, TEXT("../../Resources/Textures/UI/Logo/SR_Start_Select.png"), 1))))
		return E_FAIL;

	// 메뉴바 안의 종료버튼
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Menu_ExitButton"),
		CTexture::Create(m_pGraphic_Device, TEXT("../../Resources/Textures/UI/Logo/SR_Exit.png"), 1))))
		return E_FAIL;

	// 메뉴바 안의 종료버튼 선택시
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Menu_ExitButton_Select"),
		CTexture::Create(m_pGraphic_Device, TEXT("../../Resources/Textures/UI/Logo/SR_Exit_Select.png"), 1))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Add_To_Logo_Menu_Buffer()
{

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect_Menu"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Menu_StartButton"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Menu_ExitButton"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Add_To_Logo_Menu_Transform()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Transform_Menu"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Transform_Menu_StartButton"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Transform_Menu_ExitButton"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Add_To_Logo_Menu_Prototype()
{

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_Menu"),
		CDefault_Menu::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_Menu_StartButton"),
		CStart_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_Menu_ExitButton"),
		CExit_Button::Create(m_pGraphic_Device))))
		return E_FAIL;


	return S_OK;
}

HRESULT CLoader::Add_To_GamePlay_Textures()
{

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device,
			TEXT("../../Resources/Textures/Terrain/Tile0.jpg"), 1))))
		return E_FAIL;


	///////////////////////////
	Add_To_UI_Textures();
	////////////////////////////

	return S_OK;
}


HRESULT CLoader::Add_To_GamePlay_Buffer()
{

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 256, 256))))
		return E_FAIL;

	Add_To_UI_Buffer();


	return S_OK;
}

HRESULT CLoader::Add_To_GamePlay_Transform()
{

	Add_To_UI_Transform();


	return S_OK;
}

HRESULT CLoader::Add_To_GamePlay_Prototype()
{

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	Add_To_UI_Prototype();

	return S_OK;
}

HRESULT CLoader::Add_To_Logo_Textures()
{

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pGraphic_Device, TEXT("../../Resources/Textures/UI/Logo/menu-warlock_v02_winter.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround_2"),
		CTexture::Create(m_pGraphic_Device, TEXT("../../Resources/Textures/UI/Logo/SR_Long_menu_potwory.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround_3"),
		CTexture::Create(m_pGraphic_Device, TEXT("../../Resources/Textures/UI/Logo/menu_blyskawica_v01_winter.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround_4"),
		CTexture::Create(m_pGraphic_Device, TEXT("../../Resources/Textures/UI/Logo/SR_menu_potwory_v01_winter.png"), 1))))
		return E_FAIL;

	Add_To_Logo_Menu_Textures();



	return S_OK;
}

HRESULT CLoader::Add_To_Logo_Buffer()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect_BackGround"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect_BackGround_2"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect_BackGround_3"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect_BackGround_4"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	Add_To_Logo_Menu_Buffer();

	return S_OK;
}

HRESULT CLoader::Add_To_Logo_Transform()
{

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Transform_BackGround"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Transform_BackGround_2"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Transform_BackGround_3"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Transform_BackGround_4"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	Add_To_Logo_Menu_Transform();

	return S_OK;
}




HRESULT CLoader::Add_To_UI_Prototype() //원형객체들
{

	// 플레이어 디폴트 UI
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_GameObject_Default_PlayerUI"),
		CUI_Default_Panel::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 체력바
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_GameObject_HealthBar_UI"),
		CUI_HPDisplay::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 탄약바
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_GameObject_Ammo_UI"),
		CUI_AmmoDisplay::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 경험치바
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_GameObject_Exp_UI"),
		CUI_ExpDisplay::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}
HRESULT CLoader::Add_To_UI_Textures()// 텍스쳐 컴포넌트
{
	// 판넬
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_Component_Texture_Default_PlayerUI"),
		CTexture::Create(m_pGraphic_Device,
			TEXT("../../Resources/Textures/UI/Default_UI.png"),
			1))))
		return E_FAIL;

	//체력바
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_Component_Texture_HealthBar"),
		CTexture::Create(m_pGraphic_Device,
			TEXT("../../Resources/Textures/UI/Left/SR_HUD_bottom_left.png"),
			1))))
		return E_FAIL;

	//탄약바
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_Component_Texture_AmmoDisplay"),
		CTexture::Create(m_pGraphic_Device,
			TEXT("../../Resources/Textures/UI/Right/SR_HUD_bottom_right.png"),
			1))))
		return E_FAIL;

	//경험치바
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_Component_Texture_EXPDisplay"),
		CTexture::Create(m_pGraphic_Device,
			TEXT("../../Resources/Textures/UI/Middle/SR_EXP_Bar.png"),
			1))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Add_To_UI_Buffer()//버퍼 컴포넌트
{
	// 플레이어 디폴트 UI
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_Component_VIBuffer_PlayerUI"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 체력바
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_Component_VIBuffer_HealthBar"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 탄약바
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_Component_VIBuffer_AmmoDisplay"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 경험치바
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_Component_VIBuffer_EXPDisplay"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;



	return S_OK;
}



HRESULT CLoader::Add_To_UI_Transform() // 트랜스폼 컴포넌트
{

	// 플레이어 디폴트 UI
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_Component_Transform_Default"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 체력바
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_Component_Transform_HealthBar"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 탄약바
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_Component_Transform_AmmoDisplay"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 경험치바
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY,
		TEXT("Prototype_Component_Transform_EXPDisplay"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}





CLoader* CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID)
{
	CLoader* pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	Safe_Release(m_pGameInstance);

	Safe_Release(m_pGraphic_Device);


	DeleteCriticalSection(&m_CriticalSection);
}
