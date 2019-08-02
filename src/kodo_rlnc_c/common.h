// Copyright Steinwurf ApS 2018.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <stdint.h>

#if defined(_MSC_VER)
    #if defined(KODO_RLNC_C_STATIC)
        // When building a static library, KODO_RLNC_API should be blank
        #define KODO_RLNC_API
    #elif defined(KODO_RLNC_C_DLL_EXPORTS)
        // When building the DLL, the API symbols must be exported
        #define KODO_RLNC_API __declspec(dllexport)
    #else
        // When a program uses the DLL, the API symbols must be imported
        #define KODO_RLNC_API __declspec(dllimport)
    #endif
#else
    #if __GNUC__ >= 4
        // When building a shared library, only the API symbols with the 'default'
        // visibility should be exported to hide all other symbols. All source
        // files should be compiled with the '-fvisibility=hidden' and
        // '-fvisibility-inlines-hidden' flags to achieve this.
        #define KODO_RLNC_API __attribute__ ((visibility ("default")))
    #else
        #define KODO_RLNC_API
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------
// COMMON TYPES
//------------------------------------------------------------------

/// Callback function type used for logging
typedef void (*krlnc_log_callback_t)(const char*, const char*, void*);

/// Enum specifying the available finite fields
/// Note: the size of the enum type cannot be guaranteed, so the int32_t type
/// is used in the API calls to pass the enum values
typedef enum
{
    krlnc_binary,
    krlnc_binary4,
    krlnc_binary8,
    krlnc_binary16
}
krlnc_finite_field;

/// Enum specifying the available coding vector formats
/// Note: the size of the enum type cannot be guaranteed, so the int32_t type
/// is used in the API calls to pass the enum values
typedef enum
{
    /// Send the full encoding vector
    krlnc_full_vector,
    /// Only send a 4-byte random seed
    krlnc_seed,
    /// Send the density as a 4-byte float and the 4-byte random seed
    krlnc_sparse_seed
}
krlnc_coding_vector_format;

#ifdef __cplusplus
}
#endif
