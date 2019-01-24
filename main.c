#include "mbed.h"
#include "WIZnetInterface.h"

unsigned char MAC_Addr[6] = {0x00,0x08,0xDC,0x12,0x07,0x33};

char* Update_Key = "XGZYQDEGEKBWGA0N";
char* ServerIP = "184.106.153.149";
int Count = 15;

Serial pc(USBTX, USBRX);
SPI spi(PTD2,PTD3,PTD1);
WIZnetInterface ethernet(&spi,PTD0,PTA20);
AnalogIn temp(PTC1);
AnalogIn ain(PTC2);

int main()
{
    //Set serial port baudrate speed: 19200
    pc.baud(19200);
    pc.printf("Start\r\n");
    
    while(1) {
        int ret = ethernet.init(MAC_Addr);
        
        if (!ret) {
            pc.printf("Initialized, MAC: %s\r\n", ethernet.getMACAddress());
            ret = ethernet.connect();
            if (!ret) {
                pc.printf("IP: %s, MASK: %s, GW: %s\r\n",
                          ethernet.getIPAddress(), ethernet.getNetworkMask(), ethernet.getGateway());
            } else {
                pc.printf("Error ethernet.connect() - ret = %d\r\n", ret);
                exit(0);
            }
        } else {
            pc.printf("Error ethernet.init() - ret = %d\r\n", ret);
            exit(0);
        }  
    
    TCPSocketConnection sock;
    sock.connect("184.106.153.149", 80);
    if(sock.is_connected())
        pc.printf("Socket Connected\n\r");
    else
        pc.printf("Socket NoT Connected\n\r");
    
    
    char buffer[300];
    int ret_t;
    
    char http_cmd[256]; 
    sprintf(http_cmd,"GET /update?key=XGZYQDEGEKBWGA0N&field1=%d HTTP/1.0\n\n",ain.read_u16());
    printf("Running - %s\r\n",http_cmd);
    sock.send_all(http_cmd, sizeof(http_cmd)-1);
        
    ret_t = sock.receive(buffer, sizeof(buffer)-1);
    buffer[ret_t] = '\0';
    printf("Received %d chars from server:\n%s\r\n", ret_t, buffer);
        
    printf("Socket Closed");
    wait(1);
}
}
