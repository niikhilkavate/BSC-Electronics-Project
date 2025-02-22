
//Includes
#include <p18f4550.h>  //Include controller specific .h file

#pragma config FOSC = HS //Oscillator Selection
#pragma config WDT = OFF //Disable Watchdog timer
#pragma config LVP = OFF //Disable Low Voltage Programming
#pragma config PBADEN = OFF //Disable PORTB Analog inputs


//Declarations for LCD Connection
#define LCD_DATA    PORTD               //LCD data port
#define en          PORTEbits.RE2      // enable signal
#define rw          PORTEbits.RE1      // read/write signal
#define rs          PORTEbits.RE0     // register select signal
#define relay       PORTCbits.RC0     // register select signal

//Function Prototypes
void ADC_Init(void);     //Function to initialize the ADC
void ADC_Init1(void);     //Function to initialize the ADC
void ADC_Init2(void);     //Function to initialize the ADC
unsigned int Get_ADC_Result(void);  //Function to Get ADC result after conversion
void Start_Conversion(void);   //Function to Start of Conversion
void msdelay (unsigned int time);  //Function to generate delay
void init_LCD(void);     //Function to initialise the LCD  
void LCD_command(unsigned char cmd); //Function to pass command to the LCD
void LCD_data(unsigned char data);  //Function to write character to the LCD
void LCD_write_string(static char *str);//Function to write string to the LCD

//Start of main program
void main()
{
char msg1[] = "Irrigation system";
char msg2[] = "Temp:";
char msg3[] = "Humidity:";
char msg4[] = "Moisture:";
char msg5[] = "Motor ON";
char msg6[] = "Motor OFF";

unsigned char i, Thousands,Hundreds,Tens,Ones;
unsigned int adc_val, k=0,adc_val11;
unsigned long Voltage, adc_val3;
float Voltage1, adc_val1,adc_val2;
int result,m, j;
float mV, Temp, Press;


 ADCON1 = 0x0F;        //Configuring the PORTE pins as digital I/O
TRISD = 0x00;         //Configuring PORTD as output
TRISE = 0x00;         //Configuring PORTE as output
TRISC = 0x00;         //Configuring PORTC as output
TRISAbits.TRISA0 = 1;   // RA0 is input
TRISAbits.TRISA1 = 1;   // RA1 is input
TRISAbits.TRISA2 = 1;   // RA2 is input
ADC_Init();    // Init ADC peripheral
init_LCD();    // Init LCD Module
LCD_command (0x8F);   // Goto First line, 16th place of LCD
msdelay(15);
 LCD_command (0x07);   // Display shift left
LCD_write_string(msg1); // Display Message

while(1)
 {
   if(k==0)
 {
 ADC_Init();    // Init ADC peripheral
LCD_command(0x01);      // clear LCD
   msdelay(15);
 LCD_command(0x06);      // Shift curser right
   msdelay(15);
 LCD_command (0x80);   // Goto first line, 0th place of LCD
 LCD_write_string(msg2); // Display Message
  Start_Conversion();   //Trigger conversion
  adc_val= Get_ADC_Result();//Get the ADC output by polling GO bit
   
 Voltage = (long) adc_val*500.0; //Convert Binary result into temperature
  adc_val = Voltage /1024.0;  
       
 LCD_command (0x85);   //Goto 5th place on first line of LCD
  
 i = adc_val/1000 ;  //Get the thousands place
 // Thousands = i + 0x30;  // Convert it to ASCII
//   LCD_data (Thousands); // Display thousands place
  
// i = (adc_val%1000)/100; //Get the Hundreds place
i = adc_val/100 ;
   Hundreds = i + 0x30;  // Convert it to ASCII
  LCD_data (Hundreds);  //Display Hundreds place
 
// i = ((adc_val%1000)%100)/10; //Get the Tens place
i = ((adc_val)%100)/10;
   Tens = i + 0x30;   // Convert it to ASCII
  LCD_data (Tens);   //Display Tens place

   i = adc_val%10 ;   //Get the Ones place
  Ones = i + 30;    // Convert it to ASCII
  LCD_data (i + 0x30);  //Display Ones place

    LCD_data ('.'); // Display decimal point

    LCD_data ('0'); // Display 0 digit
   LCD_data (' '); //
LCD_data ('D'); //
LCD_data ('e'); //
LCD_data ('g'); //
LCD_data (' '); //
   LCD_data ('C'); //
 k=1;
   msdelay(300);  //Delay between conversions. It is a library function,refer delays.h file in MCC18 installation directory
 }
 else if (k==
1)
 {
 ADC_Init1();    // Init ADC peripheral
LCD_command (0xC0);   // Goto second line, 0th place of LCD
 LCD_write_string(msg3); // Display Message
  Start_Conversion();   //Trigger conversion
result = Get_ADC_Result();  // Get the humidity
mV = result /6.7;   // Calculate Relative humidity
Press = mV;   //
m = (int) Press;    // Integer part


 LCD_command (0xC9);   //Goto 9th place on second line of LCD
  
 i = m/1000 ;  //Get the thousands place
  Thousands = i + 0x30;  // Convert it to ASCII
//  LCD_data (Thousands); // Display thousands place
 
// i = (m%1000)/100; //Get the Hundreds place
i = m/100 ;   
Hundreds = i + 0x30;  // Convert it to ASCII
  LCD_data (Hundreds);  //Display Hundreds place
 
 i = ((m%100))/10; //Get the Tens place
  Tens = i + 0x30;   // Convert it to ASCII
  LCD_data (Tens);   //Display Tens place
  

 i = m%10 ;   //Get the Ones place
  Ones = i + 30;    // Convert it to ASCII
  LCD_data (i + 0x30);  //Display Ones place

 LCD_data ('%'); // Display %
LCD_data ('R'); // Display R
LCD_data ('H'); // Display H

  k=2;
   msdelay(300);  //Delay between conversions. It is a library function,refer delays.h file in MCC18 installation directory
 }
else if (k==2)
 {
 ADC_Init2();    // Init ADC peripheral
LCD_command(0x01);      // clear LCD
LCD_command (0x80);   // Goto first line, 0th place of LCD
 LCD_write_string(msg4); // Display Message
  Start_Conversion();   //Trigger conversion

    result = Get_ADC_Result();  // Get the moisture
mV = result /1.024;   // Calculate in percentage
Press = mV;   //
m = (int) Press;    // Integer part


 LCD_command (0x89);   //Goto 9th place on first line of LCD
  
 i = m/1000 ;  //Get the thousands place
  Thousands = i + 0x30;  // Convert it to ASCII
  LCD_data (Thousands); // Display thousands place
 
 i = (m%1000)/100; //Get the Hundreds place
  Hundreds = i + 0x30;  // Convert it to ASCII
  LCD_data (Hundreds);  //Display Hundreds place
 
 i = ((m%1000)%100)/10; //Get the Tens place
  Tens = i + 0x30;   // Convert it to ASCII
  LCD_data (Tens);   //Display Tens place
  
 LCD_data ('.'); // Display decimal point
i = m%10 ;   //Get the Ones place
  Ones = i + 30;    // Convert it to ASCII
  LCD_data (i + 0x30);  //Display Ones place

 LCD_data ('%'); // Display %

if ( m < 300)                                   //if soil moisture is less than 300 then make relay i.e. motor ON
 {
  LCD_command (0xC0);   // Goto second line, 0th place of LCD
  LCD_write_string(msg5); // Display Message
 relay = 1;             // Relay ON to increase moisture in soil by motor run
 }
else
  {
  LCD_command (0xC0);   // Goto second line, 0th place of LCD
  LCD_write_string(msg6); // Display Message
 relay = 0;
  }


  k=0;
   msdelay(300);  //Delay between conversions.
 }

}
}
//Function Definitions
void ADC_Init()
{
 ADCON0=0b00000000; //A/D Module is OFF and Channel 0 is selected
ADCON1=0b00001110; // Reference as VDD & VSS, AN0 set as analog pins
ADCON2=0b10001110; // Result is right Justified
    //Acquisition Time 2TAD
    //ADC Clk FOSC/64
ADCON0bits.ADON=1; //Turn ON ADC module
}
void ADC_Init1()
{
 ADCON0=0b00000100; //A/D Module is OFF and Channel 1 is selected
ADCON1=0b00001101; // Reference as VDD & VSS, AN1 set as analog pins
ADCON2=0b10001110; // Result is right Justified
    //Acquisition Time 2TAD
    //ADC Clk FOSC/64
ADCON0bits.ADON=1; //Turn ON ADC module
}

void ADC_Init2()
{
 ADCON0=0b00001000; //A/D Module is OFF and Channel 2 is selected
ADCON1=0b00001100; // Reference as VDD & VSS, AN2 set as analog pins
ADCON2=0b10001110; // Result is right Justified
    //Acquisition Time 2TAD
    //ADC Clk FOSC/64
ADCON0bits.ADON=1; //Turn ON ADC module
}

void Start_Conversion()
{
 ADCON0bits.GO=1;
}

//If you do not wish to use adc conversion interrupt you can use this
//to do conversion manually. It assumes conversion format is right adjusted
unsigned int Get_ADC_Result()
{
unsigned int ADC_Result=0;
while(ADCON0bits.DONE);
 ADC_Result=ADRESL;
 ADC_Result|=((unsigned int)ADRESH) << 8;
return ADC_Result;
}

void msdelay (unsigned int time) //Function to generate delay
{
unsigned int i, j;
 for (i = 0; i < time; i++)
for (j = 0; j < 710; j++);//Calibrated for a 1 ms delay in MPLAB
}
void init_LCD(void)  // Function to initialise the LCD
{
    LCD_command(0x38);      // initialization of 16X2 LCD in 8bit mode
   msdelay(15);
    LCD_command(0x01);      // clear LCD
   msdelay(15);
    LCD_command(0x0C);      // cursor off
   msdelay(15);
    LCD_command(0x06);      // curser right shift
   msdelay(15);
}

void LCD_command(unsigned char cmd) //Function to pass command to the LCD
{
    LCD_DATA = cmd;  //Send data on LCD data bus
   rs = 0;    //RS = 0 since command to LCD
   rw = 0;    //RW = 0 since writing to LCD
   en = 1;    //Generate High to low pulse on EN
   msdelay(15);
    en = 0;
}

void LCD_data(unsigned char data)//Function to write data to the LCD
{
    LCD_DATA = data; //Send data on LCD data bus
   rs = 1;    //RS = 1 since data to LCD
   rw = 0;    //RW = 0 since writing to LCD
   en = 1;    //Generate High to low pulse on EN
  msdelay(15);
    en = 0;
}
//Function to write string to LCD
void LCD_write_string(static char *str)   
{
   int i = 0;
   while (str[i] != 0)
    {
        LCD_data(str[i]);      // sending data on LCD byte by byte
       msdelay(15);
        i++;
    }
}