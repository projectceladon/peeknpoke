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
#define	INTE_SCU_IPC_REGISTER_DUMP		2
#define INTE_SCU_IPC_SUBSYSTEM_REGISTER_DUMP	3
#define INTE_SCU_IPC_REGISTER_UPDATE	4

typedef int bool;


struct scu_ipc_data {
	unsigned long     count;  		/* No. of registers */
	unsigned short     addr[5]; 	/* Register addresses */
	unsigned char      data[5]; 	/* Register data */
	unsigned char      mask; 		/* Valid for read-modify-write */
};

struct msic_subsystem {
	const char *name;
	unsigned short start_address;
	unsigned short end_address;
	int size;
};

enum msic_subsystems_id {
	MSIC_CHIP_ID,
	MSIC_IRQ,
	MSIC_GPIO,
	MSIC_SVID,					/* Serial Voltage ID */
	MSIC_VOLTAGE_REGULATORS,
	MSIC_RESET,
	MSIC_BCU,					/* Burst Control Unit */
	MSIC_RTC,					/* Real Time Clock   */
	MSIC_BATTERY_CHARGER,
	MSIC_ADC,					/* Analog to Digital Converter */
	MSIC_AUDIO,
	MSIC_HDMI,
	MSIC_INTEL_USB,
	MSIC_INTEL_MISCELLANEOUS,
	MSIC_VENDOR_USB,
	MSIC_VENDOR_MISCELLANEOUS,
	MSIC_MAX
};

struct msic_subsystem msic_system[MSIC_MAX] = {
		{ "Chip_Id", 					0x0000, 0x0001, 2   },
		{ "IRQ",     					0x0002, 0x003F, 62  },
		{ "GPIO",    					0x0040, 0x007F, 64  },
		{ "Serial_Voltage_ID",    		0x0080, 0x00BF, 64  },
		{ "Voltage_Regulators",			0x00C0, 0x00FF, 64  },
		{ "Reset",						0x0100, 0x0101, 2   },
		{ "Burst_Control_Unit",			0x0102, 0x013F, 62  },
		{ "Real_Time_Clock",			0x0140, 0x017F, 64  },
		{ "Battery_Charger", 			0x0180, 0x01BF, 64  },
		{ "Analog_2_Digital_Converter",	0x01C0, 0x023F, 128 },
		{ "Audio", 						0x0240, 0x027F, 64  },
		{ "HDMI", 						0x0280, 0x02BF, 64  },
		{ "Intel_USB",		 			0x02C0, 0x02FF, 64  },
		{ "Intel_Misc", 				0x0340, 0x037F, 64  },
		{ "Vendor_USB", 				0x0300, 0x033F, 64  },
		{ "Vendor_Misc", 				0x0380, 0x03FF, 128 }
};

int choosed_msic_system = MSIC_MAX;

static bool msic_ioctl_operation(int choice)
{
	int fd = -1;
	int nc = 0;
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
				scanf("%x", &ipc_data.addr[ipc_data.count]);
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

		case INTE_SCU_IPC_REGISTER_DUMP:
			printf("\nEnter Base Register Address and offset value (Both without 0x prefixed): ");
			scanf("%x %d", &base_res, &offset);

			for (offset_count = 0; offset_count < offset/4; offset_count++) {
				ipc_data.addr[0] = base_res + (offset_count * 4);
				ipc_data.addr[1] = ipc_data.addr[0] + 1;
				ipc_data.addr[2] = ipc_data.addr[1] + 1;
				ipc_data.addr[3] = ipc_data.addr[2] + 1;

				ipc_data.count = 4;

				if (!ioctl(fd, INTE_SCU_IPC_REGISTER_READ, &ipc_data)) {
					if (offset_count == 0) {
						printf("\n\n\tREGISTER ADDRESS  \tREGISTER VALUE");
					}
					for (nc = 0; nc < ipc_data.count; nc++) {
						printf("\n\t      0x%x\t\t\t0x%x", ipc_data.addr[nc], ipc_data.addr[nc]);
					}
				}
				else {
					printf("\nError issuing while reading MSIC register");
					return FALSE;
				}
			}

			if (offset % 4)
			{
				memset((void*)&ipc_data, 0, sizeof(ipc_data));
				for (offset_count = 0; offset_count < (offset % 4); offset_count++ ) {
					ipc_data.addr[offset_count] = base_res +
							(( offset / 4 ) * 4) + offset_count;
				}
				ipc_data.count = offset % 4;

				if (ipc_data.count == 3) {
					ipc_data.count ++;
				}

				if (!ioctl(fd, INTE_SCU_IPC_REGISTER_READ, &ipc_data)) {
					if (offset < 4) {
						printf("\n\n\tREGISTER ADDRESS  \tREGISTER VALUE");
					}
					for (nc = 0; nc < ipc_data.count; nc++) {
						printf("\n\t      0x%x\t\t\t0x%x", ipc_data.addr[nc],
															ipc_data.data[nc]);
					}
				}
				else {
					printf("\nError issuing while reading MSIC register");
					return FALSE;
				}
			}
			printf("\n");
			ret = TRUE;
			break;

		case INTE_SCU_IPC_REGISTER_WRITE:
			printf("\nEnter Register Address and Value to be written (Both without 0x prefixed) : ");
			scanf("%x %x", &ipc_data.addr[ipc_data.count], &temp_data);


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

		case INTE_SCU_IPC_SUBSYSTEM_REGISTER_DUMP:
			base_res = msic_system[choosed_msic_system].start_address;
			offset = msic_system[choosed_msic_system].size;

			for (offset_count = 0; offset_count < offset/4; offset_count++) {
				ipc_data.addr[0] = base_res + (offset_count * 4);
				ipc_data.addr[1] = ipc_data.addr[0] + 1;
				ipc_data.addr[2] = ipc_data.addr[1] + 1;
				ipc_data.addr[3] = ipc_data.addr[2] + 1;

				ipc_data.count = 4;

				if (!ioctl(fd, INTE_SCU_IPC_REGISTER_READ, &ipc_data)) {
					if (offset_count == 0) {
						printf("\n\n\tREGISTER ADDRESS  \tREGISTER VALUE");
					}
					for (nc = 0; nc < ipc_data.count; nc++) {
						printf("\n\t      0x%x\t\t\t0x%x", ipc_data.addr[nc],
															ipc_data.data[nc]);
					}
				}
				else {
					printf("\nError issuing while reading MSIC register");
					return FALSE;
				}
			}

			if (offset % 4)
			{
				memset((void*)&ipc_data, 0, sizeof(ipc_data));
				for (offset_count = 0; offset_count < (offset % 4);
												offset_count++ ) {
					ipc_data.addr[offset_count] = base_res +
							(( offset / 4 ) * 4) + offset_count;
				}
				ipc_data.count = offset % 4;

				if (ipc_data.count == 3) {
					ipc_data.count ++;
				}

				if (!ioctl(fd, INTE_SCU_IPC_REGISTER_READ, &ipc_data)) {
					if (offset < 4) {
						printf("\n\n\tREGISTER ADDRESS  \tREGISTER VALUE");
					}
					for (nc = 0; nc < ipc_data.count; nc++) {
						printf("\n\t      0x%x\t\t\t0x%x", ipc_data.addr[nc],
															ipc_data.data[nc]);
					}
				}
				else {
					printf("\nError issuing while reading MSIC register");
					return FALSE;
				}
			}
			printf("\n");
			ret = TRUE;
			break;

		default:
			ret = FALSE;
			break;
	}

	close(fd);
	return ret;
}

int msic_program (void)
{
    int i = 0;
	int choice = 0;
	bool ret = FALSE;

	while (TRUE) {
		choice = 0;

		/* Enter your choice */
		printf("\n 0. Read MSIC Register");
		printf("\n 1. Write MSIC Register");
		printf("\n 2. Dump MSIC Registers");
		printf("\n 3. Dump MSIC Sub-systems Registers");
		printf("\n 4. Exit\n");
		printf("\nEnter your choice : ");

		scanf("%d", &choice);

		if (choice == 4) {
			/* Exit from this App */
			exit(-1);
		}
		else if (choice < 0 || choice > 4) {
			printf("\nInvalid choice. Try again...");
		}
		else {
			if (choice == 3) {
				printf("\n");
				for (i = 0; i < MSIC_MAX; i++) {
					printf("\n %d : %s", i, msic_system[i].name);
				}
				choosed_msic_system = MSIC_MAX;
				printf("\nChoose MSIC sub-system : ");
				scanf("%d", &choosed_msic_system);
				printf("\nYou have choosen %d : %s\n", choosed_msic_system, msic_system[choosed_msic_system].name);
				if (choosed_msic_system < MSIC_CHIP_ID || choosed_msic_system >= MSIC_MAX) {
					printf("\nInvalid MSIC Subsystem");
					goto Exit;
				}
			}
			ret = msic_ioctl_operation(choice);
		/* MSIC operation fails. Break this loop and exit from app*/
			if (ret == FALSE) {
			break;
			}
		}
	}

Exit:
	printf("\n\n");
	return 0;
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

