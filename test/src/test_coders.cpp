// Copyright Steinwurf ApS 2016.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodo_rlnc_c/encoder.h>
#include <kodo_rlnc_c/decoder.h>

#include <algorithm>
#include <vector>

#include <gtest/gtest.h>


void trace_callback(const char* zone, const char* data, void* context)
{
    (void) context;

    if (strcmp(zone, "decoder_state") == 0)
    {
        printf("%s:\n", zone);
        printf("%s\n", data);
    }
}

TEST(test_coders, factory_api)
{
    uint32_t symbols = 50;
    uint32_t symbol_size = 750;
    auto decoder_factory = krlnc_new_decoder_factory(
        krlnc_binary, symbols, symbol_size);

    auto encoder_factory = krlnc_new_encoder_factory(
        krlnc_binary, symbols, symbol_size);

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
    EXPECT_EQ(
        new_symbol_size, krlnc_encoder_factory_symbol_size(encoder_factory));

    krlnc_decoder_factory_set_symbol_size(decoder_factory, new_symbol_size);
    EXPECT_EQ(
        new_symbol_size, krlnc_decoder_factory_symbol_size(decoder_factory));

    krlnc_encoder_factory_set_coding_vector_format(
        encoder_factory, krlnc_seed);
    krlnc_decoder_factory_set_coding_vector_format(
        decoder_factory, krlnc_sparse_seed);

    krlnc_delete_decoder_factory(decoder_factory);
    krlnc_delete_encoder_factory(encoder_factory);
}

TEST(test_coders, basic_api)
{
    uint32_t symbols = 50;
    uint32_t symbol_size = 750;

    auto encoder_factory = krlnc_new_encoder_factory(
        krlnc_binary4, symbols, symbol_size);

    auto decoder_factory = krlnc_new_decoder_factory(
        krlnc_binary4, symbols, symbol_size);

    krlnc_encoder_factory_set_coding_vector_format(
        encoder_factory, krlnc_full_vector);
    krlnc_decoder_factory_set_coding_vector_format(
        decoder_factory, krlnc_full_vector);

    auto encoder = krlnc_encoder_factory_build(encoder_factory);
    auto decoder1 = krlnc_decoder_factory_build(decoder_factory);
    auto decoder2 = krlnc_decoder_factory_build(decoder_factory);

    EXPECT_EQ(
        krlnc_decoder_block_size(decoder1),
        krlnc_encoder_block_size(encoder));

    EXPECT_EQ(
        krlnc_decoder_block_size(decoder2),
        krlnc_encoder_block_size(encoder));

    EXPECT_EQ(symbol_size, krlnc_encoder_symbol_size(encoder));
    EXPECT_EQ(symbols, krlnc_encoder_symbols(encoder));

    EXPECT_EQ(symbol_size, krlnc_decoder_symbol_size(decoder1));
    EXPECT_EQ(symbols, krlnc_decoder_symbols(decoder1));
    EXPECT_EQ(symbol_size, krlnc_decoder_symbol_size(decoder2));
    EXPECT_EQ(symbols, krlnc_decoder_symbols(decoder2));

    std::vector<uint8_t> data_in(krlnc_encoder_block_size(encoder));
    std::generate(data_in.begin(), data_in.end(), rand);

    EXPECT_EQ(0U, krlnc_encoder_rank(encoder));
    krlnc_encoder_set_const_symbols(
        encoder, data_in.data(), data_in.size());
    EXPECT_EQ(symbols, krlnc_encoder_rank(encoder));

    krlnc_encoder_set_systematic_on(encoder);
    EXPECT_TRUE(krlnc_encoder_is_systematic_on(encoder));
    krlnc_encoder_set_systematic_off(encoder);
    EXPECT_FALSE(krlnc_encoder_is_systematic_on(encoder));

    std::vector<uint8_t> data_out1(krlnc_decoder_block_size(decoder1));
    krlnc_decoder_set_mutable_symbols(
        decoder1, data_out1.data(), data_out1.size());

    std::vector<uint8_t> data_out2(krlnc_decoder_block_size(decoder2));
    krlnc_decoder_set_mutable_symbols(
        decoder2, data_out2.data(), data_out2.size());

    EXPECT_LE(
        krlnc_encoder_payload_size(encoder),
        krlnc_decoder_payload_size(decoder1));

    std::vector<uint8_t> payload1(krlnc_encoder_payload_size(encoder));
    std::vector<uint8_t> payload2(krlnc_decoder_payload_size(decoder1));

    EXPECT_EQ(0U, krlnc_decoder_rank(decoder1));
    EXPECT_EQ(0U, krlnc_decoder_rank(decoder2));
    EXPECT_FALSE(krlnc_decoder_is_complete(decoder1));
    EXPECT_EQ(symbols, krlnc_decoder_symbols_missing(decoder1));
    EXPECT_EQ(0U, krlnc_decoder_symbols_uncoded(decoder1));
    EXPECT_EQ(0U, krlnc_decoder_symbols_partially_decoded(decoder1));

    // Initially all symbols should be missing
    for (uint32_t i = 0; i < symbols; ++i)
    {
        EXPECT_TRUE(krlnc_decoder_is_symbol_missing(decoder1, i));
        EXPECT_FALSE(krlnc_decoder_is_symbol_partially_decoded(decoder1, i));
        EXPECT_FALSE(krlnc_decoder_is_symbol_uncoded(decoder1, i));
    }

    krlnc_encoder_set_density(encoder, 0.4f);
    EXPECT_EQ(0.4f, krlnc_encoder_density(encoder));

    while (!krlnc_decoder_is_complete(decoder2))
    {
        krlnc_encoder_write_payload(encoder, payload1.data());
        krlnc_decoder_read_payload(decoder1, payload1.data());

        krlnc_decoder_write_payload(decoder1, payload2.data());
        krlnc_decoder_read_payload(decoder2, payload2.data());
    }
    EXPECT_EQ(symbols, krlnc_decoder_rank(decoder1));
    EXPECT_EQ(symbols, krlnc_decoder_rank(decoder2));

    EXPECT_EQ(data_in, data_out1);
    EXPECT_EQ(data_in, data_out2);

    std::vector<uint8_t> coefficients(
        krlnc_encoder_coefficient_vector_size(encoder));
    krlnc_encoder_set_seed(encoder, 42U);
    krlnc_encoder_generate(encoder, coefficients.data());
    krlnc_encoder_generate_partial(encoder, coefficients.data());
    krlnc_decoder_set_seed(decoder1, 42U);
    krlnc_decoder_generate(decoder1, coefficients.data());
    krlnc_decoder_generate_partial(decoder1, coefficients.data());

    krlnc_encoder_set_zone_prefix(encoder, "encoder");
    krlnc_encoder_set_trace_stdout(encoder);
    krlnc_encoder_set_trace_callback(encoder, trace_callback, NULL);
    krlnc_encoder_set_trace_off(encoder);

    krlnc_decoder_set_zone_prefix(decoder1, "decoder");
    krlnc_decoder_set_trace_stdout(decoder1);
    krlnc_decoder_set_trace_callback(decoder1, trace_callback, NULL);
    krlnc_decoder_set_trace_off(decoder1);

    krlnc_delete_encoder(encoder);
    krlnc_delete_decoder(decoder1);
    krlnc_delete_decoder(decoder2);

    krlnc_delete_encoder_factory(encoder_factory);
    krlnc_delete_decoder_factory(decoder_factory);
}

TEST(test_coders, symbol_status_api)
{
    uint32_t symbols = 4;
    uint32_t symbol_size = 40;
    krlnc_decoder_factory_t decoder_factory = krlnc_new_decoder_factory(
        krlnc_binary8, symbols, symbol_size);

    krlnc_decoder_t decoder = krlnc_decoder_factory_build(decoder_factory);

    std::vector<uint8_t> data_out(krlnc_decoder_block_size(decoder), '\0');
    krlnc_decoder_set_mutable_symbols(
        decoder, data_out.data(), data_out.size());

    std::vector<uint8_t> symbol(krlnc_decoder_symbol_size(decoder));
    std::vector<uint8_t> coefficients(
        krlnc_decoder_coefficient_vector_size(decoder));

    coefficients = {1, 0, 0, 0};
    krlnc_decoder_read_symbol(decoder, symbol.data(), coefficients.data());

    EXPECT_EQ(0U, krlnc_decoder_symbols_uncoded(decoder));

    krlnc_decoder_update_symbol_status(decoder);

    EXPECT_EQ(1U, krlnc_decoder_symbols_uncoded(decoder));

    krlnc_decoder_set_status_updater_on(decoder);

    coefficients = {0, 1, 0, 0};
    krlnc_decoder_read_symbol(decoder, symbol.data(), coefficients.data());

    EXPECT_EQ(2U, krlnc_decoder_symbols_uncoded(decoder));

    krlnc_decoder_set_status_updater_off(decoder);

    krlnc_delete_decoder(decoder);
    krlnc_delete_decoder_factory(decoder_factory);
}
