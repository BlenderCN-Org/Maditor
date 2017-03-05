#pragma once

#if Maditor_Shared_EXPORTS
#define MADITOR_SHARED_EXPORT __declspec(dllexport)
#else
#define MADITOR_SHARED_EXPORT __declspec(dllimport)
#endif

#include "maditorsharedforward.h"


#include "madginelib.h"
