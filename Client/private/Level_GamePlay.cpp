#include "Level_GamePlay.h"
#include "GameInstance.h"
#include "UI_Default_Panel.h"
#include "CUI_Base.h"

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device } 

{}
HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI()))
		return E_FAIL;
	

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	int a = 10;
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _wstring& strLayerTag)
{

	if (FAILED(m_pGameInstance->Add_GameObject
	(LEVEL_GAMEPLAY, 
		TEXT("Prototype_GameObject_Terrain"),
		LEVEL_GAMEPLAY, strLayerTag)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Reserve_Pool(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"), strLayerTag, 6)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"),
		LEVEL_GAMEPLAY, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI()
{
	CUI_Base::UI_Parent_Desc DefaultUI_Desc{}; // 부모 UI는 총 6개의 정보 소유

	DefaultUI_Desc.vSize = { 0,0 };
	DefaultUI_Desc.fAlpha = 1.0f;
	DefaultUI_Desc.vPos = { 0.f,0.f };
	DefaultUI_Desc.ProjMatrix = {};
	DefaultUI_Desc.ViewMatrix = {};
	DefaultUI_Desc.WorldMatrix = {};

	if (FAILED(m_pGameInstance->Add_GameObject
	(LEVEL_GAMEPLAY,
	TEXT("Prototype_GameObject_Default_PlayerUI"),
	LEVEL_GAMEPLAY, TEXT("Layer_Default_PlayerUI"), &DefaultUI_Desc)))
	return E_FAIL;

	CUI_Base::UI_Child_Desc HP_Desc{};  // 자식 UI는 3개만 소유 부모 상대적으로 위치 잡을꺼임
	HP_Desc.vSize = { 262,210 }; 
	HP_Desc.fAlpha = 1.0f;
	HP_Desc.vPos = { -510.f,-255.f }; // 부모위치가 원점 상대적으로 얼만큼 잡을껀지

	if (FAILED(m_pGameInstance->Add_GameObject
	(LEVEL_GAMEPLAY,
		TEXT("Prototype_GameObject_HealthBar_UI"),
		LEVEL_GAMEPLAY, TEXT("Layer_HealthBar_UI"), &HP_Desc)))
		return E_FAIL;

	CUI_Base::UI_Child_Desc Ammo_Desc{};  // 자식 UI는 3개만 소유 부모 상대적으로 위치 잡을꺼임
	Ammo_Desc.vSize = { 270,177 };
	Ammo_Desc.fAlpha = 1.0f;
	Ammo_Desc.vPos = { 510.f,-271.f }; // 부모위치가 원점 상대적으로 얼만큼 잡을껀지

	if (FAILED(m_pGameInstance->Add_GameObject
	(LEVEL_GAMEPLAY,
		TEXT("Prototype_GameObject_Ammo_UI"),
		LEVEL_GAMEPLAY, TEXT("Layer_AmmoDisplay_UI"), &Ammo_Desc)))
		return E_FAIL;

	CUI_Base::UI_Child_Desc EXP_Desc{};  // 자식 UI는 3개만 소유 부모 상대적으로 위치 잡을꺼임
	Ammo_Desc.vSize = { 850,13.f };
	Ammo_Desc.fAlpha = 1.0f;
	Ammo_Desc.vPos = { 6.f,-353.f }; // 부모위치가 원점 상대적으로 얼만큼 잡을껀지

	if (FAILED(m_pGameInstance->Add_GameObject
	(LEVEL_GAMEPLAY,
		TEXT("Prototype_GameObject_Exp_UI"),
		LEVEL_GAMEPLAY, TEXT("Layer_AmmoDisplay_UI"), &Ammo_Desc)))
		return E_FAIL;

	return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_GamePlay::Free()
{
	__super::Free();

}
