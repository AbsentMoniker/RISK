#include "flash.h"
#include <p32xxxx.h>
#include "gamelogic.h"
#include <string.h>

#define FLASH_VIRTUAL_ADDR 0xBD1FC000
#define FLASH_PHYSICAL_ADDR (FLASH_VIRTUAL_ADDR & 0x1FFFFFFF)

const volatile unsigned char * const flashPtr = (void *)FLASH_VIRTUAL_ADDR;

void readSaveFromFlash()
{
    memcpy(saveData, (const void *)flashPtr, FLASH_ROW_SIZE);
}

void writeSaveToFlash()
{
    NVMADDR = FLASH_PHYSICAL_ADDR;
    NVMSRCADDR = ((unsigned int)saveData) & 0x1FFFFFFF;
    NVMCONbits.NVMOP = 0b0011;
    NVMCONbits.WREN = 1;
    initiateFlashOp();
    while(NVMCON & _NVMCON_WR_MASK)
    {}
    NVMCONbits.WREN = 0;
    if(NVMCON & 0x3000)
    {
        PANIC("failed to write save game");
    }
}

void eraseFlash()
{
    NVMADDR = FLASH_PHYSICAL_ADDR;
    NVMCONbits.NVMOP = 0b0100;
    NVMCONbits.WREN = 1;
    initiateFlashOp();
    while(NVMCON & _NVMCON_WR_MASK)
    {}
    NVMCONbits.WREN = 0;
    if(NVMCON & 0x3000)
    {
        PANIC("failed to erase save game");
    }
}

void initiateFlashOp()
{
    __builtin_disable_interrupts();
    NVMKEY = 0x0;
    NVMKEY = 0xAA996655;
    NVMKEY = 0x556699AA;
    NVMCONSET = _NVMCON_WR_MASK;
    __builtin_enable_interrupts();
}
