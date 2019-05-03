/**
* Name: Philip Brunet
* Lab/task: Project 2
* Date: 04/30/19
**/

#ifndef __DISK_H
#define __DISH_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define NUM_OF_CYLS 200
#define NUM_OF_HEADS 10
#define NUM_OF_SECTS 128
#define SECT_SIZE 8

#define MAX_LOGICAL_SECTOR 256000 // NUM_OF_CYLS * NUM_OF_HEADS * NUM_OF_SECTS

typedef char sector_t[SECT_SIZE];
typedef sector_t head_t[NUM_OF_SECTS];
typedef head_t cylinder_t[NUM_OF_HEADS];
typedef cylinder_t disk_t[NUM_OF_CYLS];

typedef struct
{
  int cyl;
  int head;
  int sect;
} physaddr_t;

/**
This function translates a logical to a physical address.
**/
int log2phys(int logaddr, physaddr_t *phaddr);

/**
This function translates a physical to a logical address.
**/
int phys2log(physaddr_t *phaddr);

/**
This function copies num_of_sectors sectors from the disk to the buffer starting with sector pointed to by logical_block_num. You should validate the parameters before serving the request.
**/
int readDisk(int logicalBlockNum, int numOfBlocks, void **buffer);

/**
This function copies num_of_sectors sectors from the buffer to the disk starting at sector pointed to by logical_block_num. Here, you also should validate the parameters.
**/
int writeDisk(int logicalBlockNum, int numOfSectors, void *buffer);

/**\
 * ENUM for different types of errors
 */
 typedef enum {
     SIM_DEV_SUCCESS = 0,
     SIM_DEV_INVALID_ADDRESS,
     SIM_DEV_TOO_LARGE_BLOCK_RANGE
 }SIM_DEV_RETURN_CODES;

#endif
