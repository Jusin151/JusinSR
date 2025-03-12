#pragma once

#pragma warning (disable : 4251)

#include <d3d9.h>
#include <d3dx9.h>

#include <string>
#include <map>
#include <unordered_map>
#include <queue>
#include <list>
#include <vector>
using namespace std;

#include "Engine_Struct.h"
#include "Engine_Enum.h"
#include "Engine_Function.h"
#include "Engine_Typedef.h"
#include "Engine_Macro.h"

using namespace Engine;

#pragma region FMOD
#include "fmod.h"
#include "fmod.hpp"

using namespace FMOD;
#pragma endregion

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif



