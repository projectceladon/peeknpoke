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
	close(fd);
	return;
}

