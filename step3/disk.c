/**
* Name: Philip Brunet
* Lab/task: Project 2
* Date: 05/09/19
**/

#include "disk.h"

extern disk_t disk;

int log2phys(int logaddr, physaddr_t *phaddr)
{
    // TODO: implement
    if(logaddr > MAX_LOGICAL_SECTOR)
    {
        return SIM_DEV_INVALID_ADDRESS;
    }
    int cylinderCapacity = NUM_OF_SECTS * NUM_OF_HEADS;
    phaddr->cyl = logaddr/cylinderCapacity;
    phaddr->head = (logaddr % SECT_SIZE) / NUM_OF_SECTS;
    phaddr->sect = logaddr % NUM_OF_SECTS;

    return SIM_DEV_SUCCESS;
}

int phys2log(physaddr_t *phaddr)
{
    // TODO: implement

    return phaddr->sect+phaddr->head * NUM_OF_SECTS + phaddr->cyl * NUM_OF_HEADS * NUM_OF_SECTS;
}

void printTransl(int logaddr)
{
    physaddr_t phaddr;

    printk("Logical sector number: %d\n", logaddr);
    if (log2phys(logaddr, &phaddr) == 0)
    {
        printk("Physical address: (%d, %d, %d)\n", phaddr.cyl, phaddr.head, phaddr.sect);

        if (phys2log(&phaddr) != logaddr)

            printk("ERROR: no match! %d != %d\n", phys2log(&phaddr), logaddr);
    }
    else
        printk("ERROR: invalid logical address!\n");
}

int readDisk(int logicalBlockNum, int numOfBlocks, void **buffer)
{
    // TODO: implement
    if(logicalBlockNum + numOfBlocks > MAX_LOGICAL_SECTOR)
    {
        return SIM_DEV_TOO_LARGE_BLOCK_RANGE;
    }
    physaddr_t physAddr;
    //*buffer = calloc(numOfBlocks*SECT_SIZE + 1, 1);
    for (int i = 0; i < numOfBlocks; ++i)
    {
        if(log2phys(logicalBlockNum + i, &physAddr) == SIM_DEV_SUCCESS)
        {
            memcpy(((char*)buffer + i*SECT_SIZE), disk[physAddr.cyl][physAddr.head][physAddr.sect], SECT_SIZE);
        }
        else
        {
            printk("ERROR: invalid logical address!\n");

            return SIM_DEV_INVALID_ADDRESS;
        }

    }
    *((char*)buffer + (numOfBlocks * SECT_SIZE)) = '\0';


    return SIM_DEV_SUCCESS;
}

int writeDisk(int logicalBlockNum, int numOfSectors, void *buffer)
{
    // TODO: implement
    if(logicalBlockNum + numOfSectors > MAX_LOGICAL_SECTOR)
    {
        return SIM_DEV_TOO_LARGE_BLOCK_RANGE;
    }
    physaddr_t physAddr;

    for (int i = 0; i < numOfSectors; ++i)
    {
        if(log2phys(logicalBlockNum + i, &physAddr) == SIM_DEV_SUCCESS)
        {
            memcpy(disk[physAddr.cyl][physAddr.head][physAddr.sect], ((char*)buffer + i*SECT_SIZE), SECT_SIZE);
        }
        else
        {
            printk("ERROR: invalid logical address!\n");

            return SIM_DEV_INVALID_ADDRESS;
        }
    }

    return 0;
}

