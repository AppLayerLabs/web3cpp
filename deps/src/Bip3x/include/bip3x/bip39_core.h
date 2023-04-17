/**
 * bip39. 2018
 * bip39_core.h
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#ifndef MINTER_BIP39_CORE_H
#define MINTER_BIP39_CORE_H

#include <cstddef>
#include <cstdint>
#include "config.h"

/* #undef BIP39_SHARED */
#define BIP39_EXPORTING

#ifdef BIP39_SHARED
#  ifdef BIP39_EXPORTING
#   if _MSC_VER
#       define BIP39_CORE_API __declspec(dllexport)
#   else
#       define BIP39_CORE_API __attribute__((visibility("default")))
#   endif
#  else
#   if _MSC_VER
#       define BIP39_CORE_API __declspec(dllimport)
#   else
#       define BIP39_CORE_API
#   endif
#  endif
#else
#define BIP39_CORE_API
#endif // BIP39_SHARED

#if defined(WIN32) && !defined(bzero)
#include <cstring>
#define bzero(d,l) memset(d, 0, l)
#endif

/** Return codes */
#define MINTER_OK      0 /** Success */
#define MINTER_ERROR  -1 /** General error */
#define MINTER_EINVAL -2 /** Invalid argument */
#define MINTER_ENOMEM -3 /** malloc() failed */

#endif //MINTER_BIP39_CORE_H
