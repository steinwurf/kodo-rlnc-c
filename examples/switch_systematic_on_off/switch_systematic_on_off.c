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

/// @example switch_systematic_on_off.c
///
/// This example shows how to enable or disable systematic coding for
/// an encoder.
/// Systematic coding is used to reduce the amount of work done by an
/// encoder and a decoder. This is achieved by initially sending all
/// symbols uncoded.

int main()
{
    // Seed random number generator to produce different results every time
    srand(time(NULL));

    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint8_t symbols = 10;
    uint8_t symbol_size = 100;

    int32_t finite_field = krlnc_binary8;

    // In the following we will make an encoder/decoder factory.
    // The factories are used to build actual encoders/decoder
    krlnc_encoder_factory_t encoder_factory = krlnc_new_encoder_factory(
        finite_field, symbols, symbol_size);

    krlnc_decoder_factory_t decoder_factory = krlnc_new_decoder_factory(
        finite_field, symbols, symbol_size);

    krlnc_encoder_t encoder = krlnc_encoder_factory_build(encoder_factory);
    krlnc_decoder_t decoder = krlnc_decoder_factory_build(decoder_factory);

    // Allocate some storage for a "payload" buffer that we would
    // eventually send over a network
    uint32_t payload_size = krlnc_encoder_payload_size(encoder);
    uint8_t* payload = (uint8_t*) malloc(payload_size);

    // Allocate some data to encode. In this case we make a buffer
    // with the same size as the encoder's block size (the max.
    // amount a single encoder can encode)
    uint32_t block_size = krlnc_encoder_block_size(encoder);
    uint8_t* data_in = (uint8_t*) malloc(block_size);

    // Just for fun - fill the data with random data
    uint32_t i = 0;
    for (; i < block_size; ++i)
    {
        data_in[i] = rand() % 256;
    }

    krlnc_encoder_set_const_symbols(encoder, data_in, block_size);

    uint8_t* data_out = (uint8_t*) malloc(block_size);
    krlnc_decoder_set_mutable_symbols(decoder, data_out, block_size);

    printf("Starting encoding / decoding\n");
    while (!krlnc_decoder_is_complete(decoder))
    {
        // With 50% probability toggle systematic
        if ((rand() % 2) == 0)
        {
            if (krlnc_encoder_is_systematic_on(encoder))
            {
                printf("Turning systematic OFF\n");
                krlnc_encoder_set_systematic_off(encoder);
            }
            else
            {
                printf("Turn systematic ON\n");
                krlnc_encoder_set_systematic_on(encoder);
            }
        }

        // Encode a packet into the payload buffer
        krlnc_encoder_write_payload(encoder, payload);

        if ((rand() % 2) == 0)
        {
            printf("Drop packet\n");
            continue;
        }

        // Pass that packet to the decoder
        krlnc_decoder_read_payload(decoder, payload);

        printf("Rank of decoder %d\n", krlnc_decoder_rank(decoder));

        // Symbols that were received in the systematic phase correspond
        // to the original source symbols and are therefore marked as
        // decoded
        printf("Symbols decoded %d\n", krlnc_decoder_symbols_uncoded(decoder));
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
    free(payload);

    krlnc_delete_encoder(encoder);
    krlnc_delete_decoder(decoder);

    krlnc_delete_encoder_factory(encoder_factory);
    krlnc_delete_decoder_factory(decoder_factory);

    return 0;
}
