/*
 * peeknpoke MSR access file
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

#include <inttypes.h>

#include "pnp_utils_inc.h"

#define msrdev_path	"/dev/msr%d"


/*
 *  This function reads from requested MSR address
 */
uint64_t msr_reg_read(int cpu, unsigned int reg_offset, int print_enabled)
{
	char buf[128];
	uint64_t temp = 0;
	int fd;
	int status = 0;

	snprintf(buf, sizeof(buf), msrdev_path, cpu);
	fd = open(buf, O_RDONLY | O_SYNC);

	if (fd >= 0) {
		if (pread(fd, &temp, sizeof(uint64_t), reg_offset)) {
			if (print_enabled)
				printf("MSR value of 0x%x offset is 0x%" PRIx64 "\n", reg_offset, temp);
			goto exit;
		}
	} else {
		printf("Cannot open %s\n", buf);
		status = -1;
		return status;
	}

exit:
	close(fd);
	return status;
}

/*
 *  This function writes to the requested MSR address
 */
int msr_reg_write(int cpu, unsigned int reg_offset, uint64_t value)
{
	char buf[128];
	int fd;
	uint64_t temp;
	int status = 0;

	snprintf(buf, sizeof(buf), msrdev_path, cpu);
	fd = open(buf, O_RDWR | O_SYNC);

	if (fd >= 0) {
		if (pwrite(fd, &value, sizeof(uint64_t), reg_offset) == sizeof(uint64_t)) {
			pread(fd, &temp, sizeof(uint64_t), reg_offset);
			if (temp != value) {
				printf("Cannot set 0x%" PRIu64 " to MSR 0x%x\n", value, reg_offset);
				status = -1;
				goto exit;
			}
		} else {
			printf("Cannot write to %s reg 0x%x value of 0x%" PRIx64 "\n",
			       buf, reg_offset, value);
			status = -1;
			goto exit;
		}

	} else {
		printf("Cannot open %s\n", buf);
		status = -1;
		return status;
	}

exit:
	close(fd);
	return status;
}
