// Copyright Steinwurf ApS 2018.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "encoder.h"

#include <cstring>
#include <cstdint>
#include <cassert>
#include <string>

#include <storage/storage.hpp>
#include <kodo_rlnc/coders.hpp>

#include "convert_enums.hpp"

struct krlnc_encoder
{
    kodo_rlnc::encoder::factory::pointer m_impl;
};

struct krlnc_encoder_factory
{
    template<class... Args>
    krlnc_encoder_factory(Args&&... args) :
        m_impl(std::forward<Args>(args)...)
    { }

    kodo_rlnc::encoder::factory m_impl;
};

//------------------------------------------------------------------
// ENCODER FACTORY API
//------------------------------------------------------------------

krlnc_encoder_factory_t krlnc_new_encoder_factory(
    int32_t finite_field_id, uint32_t symbols, uint32_t symbol_size)
{
    auto finite_field = c_field_to_krlnc_field(finite_field_id);
    return new krlnc_encoder_factory(finite_field, symbols, symbol_size);
}

void krlnc_delete_encoder_factory(krlnc_encoder_factory_t factory)
{
    assert(factory != nullptr);
    delete factory;
}

uint32_t krlnc_encoder_factory_symbols(krlnc_encoder_factory_t factory)
{
    assert(factory != nullptr);
    return factory->m_impl.symbols();
}

uint32_t krlnc_encoder_factory_symbol_size(krlnc_encoder_factory_t factory)
{
    assert(factory != nullptr);
    return factory->m_impl.symbol_size();
}

void krlnc_encoder_factory_set_symbols(krlnc_encoder_factory_t factory,
                                       uint32_t symbols)
{
    assert(factory != nullptr);
    factory->m_impl.set_symbols(symbols);
}

void krlnc_encoder_factory_set_symbol_size(
    krlnc_encoder_factory_t factory, uint32_t symbol_size)
{
    assert(factory != nullptr);
    factory->m_impl.set_symbol_size(symbol_size);
}

void krlnc_encoder_factory_set_coding_vector_format(
    krlnc_encoder_factory_t factory, int32_t format_id)
{
    auto format = c_format_to_krlnc_format(format_id);
    factory->m_impl.set_coding_vector_format(format);
}

krlnc_encoder_t krlnc_encoder_factory_build(krlnc_encoder_factory_t factory)
{
    assert(factory != nullptr);
    auto encoder = new krlnc_encoder();
    encoder->m_impl = factory->m_impl.build();
    return encoder;
}

void krlnc_delete_encoder(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    delete encoder;
}

//------------------------------------------------------------------
// SYMBOL STORAGE API
//------------------------------------------------------------------

uint32_t krlnc_encoder_block_size(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl->block_size();
}

uint32_t krlnc_encoder_symbol_size(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl->symbol_size();
}

uint32_t krlnc_encoder_symbols(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl->symbols();
}

void krlnc_encoder_set_const_symbol(
    krlnc_encoder_t encoder, uint32_t index, uint8_t* data, uint32_t size)
{
    assert(encoder != nullptr);
    encoder->m_impl->set_const_symbol(index, storage::storage(data, size));
}

void krlnc_encoder_set_const_symbols(
    krlnc_encoder_t encoder, uint8_t* data, uint32_t size)
{
    assert(encoder != nullptr);
    encoder->m_impl->set_const_symbols(storage::storage(data, size));
}

//------------------------------------------------------------------
// PAYLOAD API
//------------------------------------------------------------------

uint32_t krlnc_encoder_payload_size(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl->payload_size();
}

uint32_t krlnc_encoder_write_payload(krlnc_encoder_t encoder, uint8_t* payload)
{
    assert(encoder != nullptr);
    return encoder->m_impl->write_payload(payload);
}

//------------------------------------------------------------------
// ENCODER API
//------------------------------------------------------------------

uint32_t krlnc_encoder_rank(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl->rank();
}

uint8_t krlnc_encoder_is_systematic_on(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl->is_systematic_on();
}

void krlnc_encoder_set_systematic_on(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    encoder->m_impl->set_systematic_on();
}

void krlnc_encoder_set_systematic_off(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    encoder->m_impl->set_systematic_off();
}

uint8_t krlnc_encoder_in_systematic_phase(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl->in_systematic_phase();
}

//------------------------------------------------------------------
// SYMBOL API
//------------------------------------------------------------------

uint32_t krlnc_encoder_coefficient_vector_size(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl->coefficient_vector_size();
}

uint32_t krlnc_encoder_write_symbol(
    krlnc_encoder_t encoder, uint8_t* symbol_data, uint8_t* coefficients)
{
    assert(encoder != nullptr);
    return encoder->m_impl->write_symbol(symbol_data, coefficients);
}

uint32_t krlnc_encoder_write_uncoded_symbol(
    krlnc_encoder_t encoder, uint8_t* symbol_data, uint32_t index)
{
    assert(encoder != nullptr);
    return encoder->m_impl->write_uncoded_symbol(symbol_data, index);
}

//------------------------------------------------------------------
// COEFFICIENT GENERATOR API
//------------------------------------------------------------------

void krlnc_encoder_set_seed(krlnc_encoder_t encoder, uint32_t seed_value)
{
    assert(encoder != nullptr);
    encoder->m_impl->set_seed(seed_value);
}

void krlnc_encoder_generate(krlnc_encoder_t encoder, uint8_t* coefficients)
{
    assert(encoder != nullptr);
    encoder->m_impl->generate(coefficients);
}

void krlnc_encoder_generate_partial(
    krlnc_encoder_t encoder, uint8_t* coefficients)
{
    assert(encoder != nullptr);
    encoder->m_impl->generate_partial(coefficients);
}

float krlnc_encoder_density(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl->density();
}

void krlnc_encoder_set_density(krlnc_encoder_t encoder, float density)
{
    assert(encoder != nullptr);
    encoder->m_impl->set_density(density);
}

//------------------------------------------------------------------
// TRACE API
//------------------------------------------------------------------

void krlnc_encoder_set_trace_stdout(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    encoder->m_impl->set_trace_stdout();
}

void krlnc_encoder_set_trace_callback(
    krlnc_encoder_t encoder, krlnc_trace_callback_t c_callback, void* context)
{
    assert(c_callback);
    assert(encoder != nullptr);

    auto callback = [c_callback, context](const std::string& zone,
                                          const std::string& data)
    {
        c_callback(zone.c_str(), data.c_str(), context);
    };
    encoder->m_impl->set_trace_callback(callback);
}

void krlnc_encoder_set_trace_off(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    encoder->m_impl->set_trace_off();
}

void krlnc_encoder_set_zone_prefix(krlnc_encoder_t encoder, const char* prefix)
{
    assert(encoder != nullptr);
    encoder->m_impl->set_zone_prefix(std::string(prefix));
}
