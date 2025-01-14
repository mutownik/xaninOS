

#include <lib/stdiox.h>
#include <xin_fs/xin.c>
#include <lib/memory.h>
#include <terminal/interpreter.h>
#include <lib/exit_process.h>
#include <lib/string.h>




void note_input(void)
{

    uint8_t selected_character;

    if((uint32_t)&Screen.cursor[Screen.y][Screen.x] < VGA_TEXT_MEMORY)
    {
        Screen.y++;
    }

    if((uint32_t)&Screen.cursor[Screen.y][Screen.x] > VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION)
    {
        Screen.y--;
    }


    if(KeyInfo.is_bspc)
    {       
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) + (((black << 4) | white) << 8));

        if(!Screen.x)
        {
            Screen.y--;
            Screen.x = 79;
            return;
        }

        Screen.x--;

        Screen.cursor[Screen.y][Screen.x] = '\0';
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) + (((lred << 4) | white) << 8));

        msleep(10);
        KeyInfo.is_bspc = false;
        letters_refresh(&Screen.cursor[Screen.y][Screen.x]);
    
    }

    else if(KeyInfo.is_up)
    {

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));

        Screen.y--;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((lred << 4) | white) << 8));

    }

    else if(KeyInfo.is_down)
    {

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));

        Screen.y++;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((lred << 4) | white) << 8));

    }

    else if(KeyInfo.is_right)
    {

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));

        Screen.x++;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((lred << 4) | white) << 8));


        if(Screen.x == 80)
        {
            Screen.x = 0x0;
            Screen.y++;
        }

    }

    else if(KeyInfo.is_left)
    {


        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));

        Screen.x--;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((lred << 4) | white) << 8));


        if(Screen.x == 80)
        {
            Screen.x = 0x0;
            Screen.y++;
        }


    }

    else if(getscan() == ENTER)
    {
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));
        xprintf("\r\n");
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((lred << 4) | white) << 8));
    }

    else
    {
        if(KeyInfo.character)
        {
            char character_saved_tmp = (char)Screen.cursor[Screen.y][Screen.x];
            xprintf("%c", getchar());
            letters_refresh_add(&Screen.cursor[Screen.y][Screen.x], character_saved_tmp);
        }
    }  

    /*
    else if(getscan() == DELETE_KEY)
    {
        Screen.cursor[Screen.y][Screen.x] = '\0';
        selected_character = '\0';
    }
    */

 
}

void xin_note(char* file_name)
{
    clearScr();
    keyboard_handle = note_input;
    use_backspace = true;

    xin_entry* xin_file = xin_find_entry(file_name);
    
    if(xin_file == nullptr)
    {
        xprintf("%zno such file or directory %s\n",set_output_color(red,white),program_parameters);
        keyboard_handle = nullptr;
        while(1)
        {
            if((getscan() == F4_KEY) || (getscan() == ENTER))
            {
                exit_process();
            }
        }   
    
    }
        
    else
    {
    
        if(xin_file->entry_type == XIN_DIRECTORY)
        {
            xprintf("%zyou cant edit directory\n",set_output_color(red,white));
            xprintf("%zuse F4 key to exit\n",set_output_color(red,white));
            while(keyboard_scan_code != F4_KEY);
            exit_process();
        }


        //for(uint8_t* xin_pointer_table = (uint8_t*)(XIN_POINTER_TABLE + xin_entry->starting_sector); *xin_pointer_table != XIN_EOF; xin_pointer_table++)
        
        char* data_pointer = xin_file->starting_sector * SECTOR_SIZE;

        
        for(int i = 0;  i < (VGA_SCREEN_RESOLUTION / 2); i++)
        {
            xprintf("%c",data_pointer[i]);
        }

        while(getscan() != F4_KEY && getscan() != ESC);

        uint32_t file_data_counter = 0x0;

        data_pointer = (char*)(xin_file->starting_sector * SECTOR_SIZE);

        for(char* i = (char*)VGA_TEXT_MEMORY; 
                (uint32_t)i < VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION; i+=2)
                if(*i == '\0')
                    *i = 0x20;

        for(char* i = (char*)VGA_TEXT_MEMORY; 
                (uint32_t)i < VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION; i+=2, file_data_counter++)
                data_pointer[file_data_counter] = *i;

        xin_file->entry_size = file_data_counter;

    }


    keyboard_handle = nullptr;
    exit_process();
}