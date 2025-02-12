#pragma once

#ifdef KITAENGINE_EXPORTS
#define KITAENGINE_API __declspec(dllexport)
#else
    #define KITAENGINE_API __declspec(dllimport)
#endif