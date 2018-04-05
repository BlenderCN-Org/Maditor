#pragma once

/// @cond

#if Maditor_Shared_EXPORTS
#define MADITOR_SHARED_EXPORT __declspec(dllexport)
#else
#define MADITOR_SHARED_EXPORT __declspec(dllimport)
#endif

#define _HAS_AUTO_PTR_ETC 1

#include "maditorsharedforward.h"


#include <Madgine/interfaceslib.h>






/// @endcond