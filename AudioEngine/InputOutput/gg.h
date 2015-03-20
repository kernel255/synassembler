
#pragma once

#ifdef FIND_MEM_LEAK

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

/*

#ifdef _DEBUG
#ifndef DEBUG_NEW
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif //DEBUG_NEW
#endif //DEBUG

*/

#endif //FIND_MEM_LEAK