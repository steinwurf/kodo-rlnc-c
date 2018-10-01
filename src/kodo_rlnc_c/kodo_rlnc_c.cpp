// Copyright Steinwurf ApS 2018.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "encoder.h"
#include "decoder.h"

#include <cstring>
#include <cstdint>
#include <cassert>
#include <string>

#include <storage/storage.hpp>
#include <kodo_rlnc/coders.hpp>

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

fifi::api::field c_field_to_krlnc_field(int32_t c_field)
{
    switch (c_field)
    {
    case krlnc_binary:
        return fifi::api::field::binary;
    case krlnc_binary4:
        return fifi::api::field::binary4;
    case krlnc_binary8:
        return fifi::api::field::binary8;
    case krlnc_binary16:
        return fifi::api::field::binary16;
    default:
        assert(false && "Unknown field");
        return fifi::api::field::binary;
    }
}

kodo_rlnc::coding_vector_format c_format_to_krlnc_format(int32_t c_format)
{
    switch (c_format)
    {
    case krlnc_full_vector:
        return kodo_rlnc::coding_vector_format::full_vector;
    case krlnc_seed:
        return kodo_rlnc::coding_vector_format::seed;
    case krlnc_sparse_seed:
        return kodo_rlnc::coding_vector_format::sparse_seed;
    default:
        assert(false && "Unknown coding vector format");
        return kodo_rlnc::coding_vector_format::full_vector;
    }
}

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
// PAYLOAD API DECODER
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
// PAYLOAD API ENCODER
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
// SYMBOL STORAGE API DECODER
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
// SYMBOL STORAGE API ENCODER
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

//------------------------------------------------------------------
// COEFFICIENT GENERATOR API
//------------------------------------------------------------------

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
