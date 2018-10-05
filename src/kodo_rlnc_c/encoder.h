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

/// Opaque pointer used for the encoder factory
typedef struct krlnc_encoder_factory* krlnc_encoder_factory_t;

/// Opaque pointer used for encoder
typedef struct krlnc_encoder* krlnc_encoder_t;

//------------------------------------------------------------------
// ENCODER FACTORY API
//------------------------------------------------------------------

/// Build a new encoder factory
/// @param finite_field_id The finite field that should be used by the encoder.
/// @param symbols The maximum number of symbols supported by encoders
///        built with this factory.
/// @param symbol_size The maximum symbol size in bytes supported by
///        encoders built using the returned factory
/// @return A new factory capable of building encoders using the
///         selected parameters.
KODO_RLNC_API
krlnc_encoder_factory_t krlnc_new_encoder_factory(
    int32_t finite_field_id, uint32_t symbols, uint32_t symbol_size);

/// Deallocate and release the memory consumed by a factory
/// @param factory The factory which should be deallocated
KODO_RLNC_API
void krlnc_delete_encoder_factory(krlnc_encoder_factory_t factory);

/// Return the number of symbols in a block
/// @param factory The factory to query
/// @return the number of symbols in a block
KODO_RLNC_API
uint32_t krlnc_encoder_factory_symbols(krlnc_encoder_factory_t factory);

/// Return the symbol size in bytes
/// @param factory The factory to query
/// @return symbol_size the symbol size in bytes
KODO_RLNC_API
uint32_t krlnc_encoder_factory_symbol_size(krlnc_encoder_factory_t factory);

/// Set the number of symbols
/// @param factory The factory which should be configured
/// @param symbols the number of symbols
KODO_RLNC_API
void krlnc_encoder_factory_set_symbols(
    krlnc_encoder_factory_t factory, uint32_t symbols);

/// Set the symbol size
/// @param factory The factory which should be configured
/// @param symbol_size the symbol size in bytes
KODO_RLNC_API
void krlnc_encoder_factory_set_symbol_size(
    krlnc_encoder_factory_t factory, uint32_t symbol_size);

/// Set the coding vector format
/// @param factory The factory which should be configured
/// @param format_id The selected coding vector format
KODO_RLNC_API
void krlnc_encoder_factory_set_coding_vector_format(
    krlnc_encoder_factory_t factory, int32_t format_id);

/// Build the actual encoder
/// @param factory The encoder factory which should be used to build the encoder
/// @return pointer to an instantiation of an encoder
KODO_RLNC_API
krlnc_encoder_t krlnc_encoder_factory_build(krlnc_encoder_factory_t factory);

/// Deallocate and release the memory consumed by an encoder
/// @param encoder The encoder which should be deallocated
KODO_RLNC_API
void krlnc_delete_encoder(krlnc_encoder_t encoder);

//------------------------------------------------------------------
// SYMBOL STORAGE API
//------------------------------------------------------------------

/// Return the symbol size of the encoder.
/// @param encoder The encoder to check
/// @return The size of a symbol in bytes
KODO_RLNC_API
uint32_t krlnc_encoder_symbol_size(krlnc_encoder_t encoder);

/// Return the number of symbols in a block (i.e. the generation size).
/// @param encoder The encoder to check
/// @return The number of symbols
KODO_RLNC_API
uint32_t krlnc_encoder_symbols(krlnc_encoder_t encoder);

/// Return the block size of an encoder.
/// @param encoder The encoder to query.
/// @return The block size, i.e. the total size in bytes that this encoder
///         operates on.
KODO_RLNC_API
uint32_t krlnc_encoder_block_size(krlnc_encoder_t encoder);

/// Specifies the source data for a given symbol.
/// @param encoder The encoder which will encode the symbol
/// @param index The index of the symbol in the coding block
/// @param data The buffer containing the data to be encoded
/// @param size The size of the symbol buffer
KODO_RLNC_API
void krlnc_encoder_set_const_symbol(
    krlnc_encoder_t encoder, uint32_t index, uint8_t* data, uint32_t size);

/// Specify the source data for all symbols. This will specify all
/// symbols.
/// @param encoder The encoder which will encode the data
/// @param data The buffer containing the data to be encoded
/// @param size The size of the buffer to be encoded
KODO_RLNC_API
void krlnc_encoder_set_const_symbols(
    krlnc_encoder_t encoder, uint8_t* data, uint32_t size);

//------------------------------------------------------------------
// PAYLOAD API
//------------------------------------------------------------------

/// Return the payload size of an encoder, which is the size of a generated
/// payload.
/// @param encoder The encoder to query.
/// @return The payload size in bytes
KODO_RLNC_API
uint32_t krlnc_encoder_payload_size(krlnc_encoder_t encoder);

/// Write a symbol into the provided payload buffer.
/// @param encoder The encoder to use.
/// @param payload The buffer which should contain the symbol.
/// @return The total bytes used from the payload buffer
KODO_RLNC_API
uint32_t krlnc_encoder_write_payload(
    krlnc_encoder_t encoder, uint8_t* payload);

//------------------------------------------------------------------
// ENCODER API
//------------------------------------------------------------------

/// Return the rank of an encoder that indicates how many symbols
/// are available for encoding.
/// @param encoder The encoder to query
/// @return The rank of the encoder
KODO_RLNC_API
uint32_t krlnc_encoder_rank(krlnc_encoder_t encoder);

/// Return whether the encoder is in the systematic mode, i.e. if it will
/// initially send the original source symbols with a simple header.
/// @param encoder The encoder
/// @return Non-zero if the encoder is in the systematic mode, otherwise 0
KODO_RLNC_API
uint8_t krlnc_encoder_is_systematic_on(krlnc_encoder_t encoder);

/// Switch the systematic encoding on
/// @param encoder The encoder
KODO_RLNC_API
void krlnc_encoder_set_systematic_on(krlnc_encoder_t encoder);

/// Switch the systematic encoding off
/// @param encoder The encoder
KODO_RLNC_API
void krlnc_encoder_set_systematic_off(krlnc_encoder_t encoder);

/// Return whether the encoder is in the systematic phase, i.e. there
/// is a systematic packet to send
/// @param encoder The encoder
/// @return Non-zero if the encoder is in the systematic phase, otherwise 0
KODO_RLNC_API
uint8_t krlnc_encoder_in_systematic_phase(krlnc_encoder_t encoder);

//------------------------------------------------------------------
// SYMBOL API
//------------------------------------------------------------------

/// Returns the size of the coefficient vector.
/// @param encoder The encoder to check
/// @return The size of the coefficient vector in bytes
KODO_RLNC_API
uint32_t krlnc_encoder_coefficient_vector_size(krlnc_encoder_t encoder);

/// Writes an encoded symbol according to the provided symbol coefficients.
/// @param encoder The encoder to use.
/// @param symbol_data The destination buffer for the encoded symbol
/// @param coefficients The desired coding coefficients that should
///        be used to calculate the encoded symbol.
/// @return The number of bytes used.
KODO_RLNC_API
uint32_t krlnc_encoder_write_symbol(
    krlnc_encoder_t encoder, uint8_t* symbol_data, uint8_t* coefficients);

/// Writes a systematic/uncoded symbol that corresponds to the provided
/// symbol index.
/// @param encoder The encoder to use.
/// @param symbol_data The destination of the uncoded source symbol.
/// @param index The index of this uncoded symbol in the data block.
/// @return The number of bytes used.
KODO_RLNC_API
uint32_t krlnc_encoder_write_uncoded_symbol(
    krlnc_encoder_t encoder, uint8_t* symbol_data, uint32_t index);

//------------------------------------------------------------------
// COEFFICIENT GENERATOR API
//------------------------------------------------------------------

/// Set the seed of the coefficient generator.
/// @param encoder The encoder to use
/// @param seed_value The seed value for the generator.
KODO_RLNC_API
void krlnc_encoder_set_seed(krlnc_encoder_t encoder, uint32_t seed_value);

/// Fills the input buffer with symbol coefficients used for either
/// encoding or decoding a symbol.
/// @param encoder The encoder to use.
/// @param coefficients Pointer to the memory where the coefficients should
///        be stored. The coefficient buffer should have at least
///        krlnc_encoder_coefficient_vector_size() capacity.
KODO_RLNC_API
void krlnc_encoder_generate(krlnc_encoder_t encoder, uint8_t* coefficients);

/// Generate a "partial" coding vector that will only contain non-zero
/// coefficients for the symbols that are currently defined.
/// This allows encoding before defining all original source symbols,
/// i.e. on-the-fly encoding.
/// @param encoder The encoder to use.
/// @param coefficients Pointer to the memory where the coefficients should
///        be stored. The coefficient buffer should have at least
///        krlnc_encoder_coefficient_vector_size() capacity.
KODO_RLNC_API
void krlnc_encoder_generate_partial(
    krlnc_encoder_t encoder, uint8_t* coefficients);

/// Returns the density of the generated coding vector coefficients.
/// @param encoder The encoder to query
/// @return The coding vector density as a float
KODO_RLNC_API
float krlnc_encoder_density(krlnc_encoder_t encoder);

/// Sets the density of the generated coding vector coefficients.
/// @param encoder The encoder to use
/// @param density The density value (0.0 < density <= 1.0)
KODO_RLNC_API
void krlnc_encoder_set_density(krlnc_encoder_t encoder, float density);

//------------------------------------------------------------------
// TRACE API
//------------------------------------------------------------------

/// Enables the default trace function of the encoder, which prints to
/// the standard output.
/// @param encoder The encoder to use
KODO_RLNC_API
void krlnc_encoder_set_trace_stdout(krlnc_encoder_t encoder);

/// Registers a custom callback that will get the trace output of an encoder.
/// The function takes a void pointer which will be available when
/// the krlnc_trace_callback_t function is invoked by the library. This allows
/// the user to pass custom information to the callback function. If no context
/// is needed the pointer can be set to NULL.
/// @param encoder The encoder to use
/// @param callback The callback that processes the trace output
/// @param context A void pointer which is forwarded to the callback function.
KODO_RLNC_API
void krlnc_encoder_set_trace_callback(
    krlnc_encoder_t encoder, krlnc_trace_callback_t callback, void* context);

/// Disables the trace function of the encoder.
/// @param encoder The encoder to use
KODO_RLNC_API
void krlnc_encoder_set_trace_off(krlnc_encoder_t encoder);

/// Sets the zone prefix that should be used for the trace output of
/// a particular encoder instance. The zone prefix can help to
/// differentiate the output that is coming from various encoder instances.
/// @param encoder The encoder to use
/// @param prefix The zone prefix for the trace output
KODO_RLNC_API
void krlnc_encoder_set_zone_prefix(krlnc_encoder_t encoder, const char* prefix);

#ifdef __cplusplus
}
#endif
