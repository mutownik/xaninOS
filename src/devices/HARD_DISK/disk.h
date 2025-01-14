#include <lib/stdiox.h>

#define SECTOR_SIZE 0x200

/* all ports all 8bit with exeception of DATA_REGISTER (16bit) */

enum AtaRegisters
{
    


    
    ATA_MASTER = 0x1,
    ATA_SLAVE = 0x0,

    ATA_FIRST_BUS = 0x1F0,
    ATA_SECONDARY_BUS = 0x170,
        

    ATA_DATA_REGISTER = 0x0,

    ATA_ERROR_REGISTER = 0x1,
    ATA_FEATURES_REGISTER = 0x1,

    ATA_SECTOR_COUNT_REGISTER = 0x2,

    ATA_SECTOR_NUMBER_LOW = 0x3,
    ATA_SECTOR_NUMBER_MID = 0x4,
    ATA_SECTOR_NUMBER_HIGH = 0x5,

    ATA_DRIVE_REGISTER = 0x6,
    ATA_STATUS_REGISTER = 0x7,
    ATA_COMMAND_REGISTER = 0x7,

    ATA_CONTROL_REGISTER = 0x206

};



enum AtaCommands
{

    ATA_IDENTIFY = 0xEC,
    ATA_READ = 0x20 

};

void init_disk(uint16_t base, uint8_t master);
void disk_read(uint16_t base, uint8_t master, uint32_t sector_number, uint32_t bytes_number, uint16_t* where); 
	
