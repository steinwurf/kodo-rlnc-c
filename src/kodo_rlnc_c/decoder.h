// Copyright Steinwurf ApS 2018.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <stdint.h>

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------
// KODO-RLNC-C TYPES
//------------------------------------------------------------------

/// Opaque pointer used for the decoder factory
typedef struct krlnc_decoder_factory* krlnc_decoder_factory_t;

/// Opaque pointer used for decoder
typedef struct krlnc_decoder* krlnc_decoder_t;

//------------------------------------------------------------------
// DECODER FACTORY API
//------------------------------------------------------------------

/// Build a new decoder factory
/// @param finite_field The finite field that should be used by the decoder.
/// @param symbols The maximum number of symbols supported by decoders
///        built with this factory.
/// @param symbol_size The maximum symbol size in bytes supported by
///        decoders built using the returned factory
/// @return A new factory capable of building decoders using the
///         selected parameters.
KODO_RLNC_API
krlnc_decoder_factory_t krlnc_new_decoder_factory(
    int32_t finite_field_id, uint32_t symbols, uint32_t symbol_size);

/// Deallocate and release the memory consumed by a factory
/// @param factory The factory which should be deallocated
KODO_RLNC_API
void krlnc_delete_decoder_factory(krlnc_decoder_factory_t factory);

/// Return the number of symbols in a block
/// @param factory The factory to query
/// @return the number of symbols in a block
KODO_RLNC_API
uint32_t krlnc_decoder_factory_symbols(krlnc_decoder_factory_t factory);

/// Return the symbol size in bytes
/// @param factory The factory to query
/// @return the symbol size in bytes
KODO_RLNC_API
uint32_t krlnc_decoder_factory_symbol_size(krlnc_decoder_factory_t factory);

/// Set the number of symbols
/// @param factory The factory which should be configured
/// @param symbols the number of symbols
KODO_RLNC_API
void krlnc_decoder_factory_set_symbols(
    krlnc_decoder_factory_t factory,uint32_t symbols);

/// Set the symbol size
/// @param factory The factory which should be configured
/// @param symbol_size the symbol size in bytes
KODO_RLNC_API
void krlnc_decoder_factory_set_symbol_size(
    krlnc_decoder_factory_t factory, uint32_t symbol_size);

/// Set the coding vector format
/// @param factory The factory which should be configured
/// @param format_id The selected coding vector format
KODO_RLNC_API
void krlnc_decoder_factory_set_coding_vector_format(
    krlnc_decoder_factory_t factory, int32_t format_id);

/// Build the actual decoder
/// @param factory The decoder factory which should be used to build the decoder
/// @return pointer to an instantiation of an decoder
KODO_RLNC_API
krlnc_decoder_t krlnc_decoder_factory_build(krlnc_decoder_factory_t factory);

/// Deallocate and release the memory consumed by an decoder
/// @param decoder The decoder which should be deallocated
KODO_RLNC_API
void krlnc_delete_decoder(krlnc_decoder_t decoder);

//------------------------------------------------------------------
// SYMBOL STORAGE API
//------------------------------------------------------------------

/// Return the number of symbols in a block (i.e. the generation size).
/// @param decoder The decoder to check
/// @return The number of symbols
KODO_RLNC_API
uint32_t krlnc_decoder_symbols(krlnc_decoder_t decoder);

/// Return the symbol size of the decoder.
/// @param decoder The decoder to check
/// @return The size of a symbol in bytes
KODO_RLNC_API
uint32_t krlnc_decoder_symbol_size(krlnc_decoder_t decoder);

/// Return the block size of an decoder.
/// @param decoder The decoder to query.
/// @return The block size, i.e. the total size in bytes that this decoder
///         operates on.
KODO_RLNC_API
uint32_t krlnc_decoder_block_size(krlnc_decoder_t decoder);

/// Specifies the data buffer where the decoder should store a given symbol.
/// @param decoder The decoder which will decode the symbol
/// @param index The index of the symbol in the coding block
/// @param data The buffer that should contain the decoded symbol
/// @param size The size of the symbol buffer
KODO_RLNC_API
void krlnc_decoder_set_mutable_symbol(
    krlnc_decoder_t decoder, uint32_t index, uint8_t* data, uint32_t size);

/// Specify the data buffer where the decoder should store the decoded
/// symbols.
/// This will specify the storage for all symbols.
/// @param decoder The decoder which will decode the data
/// @param data The buffer that should contain the decoded symbols
/// @param size The size of the buffer to be decoded
KODO_RLNC_API
void krlnc_decoder_set_mutable_symbols(
    krlnc_decoder_t decoder, uint8_t* data, uint32_t size);

//------------------------------------------------------------------
// PAYLOAD API
//------------------------------------------------------------------

/// Return the payload size of an decoder, which is the expected size of a
/// given payload.
/// @param decoder The decoder to query.
/// @return The payload size in bytes
KODO_RLNC_API
uint32_t krlnc_decoder_payload_size(krlnc_decoder_t decoder);

/// Read symbol from the given payload buffer.
/// @param decoder The decoder to use.
/// @param payload The buffer storing the payload of an encoded symbol.
///        The payload buffer may be changed by this operation,
///        so it cannot be reused. If the payload is needed at several places,
///        make sure to keep a copy of the original payload.
KODO_RLNC_API
void krlnc_decoder_read_payload(krlnc_decoder_t decoder, uint8_t* payload);

/// Write a recoded symbol into the provided payload buffer.
/// @param decoder The decoder to use.
/// @param payload The buffer which should contain the recoded symbol.
/// @return The total bytes used from the payload buffer
KODO_RLNC_API
uint32_t krlnc_decoder_write_payload(
    krlnc_decoder_t decoder, uint8_t* payload);



//------------------------------------------------------------------
// DECODER API
//------------------------------------------------------------------

/// Check whether decoding is complete.
/// @param decoder The decoder to query
/// @return Non-zero value if the decoding is complete, otherwise 0
KODO_RLNC_API
uint8_t krlnc_decoder_is_complete(krlnc_decoder_t decoder);

/// Check whether decoding is partially complete. This means that some
/// symbols in the decoder are fully decoded. You can use the
/// krlnc_decoder_is_symbol_uncoded() function to determine which symbols.
/// @param decoder The decoder to query
/// @return Non-zero value if the decoding is partially complete, otherwise 0
KODO_RLNC_API
uint8_t krlnc_decoder_is_partially_complete(krlnc_decoder_t decoder);

/// Return the rank of a decoder that indicates how many symbols are
/// decoded or partially decoded.
/// @param decoder The decoder to query
/// @return The rank of the decoder
KODO_RLNC_API
uint32_t krlnc_decoder_rank(krlnc_decoder_t decoder);

/// Returns the number of missing symbols.
/// @param decoder The decoder to query
/// @return The number of missing symbols at the decoder
KODO_RLNC_API
uint32_t krlnc_decoder_symbols_missing(krlnc_decoder_t decoder);

/// Returns the number of partially decoded symbols.
/// @param decoder The decoder to query
/// @return The number of partially decoded symbols at the decoder
KODO_RLNC_API
uint32_t krlnc_decoder_symbols_partially_decoded(krlnc_decoder_t decoder);

/// Returns the number of uncoded (i.e. fully decoded) symbols.
/// @param decoder The decoder to query
/// @return The number of uncoded symbols at the decoder
KODO_RLNC_API
uint32_t krlnc_decoder_symbols_uncoded(krlnc_decoder_t decoder);

/// Indicates whether a symbol is missing at a decoder.
/// @param decoder The decoder to query
/// @param index Index of the symbol whose state should be checked
/// @return Non-zero value if the symbol is missing, otherwise 0
KODO_RLNC_API
uint8_t krlnc_decoder_is_symbol_missing(
    krlnc_decoder_t decoder, uint32_t index);

/// Indicates whether a symbol has been partially decoded at a decoder.
/// @param decoder The decoder to query
/// @param index Index of the symbol whose state should be checked
/// @return Non-zero value if the symbol has been partially decoded,
///         otherwise 0
KODO_RLNC_API
uint8_t krlnc_decoder_is_symbol_partially_decoded(
    krlnc_decoder_t decoder, uint32_t index);

/// Indicates whether a symbol is available in an uncoded (i.e. fully decoded)
/// form at the decoder.
/// @param decoder The decoder to query
/// @param index Index of the symbol whose state should be checked
/// @return Non-zero value if the symbol is uncoded, otherwise 0
KODO_RLNC_API
uint8_t krlnc_decoder_is_symbol_uncoded(
    krlnc_decoder_t decoder, uint32_t index);

/// Indicates if a symbol is partially or fully decoded. A symbol with
/// a pivot element is defined in the coding matrix of a decoder.
/// @param decoder The decoder to query
/// @param index Index of the symbol whose state should be checked
/// @return Non-zero value if the symbol is defined, otherwise 0
KODO_RLNC_API
uint8_t krlnc_decoder_is_symbol_pivot(krlnc_decoder_t decoder, uint32_t index);

//------------------------------------------------------------------
// SYMBOL API
//------------------------------------------------------------------


//------------------------------------------------------------------
// COEFFICIENT GENERATOR API
//------------------------------------------------------------------


//------------------------------------------------------------------
// TRACE API
//------------------------------------------------------------------

/// Enables the default trace function of the decoder, which prints to
/// the standard output.
/// @param coder The decoder to use
KODO_RLNC_API
void krlnc_decoder_set_trace_stdout(krlnc_decoder_t decoder);

/// Registers a custom callback that will get the trace output of an decoder.
/// The function takes a void pointer which will be available when
/// the krlnc_trace_callback_t function is invoked by the library. This allows
/// the user to pass custom information to the callback function. If no context
/// is needed the pointer can be set to NULL.
/// @param coder The decoder to use
/// @param callback The callback that processes the trace output
/// @param context A void pointer which is forwarded to the callback function.
KODO_RLNC_API
void krlnc_decoder_set_trace_callback(
    krlnc_decoder_t decoder, krlnc_trace_callback_t callback, void* context);

/// Disables the trace function of the decoder.
/// @param coder The decoder to use
KODO_RLNC_API
void krlnc_decoder_set_trace_off(krlnc_decoder_t decoder);

/// Sets the zone prefix that should be used for the trace output of
/// a particular decoder instance. The zone prefix can help to
/// differentiate the output that is coming from various coder instances.
/// @param coder The decoder to use
/// @param prefix The zone prefix for the trace output
KODO_RLNC_API
void krlnc_decoder_set_zone_prefix(krlnc_decoder_t decoder, const char* prefix);

#ifdef __cplusplus
}
#endif

// Clean up define
#undef KODO_RLNC_API
