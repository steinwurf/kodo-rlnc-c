// Copyright Steinwurf ApS 2018.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <kodo_rlnc_c/encoder.h>
#include <kodo_rlnc_c/decoder.h>

/// @example encode_decode_using_coefficients.c
///
/// Example showing how to use the Symbol API with direct coefficient
/// access. Using this approach we have full control over the coding vector
/// (we can even rewrite coefficients if desired). However, we also have to
/// manage extra functionality such as systematic encoding ourselves.

int main()
{
    // Seed the random number generator to produce different data every time
    srand(time(NULL));

    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t symbols = 10;
    uint32_t symbol_size = 100;

    // Here we select the finite field to use
    int32_t finite_field = krlnc_binary8;

    krlnc_encoder_t encoder = krlnc_create_encoder(
        finite_field, symbols, symbol_size);

    krlnc_decoder_t decoder = krlnc_create_decoder(
        finite_field, symbols, symbol_size);

    // Allocate some storage for a coded symbol
    uint8_t* symbol = (uint8_t*) malloc(symbol_size);

    // Allocate some storage for the coding vector coefficients
    uint32_t vector_size = krlnc_encoder_coefficient_vector_size(encoder);
    uint8_t* coefficients = (uint8_t*) malloc(vector_size);

    uint32_t block_size = krlnc_encoder_block_size(encoder);
    uint8_t* data_in = (uint8_t*) malloc(block_size);
    uint8_t* data_out = (uint8_t*) malloc(block_size);

    uint32_t i = 0;
    for (; i < block_size; ++i)
        data_in[i] = rand() % 256;

    krlnc_encoder_set_symbols_storage(encoder, data_in);
    krlnc_decoder_set_symbols_storage(decoder, data_out);

    while (!krlnc_decoder_is_complete(decoder))
    {
        // Generate an encoding vector
        krlnc_encoder_generate(encoder, coefficients);

        // Write a coded symbol to the symbol buffer
        krlnc_encoder_produce_symbol(encoder, symbol, coefficients);

        // Pass that symbol and the corresponding coefficients to the decoder
        krlnc_decoder_consume_symbol(decoder, symbol, coefficients);
        printf("Symbol processed by decoder, current rank = %d\n",
               krlnc_decoder_rank(decoder));
    }

    if (memcmp(data_in, data_out, block_size) == 0)
    {
        printf("Data decoded correctly\n");
    }
    else
    {
        printf("Unexpected failure to decode, please file a bug report :)\n");
    }

    free(data_in);
    free(data_out);
    free(symbol);
    free(coefficients);

    krlnc_delete_encoder(encoder);
    krlnc_delete_decoder(decoder);

    return 0;
}
