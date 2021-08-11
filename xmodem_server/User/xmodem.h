#ifndef XMODEM_SENDER_H_
#define XMODEM_SENDER_H_

#include "target.h"

/* These definitions are for xmodem protocol. */
#define SOH	       0x01 /* Used for 128 byte xmodem*/
#define STX	       0x02 /* 1KB xmodem*/
#define ACK	       0x06
#define NAK	       0x15
#define EOT	       0x04
#define CANCEL	   0x18
#define ESC	       0x1B // ?

#define XMODEM_DATASIZE	128	/* The data size is ALWAYS 128 */
#define XMODEM_1KDATASIZE 1024	/* Unless it's 1024!!! */
#define XMODEM_PACKETSIZE 133	/* data + packet headers and crc */
#define XMODEM_1KPACKETSIZE 1024 + 5	/* data + packet headers and crc */
#define XMODEM_DATAOFFSET 3	/* Offset to start of actual data */

int xmodem_init_xfer (struct serial *desc);
int send_xmodem_packet (struct serial *desc, unsigned char *payload, uint16_t blknum);
void xmodem_finish_xfer (struct serial *desc);
void xmodem_flush (struct serial *desc);


#endif /* XMODEM_SENDER_H_ */
