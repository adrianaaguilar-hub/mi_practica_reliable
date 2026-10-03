#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <poll.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netinet/in.h>

#include "rlib.h"

/*
    Add your own defines below. Remember that you have the following constans
already defined:
    - ACK_PACKET_SIZE: Size of ACK packets
    - DATA_PACKET_HEADER: Data packet's header size
--------------------------------------------------------------------------------
*/

//------------------------------------------------------------------------------

/*
    Global data: Add your own data fields below. The information in this global
data is persistent and it can be accesed from all functions.
--------------------------------------------------------------------------------

*/

#define RETRANSMISSION_TIMER 0

static long retransmission_timeout;
static uint32_t next_tx_seqno;
static uint32_t next_rx_seqno;
static int waiting_for_ack;
static int last_payload_size;

//------------------------------------------------------------------------------

/*
    Callback functions: The following functions are called on the corresponding
event as explained in rlib.h file. You should implement these functions.
--------------------------------------------------------------------------------
*/

/*
    Creates a new connection. You should declare any variable needed in the
global data section and make initializations here as required.
*/
void connection_initialization(int window_size, long timeout_in_ns)
{
 (void)window_size;

    retransmission_timeout = timeout_in_ns;
    next_tx_seqno = 1;
    next_rx_seqno = 1;
    waiting_for_ack = 0;
    last_payload_size = 0;
}

// This callback is called when a packet pkt of size pkt_size is received
void receive_callback(packet_t *pkt, size_t pkt_size)
{

}

// Callback called when the application has data to be sent
void send_callback()
{
}

/*
    This function is called when timer timer_number expires. The function of the
timer depends on the protocol programmer.
*/
void timer_callback(int timer_number)
{
}

//------------------------------------------------------------------------------
