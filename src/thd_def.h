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

#ifdef THD_LIB
#ifdef _WIN32
#ifdef PD_INTERNAL
#define CPD_EXTERN __declspec(dllexport) extern
#else
#define CPD_EXTERN __declspec(dllimport) extern
#endif
#else
#define CPD_EXTERN extern
#endif

#if defined(_MSC_VER) && !defined(_LANGUAGE_C_PLUS_PLUS) && !defined(__cplusplus)
#define CPD_EXTERN_STRUCT extern struct
#else
#define CPD_EXTERN_STRUCT struct
#endif

#else
#define CPD_EXTERN
#define CPD_EXTERN_STRUCT
#endif

#endif // THD_DEF_H

