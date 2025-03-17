﻿#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLoader() = default;

public:
	_bool isFinished() const {
		return m_isFinished;
	}

public:
	HRESULT Initialize(LEVEL eNextLevelID);
	HRESULT Loading();

	void Output_LoadingText() {
		SetWindowText(g_hWnd, m_szLoadingText);
	}


private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	LEVEL						m_eNextLevelID = { LEVEL_END };
	_bool						m_isFinished = { false };
	CGameInstance* m_pGameInstance = { nullptr };

private:
	HANDLE						m_hThread = {};
	CRITICAL_SECTION			m_CriticalSection = {};
	_tchar						m_szLoadingText[MAX_PATH] = {};

public:
	HRESULT Loading_For_Logo();
	HRESULT Loading_For_GamePlay();
	


public:
	static CLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID);
	virtual void Free() override;

////////////////////////////////////////////////////////////
	HRESULT Add_To_Logo_Textures();
	HRESULT Add_To_Logo_Prototype();

	HRESULT Add_To_Logo_Menu_Textures();
	HRESULT Add_To_Logo_Menu_Prototype();
////////////////////////////////////////////////////////////
	HRESULT Add_To_GamePlay_Textures();
	HRESULT Add_To_GamePlay_Buffer();
	HRESULT Add_To_GamePlay_Prototype();
//////////////////////////////////////////////////////////// UI 텍스쳐 컴포넌트
	HRESULT Add_To_UI_Textures();

	              HRESULT Add_To_UI_Left_DisPlay_Textures();
				  HRESULT Add_To_UI_Mid_DisPlay_Textures();
				  HRESULT Add_To_UI_Right_DisPlay_Textures();
				  HRESULT Add_To_UI_Menu_DisPlay_Textures();
 //////////////////////////////////////////////////////////// UI 버퍼 컴포넌트

			    
//////////////////////////////////////////////////////////// UI 트랜스폼 컴포넌트

//////////////////////////////////////////////////////////// UI 원본 생성
    HRESULT Add_To_UI_Prototype();

				  HRESULT Add_To_UI_Left_DisPlay_Prototype();
				  HRESULT Add_To_UI_Mid_DisPlay_Prototype();
				  HRESULT Add_To_UI_Right_DisPlay_Prototype();
				  HRESULT Add_To_UI_Right_Menu_Prototype();
/////////////////////////////////////////////////////////////

				  HRESULT Add_To_Shop_Textures();
				  HRESULT Add_To_Shop_Prototype();

		
};

END