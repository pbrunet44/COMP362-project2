/**
* Name: Philip Brunet
* Lab/task: Project 2
* Date: 04/30/19
**/

#include "disk.h"

disk_t disk;

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

    printf("Logical sector number: %d\n", logaddr);
    if (log2phys(logaddr, &phaddr) == 0)
    {
        printf("Physical address: (%d, %d, %d)\n", phaddr.cyl, phaddr.head, phaddr.sect);

        if (phys2log(&phaddr) != logaddr)

            printf("ERROR: no match! %d != %d\n", phys2log(&phaddr), logaddr);
    }
    else
        printf("ERROR: invalid logical address!\n");
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
            printf("ERROR: invalid logical address!\n");

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
            printf("ERROR: invalid logical address!\n");

            return SIM_DEV_INVALID_ADDRESS;
        }
    }

    return 0;
}
/***
 * Generates random readable/printable content for testing
 */

char *generateContent(int size)
{
    char *content = malloc(size);

    int firstPrintable = ' ';
    int len = '~' - firstPrintable;

    for (int i = 0; i < size - 1; i++)
        *(content + i) = firstPrintable + rand() % len;

    content[size - 1] = '\0';
    return content;
}


#ifdef __DEBUG
int main(int argc, char *argv[])
{
    // TODO: extend to also test for reading and writing
    
    physaddr_t phaddr;

    char** readbuf = calloc(3*SECT_SIZE + 1, 1);
    char* writebuf = generateContent((3*SECT_SIZE));
    int logaddr;

    logaddr = rand() % MAX_LOGICAL_SECTOR;
    printTransl(logaddr);
    writeDisk(logaddr,3, writebuf);
    readDisk(logaddr,3, readbuf);
    printf("Read result is %s\noriginal string was %s\n", readbuf, writebuf);
    free(readbuf);
    readbuf = calloc(4*SECT_SIZE + 1, 1);
    logaddr = rand() % MAX_LOGICAL_SECTOR;
    printTransl(logaddr);
    writebuf = generateContent((5*SECT_SIZE));
    writeDisk(logaddr,4, writebuf);
    readDisk(logaddr,4, readbuf);
    printf("Read result is %s\noriginal string was %s\n", readbuf, writebuf);
    free(readbuf);
    /*
    if (argc < 2)
        while (1)
        {
            logaddr = rand() % MAX_LOGICAL_SECTOR;
            printTransl(logaddr);
            switch (rand() % 2)
            {
                case 0:
                    readDisk(logaddr, rand() % 8, (void *) &buf);
                    break;
                case 1:
                    writeDisk(logaddr, rand() % 8, buf);
                    break;
            }
            usleep(10);
        }

    switch (argv[1][0]) // "l" for logical "p" for physical
    {
        case 'l': // logical
            if (argv[2] == NULL)
                break;
            logaddr = strtol(argv[2], NULL, 10) % MAX_LOGICAL_SECTOR;
            printTransl(logaddr);
            break;

        case 'p': // physical
            if (argv[2] == NULL || argv[3] == NULL || argv[4] == NULL)
                break;
            phaddr.cyl = strtol(argv[2], NULL, 10);
            phaddr.head = strtol(argv[3], NULL, 10);
            phaddr.sect = strtol(argv[4], NULL, 10);
            logaddr = phys2log(&phaddr);
            printTransl(logaddr);
            break;
    }*/
}
#endif
