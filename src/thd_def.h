/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef THD_DEF_H
#define THD_DEF_H

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#ifdef THD_LIB_EXPORT
#ifdef _WIN32
#define THD_EXTERN __declspec(dllexport) extern
#else
#define THD_EXTERN extern
#endif
#elif THD_LIB_IMPORY
#ifdef _WIN32
#define THD_EXTERN __declspec(dllimport) extern
#else
#define THD_EXTERN extern
#endif
#else
#define THD_EXTERN
#endif

#endif // THD_DEF_H

