#include "nRF.h"

static int nRF_stdw(char c, FILE *stream);

static FILE nRF_stdout = FDEV_SETUP_STREAM(nRF_stdw, NULL, _FDEV_SETUP_WRITE);

int buf_curlen = 0;
static int nRF_stdw(char c, FILE *stream)//�������� ����������
{
	nRF.buf[buf_curlen] = (unsigned char) c;
	buf_curlen++;
	
	if(buf_curlen >= 32)
	{
		nRF_send(buf_curlen);
		buf_curlen = 0;
	}
	return 0;
}

void nRF_write_multi(unsigned char a, unsigned char len)
{
	SPCR = (1<<SPE) | (1<<CPOL) | (1<<CPHA) | (0<<DORD) | (1<<MSTR); SPSR = (1<<SPI2X);
	cslo();
	nRF_CSN_port &= ~(1<<nRF_CSN_pin);
	SPI_transmit_receive(a);
	for(unsigned char i = 0; i < len; i++)
		SPI_transmit_receive(nRF.buf[i]);
		
	nRF_CSN_port |= (1<<nRF_CSN_pin);
	cshi();	
	SPCR = 0;
	
	SPCR = (1<<SPE) | (0<<CPOL) | (0<<CPHA) | (0<<DORD) | (1<<MSTR); SPSR = (1<<SPI2X);
	cslo();
	nRF_CSN_port &= ~(1<<nRF_CSN_pin);
	SPI_transmit_receive(a);
	for(unsigned char i = 0; i < len; i++)
		SPI_transmit_receive(nRF.buf[i]);
	nRF_CSN_port |= (1<<nRF_CSN_pin);
	cshi();
	SPCR = 0;
	
	for(unsigned char i = 0; i < 32; i++)
		nRF.buf[i] = 0;
}

void nRF_write_multi_alt(unsigned char a, unsigned char length)
{
	SPCR = (1<<SPE) | (1<<CPOL) | (1<<CPHA) | (0<<DORD) | (1<<MSTR); SPSR = (1<<SPI2X);
	cslo();
	nRF_CSN_port &= ~(1<<nRF_CSN_pin);
	SPI_transmit_receive(a);
	for(unsigned char i = 31; i >= 0; i--)
		SPI_transmit_receive(nRF.buf[i]);
		
	nRF_CSN_port |= (1<<nRF_CSN_pin);
	cshi();
	SPCR = 0;
	
	SPCR = (1<<SPE) | (0<<CPOL) | (0<<CPHA) | (0<<DORD) | (1<<MSTR); SPSR = (1<<SPI2X);
	cslo();
	nRF_CSN_port &= ~(1<<nRF_CSN_pin);
	SPI_transmit_receive(a);
	for(unsigned char i = 31; i >= 0; i--)
		SPI_transmit_receive(nRF.buf[i]);
	nRF_CSN_port |= (1<<nRF_CSN_pin);
	cshi();
	SPCR = 0;
	
	for(unsigned char i = 31; i >= 0; i--)
		nRF.buf[i] = 0;
}

void nRF_write(unsigned char a, unsigned char b)
{
	SPCR = (1<<SPE) | (1<<CPOL) | (1<<CPHA) | (0<<DORD) | (1<<MSTR); SPSR = (1<<SPI2X);
	cslo();
	nRF_CSN_port &= ~(1<<nRF_CSN_pin);
	SPI_transmit_receive(a);
	SPI_transmit_receive(b);
	nRF_CSN_port |= (1<<nRF_CSN_pin);
	cshi();
	SPCR = 0;
	
	SPCR = (1<<SPE) | (0<<CPOL) | (0<<CPHA) | (0<<DORD) | (1<<MSTR); SPSR = (1<<SPI2X);
	cslo();
	nRF_CSN_port &= ~(1<<nRF_CSN_pin);
	SPI_transmit_receive(a);
	SPI_transmit_receive(b);
	nRF_CSN_port |= (1<<nRF_CSN_pin);
	cshi();
	SPCR = 0;
}

unsigned char nRF_readReg(unsigned char a)
{
	unsigned char c;
	SPCR = (1<<SPE) | (1<<CPOL) | (1<<CPHA) | (0<<DORD) | (1<<MSTR); SPSR = (1<<SPI2X);
	cslo();
	nRF_CSN_port &= ~(1<<nRF_CSN_pin);
	c = SPI_transmit_receive(a & 0x1F);
	c = SPI_transmit_receive(0x00);
	nRF_CSN_port |= (1<<nRF_CSN_pin);
	cshi();
	SPCR = 0;
	
	SPCR = (1<<SPE) | (0<<CPOL) | (0<<CPHA) | (0<<DORD) | (1<<MSTR); SPSR = (1<<SPI2X);
	cslo();
	nRF_CSN_port &= ~(1<<nRF_CSN_pin);
	c = SPI_transmit_receive(a & 0x1F);
	c = SPI_transmit_receive(0x00);
	nRF_CSN_port |= (1<<nRF_CSN_pin);
	cshi();
	SPCR = 0;
		
	return c;
}

unsigned char nRF_readReg_a(unsigned char a, int length)
{
	unsigned char c;
	SPCR = (1<<SPE) | (1<<CPOL) | (1<<CPHA) | (0<<DORD) | (1<<MSTR); SPSR = (1<<SPI2X);
	cslo();
	nRF_CSN_port &= ~(1<<nRF_CSN_pin);
	c = SPI_transmit_receive(a & 0x1F);
	for(unsigned char i = 0; i < length; i++)
		nRF.buf[i] = SPI_transmit_receive(0x00);
	nRF_CSN_port |= (1<<nRF_CSN_pin);
	cshi();
	SPCR = 0;
		
	SPCR = (1<<SPE) | (0<<CPOL) | (0<<CPHA) | (0<<DORD) | (1<<MSTR); SPSR = (1<<SPI2X);
	cslo();
	nRF_CSN_port &= ~(1<<nRF_CSN_pin);
	c = SPI_transmit_receive(a & 0x1F);
	for(unsigned char i = 0; i < length; i++)
		nRF.buf[i] = SPI_transmit_receive(0x00);
	nRF_CSN_port |= (1<<nRF_CSN_pin);
	cshi();
	SPCR = 0;
	return c;
}

void nRF_stdef()
{
	stdout = &nRF_stdout;
}

// nRF_init(������� � ���)
void nRF_init(int freq)
{
	//stdout = &nRF_stdout;
	
	// ��������� ��
	nRF_CE_ddr |= (1<<nRF_CE_pin);
	nRF_CSN_ddr |= (1<<nRF_CSN_pin);
	nRF_CSN_port &= ~(1<<nRF_CSN_pin);
	nRF_CE_port &= ~(1<<nRF_CE_pin);					
	
	/* ��������� CONFIG: 
	7: ������ = 0 -> �������� ������ 0,
	6: MASK_RX_DR = 0 -> ������� ���������� RX_DR ��� '0' �� ��� IRQ, 
	5: MASK_TX_DS = 0 -> ������� ���������� RX_DR ��� '0' �� ��� IRQ,
	4: MASK_MAX_RT = 0 -> ������� ���������� RX_DR ��� '0' �� ��� IRQ,
	3: EN_CRC = 1 -> �������� CRC, 
	2: CRCO = 0 -> �������� ��, 
	1: PWR_UP = 1 -> CRC 2 �����, 
	0: PRIM_RX = 0 -> TX Control */
	nRF_write_reg(0x00, 0b00001010);
	 
	/* ��������� SETUP: 
	7: CONT_WAVE = 0 -> "continuous carrier transmit when high is disabled",
	6: ������ = 0 -> �������� ������ 0,
	5: RF_DR_LOW = 0 -> ������ � RF_DR_HIGH, �������� 2000 ����/�,
	4: PLL_LOCK = 0 -> PLL Lock ��������, ������������ ������ ��� ������������,
	3: RF_DR_HIGH = 1 -> ������ � RF_DR_LOW, �������� 250 ����/�, 
	2: RF_PWR(_A) = 1 -> 0 ��� (RF_PWR(_A) | RF_PWR(_B) = 00), 
	1: RF_PWR(_B) = 1 -> 0 ��� (RF_PWR(_A) | RF_PWR(_B) = 00), 
	0: ����� = 0 -> ��� ������� */
	nRF_write_reg(0x06, 0b00001110);   	// �������� 2000 ����/� (RF_DR_LOW = 0, RF_DR_HIGH = 1), �������� �������� 0 ���

	/* ��������� STATUS
	7: ������ = 0 -> �������� ������ 0,
	6: RX_DR = 1 -> ����� ���������� �������� ������,
	5: TX_DS = 1 -> ����� ���������� �������� ������,
	4: MAX_RT = 1 -> ����� ���������� ������������ ������� ��������,
	3-0: �������� ������ ��� ������ */
	nRF_write_reg(0x07, 0b01110000);	

	nRF_write_reg(0x01, 0b00111111);			// �������� AutoACK �� pipe0 (0-���)
	nRF_write_reg(0x02, 0b00111111);			// �������� RX-����� �� pipe0 (0-���)
	nRF_write_reg(0x03, 0b00000011);	// AW
	nRF_write_reg(0x04, 0b00011111);	// RETR
	nRF_write_reg(0x05, 0x4C);	// ������ ���������� ������

	// ������ ������ RX ����� 0
	nRF.buf[0] = 0xE7;
	nRF.buf[1] = 0xE7;
	nRF.buf[2] = 0xE7;
	nRF.buf[3] = 0xE7;
	nRF.buf[4] = 0xE7;

	nRF_write_multi(0x0A, 5);
	
	// ������ ������ RX ����� 1
	nRF.buf[0] = 0xE7;
	nRF.buf[1] = 0xE7;
	nRF.buf[2] = 0xE7;
	nRF.buf[3] = 0xE7;
	nRF.buf[4] = 0xE7;

	nRF_write_multi(0x0B, 5);
	// ����� ������ ������ RX ����� 0 & 1

	// ������ ������ TX ����� 0
	nRF.buf[0] = 0xE7;
	nRF.buf[1] = 0xE7;
	nRF.buf[2] = 0xE7;
	nRF.buf[3] = 0xE7;
	nRF.buf[4] = 0xE7;

	nRF_write_multi(0x10, 5);
	// ����� ������ ������ RX ����� 0
	
	nRF_write_reg(0x1C, 0x03);			// DYNPD
	nRF_write_reg(0x1D, 0b00000111);	// FEATURE

	// ������� ������
	nRF_write(0xE2, 0);
	nRF_write(0xE1, 0);
	
	nRF_write_reg(0x07, 0b01110000);
}

unsigned char nRF_send(int len)
{
	nRF_write_reg(0x05, 0x4C);	// ������ ���������� ������
	uint8_t fifoReg = nRF_readReg(0x17);
	printf("FIFO FIRST READ %x\r\n", fifoReg);
	
	if(fifoReg & (1<<5))
		nRF_TxComplete();
	
	//nRF_write(0xE1, 0x00);
	nRF_write(0xE2, 0x00);
	
	nRF_write_reg(0x07, 0b01110000);
	fifoReg = nRF_readReg(0x17);
	printf("FIFO BEFORE BUFFER %x\r\n", fifoReg);
	_delay_us(100);
	nRF_write_multi(0xA0, len);
	fifoReg = nRF_readReg(0x17);
	printf("FIFO AFTER BUFFER %x\r\n", fifoReg);
	
	//nRF_CE_port |= (1<<nRF_CE_pin);
	//_delay_us(20);
	//nRF_CE_port &= ~(1<<nRF_CE_pin);
	//_delay_us(500);
	
	if(nRF_TxComplete())
		return 1;
	return 0;
}

unsigned char nRF_send_other(char * data)
{
	int i, n, N = strlen(data);
	
	if(N > 32)
		N = 32;

	for(i = 31, n = 0; (i >= 31-N) && (n < N); i--, n++)
		nRF.buf[i] = (unsigned char) data[n];
	
	printf("NRF BUFFER %s\r\n", nRF.buf);
	
	return nRF_send(N);
}

uint8_t nRF_TxComplete()
{
	uint8_t fifoReg, statusReg;
	uint8_t txBufferIsEmpty;
	uint8_t packetWasSent, packetCouldNotBeSent;
	uint8_t txAttemptCount = 0;
	uint8_t result = 0; // Default to indicating a failure.

	// TX buffer can store 3 packets, sends retry up to 15 times, and the retry wait time is about half
	// the time necessary to send a 32 byte packet and receive a 32 byte ACK response.  3 x 15 x 2 = 90
	const static uint8_t MAX_TX_ATTEMPT_COUNT = 90;

	while (txAttemptCount++ < MAX_TX_ATTEMPT_COUNT)
	{
		fifoReg = nRF_readReg(0x17);
		statusReg = nRF_readReg(0x07);
		txBufferIsEmpty = fifoReg & (1<<4);

		if (txBufferIsEmpty)
		{
			result = 1; // Indicate success.
			printf("FIFO EMPTY SUCC %d\r\n", fifoReg);
			break;
		}

		nRF_CE_port |= (1<<nRF_CE_pin);
		_delay_us(20);
		nRF_CE_port &= ~(1<<nRF_CE_pin);
		_delay_us(500);
		
		statusReg = nRF_readReg(0x07);
		packetWasSent = statusReg & (1<<5);
		packetCouldNotBeSent = statusReg & (1<<4);

		if (packetWasSent)
		{
			nRF_write_reg(0x07, 0b00100000);	   // Clear TX success flag.
			printf("STAT TX_DS SUCC %d\r\n", statusReg);
		}
		else if (packetCouldNotBeSent)
		{
			nRF_write(0xE1, 0);							// Clear TX buffer.
			nRF_write_reg(0x07, 0b00010000);          // Clear max retry flag.
			printf("PACKET LOSS SAD %d\r\n", statusReg);
			break;
		}
	}

	return result;
}