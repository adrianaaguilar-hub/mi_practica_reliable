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

static long timeout_ns;
static uint32_t next_send_sequence;
static uint32_t next_receive_sequence;
static int waiting_for_ack;
static int last_data_size;
static char last_data[MAX_PAYLOAD];

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

    timeout_ns = timeout_in_ns;
    next_send_sequence = 1;
    next_receive_sequence = 1;
    waiting_for_ack = 0;
    last_data_size = 0;
}

// This callback is called when a packet pkt of size pkt_size is received
void receive_callback(packet_t *pkt, size_t pkt_size)
{
     if (!VALIDATE_CHECKSUM(pkt))
    {
        return;
    }

    if (pkt_size == ACK_PACKET_SIZE)
    {
        if (waiting_for_ack == 1 &&
            pkt->ackno == next_send_sequence)
        {
            CLEAR_TIMER(RETRANSMISSION_TIMER);
            waiting_for_ack = 0;
            next_send_sequence++;
            RESUME_TRANSMISSION();
        }

        return;
    }

    if (pkt->len < DATA_PACKET_HEADER ||
        pkt->len > DATA_PACKET_HEADER + MAX_PAYLOAD)
    {
        return;
    }

     if (pkt->seqno == next_receive_sequence)
    {
        size_t data_size = pkt->len - DATA_PACKET_HEADER;

        ACCEPT_DATA(pkt->data, data_size);
        SEND_ACK_PACKET(pkt->seqno);
        next_receive_sequence++;
    }
    else if (pkt->seqno < next_receive_sequence)
    {
        SEND_ACK_PACKET(pkt->seqno);
    }
}

// Callback called when the application has data to be sent
void send_callback()
{
   int bytes_read;

    if (waiting_for_ack == 1)
    {
        return;
    }

    bytes_read = READ_DATA_FROM_APP_LAYER(last_data, MAX_PAYLOAD);

    if (bytes_read <= 0)
    {
        return;
    }

    last_data_size = bytes_read;

    SEND_DATA_PACKET(
        DATA_PACKET_HEADER + last_data_size,
        0,
        next_send_sequence,
        last_data
    );

    waiting_for_ack = 1;
    SET_TIMER(RETRANSMISSION_TIMER, timeout_ns);
    PAUSE_TRANSMISSION();
}

/*
    This function is called when timer timer_number expires. The function of the
timer depends on the protocol programmer.
*/
void timer_callback(int timer_number)
{
    (void)timer_number;
}

//------------------------------------------------------------------------------
