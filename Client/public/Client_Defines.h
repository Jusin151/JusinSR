#pragma once

/* 특정 헤더파일에 소속되지 않는 전역정보들을 선언해두는 역활 */
/* 기타 다른 정의파일(.h)들을 인클루드하는 역활. */
#include <Windows.h>
#include <process.h>
#include "Client_Enum.h"
#include "unordered_map"

namespace Client
{
	//기본 1280 720
	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;
}

extern HWND g_hWnd;


using namespace Client;