#ifndef ODBC_TEST_GOOGLETEST_H_INCLUDED
#define ODBC_TEST_GOOGLETEST_H_INCLUDED
//------------------------------------------------------------------------------
// Disable the annoying warning when comparing doubles
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4244)
#endif
//------------------------------------------------------------------------------
#include "gtest/gtest.h"
#include "gmock/gmock.h"
//------------------------------------------------------------------------------
#ifdef _MSC_VER
#pragma warning(pop)
#endif
//------------------------------------------------------------------------------
#endif
