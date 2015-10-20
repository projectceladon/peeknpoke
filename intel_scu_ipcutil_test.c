/*
 * peeknpoke MSIC access file
 *
 * Copyright (c) 2012, Intel Corporation.
 * Asutosh Pathak <asutosh.pathak@intel.com>
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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/ioctl.h>
#include <string.h>

#define TRUE 	1
#define FALSE 	0

/* ioctl commnds */
#define	INTE_SCU_IPC_REGISTER_READ		0
#define INTE_SCU_IPC_REGISTER_WRITE		1
#define INTE_SCU_IPC_REGISTER_UPDATE	4

typedef int bool;


struct scu_ipc_data {
	unsigned long     count;  		/* No. of registers */
	unsigned short     addr[5]; 	/* Register addresses */
	unsigned char      data[5]; 	/* Register data */
	unsigned char      mask; 		/* Valid for read-modify-write */
};

static bool msic_ioctl_operation(int choice)
{
	int fd = -1;
	unsigned long nc = 0;
	bool ret = FALSE;
	unsigned short base_res = 0x00;
	int offset = 0;
	int offset_count = 0;
	unsigned short	temp_data = 0x0;
	struct scu_ipc_data ipc_data;

    	fd = open("/dev/mid_ipc", O_RDWR);
    	if (fd == -1) {
		printf("\nOpening /dev/mid_ipc error, app aborted");
		return FALSE;
	}

	memset((void*)&ipc_data, 0, sizeof(ipc_data));

	switch (choice) {
		case INTE_SCU_IPC_REGISTER_READ:
			printf("\nEnter Register Address(s) without 0x prefixed(MAX FOUR and then ends with *) : ");
			while (getchar() != '*') {		//TODO need to change the logic here
				scanf("%hx", &ipc_data.addr[ipc_data.count]);
				ipc_data.count += 1;
			}
			ipc_data.count -= 1;	// TODO need to change the logic here
			if (ipc_data.count == 3) {
				ipc_data.count ++;
			}


			if (!ioctl(fd, INTE_SCU_IPC_REGISTER_READ, &ipc_data)) {
				printf("\n\n\tREGISTER ADDRESS  \tREGISTER VALUE");
				for (nc = 0; nc < ipc_data.count; nc++) {
					printf("\n\t      0x%x\t\t\t0x%x", ipc_data.addr[nc],
													ipc_data.data[nc]);
				}
				printf("\n");
				ret = TRUE;
			}
			else {
				printf("\nError issuing while reading MSIC register");
				ret = FALSE;
			}
			break;

		case INTE_SCU_IPC_REGISTER_WRITE:
			printf("\nEnter Register Address and Value to be written (Both without 0x prefixed) : ");
			scanf("%hx %hx", &ipc_data.addr[ipc_data.count], &temp_data);


			ipc_data.data[ipc_data.count] = temp_data;
			ipc_data.count = 1;


			if (!ioctl(fd, INTE_SCU_IPC_REGISTER_WRITE, &ipc_data)) {
				printf("\nMSIC register write successful\n");
				ret = TRUE;
			}
			else {
				printf("\nError issuing while Writing MSIC register");
				ret = FALSE;
			}
			break;

		default:
			ret = FALSE;
			break;
	}

	close(fd);
	return ret;
}


void msic_read(unsigned short msic_addr)
{
	int fd = -1;
	int nc = 0;
	struct scu_ipc_data ipc_data;

	fd = open("/dev/mid_ipc", O_RDWR);
	if (fd == -1) {
		printf("\nOpening /dev/mid_ipc error, app aborted");
		return;
	}

	memset((void*)&ipc_data, 0, sizeof(ipc_data));

	ipc_data.addr[ipc_data.count] = msic_addr;
	ipc_data.count = 1;

	if (!ioctl(fd, INTE_SCU_IPC_REGISTER_READ, &ipc_data)) {
		printf("0x%x", ipc_data.data[nc]);
	}
	else {
		printf("\nError issuing while reading MSIC register");
	}

	close(fd);
}

void msic_write(unsigned short msic_addr, unsigned short data)
{
	int fd = -1;
	int nc = 0;
	struct scu_ipc_data ipc_data;

	fd = open("/dev/mid_ipc", O_RDWR);
	if (fd == -1) {
		printf("\nOpening /dev/mid_ipc error, app aborted");
		return;
	}

	memset((void*)&ipc_data, 0, sizeof(ipc_data));

	ipc_data.addr[ipc_data.count] = msic_addr;
	ipc_data.data[ipc_data.count] = data;
	ipc_data.count = 1;

	if (ioctl(fd, INTE_SCU_IPC_REGISTER_WRITE, &ipc_data)) {
		printf("\nError issuing while Writing MSIC register");
	}

	return;
}

