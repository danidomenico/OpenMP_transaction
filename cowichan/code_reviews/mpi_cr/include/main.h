// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#define IS_PARALLEL

#define TEST_OUTPUT

#define CHAIN_STAGE

//#define CHAIN_OUTPUT

#define TEST_TIME

#include <boost/mpi.hpp>
namespace mpi = boost::mpi;

#if defined(WIN32)   // Windows
#include <windows.h>
#else                // Linux
#include <sys/times.h>
typedef uint64_t INT64;
#endif               // end of WIN32/Linux definitions

#include "../include/type.h"
#include "../include/generic.h"
#include "../include/util.h"

#endif /* MAIN_H */
