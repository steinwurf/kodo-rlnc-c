// Copyright Steinwurf ApS 2018.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <stdint.h>
#include <stdlib.h>
#include <kodo_rlnc_c/decoder.h>

#ifdef _WIN32
    #include <winsock2.h>
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
#endif

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <assert.h>

// Count the total number of packets received in order to decode
unsigned int rx_packets;

static void exit_on_sigint(int sig)
{
    (void) sig;
    printf("\nTotal number of received packets: %d\n", rx_packets);
    exit(0);
}


int main(int argc, char* argv[])
{
    // Variables needed for the network / socket usage
    int32_t socket_descriptor = 0;
    int32_t return_code = 0;
    int32_t bytes_received = 0;
    socklen_t remote_address_size;
    struct sockaddr_in remote_address;
    struct sockaddr_in local_address;

    // Variables needed for the coding
    uint32_t symbols = 32;
    uint32_t symbol_size = 160;
    int32_t finite_field = krlnc_binary8;

    krlnc_decoder_factory_t decoder_factory = NULL;
    krlnc_decoder_t decoder = NULL;

    // The buffer used to receive incoming packets
    uint32_t payload_size = 0;
    uint8_t* payload = 0;

    // Keeps track of which symbols have been decoded
    uint8_t* decoded = (uint8_t*) malloc(sizeof(uint8_t) * symbols);

    // Initialize winsock if on Windows
#ifdef _WIN32

    WORD versionWanted = MAKEWORD(1, 1);
    WSADATA wsaData;

    return_code = WSAStartup(versionWanted, &wsaData);

    if (return_code != 0)
    {
        // Tell the user that we could not find a usable
        // Winsock DLL.
        printf("WSAStartup failed with error: %d\n", return_code);
        exit(1);
    }

#endif

    // Initialize global variables
    rx_packets = 0;

    if (argc < 3)
    {
        printf("usage : %s <port> <symbols>\n", argv[0]);
        exit(1);
    }

    // Socket creation
    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_descriptor < 0)
    {
        printf("%s: cannot open socket \n", argv[0]);
        exit(1);
    }

    // Bind local server port
    local_address.sin_family = AF_INET;
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);
    local_address.sin_port = htons(atoi(argv[1]));
    return_code = bind(socket_descriptor, (struct sockaddr*) &local_address,
                       sizeof(local_address));

    if (return_code < 0)
    {
        printf("%s: cannot bind port number %d \n", argv[0], atoi(argv[1]));
        exit(1);
    }

    // Install signal handler
    signal(SIGINT, exit_on_sigint);

    // Initialize the factory with the chosen symbols and symbol size
    symbols = atoi(argv[2]);

    // Create the encoder factory
    decoder_factory = krlnc_new_decoder_factory(
         finite_field, symbols, symbol_size);
    decoder = krlnc_decoder_factory_build(decoder_factory);

    // Create the buffer needed for the payload
    payload_size = krlnc_decoder_payload_size(decoder);
    payload = (uint8_t*) malloc(payload_size);

    uint32_t block_size = krlnc_decoder_block_size(decoder);
    uint8_t* data_out = (uint8_t*) malloc(block_size);
    krlnc_decoder_set_mutable_symbols(decoder, data_out, block_size);

    // Zero initialize the decoded array */
    memset(decoded, '\0', sizeof(uint8_t) * symbols);

    printf("%s: waiting for data on UDP port %u\n", argv[0], atoi(argv[1]));

    // Receiver loop
    while (!krlnc_decoder_is_complete(decoder))
    {
        // Receive message
        remote_address_size = sizeof(remote_address);

        bytes_received = recvfrom(
            socket_descriptor, payload, payload_size, 0,
            (struct sockaddr*) &remote_address, &remote_address_size);

        if (bytes_received < 0)
        {
            printf("%s: recvfrom error %d\n", argv[0], bytes_received);
            fflush(stdout);
            continue;
        }

        // Print received message
        printf("%s: UDP packet from %s:%u : %d\n",
               argv[0],inet_ntoa(remote_address.sin_addr),
               ntohs(remote_address.sin_port), bytes_received);

        ++rx_packets;

        // Packet got through - pass that packet to the decoder
        krlnc_decoder_read_payload(decoder, payload);

        if (krlnc_decoder_is_partially_complete(decoder))
        {
            uint32_t i = 0;
            for (; i < symbols; ++i)
            {
                if (!krlnc_decoder_is_symbol_uncoded(decoder, i))
                    continue;

                if (!decoded[i])
                {
                    // Update that this symbol now has been decoded,
                    // in a real application we could copy out the symbol
                    // using the krlnc_copy_from_symbol(..) or use the data_out
                    // directly.
                    printf("Symbol %d was decoded\n", i);
                    decoded[i] = 1;
                }
            }
        }
    }

    printf("Data decoded!\n");

    // Cleanup
    free(decoded);
    free(payload);

    krlnc_delete_decoder(decoder);
    krlnc_delete_decoder_factory(decoder_factory);

    return 0;
}
