// Copyright Steinwurf ApS 2018.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <kodo_rlnc_c/encoder.h>
#include <kodo_rlnc_c/decoder.h>

/// @example use_log_layers.c
///
/// Simple example to show the log functionality.

// Helper function to determine if a string ends with a certain suffix
int ends_with(const char* str, const char* suffix)
{
    if (!str || !suffix) return 0;
    size_t len_str = strlen(str);
    size_t len_suffix = strlen(suffix);
    if (len_suffix > len_str) return 0;
    return strncmp(str + len_str - len_suffix, suffix, len_suffix) == 0;
}

// This callback function will be called when the decoder produces log output
void log_callback(const char* zone, const char* data, void* context)
{
    (void) context;
    // The zone string starts with our custom prefix, so it is easier to
    // check if the zone ends with a given suffix
    if (ends_with(zone, "decoder_state") ||
        ends_with(zone, "symbol_coefficients_before_read_symbol") ||
        ends_with(zone, "symbol_index_before_read_decoded_symbol"))
    {
        printf("%s:\n", zone);
        printf("%s\n", data);
    }
}

int main()
{
    // Seed random number generator to produce different results every time
    srand(time(NULL));

    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t symbols = 3;
    uint32_t symbol_size = 16;
    int32_t finite_field = krlnc_binary8;

    krlnc_encoder_t encoder = krlnc_create_encoder(
        finite_field, symbols, symbol_size);

    krlnc_decoder_t decoder = krlnc_create_decoder(
        finite_field, symbols, symbol_size);

    // Allocate some storage for a "payload" the payload is what we would
    // eventually send over a network
    uint32_t payload_size = krlnc_encoder_max_payload_size(encoder);
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

    // Install the stdout log function for the encoder (everything will
    // be printed to stdout without filtering)
    krlnc_encoder_set_log_stdout(encoder);
    // Set a custom zone prefix for the encoder (this helps to differentiate
    // the log output of the encoder and the decoder)
    krlnc_encoder_set_zone_prefix(encoder, "Encoder");

    // Install a custom log function for the decoder (we can process and
    // filter the data in our log callback)
    krlnc_decoder_set_log_callback(decoder, log_callback, NULL);
    // Set a custom zone prefix for the decoder
    krlnc_decoder_set_zone_prefix(decoder, "Decoder");

    krlnc_encoder_set_symbols_storage(encoder, data_in);

    uint8_t* data_out = (uint8_t*) malloc(block_size);
    krlnc_decoder_set_symbols_storage(decoder, data_out);

    while (!krlnc_decoder_is_complete(decoder))
    {
        krlnc_encoder_produce_payload(encoder, payload);

        if ((rand() % 2) == 0)
        {
            continue;
        }

        krlnc_decoder_consume_payload(decoder, payload);
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
