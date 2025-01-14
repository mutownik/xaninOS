#pragma once

#include <terminal/vty.h>
#include <tetris/tetris.c>
#include <xaninApps/help.c>
#include <lib/signal.h>

void add_y(uint8_t yadd)
{
    cursor += yadd * 80;
    y++;
}

void set_y(uint8_t yset)
{
    cursor = (uint16_t*)VRAM + (yset * 80);
    y = yset;
}

 
void screen_init(void)
{
    //screen.cursor = VGA_TEXT_MEMORY;
    
    Screen.cursor = screen_buffer;

    for(int i = 0; i < VGA_HEIGHT; i++)
        Screen.cursor[i] = (uint16_t*)(VGA_TEXT_MEMORY + (80 * i * 2));  

    Screen.x = 0x0;
    Screen.y = 0x0;
    
}

void letters_refresh(uint16_t* cursor_current_positon)
{

    cursor_current_positon++;

    for(uint16_t* i = cursor_current_positon; (uint32_t)i < VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION; i++)
    {
        *(i - 1) = *i;
    }
}

void letters_refresh_add(uint16_t* cursor_current_positon, char character_saved)
{

    //cursor_current_positon--;

    char tmp;

    for(uint16_t* i = cursor_current_positon; (uint32_t)i < VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION; i++)
    {
        tmp = *(char*)(i);
        *(char*)(i) = character_saved;
        character_saved = tmp;
    }
}

void terminal_keyboard(void)
{
    if(KeyInfo.is_left)
    {        
        //Screen.cursor[Screen.y][Screen.x] = (uint16_t)(selected_character | ((black << 4) | white) << 8);
        
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));
        


        if((char)Screen.cursor[Screen.y][Screen.x - 1] == character_blocked)
        {
            return;
        }

        if(!Screen.x)
        {
            Screen.x = 0;
            Screen.y--;
        }


        Screen.x--;
        
        if(index)
            index--;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | ((lred << 4) | white) << 8);
    }

    else if(KeyInfo.is_right)
    {
        //Screen.cursor[Screen.y][Screen.x] = (uint16_t)(selected_character | ((black << 4) | white) << 8);
        
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));
        

        if(&Screen.cursor[Screen.y][Screen.x + 1] >= &Screen.cursor[8][79])
        {
            return;
        }

        Screen.x++;

        if(Screen.x == 80)
        {
            Screen.x = 0;
            Screen.y++;
        }


        //if(index)
        //    index--;


        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | ((lred << 4) | white) << 8);    
    }



}


void terminal_refresh(void)
{
    
    if(KeyInfo.scan_code == ARROW_LEFT)
    {
        if(*(cursor-1) == (uint16_t)('>' | ((black << 4) | white) << 8))
            return;

        cursor--;
    }

    else if(KeyInfo.scan_code == ARROW_RIGHT)
    {
        cursor++;
    }

    else if(KeyInfo.scan_code == ENTER)
    {

        if(strlen(keyboard_command) != 0)
        {
            y++;
            x = 0;
            //cursor = (uint16_t*)VRAM;
            //add_y(y);

            if(!no_enter)
            {
                /*
                    cursor = (uint16_t)('>' | ((black << 4) | white) << 8); 
                    cursor++;
                    x++;
                */

            }
            
        }

        index = 0x0;       

    }



    else
    {
        if(KeyInfo.character != '\0')
        {
            *cursor = (uint16_t)(KeyInfo.character | ((black << 4) | white) << 8); 
            keyboard_command[index] = KeyInfo.character;
            index++;
            cursor++;
            x++;

        }
    }

    KeyInfo.character = 0x0;


}