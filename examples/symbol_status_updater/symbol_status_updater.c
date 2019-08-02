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

/// @example symbol_status_updater.c
///
/// Simple example showing the result of enabling the symbol status updater.
int main()
{
    // Seed the random number generator to produce different data every time
    srand(time(NULL));

    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t symbols = 50;
    uint32_t symbol_size = 160;

    // Here we select the finite field to use.
    // To show the effect of the symbol status updater we need to use a
    // smaller field.
    int32_t finite_field = krlnc_binary;

    krlnc_encoder_t encoder = krlnc_create_encoder(
        finite_field, symbols, symbol_size);

    krlnc_decoder_t decoder1 = krlnc_create_decoder(
        finite_field, symbols, symbol_size);

    // Create a second decoder where the status updater will be turned off.
    krlnc_decoder_t decoder2 = krlnc_create_decoder(
        finite_field, symbols, symbol_size);

    krlnc_decoder_set_status_updater_on(decoder2);

    printf("decoder 1 status updater: %d\n",
           krlnc_decoder_is_status_updater_enabled(decoder1));
    printf("decoder 2 status updater: %d\n",
           krlnc_decoder_is_status_updater_enabled(decoder2));

    uint32_t payload_size = krlnc_encoder_max_payload_size(encoder);
    uint8_t* payload1 = (uint8_t*) malloc(payload_size);
    uint8_t* payload2 = (uint8_t*) malloc(payload_size);

    uint32_t block_size = krlnc_encoder_block_size(encoder);
    uint8_t* data_in = (uint8_t*) malloc(block_size);
    uint8_t* data_out1 = (uint8_t*) malloc(block_size);
    uint8_t* data_out2 = (uint8_t*) malloc(block_size);

    uint32_t i = 0;
    for (; i < block_size; ++i)
        data_in[i] = rand() % 256;

    krlnc_encoder_set_symbols_storage(encoder, data_in);
    krlnc_decoder_set_symbols_storage(decoder1, data_out1);
    krlnc_decoder_set_symbols_storage(decoder2, data_out2);

    // Skip the systematic phase as the effect of the symbol status decoder is
    // only visible when reading coded packets.
    krlnc_encoder_set_systematic_off(encoder);

    while (!krlnc_decoder_is_complete(decoder1))
    {
        krlnc_encoder_produce_payload(encoder, payload1);
        // Copy payload1, because the decoder modifies the payload buffer
        // during read_payload, so we cannot use the same payload twice!
        memcpy(payload2, payload1, payload_size);

        // Pass the generated packet to the decoder
        krlnc_decoder_consume_payload(decoder1, payload1);
        krlnc_decoder_consume_payload(decoder2, payload2);
        printf("decoder1: %d\n", krlnc_decoder_symbols_decoded(decoder1));
        printf("decoder2: %d\n", krlnc_decoder_symbols_decoded(decoder2));
        printf("-----------------\n");
    }

    if (memcmp(data_in, data_out1, block_size) == 0 &&
        memcmp(data_in, data_out2, block_size) == 0)
    {
        printf("Data decoded correctly\n");
    }
    else
    {
        printf("Unexpected failure to decode, please file a bug report :)\n");
    }

    free(data_in);
    free(data_out1);
    free(data_out2);
    free(payload1);
    free(payload2);

    krlnc_delete_encoder(encoder);
    krlnc_delete_decoder(decoder1);
    krlnc_delete_decoder(decoder2);

    return 0;
}
