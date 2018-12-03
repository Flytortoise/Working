#define GPACON (*((volatile unsigned long *)0x56000000))
#define GPADAT (*((volatile unsigned long *)0x56000004))
#define GPBCON (*((volatile unsigned long *)0x56000010))
#define GPBDAT (*((volatile unsigned long *)0x56000014))

#define ledA_on() ((GPADAT) &= (0xf9ffffff))
#define ledB_on() ((GPBDAT) &= (0xffffff9f))
#define ledA_off() ((GPADAT) |= (0x06000000))
#define ledB_off() ((GPBDAT) |= (0x00000060))

#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4

#define ON 1
#define OFF 0


void led_init()
{
	GPACON &= 0xf9ffffff;
	GPBCON &= 0xffffd7ff; 
}

void delay(){
	unsigned int k = 0;

	for( k = 0; k < 0x1000000; k++){
	}
	
}

void delay2(){
	int i = 0;
	for(i = 0; i < 0x1000; i++);
}

void delay_c(unsigned int t){
	while(t--);
}


void LED_init()
{
	GPACON = 0;
	GPBCON = (0X05 << 10);
}

void Led_Control(unsigned char led, unsigned char ison)
{
	
	if(ison == ON)
	{
		switch(led)
		{
			case LED1 :
				GPBDAT &= ~(0x01 << 5);
			break;
			
			case LED2 :
				GPBDAT &= ~(0x01 << 6);
			break;
			
			case LED3 :
				GPADAT &= ~(0x01 << 25);
			break;
			
			case LED4 :
				GPADAT &= ~(0x01 << 26);
			break;
			
			default :
				break;
				
		}
	}
	else
	{
		switch(led)
		{
			case LED1 :
				GPBDAT |= (0x01 << 5);
			break;
			
			case LED2 :
				GPBDAT |= (0x01 << 6);
			break;
			
			case LED3 :
				GPADAT |= (0x01 << 25);
			break;
			
			case LED4 :
				GPADAT |= (0x01 << 26);
			break;
			
			default :
				break;
		}
	}
}

void LED_S()
{
	Led_Control(LED1,ON);
	Led_Control(LED2,ON);
	Led_Control(LED3,ON);
	Led_Control(LED4,ON);
	delay_c(0x500000);
	Led_Control(LED1,OFF);
	Led_Control(LED2,OFF);
	Led_Control(LED3,OFF);
	Led_Control(LED4,OFF);
	delay_c(0x500000);
	
}

void LED_Display(unsigned int count)
{
	switch(count)
	{
		case 0:
			Led_Control(LED1,OFF);
			Led_Control(LED2,OFF);
			Led_Control(LED3,OFF);
			Led_Control(LED4,OFF);
		break;
		
		case 1:
			Led_Control(LED1,ON);
			Led_Control(LED2,OFF);
			Led_Control(LED3,OFF);
			Led_Control(LED4,OFF);
		break;
		
		case 2:
			Led_Control(LED1,OFF);
			Led_Control(LED2,ON);
			Led_Control(LED3,OFF);
			Led_Control(LED4,OFF);
		break;
		
		case 3:
			Led_Control(LED1,ON);
			Led_Control(LED2,ON);
			Led_Control(LED3,OFF);
			Led_Control(LED4,OFF);
		break;
		
		case 4:
			Led_Control(LED1,OFF);
			Led_Control(LED2,OFF);
			Led_Control(LED3,ON);
			Led_Control(LED4,OFF);
		break;
		
		case 5:
			Led_Control(LED1,ON);
			Led_Control(LED2,OFF);
			Led_Control(LED3,ON);
			Led_Control(LED4,OFF);
		break;
		
		case 6:
			Led_Control(LED1,OFF);
			Led_Control(LED2,ON);
			Led_Control(LED3,ON);
			Led_Control(LED4,OFF);
		break;
		
		case 7:
			Led_Control(LED1,ON);
			Led_Control(LED2,ON);
			Led_Control(LED3,ON);
			Led_Control(LED4,OFF);
		break;
		
		case 8:
			Led_Control(LED1,OFF);
			Led_Control(LED2,OFF);
			Led_Control(LED3,OFF);
			Led_Control(LED4,ON);
		break;
		
		case 9:
			Led_Control(LED1,ON);
			Led_Control(LED2,OFF);
			Led_Control(LED3,OFF);
			Led_Control(LED4,ON);
		break;
		
		case 10:
			Led_Control(LED1,OFF);
			Led_Control(LED2,ON);
			Led_Control(LED3,OFF);
			Led_Control(LED4,ON);
		break;
		
		case 11:
			Led_Control(LED1,ON);
			Led_Control(LED2,ON);
			Led_Control(LED3,OFF);
			Led_Control(LED4,ON);
		break;
		
		case 12:
			Led_Control(LED1,OFF);
			Led_Control(LED2,OFF);
			Led_Control(LED3,ON);
			Led_Control(LED4,ON);
		break;
		
		case 13:
			Led_Control(LED1,ON);
			Led_Control(LED2,OFF);
			Led_Control(LED3,ON);
			Led_Control(LED4,ON);
		break;
		
		case 14:
			Led_Control(LED1,OFF);
			Led_Control(LED2,ON);
			Led_Control(LED3,ON);
			Led_Control(LED4,ON);
		break;
		
		case 15:
			Led_Control(LED1,ON);
			Led_Control(LED2,ON);
			Led_Control(LED3,ON);
			Led_Control(LED4,ON);
		break;
		
		default :
			break;
	}
}












int main()
{
    LED_init();

    // while(1){
    	LED_S();	
    // }

    
    return 0;
}