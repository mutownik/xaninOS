
#include <stdint.h>
#include <pit/pit.c>
#include <IDT/idt.c>
#include <lib/stdlibx.h>
#include <terminal/vty.c>
#include <terminal/interpreter.c>
#include <lib/math.h>
#include <fs/ustar.c>
#include <keyboard/keyboardInit.c>
//#include <devices/DMA/dma.c>

char* tmpStr;


/*--------------------------------------/
|wesolego nowego roku :))               |
|niech xanin rośnie i rośnie            |
|ja, rok 2021, 31 grudzień, 23:52:35    |     
/--------------------------------------*/



void _start(void)
{

    asm("cli");

    COMMAND = comBuf;


    keyboard_init();
    set_pit();
    set_idt();
    //dma_controller_reset();
    

    clearScr();

    disableCursor();

   
    getCpuSpeed();

    getTime();

    srand(time.seconds);
    clearScr();

    file_system_init();



    tuiInit:

    clearScr();
    


    x = 0;y = 0;


    xprintf("xaninOS\n");
    xprintf("version 22.01v\n");

    getTime();
    xprintf("weekday: %s\n\n",weekDaysLUT[time.weekDay]);


    *cursor = (uint16_t)('>' | ((black << 4) | white) << 8); 
    cursor++;
    x++;

    app_exited = false;
    



    while(1)
    {


        if(app_exited)
        {
            app_exited = false;
            for(int i = 0; i < sizeof(comBuf); i++)
                COMMAND[i] = '\0';
            goto tuiInit;
        }

      
        if(keyboard_scan_code == ENTER)
        {
            scan();
        }

    }
    
}