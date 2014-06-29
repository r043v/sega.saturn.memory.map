#define USB_FLAGS (*(volatile uint8_t*)(0x22200001))
#define USB_RXF     (1 << 0)
#define USB_TXE     (1 << 1)
#define USB_PWREN   (1 << 7)
#define USB_FIFO (*(volatile uint8_t*)(0x22100001))

uint8_t RecvByte(void)
{
    while ((USB_FLAGS & USB_RXF) != 0) ;
    return USB_FIFO;
}

uint32_t RecvDword(void)
{
    uint32_t tmp = RecvByte();
    tmp = (tmp << 8) | RecvByte();
    tmp = (tmp << 8) | RecvByte();
    tmp = (tmp << 8) | RecvByte();

    return tmp;
}

void SendByte(uint8_t byte)
{
    while ((USB_FLAGS & USB_TXE) != 0) ;
    USB_FIFO = byte;
}
