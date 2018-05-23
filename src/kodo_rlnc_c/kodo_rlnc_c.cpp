// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "kodo_rlnc_c.h"

#include <cstring>
#include <cstdint>
#include <cassert>
#include <string>

#include <storage/storage.hpp>
#include <kodo_rlnc/on_the_fly_codes.hpp>
#include "on_the_fly_encoder_stack.hpp"
#include "on_the_fly_decoder_stack.hpp"
namespace {
fifi::api::field build_finite_field(int32_t finite_field_id)
{
    switch (finite_field_id)
    {
        case krlnc_binary:
            return fifi::api::field::binary;
        case krlnc_binary4:
            return fifi::api::field::binary4;
        case krlnc_binary8:
            return fifi::api::field::binary8;
    default:
        assert(false && "Unknown field");
        return fifi::api::field::binary;
    }
}
}

struct krlnc_decoder
{
    kodo_rlnc_c::on_the_fly_decoder_stack m_impl;
};

struct krlnc_decoder_factory
{
    template<class... Args>
    krlnc_decoder_factory(Args&&... args) :
        m_impl(std::forward<Args>(args)...)
    { }
    kodo_rlnc_c::on_the_fly_decoder_stack::config m_impl;
};

struct krlnc_encoder
{
    kodo_rlnc_c::on_the_fly_encoder_stack m_impl;
};

struct krlnc_encoder_factory
{
    template<class... Args>
    krlnc_encoder_factory(Args&&... args) :
        m_impl(std::forward<Args>(args)...)
    { }

    kodo_rlnc_c::on_the_fly_encoder_stack::config m_impl;
};

//------------------------------------------------------------------
// ENCODER FACTORY API
//------------------------------------------------------------------

krlnc_encoder_factory_t krlnc_new_encoder_factory(
    int32_t finite_field_id, uint32_t symbols, uint32_t symbol_size)
{
    auto finite_field = build_finite_field(finite_field_id);
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

void krlnc_encoder_factory_set_symbols(krlnc_encoder_factory_t factory, uint32_t symbols)
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

krlnc_encoder_t krlnc_encoder_factory_build(krlnc_encoder_factory_t factory)
{
    assert(factory != nullptr);
    auto encoder = new krlnc_encoder();
    encoder->m_impl.initialize(factory->m_impl);
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
    auto finite_field = build_finite_field(finite_field_id);
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

void krlnc_decoder_factory_set_symbols(krlnc_decoder_factory_t factory, uint32_t symbols)
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

krlnc_decoder_t krlnc_decoder_factory_build(krlnc_decoder_factory_t factory)
{
    assert(factory != nullptr);
    auto decoder = new krlnc_decoder();
    decoder->m_impl.initialize(factory->m_impl);
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
    return decoder->m_impl.payload_size();
}

void krlnc_decoder_read_payload(krlnc_decoder_t decoder, uint8_t* payload)
{
    assert(decoder != nullptr);
    decoder->m_impl.read_payload(payload);
}

//------------------------------------------------------------------
// PAYLOAD API ENCODER
//------------------------------------------------------------------

uint32_t krlnc_encoder_payload_size(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl.payload_size();
}

uint32_t krlnc_encoder_write_payload(krlnc_encoder_t encoder, uint8_t* payload)
{
    assert(encoder != nullptr);
    return encoder->m_impl.write_payload(payload);
}

//------------------------------------------------------------------
// SYMBOL STORAGE API DECODER
//------------------------------------------------------------------

uint32_t krlnc_decoder_block_size(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl.block_size();
}

uint32_t krlnc_decoder_symbol_size(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl.symbol_size();
}

uint32_t krlnc_decoder_symbols(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl.symbols();
}

void krlnc_decoder_set_mutable_symbols(
    krlnc_decoder_t decoder, uint8_t* data, uint32_t size)
{
    assert(decoder != nullptr);
    decoder->m_impl.set_mutable_symbols(storage::storage(data, size));
}

//------------------------------------------------------------------
// SYMBOL STORAGE API ENCODER
//------------------------------------------------------------------

uint32_t krlnc_encoder_block_size(krlnc_encoder_t encoder)
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

void krlnc_encoder_set_const_symbols(
    krlnc_encoder_t encoder, uint8_t* data, uint32_t size)
{
    assert(encoder != nullptr);
    encoder->m_impl.set_const_symbols(storage::storage(data, size));
}

//------------------------------------------------------------------
// DECODER API
//------------------------------------------------------------------

uint8_t krlnc_decoder_is_complete(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl.is_complete();
}

uint32_t krlnc_decoder_rank(krlnc_decoder_t decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl.rank();
}

//------------------------------------------------------------------
// ENCODER API
//------------------------------------------------------------------

uint8_t krlnc_is_systematic_on(krlnc_encoder_t encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl.is_systematic_on();
}

void krlnc_set_systematic_on(krlnc_encoder_t encoder)
{
    encoder->m_impl.set_systematic_on();
}

void krlnc_encoder_set_systematic_off(krlnc_encoder_t encoder)
{
    encoder->m_impl.set_systematic_off();
}
