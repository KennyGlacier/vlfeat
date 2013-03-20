/** @file generic.h
 ** @brief Generic (@ref generic)
 ** @author Andrea Vedaldi
 **/

/*
Copyright (C) 2007-12 Andrea Vedaldi and Brian Fulkerson.
Copyright (C) 2013 Andrea Vedaldi.
All rights reserved.

This file is part of the VLFeat library and is made available under
the terms of the BSD license (see the COPYING file).
*/

#ifndef VL_GENERIC_H
#define VL_GENERIC_H

#include "host.h"
#include "random.h"

#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <assert.h>

/** @brief Library version string */
#define VL_VERSION_STRING "0.9.16"

/** @brief Maximum length (in characters) of an error message */
#define VL_ERR_MSG_LEN 1024

/** @name Type identidifers for atomic data types
 ** @{ */

#define VL_TYPE_FLOAT   1     /**< @c float type */
#define VL_TYPE_DOUBLE  2     /**< @c double type */
#define VL_TYPE_INT8    3     /**< @c ::vl_int8 type */
#define VL_TYPE_UINT8   4     /**< @c ::vl_uint8 type */
#define VL_TYPE_INT16   5     /**< @c ::vl_int16 type */
#define VL_TYPE_UINT16  6     /**< @c ::vl_uint16 type */
#define VL_TYPE_INT32   7     /**< @c ::vl_int32 type */
#define VL_TYPE_UINT32  8     /**< @c ::vl_uint32 type */
#define VL_TYPE_INT64   9     /**< @c ::vl_int64 type */
#define VL_TYPE_UINT64  10    /**< @c ::vl_uint64 type */

typedef vl_uint32 vl_type ;

/** @brief Get the name of a data type.
 ** @param type data type.
 ** @return data name of the data type.
 **
 ** @c type is one of ::VL_TYPE_FLOAT, ::VL_TYPE_DOUBLE,
 ** ::VL_TYPE_INT8, ::VL_TYPE_INT16, ::VL_TYPE_INT32, ::VL_TYPE_INT64,
 ** ::VL_TYPE_UINT8, ::VL_TYPE_UINT16, ::VL_TYPE_UINT32, ::VL_TYPE_UINT64.
 **/

VL_INLINE char const *
vl_get_type_name (vl_type type)
{
  switch (type) {
    case VL_TYPE_FLOAT   : return "float"  ;
    case VL_TYPE_DOUBLE  : return "double" ;
    case VL_TYPE_INT8    : return "int8"   ;
    case VL_TYPE_INT16   : return "int16"  ;
    case VL_TYPE_INT32   : return "int32"  ;
    case VL_TYPE_INT64   : return "int64"  ;
    case VL_TYPE_UINT8   : return "int8"   ;
    case VL_TYPE_UINT16  : return "int16"  ;
    case VL_TYPE_UINT32  : return "int32"  ;
    case VL_TYPE_UINT64  : return "int64"  ;
    default: return NULL ;
  }
}

/** @brief Get data type size.
 ** @param type data type.
 ** @return size (in byte)
 **
 ** @c type is one of ::VL_TYPE_FLOAT, ::VL_TYPE_DOUBLE,
 ** ::VL_TYPE_INT8, ::VL_TYPE_INT16, ::VL_TYPE_INT32, ::VL_TYPE_INT64,
 ** ::VL_TYPE_UINT8, ::VL_TYPE_UINT16, ::VL_TYPE_UINT32, ::VL_TYPE_UINT64.
 **/

VL_INLINE vl_size
vl_get_type_size (vl_type type)
{
  vl_size dataSize = 0 ;
  switch (type) {
    case VL_TYPE_DOUBLE : dataSize = sizeof(double) ; break ;
    case VL_TYPE_FLOAT  : dataSize = sizeof(float) ; break ;
    case VL_TYPE_INT64  : case VL_TYPE_UINT64 : dataSize = sizeof(vl_int64) ; break ;
    case VL_TYPE_INT32  : case VL_TYPE_UINT32 : dataSize = sizeof(vl_int32) ; break ;
    case VL_TYPE_INT16  : case VL_TYPE_UINT16 : dataSize = sizeof(vl_int16) ; break ;
    case VL_TYPE_INT8   : case VL_TYPE_UINT8  : dataSize = sizeof(vl_int8)  ; break ;
    default:
      abort() ;
  }
  return dataSize ;
}

/** @} */



/** @internal @brief VLFeat global state */

VL_EXPORT char const * vl_get_version_string () ;
VL_EXPORT char * vl_configuration_to_string_copy () ;
VL_EXPORT void vl_set_simd_enabled (vl_bool x) ;
VL_EXPORT vl_bool vl_get_simd_enabled () ;
VL_EXPORT vl_bool vl_cpu_has_sse3 () ;
VL_EXPORT vl_bool vl_cpu_has_sse2 () ;
VL_EXPORT vl_size vl_get_num_cpus () ;
VL_EXPORT vl_size vl_get_num_threads () ;
VL_EXPORT vl_size vl_get_max_num_threads () ;
VL_EXPORT VlRand * vl_get_rand () ;

/** @} */

/** ------------------------------------------------------------------
 ** @name Error handling
 ** @{ */

#define VL_ERR_OK       0  /**< No error */
#define VL_ERR_OVERFLOW 1  /**< Buffer overflow error */
#define VL_ERR_ALLOC    2  /**< Resource allocation error */
#define VL_ERR_BAD_ARG  3  /**< Bad argument or illegal data error */
#define VL_ERR_IO       4  /**< Input/output error */
#define VL_ERR_EOF      5  /**< End-of-file or end-of-sequence error */
#define VL_ERR_NO_MORE  5  /**< End-of-sequence @deprecated */

VL_EXPORT int vl_get_last_error () ;
VL_EXPORT char const *  vl_get_last_error_message () ;
VL_EXPORT int vl_set_last_error (int error, char const * errorMessage, ...) ;
/** @} */

/** ------------------------------------------------------------------
 ** @name Memory allocation
 ** @{ */
VL_EXPORT void
vl_set_alloc_func (void *(*malloc_func)  (size_t),
                   void *(*realloc_func) (void*,size_t),
                   void *(*calloc_func)  (size_t, size_t),
                   void  (*free_func)    (void*)) ;
VL_EXPORT void *vl_malloc  (size_t n) ;
VL_EXPORT void *vl_realloc (void *ptr, size_t n) ;
VL_EXPORT void *vl_calloc  (size_t n, size_t size) ;
VL_EXPORT void *vl_memalign  (size_t n, size_t size) ;
VL_EXPORT void  vl_free    (void* ptr) ;
/** @} */

/** ------------------------------------------------------------------
 ** @name Logging
 ** @{ */
/** @brief Customizable printf function pointer type */
typedef int(*printf_func_t) (char const *format, ...) ;
VL_EXPORT void vl_set_printf_func (printf_func_t printf_func) ;
VL_EXPORT printf_func_t vl_get_printf_func () ;

/** @def VL_PRINTF
 ** @brief Call user-customizable @c printf function
 **
 ** The function calls the user customizable @c printf.
 **/

/** @def VL_PRINT
 ** @brief Same as ::VL_PRINTF (legacy code)
 **/

#define VL_PRINTF (*vl_get_printf_func())
#define VL_PRINT (*vl_get_printf_func())
/** @} */

/** ------------------------------------------------------------------
 ** @name Common operations
 ** @{ */

/** @brief Min operation
 ** @param x value
 ** @param y value
 ** @return the minimum of @a x and @a y.
 **/
#define VL_MIN(x,y) (((x)<(y))?(x):(y))

/** @brief Max operation
 ** @param x value.
 ** @param y value.
 ** @return the maximum of @a x and @a y.
 **/
#define VL_MAX(x,y) (((x)>(y))?(x):(y))

/** @brief Signed left shift operation
 **
 ** The macro is equivalent to the builtin @c << operator, but it
 ** supports negative shifts too.
 **
 ** @param x value.
 ** @param n number of shift positions.
 ** @return @c x << n .
 **/
#define VL_SHIFT_LEFT(x,n) (((n)>=0)?((x)<<(n)):((x)>>-(n)))
/* @} */

/** ------------------------------------------------------------------
 ** @name Measuring time
 ** @{
 **/
VL_EXPORT void vl_tic () ;
VL_EXPORT double vl_toc () ;
VL_EXPORT double vl_get_cpu_time () ;
/** @} */

/* VL_GENERIC_H */
#endif
