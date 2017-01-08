#include <netcode.h>
#include <stdlib.h>
#include <assert.h>

#if    defined(__386__) || defined(i386)    || defined(__i386__)  \
    || defined(__X86)   || defined(_M_IX86)                       \
    || defined(_M_X64)  || defined(__x86_64__)                    \
    || defined(alpha)   || defined(__alpha) || defined(__alpha__) \
    || defined(_M_ALPHA)                                          \
    || defined(ARM)     || defined(_ARM)    || defined(__arm__)   \
    || defined(WIN32)   || defined(_WIN32)  || defined(__WIN32__) \
    || defined(_WIN32_WCE) || defined(__NT__)                     \
    || defined(__MIPSEL__)
  #define NETCODE_LITTLE_ENDIAN 1
#else
  #define NETCODE_BIG_ENDIAN 1
#endif

#define NETCODE_PLATFORM_WINDOWS                    1
#define NETCODE_PLATFORM_MAC                        2
#define NETCODE_PLATFORM_UNIX                       3

#if defined(_WIN32)
#define NETCODE_PLATFORM NETCODE_PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define NETCODE_PLATFORM NETCODE_PLATFORM_MAC
#else
#define NETCODE_PLATFORM NETCODE_PLATFORM_UNIX
#endif

#define NETCODE_MAX_SERVERS_PER_CONNECT 8
#define NETCODE_KEY_BYTES 32
#define NETCODE_MAC_BYTES 16
#define NETCODE_NONCE_BYTES 8
#define NETCODE_CONNECT_TOKEN_BYTES 1024
#define NETCODE_CHALLENGE_TOKEN_BYTES 256

// ----------------------------------------------------------------

#define NETCODE_CONNECTION_REQUEST_PACKET           0
#define NETCODE_CONNECTION_DENIED_PACKET            1
#define NETCODE_CONNECTION_CHALLENGE_PACKET         2
#define NETCODE_CONNECTION_RESPONSE_PACKET          3
#define NETCODE_CONNECTION_CONFIRM_PACKET           4
#define NETCODE_CONNECTION_KEEP_ALIVE_PACKET        5
#define NETCODE_CONNECTION_PAYLOAD_PACKET           6
#define NETCODE_CONNECTION_DISCONNECT_PACKET        7

struct netcode_connection_request_packet_t
{
    uint8_t packet_type;
    uint64_t connect_token_expire_timestamp;
    uint8_t connect_token_nonce[NETCODE_NONCE_BYTES];
    uint8_t connect_token_data[NETCODE_CONNECT_TOKEN_BYTES];
};

struct netcode_connection_denied_packet_t
{
    uint8_t packet_type;
};

struct netcode_connection_challenge_packet_t
{
    uint8_t packet_type;
    uint8_t challenge_token_nonce[NETCODE_NONCE_BYTES];
    uint8_t challenge_token_data[NETCODE_CHALLENGE_TOKEN_BYTES];
};

struct netcode_connection_response_packet_t
{
    uint8_t packet_type;
    uint8_t challenge_token_nonce[NETCODE_NONCE_BYTES];
    uint8_t challenge_token_data[NETCODE_CHALLENGE_TOKEN_BYTES];
};

struct netcode_connection_confirm_packet_t
{
    uint8_t packet_type;
    uint32_t client_index;
};

struct netcode_connection_keep_alive_packet_t
{
    uint8_t packet_type;
};

struct netcode_connection_payload_packet_t
{
    uint8_t packet_type;
};

struct netcode_connection_disconnect_packet_t
{
    uint8_t packet_type;
};

void netcode_write_uint8( uint8_t ** p, uint8_t value )
{
    **p = value;
    ++(*p);
}

void netcode_write_uint16( uint8_t ** p, uint16_t value )
{
    (*p)[0] = value >> 8;
    (*p)[1] = value & 0xFF;
    *p += 2;
}

void netcode_write_uint32( uint8_t ** p, uint32_t value )
{
    (*p)[0] = value >> 24;
    (*p)[1] = ( value >> 16 ) & 0xFF;
    (*p)[2] = ( value >> 8  ) & 0xFF;
    (*p)[3] = value & 0xFF;
    *p += 4;
}

void netcode_write_uint64( uint8_t ** p, uint64_t value )
{
    (*p)[0] = value >> 56;
    (*p)[1] = ( value >> 48 ) & 0xFF;
    (*p)[2] = ( value >> 40 ) & 0xFF;
    (*p)[3] = ( value >> 32 ) & 0xFF;
    (*p)[4] = ( value >> 24 ) & 0xFF;
    (*p)[5] = ( value >> 16 ) & 0xFF;
    (*p)[6] = ( value >> 8  ) & 0xFF;
    (*p)[7] = value & 0xFF;
    *p += 8;
}

void netcode_write_bytes( uint8_t ** p, const uint8_t * byte_array, int num_bytes )
{
    for ( int i = 0; i < num_bytes; ++i )
    {
        netcode_write_uint8( p, byte_array[i] );
    }
}

int netcode_write_packet( void * packet, uint8_t * buffer, int buffer_length )
{
    uint8_t packet_type = ((uint8_t*)packet)[0];

    if ( packet_type == NETCODE_CONNECTION_REQUEST_PACKET )
    {
        // non-encrypted packets (connection request packet only)

        assert( buffer_length >= 1 + 8 + NETCODE_NONCE_BYTES + NETCODE_CONNECT_TOKEN_BYTES );

        struct netcode_connection_request_packet_t * p = (struct netcode_connection_request_packet_t*) NULL;

        uint8_t * start = buffer;

        netcode_write_uint8( &buffer, NETCODE_CONNECTION_REQUEST_PACKET );
        netcode_write_uint64( &buffer, p->connect_token_expire_timestamp );
        netcode_write_bytes( &buffer, p->connect_token_nonce, NETCODE_NONCE_BYTES );
        netcode_write_bytes( &buffer, p->connect_token_data, NETCODE_CONNECT_TOKEN_BYTES );

        assert( buffer - start == 1 + 8 + NETCODE_NONCE_BYTES + NETCODE_CONNECT_TOKEN_BYTES );

        return buffer - start;
    }
    else
    {
        // encrypted packets
    }

    return 0;
}

void * netcode_read_packet( uint8_t * packet, int buffer_length )
{
    (void) packet;
    (void) buffer_length;

    return NULL;
}

// ----------------------------------------------------------------

struct netcode_t
{
    int initialized;
};

static struct netcode_t netcode;

int netcode_init()
{
    assert( !netcode.initialized );

    // ...

    netcode.initialized = 1;

	return 0;
}

void netcode_term()
{
	assert( netcode.initialized );

    // ...

    netcode.initialized = 0;
}

// ----------------------------------------------------------------

struct netcode_client_t
{
	int dummy;
};

struct netcode_client_t * netcode_client_create()
{
    assert( netcode.initialized );

	struct netcode_client_t * client = (struct netcode_client_t*) malloc( sizeof( struct netcode_client_t ) );

    if ( !client )
        return NULL;

	// ...

	return client;
}

void netcode_client_update( struct netcode_client_t * client, double time )
{
    assert( client );

	(void) client;
	(void) time;
	
	// ...
}

int netcode_client_get_client_index( struct netcode_client_t * client )
{
    assert( client );

	(void) client;

	// ...

	return -1;
}

void netcode_client_connect( struct netcode_client_t * client, const char * token )
{
    assert( client );
    assert( token );

	(void) client;
	(void) token;

	// ...
}

void netcode_client_send_packet_to_server( struct netcode_client_t * client, const uint8_t * packet_data, int packet_size )
{
    assert( client );
    assert( packet_data );
    assert( packet_size );

	(void) client;
	(void) packet_data;
	(void) packet_size;

	// ...
}

int netcode_client_receive_packet_from_server( struct netcode_client_t * client, uint8_t * buffer, int buffer_length )
{
    assert( client );
    assert( buffer );
    assert( buffer_length > 0 );

	(void) client;
	(void) buffer;
	(void) buffer_length;

	// ...

	return 0;
}

void netcode_client_disconnect( struct netcode_client_t * client )
{
    assert( client );

	(void) client;

	// ...
}

void netcode_client_destroy( struct netcode_client_t * client )
{
    assert( client );

    // ...

	free( client );
}

// ----------------------------------------------------------------

struct netcode_server_t
{
	int dummy;
};

struct netcode_server_t * netcode_server_create( uint16_t port )
{
	(void) port;

    assert( netcode.initialized );

    struct netcode_server_t * server = (struct netcode_server_t*) malloc( sizeof( struct netcode_server_t ) );

    if ( !server )
        return NULL;

    // ...

    return server;
}

void netcode_server_start( struct netcode_server_t * server, int max_clients )
{
	(void) server;
	(void) max_clients;

	// ...
}

void netcode_server_update( struct netcode_server_t * server, double time )
{
	(void) server;
	(void) time;

	// ...
}

int netcode_server_is_client_connected( struct netcode_server_t * server, int client_index )
{
	(void) server;
	(void) client_index;

	// ...

	return 0;
}

int netcode_server_receive_packet_from_client( struct netcode_server_t * server, int client_index, uint8_t * buffer, int buffer_length )
{
	(void) server;
	(void) client_index;
	(void) buffer;
	(void) buffer_length;

	// ...

	return 0;
}

void netcode_server_send_packet_to_client( struct netcode_server_t * server, int client_index, const uint8_t * packet_data, int packet_size )
{
	(void) server;
	(void) client_index;
	(void) packet_data;
	(void) packet_size;

	// ...
}

void netcode_server_disconnect_client( struct netcode_server_t * server, int client_index )
{
	(void) server;
	(void) client_index;

	// ...
}

void netcode_server_disconnect_all_clients( struct netcode_server_t * server )
{
	(void) server;

	// ...
}

void netcode_server_stop( struct netcode_server_t * server )
{
	(void) server;

	// ...
}

void netcode_server_destroy( struct netcode_server_t * server )
{
    assert( server );

	(void) server;

	// ...

    free( server );
}