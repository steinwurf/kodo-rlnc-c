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

/// @example encode_decode_simple.c
///
/// Simple example showing how to encode and decode a block
/// of memory.

int main()
{
    // Seed the random number generator to produce different data every time
    srand((uint32_t)time(NULL));

    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t symbols = 10;
    uint32_t symbol_size = 100;

    // Here we select the finite field to use.
    // Some common choices are: krlnc_binary, krlnc_binary4, krlnc_binary8
    int32_t finite_field = krlnc_binary;

    // First, we create an encoder and a decoder
    krlnc_encoder_t encoder = krlnc_create_encoder(
        finite_field, symbols, symbol_size);

    krlnc_decoder_t decoder = krlnc_create_decoder(
        finite_field, symbols, symbol_size);

    uint32_t bytes_used;
    uint32_t payload_size = krlnc_encoder_max_payload_size(encoder);
    uint8_t* payload = (uint8_t*) malloc(payload_size);

    size_t block_size = (size_t) krlnc_encoder_block_size(encoder);
    uint8_t* data_in = (uint8_t*) malloc(block_size);
    uint8_t* data_out = (uint8_t*) malloc(block_size);

    uint64_t i = 0;
    for (; i < block_size; ++i)
        data_in[i] = rand() % 256;

    krlnc_encoder_set_symbols_storage(encoder, data_in);
    krlnc_decoder_set_symbols_storage(decoder, data_out);

    // Most of the network coding algorithms supports a mode of operation
    // which is known as systematic coding. This basically means that
    // initially all symbols are sent once un-coded. The rational behind this
    // is that if no errors occur during the transmission we will not have
    // performed any unnecessary coding operations. An encoder will exit the
    // systematic phase automatically once all symbols have been sent un-coded
    // once.
    if (krlnc_encoder_is_systematic_on(encoder))
    {
        printf("Systematic encoding enabled\n");
    }
    else
    {
        printf("Systematic encoding disabled\n");
    }

    // If we do not wish to use systematic encoding, but to do full coding
    // from the beginning we can turn systematic coding off:
    //
    // krlnc_encoder_set_systematic_off(encoder);

    while (!krlnc_decoder_is_complete(decoder))
    {
        // The encoder will use a certain amount of bytes of the payload
        // buffer. It will never use more than payload_size, but it might
        // use less.
        bytes_used = krlnc_encoder_produce_payload(encoder, payload);
        printf("Payload generated by encoder, rank = %d, bytes used = %d\n",
               krlnc_encoder_rank(encoder), bytes_used);

        // Pass the generated packet to the decoder
        krlnc_decoder_consume_payload(decoder, payload);
        printf("Payload processed by decoder, current rank = %d\n",
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
    free(payload);

    krlnc_delete_encoder(encoder);
    krlnc_delete_decoder(decoder);

    return 0;
}
