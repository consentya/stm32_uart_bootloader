
#include "target.h"

int xmodem_init_xfer (struct serial *desc);
void send_xmodem_packet (struct serial *desc, unsigned char *packet, int len,
			 int hashmark);
void xmodem_finish_xfer (struct serial *desc);

#define XMODEM_DATASIZE	128	/* The data size is ALWAYS 128 */
#define XMODEM_1KDATASIZE 1024	/* Unless it's 1024!!! */
#define XMODEM_PACKETSIZE 133	/* data + packet headers and crc */
#define XMODEM_1KPACKETSIZE 1024 + 5	/* data + packet headers and crc */
#define XMODEM_DATAOFFSET 3	/* Offset to start of actual data */
