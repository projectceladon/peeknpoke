/*
 * peeknpoke memory dump file
 *
 * Copyright (c) 2012, Intel Corporation.
 * Hari Kanigeri <hari.k.kanigeri@intel.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include "pnp_utils_inc.h"

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

/*
 *  This function reads the and prints the requested register value.
 */
int reg_read(unsigned int target, unsigned int dataBitSize)
{
	unsigned int read_val = 0;
	int status = 0;
	int fd;
	char *map_base;
	char *virt_addr;

	fd = open64("/dev/mem", O_RDWR | O_SYNC);
	if (fd >= 0) {
		map_base = mmap64(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
				target & ~MAP_MASK);
		if (map_base == MAP_FAILED) {
			printf("Failed to do memory mapping \n");
			close(fd);
			return -1;
		}
	} else {
		printf("Failed opening /dev/mem file\n");
		return -2;
	}
	virt_addr = map_base + (target & MAP_MASK);

	switch (dataBitSize) {
	case 32:
		read_val = (*(volatile unsigned int *) (virt_addr));
		break;
	case 16:
		read_val = (*(volatile unsigned short *) (virt_addr));
		break;
	case 8:
		read_val = (*(volatile unsigned char *) (virt_addr));
		break;
	default:
		printf("ILLEGAL dataBitSize: Enter 8, 16, or 32 bits \n");
		status = -1;
	}
	printf(" The value of register 0x%x is 0x%x\n", target, read_val);
	if (munmap(map_base, MAP_SIZE) == -1)
		printf("Memory Unmap failed \n");
	close(fd);
	return status;
}

/*
 *  This function writes to the requested register value.
 */
int reg_write(unsigned int target, unsigned int dataBitSize, unsigned int value)
{
	int status = 0;
	int fd;
	char *map_base;
	char *virt_addr;

	fd = open64("/dev/mem", O_RDWR | O_SYNC);
	if (fd) {
		map_base = mmap64(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
				target & ~MAP_MASK);
		if (map_base == MAP_FAILED) {
			printf("Failed to do memory mapping \n");
			close(fd);
			return -1;
		}
	} else {
		printf("Failed opening /dev/mem file\n");
		return -2;
	}
	virt_addr = map_base + (target & MAP_MASK);

	switch (dataBitSize) {
	case 32:
		(*(volatile unsigned int *) (virt_addr)) = value;
		break;
	case 16:
		(*(volatile unsigned short *) (virt_addr)) = (unsigned short) value;
		break;
	case 8:
		(*(volatile unsigned char *) (virt_addr)) = (unsigned char) value;
		break;
	default:
		printf("ILLEGAL dataBitSize: Enter 8, 16, or 32 bits \n");
		status = -1;
	}
	if (munmap(map_base, MAP_SIZE) == -1)
		printf("Memory Unmap failed \n");
	close(fd);
	return status;
}

/*
 *  This function dumps the requested number of words
 */
int addr_range_dump(unsigned int target, unsigned int numOfWords)
{
	unsigned int read_val = 0;
	int status = 0;
	int fd;
	char *map_base;
	char *virt_addr;

	if (numOfWords > MAP_SIZE/sizeof(numOfWords)) {
		printf("Number of Memory register dump request is exceding MAX Limit, which is %lu. \n", MAP_SIZE/sizeof(numOfWords));
		printf("Dumping only %lu numbers of memory mapped registers. \n", MAP_SIZE/sizeof(numOfWords));
		numOfWords = MAP_SIZE/sizeof(numOfWords);
	}
	fd = open64("/dev/mem", O_RDWR | O_SYNC);
	if (fd) {
		map_base = mmap64(0, MAP_SIZE, PROT_READ	| PROT_WRITE, MAP_SHARED, fd,
				target & ~MAP_MASK);
		if (map_base == MAP_FAILED) {
			printf("Failed to do memory mapping \n");
			close(fd);
			return -1;
		}
	} else {
		printf("Failed opening /dev/mem file\n");
		return -2;
	}
	virt_addr = map_base + (target & MAP_MASK);
	printf(" Address    Value\n");
	printf("________________________\n");
	read_val = (*(volatile unsigned int *) (virt_addr));
	while (numOfWords != 0) {
		read_val = *(volatile unsigned int *) (virt_addr);
		printf(" 0x%x....... 0x%x\n", target, read_val);
		virt_addr = virt_addr + 4;
		target = target + 4;
		numOfWords--;
	}
	if (munmap((unsigned int*) map_base, MAP_SIZE) == -1)
		printf("Memory Unmap failed \n");
	close(fd);
	return status;
}
