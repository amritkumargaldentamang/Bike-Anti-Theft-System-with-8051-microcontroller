#include<reg51.h>
sbit RS = P0^0;
sbit RW = P0^1;
sbit EN = P0^2;
sbit VIBRATION_SENSOR = P1^0;   // Vibration sensor connected to P1.0
sbit BUZZER = P1^7; 	//NOW CONNECTED TO RELAY


#define NUMBER "+9779840007498"
#define LCD_data P2
#define ON 0
#define OFF 1
//for LCD display
void LCD_cmd(unsigned char command);
void LCD_data_write(unsigned char data_);
void LCD_init();
void LCD_string_write(unsigned char *string);

//for serial communication
void ser_init();
void tx(unsigned char send);
void tx_str(unsigned char *s);   
void sms(unsigned char *num1, unsigned char *msg);

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 1275; j++);
}

void main()
{
    ser_init();
		while(1)
        {
								if(VIBRATION_SENSOR == OFF)
								{
										BUZZER = OFF;
//										LED=OFF;
										LCD_init();
										LCD_string_write("No message!!!");
										delay_ms(50);	
										LCD_cmd(0x01);
                                        
								}
								else
								{
										BUZZER =  ON; 
//										LED = ON;
										delay_ms(500);
										BUZZER = OFF; 
//										LED =OFF;
										LCD_init();
										LCD_string_write("Hello! Warning!!!");
										delay_ms(50);
                                        LCD_cmd(0x01);
										sms(NUMBER, "Chori vayo hai");
                                        
								}
								
        }
}

//functions for LCD
void LCD_cmd(unsigned char command)
{
    LCD_data = command; //send command to LCD
    RS = 0;   // send a command mode
    RW = 0;     //write data
    EN = 1;     //enable signal to latch command
    delay_ms(10);
    EN = 0;
}

void LCD_data_write(unsigned char data_)
{
    LCD_data = data_;   //send data to LCD
    RS = 1;     //data mode
    RW = 0;     //write mode
    EN = 1;     //enable signal to latch data
    delay_ms(10);   //delayfor some time
    EN = 0;
}

void LCD_init()
{
    LCD_cmd(0x38);  //initialize LCD in 8-bit mode, 2 lines and 5x 7 matrix
    delay_ms(10);
    LCD_cmd(0x0e);  //display on, curson on and blinking
    delay_ms(10);
    LCD_cmd(0x01);  //clear display
    delay_ms(10);

}

void LCD_string_write(unsigned char *string)
{
    int i = 0;
    while(string[i]!= '\0')
    {
        LCD_data_write(string[i++]);
    }
}
//code for SIM
void ser_init()
{
    SCON=0x50;		//0101 0000 :: using Mode 1, receive enable
    TMOD=0x20;		//0010 0001 :: using timer 1 in mode 2, timer 0 in mode 1
    TH1=0xFD;	   	//1111 1101   :: auto reload bit for timer 1 to set baud rate for 9600
    TL1=0xFD;		//1111 1101
    TR1=1;		    //start the timer 1
}

void tx(unsigned char send)
{
    SBUF=send;      //load character into serial buffer 
    while(TI==0);   //wait until timer flag is set
    TI=0;  
}

void tx_str(unsigned char *s)
{
    while(*s)
    {
        tx(*s++);
    }
}

void sms(unsigned char *num1,unsigned char *msg)
{
    tx_str("AT");
    tx(0x0d);
    //delay_ms(20);

    tx_str("AT+CMGF=1");
    tx(0x0d);
    //delay_ms(20);

    tx_str("AT+CMGS=");
		tx('"');
    while(*num1)
    {
        tx(*num1++);
    }
		tx('"');
    tx(0x0d);
    //delay_ms(100);

    while(*msg)
    {
        tx(*msg++);
    }
    tx(0x0d);
    tx(0x1a);
    //delay_ms(30);
}
