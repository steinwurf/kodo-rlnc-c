// Copyright Steinwurf ApS 2018.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "decoder.h"

#include <cstring>
#include <cstdint>
#include <cassert>
#include <string>

#include <storage/storage.hpp>
#include <kodo_rlnc/coders.hpp>

#include "convert_enums.hpp"

struct krlnc_decoder
{
    kodo_rlnc::decoder::factory::pointer m_impl;
};

struct krlnc_decoder_factory
{
    template<class... Args>
    krlnc_decoder_factory(Args&&... args) :
        m_impl(std::forward<Args>(args)...)
    { }
    kodo_rlnc::decoder::factory m_impl;
};

//------------------------------------------------------------------
// DECODER FACTORY API
//------------------------------------------------------------------

krlnc_decoder_factory_t krlnc_new_decoder_factory(
    int32_t finite_field_id, uint32_t symbols, uint32_t symbol_size)
{
    auto finite_field = c_field_to_krlnc_field(finite_field_id);
    return new krlnc_decoder_factory(finite_field, symbols, symbol_size);
}

void krlnc_delete_decoder_factory(krlnc_decoder_factory_t factory)
{
    assert(factory != nullptr);
    delete factory;
}

uint32_t krlnc_decoder_factory_symbols(krlnc_decoder_factory_t factory)
{
    assert(factory != nullptr);
    return factory->m_impl.symbols();
}

uint32_t krlnc_decoder_factory_symbol_size(krlnc_decoder_factory_t factory)
{
    assert(factory != nullptr);
    return factory->m_impl.symbol_size();
}

void krlnc_decoder_factory_set_symbols(krlnc_decoder_factory_t factory,
                                       uint32_t symbols)
{
    assert(factory != nullptr);
    factory->m_impl.set_symbols(symbols);
}

void krlnc_decoder_factory_set_symbol_size(
    krlnc_decoder_factory_t factory, uint32_t symbol_size)
{
    assert(factory != nullptr);
    factory->m_impl.set_symbol_size(symbol_size);
}

void krlnc_decoder_factory_set_coding_vector_format(
    krlnc_decoder_factory_t factory, int32_t format_id)
{
    auto format = c_format_to_krlnc_format(format_id);
    factory->m_impl.set_coding_vector_format(format);
}

krlnc_decoder_t krlnc_decoder_factory_build(krlnc_decoder_factory_t factory)
{
    assert(factory != nullptr);
    auto decoder = new krlnc_decoder();
    decoder->m_impl = factory->m_impl.build();
    return decoder;
}

void krlnc_delete_decoder(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    delete decoder;
}

//------------------------------------------------------------------
// SYMBOL STORAGE API
//------------------------------------------------------------------

uint32_t krlnc_decoder_block_size(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl->block_size();
}

uint32_t krlnc_decoder_symbol_size(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl->symbol_size();
}

uint32_t krlnc_decoder_symbols(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl->symbols();
}

void krlnc_decoder_set_mutable_symbol(
    krlnc_decoder_t decoder, uint32_t index, uint8_t* data, uint32_t size)
{
    assert(decoder != nullptr);
    decoder->m_impl->set_mutable_symbol(index, storage::storage(data, size));
}

void krlnc_decoder_set_mutable_symbols(
    krlnc_decoder_t decoder, uint8_t* data, uint32_t size)
{
    assert(decoder != nullptr);
    decoder->m_impl->set_mutable_symbols(storage::storage(data, size));
}

//------------------------------------------------------------------
// PAYLOAD API
//------------------------------------------------------------------

uint32_t krlnc_decoder_payload_size(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl->payload_size();
}

void krlnc_decoder_read_payload(krlnc_decoder_t decoder, uint8_t* payload)
{
    assert(decoder != nullptr);
    decoder->m_impl->read_payload(payload);
}

uint32_t krlnc_decoder_write_payload(krlnc_decoder_t decoder, uint8_t* payload)
{
    assert(decoder != nullptr);
    return decoder->m_impl->write_payload(payload);
}

//------------------------------------------------------------------
// DECODER API
//------------------------------------------------------------------

uint8_t krlnc_decoder_is_complete(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl->is_complete();
}

uint8_t krlnc_decoder_is_partially_complete(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl->is_partially_complete();
}

uint32_t krlnc_decoder_rank(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl->rank();
}

uint32_t krlnc_decoder_symbols_missing(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl->symbols_missing();
}

uint32_t krlnc_decoder_symbols_partially_decoded(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl->symbols_partially_decoded();
}

uint32_t krlnc_decoder_symbols_uncoded(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl->symbols_uncoded();
}

uint8_t krlnc_decoder_is_symbol_missing(krlnc_decoder_t decoder, uint32_t index)
{
    assert(decoder != nullptr);
    return decoder->m_impl->is_symbol_missing(index);
}

uint8_t krlnc_decoder_is_symbol_partially_decoded(
    krlnc_decoder_t decoder, uint32_t index)
{
    assert(decoder != nullptr);
    return decoder->m_impl->is_symbol_partially_decoded(index);
}

uint8_t krlnc_decoder_is_symbol_uncoded(krlnc_decoder_t decoder, uint32_t index)
{
    assert(decoder != nullptr);
    return decoder->m_impl->is_symbol_uncoded(index);
}

uint8_t krlnc_decoder_is_symbol_pivot(krlnc_decoder_t decoder, uint32_t index)
{
    assert(decoder != nullptr);
    return decoder->m_impl->is_symbol_pivot(index);
}

void krlnc_decoder_update_symbol_status(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    decoder->m_impl->update_symbol_status();
}

void krlnc_decoder_set_status_updater_on(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    decoder->m_impl->set_status_updater_on();
}

void krlnc_decoder_set_status_updater_off(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    decoder->m_impl->set_status_updater_off();
}

uint8_t krlnc_decoder_is_status_updater_enabled(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl->is_status_updater_enabled();
}

//------------------------------------------------------------------
// SYMBOL API
//------------------------------------------------------------------

uint32_t krlnc_decoder_coefficient_vector_size(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl->coefficient_vector_size();
}

void krlnc_decoder_read_symbol(
    krlnc_decoder_t decoder, uint8_t* symbol_data, uint8_t* coefficients)
{
    assert(decoder != nullptr);
    decoder->m_impl->read_symbol(symbol_data, coefficients);
}

void krlnc_decoder_read_uncoded_symbol(
    krlnc_decoder_t decoder, uint8_t* symbol_data, uint32_t index)
{
    assert(decoder != nullptr);
    decoder->m_impl->read_uncoded_symbol(symbol_data, index);
}

//------------------------------------------------------------------
// COEFFICIENT GENERATOR API
//------------------------------------------------------------------

void krlnc_decoder_set_seed(krlnc_decoder_t decoder, uint32_t seed_value)
{
    assert(decoder != nullptr);
    decoder->m_impl->set_seed(seed_value);
}

void krlnc_decoder_generate(krlnc_decoder_t decoder, uint8_t* coefficients)
{
    assert(decoder != nullptr);
    decoder->m_impl->generate(coefficients);
}

void krlnc_decoder_generate_partial(
    krlnc_decoder_t decoder, uint8_t* coefficients)
{
    assert(decoder != nullptr);
    decoder->m_impl->generate_partial(coefficients);
}

//------------------------------------------------------------------
// TRACE API
//------------------------------------------------------------------

void krlnc_decoder_set_trace_stdout(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    decoder->m_impl->set_trace_stdout();
}

void krlnc_decoder_set_trace_callback(
    krlnc_decoder_t decoder, krlnc_trace_callback_t c_callback, void* context)
{
    assert(c_callback);
    assert(decoder != nullptr);

    auto callback = [c_callback, context](const std::string& zone,
                                          const std::string& data)
    {
        c_callback(zone.c_str(), data.c_str(), context);
    };
    decoder->m_impl->set_trace_callback(callback);
}

void krlnc_decoder_set_trace_off(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    decoder->m_impl->set_trace_off();
}

void krlnc_decoder_set_zone_prefix(krlnc_decoder_t decoder, const char* prefix)
{
    assert(decoder != nullptr);
    decoder->m_impl->set_zone_prefix(std::string(prefix));
}
