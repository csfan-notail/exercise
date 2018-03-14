#ifndef _XRA1403_H__
#define _XRA1403_H__

struct xra1403_platform_data {
	/* number assigned to the first GPIO */
	unsigned	base;
};

/*	start define command_byte	*/
#define	COMMAND_BYTE_GSR1		0x00
#define	COMMAND_BYTE_GSR2		0X01
#define	COMMAND_BYTE_OCR1		0X02
#define	COMMAND_BYTE_OCR2		0X03
#define	COMMAND_BYTE_PIR1		0X04
#define	COMMAND_BYTE_PIR2		0X05
#define COMMAND_BYTE_GCR1 		0X06
#define COMMAND_BYTE_GCR2		0X07
#define	COMMAND_BYTE_PUR1		0X08
#define	COMMAND_BYTE_PUR2		0X09
#define	COMMAND_BYTE_IER1		0xA
#define COMMAND_BYTE_IER2		0xB
#define	COMMAND_BYTE_TSCR1		0xC
#define COMMAND_BYTE_TSCR2		0xD
#define	COMMAND_BYTE_ISR1		0xE
#define COMMAND_BYTE_ISR2		0xF
#define COMMAND_BYTE_REIR1	0x10
#define COMMAND_BYTE_REIR2	0x11
#define COMMAND_BYTE_FEIR1	0X12
#define COMMAND_BYTE_FEIR2	0x13
#define COMMAND_BYTE_IFR1		0x14
#define COMMAND_BYTE_IFR2		0x15

#define COMMAND_NUM		0x15
/*	 end define command_byte	*/


/*	start define shift	*/
#define	COMMAND_BYTE_SHIFT	1
#define COMMAND_SHIFT			8
#define COMMAND_RDWR_SHIFT	7
/*	end define shift	*/

/*	start define wr/rd	*/
#define COMMAND_RD			1
#define	COMMAND_WR			0
/*	end define wr/rd	*/


#endif





