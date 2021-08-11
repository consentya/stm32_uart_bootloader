
#include "xmodem.h"
#include <string.h> /* memset() */

#define CRC16 0x1021		/* Generator polynomial (X^16 + X^12 + X^5 + 1) */

static int blknum;		/* XMODEM block number */
static int crcflag;		/* Sez we are using CRC's instead of cksums */
static uint8_t  xmd_packet[XMODEM_PACKETSIZE] = {0};

#define CRC_TABLE_SIZE       ( 256 * sizeof(unsigned short) )
static unsigned short        crctab[CRC_TABLE_SIZE];

/* Call this to init the fast CRC-16 calculation table.  */
static void
crcinit (void)
{
  static int crctab_inited = 0;
  int val;

  if (crctab_inited == 1)
    return;

  for (val = 0; val <= 255; val++)
    {
      int i;
      unsigned int crc;

      crc = val << 8;

      for (i = 0; i < 8; ++i)
	{
	  crc <<= 1;

	  if (crc & 0x10000)
	    crc ^= CRC16;
	}

      crctab[val] = crc;
    }

  crctab_inited = 1;
}

/* Calculate a CRC-16 for the LEN byte message pointed at by P.  */

static unsigned short
docrc (unsigned char *p, int len)
{
  unsigned short crc = 0;

  while (len-- > 0)
    crc = (crc << 8) ^ crctab[(crc >> 8) ^ *p++];

  return crc;
}

/* Start up the transmit process.  Reset state variables.  Wait for receiver to
   send NAK or CRC request.  */

int
xmodem_init_xfer (struct serial *desc)
{
  int c;
  int i;

  blknum = 1;
  crcflag = 0;
  crcinit ();

  for (i = 1; i <= 10; i++)
  {
        c =  desc->recv_func (0);

        switch (c)
		{
			case 'C':
			  crcflag = 1;
			  return 1;
			case NAK:
			  return 0;
			default:
			  // xmodem_init_xfer: Got unexpected character
			  continue;
			case CANCEL:		/* target aborted load */
			  // "Got a CANCEL from the target.";
			  continue;
		}
   }
   // "xmodem_init_xfer:  Too many unexpected characters.");

  return 0;
}

/* Take 128 bytes of data and make a packet out of it.

 *      Each packet looks like this:
 *      +-----+-------+-------+------+-----+
 *      | SOH | Seq1. | Seq2. | data | SUM |
 *      +-----+-------+-------+------+-----+
 *      SOH  = 0x01
 *      Seq1 = The sequence number.
 *      Seq2 = The complement of the sequence number.
 *      Data = A 128 bytes of data.
 *      SUM  = Add the contents of the 128 bytes and use the low-order
 *             8 bits of the result.
 *
 * send_xmodem_packet fills in the XMODEM fields of PACKET and sends it to the
 * remote system.  PACKET must be XMODEM_PACKETSIZE bytes long.  The data must
 * start 3 bytes after the beginning of the packet to leave room for the
 * XMODEM header.  LEN is the length of the data portion of the packet (and
 * must be <= 128 bytes).  If it is < 128 bytes, ^Z padding will be added.
 */

void my_memcpy(uint8_t* dst, const uint8_t* src, uint16_t copy_len)
{
	uint8_t* src_ptr = src;
	uint8_t* end_ptr = src + copy_len; /* begin: 0 , len = 5 => 0 ... 4 < 5*/

	while(src_ptr < end_ptr)
	{
		*dst++ = *src_ptr++;
	}
}

int
send_xmodem_packet (struct serial *desc, unsigned char *payload, uint16_t blknum)
{
  int retries;
  int c;
  int crc;
  int datasize = XMODEM_DATASIZE;
  // =================== STEP 1: BUILD XMODEM FRAME ========================
  /* build the packet header */
  xmd_packet[0] = SOH;
  xmd_packet[1] = blknum;
  xmd_packet[2] = (uint8_t)(~blknum); // complement => a=  1010 ~a = 0101

  /* Prepare payload data */
  my_memcpy (xmd_packet + 3, payload, datasize); // use custom memcpy

  if (crcflag) // 'C' -> CRC16
  {
      crc = docrc (payload, datasize);
      /* ABCD -> RIGHT SHIFT 8 times */
      xmd_packet[3 + datasize] = crc >> 8; // High Byte Order
      xmd_packet[3 + datasize + 1] = crc & 0xFF;  // Low Byte Order
   }
  else
   {
	  // Do nothing
   }

  // =================== STEP 2: SENDING XMODEM FRAME ========================
  /* Waiting ACK */
  for (retries = 3; retries >= 0; retries--)
  {
	   // Send frame to serial port
       desc->send_func (xmd_packet, XMODEM_PACKETSIZE);

       // waiting ACK
       c = desc->recv_func (3); // 3 is time out value = 3 seconds
       switch (c)
		{
			case ACK:
			  return c;
			case NAK:
			   continue;
			  continue;
			case CANCEL:
			 // "xmodem_send_packet: Transfer aborted by receiver.";
			default:
			 // "xmodem_send_packet: Got unexpected character"
			  continue;
		}
    }

  	return c;
 }

/* Finish off the transfer.  Send out the EOT, and wait for an ACK.  */

void
xmodem_finish_xfer (struct serial *desc)
{
  int retries;
  uint8_t c = EOT;

  for (retries = 10; retries >= 0; retries--)
   {
      desc->send_func (&c, 1);	/* Send an EOT */
      c = desc->recv_func (3);
      switch (c)
	  {
			case ACK:
			  return;
			case NAK:
			  continue;
			case CANCEL:
			 // "xmodem_finish_xfer: Transfer aborted by receiver."
			default:
			  // "xmodem_send_packet: Got unexpected character
			  continue;
	}
    }

  // "xmodem_finish_xfer:  Excessive retries.";
}

void xmodem_flush (struct serial *desc)
{
	(void)desc->recv_func (3); // Clear UART Data Register
}
