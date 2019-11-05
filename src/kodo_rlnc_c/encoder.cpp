// Copyright Steinwurf ApS 2018.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "encoder.h"

#include <cstring>
#include <cstdint>
#include <cassert>
#include <string>

#include <kodo_rlnc/coders.hpp>

#include "convert_enums.hpp"

struct krlnc_encoder
{
    template<class... Args>
    krlnc_encoder(Args&&... args) :
        m_impl(std::forward<Args>(args)...)
    { }

    kodo_rlnc::encoder m_impl;
};

//------------------------------------------------------------------
// ENCODER BASIC API
//------------------------------------------------------------------

krlnc_encoder_t krlnc_create_encoder(
    int32_t finite_field_id, uint32_t symbols, uint32_t symbol_size)
{
    auto finite_field = c_field_to_krlnc_field(finite_field_id);
    return new krlnc_encoder(finite_field, symbols, symbol_size);
}

void krlnc_delete_encoder(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    delete encoder;
}

void krlnc_reset_encoder(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    encoder->m_impl.reset();
}

void krlnc_encoder_set_coding_vector_format(
    krlnc_encoder_t encoder, int32_t format_id)
{
    auto format = c_format_to_krlnc_format(format_id);
    encoder->m_impl.set_coding_vector_format(format);
}

//------------------------------------------------------------------
// SYMBOL STORAGE API
//------------------------------------------------------------------

uint64_t krlnc_encoder_block_size(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl.block_size();
}

uint32_t krlnc_encoder_symbol_size(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl.symbol_size();
}

uint32_t krlnc_encoder_symbols(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl.symbols();
}

void krlnc_encoder_set_symbol_storage(
    krlnc_encoder_t encoder, uint8_t* data, uint32_t index)
{
    assert(encoder != nullptr);
    encoder->m_impl.set_symbol_storage(data, index);
}

void krlnc_encoder_set_symbols_storage(
    krlnc_encoder_t encoder, uint8_t* data)
{
    assert(encoder != nullptr);
    encoder->m_impl.set_symbols_storage(data);
}

//------------------------------------------------------------------
// PAYLOAD API
//------------------------------------------------------------------

uint32_t krlnc_encoder_max_payload_size(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl.max_payload_size();
}

uint32_t krlnc_encoder_produce_payload(
    krlnc_encoder_t encoder, uint8_t* payload)
{
    assert(encoder != nullptr);
    return encoder->m_impl.produce_payload(payload);
}

//------------------------------------------------------------------
// ENCODER API
//------------------------------------------------------------------

uint32_t krlnc_encoder_rank(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl.rank();
}

uint8_t krlnc_encoder_is_systematic_on(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl.is_systematic_on();
}

void krlnc_encoder_set_systematic_on(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    encoder->m_impl.set_systematic_on();
}

void krlnc_encoder_set_systematic_off(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    encoder->m_impl.set_systematic_off();
}

uint8_t krlnc_encoder_in_systematic_phase(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl.in_systematic_phase();
}

//------------------------------------------------------------------
// SYMBOL API
//------------------------------------------------------------------

uint32_t krlnc_encoder_coefficient_vector_size(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl.coefficient_vector_size();
}

uint32_t krlnc_encoder_produce_symbol(
    krlnc_encoder_t encoder, uint8_t* symbol_data, uint8_t* coefficients)
{
    assert(encoder != nullptr);
    return encoder->m_impl.produce_symbol(symbol_data, coefficients);
}

uint32_t krlnc_encoder_produce_systematic_symbol(
    krlnc_encoder_t encoder, uint8_t* symbol_data, uint32_t index)
{
    assert(encoder != nullptr);
    return encoder->m_impl.produce_systematic_symbol(symbol_data, index);
}

//------------------------------------------------------------------
// COEFFICIENT GENERATOR API
//------------------------------------------------------------------

void krlnc_encoder_set_seed(krlnc_encoder_t encoder, uint32_t seed_value)
{
    assert(encoder != nullptr);
    encoder->m_impl.set_seed(seed_value);
}

void krlnc_encoder_generate(krlnc_encoder_t encoder, uint8_t* coefficients)
{
    assert(encoder != nullptr);
    encoder->m_impl.generate(coefficients);
}

void krlnc_encoder_generate_partial(
    krlnc_encoder_t encoder, uint8_t* coefficients)
{
    assert(encoder != nullptr);
    encoder->m_impl.generate_partial(coefficients);
}

float krlnc_encoder_density(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl.density();
}

void krlnc_encoder_set_density(krlnc_encoder_t encoder, float density)
{
    assert(encoder != nullptr);
    encoder->m_impl.set_density(density);
}

//------------------------------------------------------------------
// LOG API
//------------------------------------------------------------------

void krlnc_encoder_set_log_stdout(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    encoder->m_impl.set_log_stdout();
}

void krlnc_encoder_set_log_callback(
    krlnc_encoder_t encoder, krlnc_log_callback_t c_callback, void* context)
{
    assert(c_callback);
    assert(encoder != nullptr);

    auto callback = [c_callback, context](const std::string& zone,
                                          const std::string& data)
    {
        c_callback(zone.c_str(), data.c_str(), context);
    };
    encoder->m_impl.set_log_callback(callback);
}

void krlnc_encoder_set_log_off(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    encoder->m_impl.set_log_off();
}

void krlnc_encoder_set_zone_prefix(krlnc_encoder_t encoder, const char* prefix)
{
    assert(encoder != nullptr);
    encoder->m_impl.set_zone_prefix(std::string(prefix));
}
