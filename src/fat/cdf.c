#include <fat/fat_driver.h>
#include <lib/stdiox.h>
#include <lib/algorithm.h>
#include <lib/string.h>
#include <lib/exit_process.h>

void cdf(char* folder_name)
{

    uint8_t folder_name_len = strlen(folder_name);

    for(int i = 0; i < FILENAME_MAX_LENGTH; i++)
    {
    
        if(i < folder_name_len)
        {
            if(folder_name[i] < 0x20)
                folder_name[i] = 0x20;      
        }
    
        else
        {
            folder_name[i] = 0x20;
        }
    }

    root_directory_entry* folder = find_file(folder_name);

    if(folder == nullptr)
    {
        xprintf("%zNO SUCH FOLDER\n", set_output_color(red,white));
        while(keyboard_scan_code != ENTER);
        exit_process();
    }

    fat.current_folder = (uint8_t*)(folder->starting_cluster * CLUSTER_SIZE); 
    
    //if(used_folder == nullptr)
    
    if(used_folder == ROOT_DIRECTORY_START)
        used_folder = (fat_folder*)(folder->starting_cluster * CLUSTER_SIZE); 

    else
    {
        for(fat_folder* i = used_folder; (uint32_t)i < ((uint32_t)used_folder + (CLUSTER_SIZE * 2)); i++)
        {
     
            if(cmpstr(folder_name, (char*)i))
            {
         	    //xprintf("%zFILE EXIST", set_output_color(green,white));
                used_folder = i->starting_cluster * CLUSTER_SIZE;
		    }
	
        }

    }


    xprintf("folder: %d\n", fat.current_folder);

    while(keyboard_scan_code != ENTER);
    exit_process();

    for(int i = 0; i < 8; i++)
        fat_current_folder[i] = folder_name[i];

}
