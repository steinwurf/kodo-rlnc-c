// Copyright Steinwurf ApS 2016.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodo_rlnc_c/kodo_rlnc_c.h>

#include <algorithm>
#include <vector>

#include <gtest/gtest.h>

TEST(test_kodo_rlnc_c, factory_api)
{
    uint32_t symbols = 50;
    uint32_t symbol_size = 750;
    auto decoder_factory = krlnc_new_decoder_factory(
        krlnc_binary8, symbols, symbol_size);

    auto encoder_factory = krlnc_new_encoder_factory(
        krlnc_binary8, symbols, symbol_size);

    EXPECT_EQ(symbols, krlnc_decoder_factory_symbols(decoder_factory));
    EXPECT_EQ(symbols, krlnc_encoder_factory_symbols(encoder_factory));

    EXPECT_EQ(symbol_size, krlnc_encoder_factory_symbol_size(encoder_factory));
    EXPECT_EQ(symbol_size, krlnc_decoder_factory_symbol_size(decoder_factory));

    uint32_t new_symbols = 25;

    krlnc_encoder_factory_set_symbols(encoder_factory, new_symbols);
    EXPECT_EQ(new_symbols, krlnc_encoder_factory_symbols(encoder_factory));

    krlnc_decoder_factory_set_symbols(decoder_factory, new_symbols);
    EXPECT_EQ(new_symbols, krlnc_decoder_factory_symbols(decoder_factory));

    uint32_t new_symbol_size = 300;

    krlnc_encoder_factory_set_symbol_size(encoder_factory, new_symbol_size);
    EXPECT_EQ(new_symbol_size, krlnc_encoder_factory_symbol_size(encoder_factory));

    krlnc_decoder_factory_set_symbol_size(decoder_factory, new_symbol_size);
    EXPECT_EQ(new_symbol_size, krlnc_decoder_factory_symbol_size(decoder_factory));

    krlnc_delete_decoder_factory(decoder_factory);
    krlnc_delete_encoder_factory(encoder_factory);
}

TEST(test_kodo_rlnc_c, api)
{
    uint32_t symbols = 50;
    uint32_t symbol_size = 750;
    auto decoder_factory = krlnc_new_decoder_factory(
        krlnc_binary8, symbols, symbol_size);

    auto encoder_factory = krlnc_new_encoder_factory(
        krlnc_binary8, symbols, symbol_size);

    auto decoder = krlnc_decoder_factory_build(decoder_factory);
    auto encoder = krlnc_encoder_factory_build(encoder_factory);

    EXPECT_EQ(
        krlnc_decoder_block_size(decoder),
        krlnc_encoder_block_size(encoder));

    EXPECT_EQ(symbol_size, krlnc_decoder_symbol_size(decoder));
    EXPECT_EQ(symbols, krlnc_decoder_symbols(decoder));

    EXPECT_EQ(symbol_size, krlnc_encoder_symbol_size(encoder));
    EXPECT_EQ(symbols, krlnc_encoder_symbols(encoder));

    std::vector<uint8_t> data_in(krlnc_encoder_block_size(encoder));
    std::generate(data_in.begin(), data_in.end(), rand);
    krlnc_encoder_set_const_symbols(
        encoder, data_in.data(), data_in.size());

    std::vector<uint8_t> data_out(krlnc_decoder_block_size(decoder));
    krlnc_decoder_set_mutable_symbols(
        decoder, data_out.data(), data_out.size());

    EXPECT_LE(
        krlnc_encoder_payload_size(encoder),
        krlnc_decoder_payload_size(decoder));

    std::vector<uint8_t> payload(krlnc_encoder_payload_size(encoder));

    EXPECT_TRUE(krlnc_is_systematic_on(encoder));
    krlnc_encoder_set_systematic_off(encoder);
    EXPECT_FALSE(krlnc_is_systematic_on(encoder));
    krlnc_set_systematic_on(encoder);
    EXPECT_TRUE(krlnc_is_systematic_on(encoder));
    krlnc_encoder_set_systematic_off(encoder);

    EXPECT_EQ(0U, krlnc_decoder_rank(decoder));
    while (!krlnc_decoder_is_complete(decoder))
    {
        krlnc_encoder_write_payload(encoder, payload.data());
        krlnc_decoder_read_payload(decoder, payload.data());
    }
    EXPECT_EQ(symbols, krlnc_decoder_rank(decoder));

    EXPECT_EQ(data_in, data_out);

    krlnc_delete_decoder(decoder);
    krlnc_delete_encoder(encoder);
    krlnc_delete_decoder_factory(decoder_factory);
    krlnc_delete_encoder_factory(encoder_factory);
}
