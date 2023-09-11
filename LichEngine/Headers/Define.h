#pragma once
#ifdef LICH_ENGINE
#define LICH_ENGINE __declspec(dllexport)
#else
#define LICH_ENGINE __declspec(dllimport)
#endif

#define LAPI extern "C" __declspec(dllexport)


#define LASSERTION

#define LICH_EDITOR
//#define LICH_GAME

#define NOMINMAX
//
//#define _CRTDBG_MAP_ALLOC
//#include <cstdlib>
//#include <crtdbg.h>
//#ifdef _DEBUGA
//#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW
//#endif