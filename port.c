/*
 * peeknpoke North complex registers file
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

#define PUNIT_PORT			0x04
#define PCI_READ_COMMAND	0x10
#define PCI_WRITE_COMMAND	0x11

#define CUNIT_PATH          "/sys/devices/pci0000:00/0000:00:00.0/config"
#define PORT_DEV 			"/dev/port"

#define PCI_DEV_PATH		"/sys/devices/pci0000:00/000%d:00:0%d.%d/config"

/*
 *  This function reads from the North complex port register
 */
int read_nc_port(int reg, int port, int *ret_val)
{
	static int fd = -1;
	unsigned int val;
	int cmd = (PCI_READ_COMMAND << 24) | (port << 16) |
							(reg << 8) | (0xf0) ;
	int ret, value;

	if (fd < 0)
		fd = open(CUNIT_PATH,  O_RDWR | O_SYNC);
	if (fd < 0) {
		printf("open CUNIT_PATH ret = %d line %d\n", fd, __LINE__);
		goto error;
	}

	ret = lseek(fd, 0xD0, SEEK_SET);
	if (ret < 0) {
		printf("lseek failed ret = %d line %d\n", fd, __LINE__);
		goto error;
	}

	ret = write(fd, &cmd, 4);
	if (ret < 0) {
		printf("write failed ret = %d line %d\n", fd, __LINE__);
		goto error;
	}

	ret = lseek(fd, 0xD4, SEEK_SET);
	if (ret < 0) {
		printf("lseek failed ret = %d line %d\n", fd, __LINE__);
		goto error;
	}

	ret = read(fd, &value, 4);
	if (ret < 0) {
		printf("lseek failed ret = %d line %d\n", fd, __LINE__);
		goto error;
	}
	printf("Port 0x%x Register 0x%x value is 0x%x \n", port, reg, value);
	*ret_val = value;

    return 0;

error:
	return -1;

}

/*
 *  This function writes to the North complex port register
 */
int write_nc_port(int reg, int port, int value)
{
	static int fd = -1;
	unsigned int val;
	int cmd = (PCI_WRITE_COMMAND << 24) | (port << 16) |
							(reg << 8) | (0xf0);
	int ret;

	if (fd < 0)
		fd = open(CUNIT_PATH,  O_RDWR | O_SYNC);
	if (fd < 0) {
		printf("open CUNIT_PATH ret = %d line %d\n", fd, __LINE__);
		return -1;
	}
	ret = lseek(fd, 0xD4, SEEK_SET);
	if (ret < 0) {
		printf("lseek failed ret = %d line %d\n", fd, __LINE__);
		goto error;
	}

	ret = write(fd, &value, 4);
	if (ret < 0) {
		printf("write failed ret = %d line %d\n", fd, __LINE__);
		goto error;
	}

	ret = lseek(fd, 0xD0, SEEK_SET);
	if (ret < 0) {
		printf("lseek failed ret = %d line %d\n", fd, __LINE__);
		goto error;
	}

	ret = write(fd, &cmd, 4);
	if (ret < 0) {
		printf("write failed ret = %d line %d\n", fd, __LINE__);
		goto error;
	}
    return 0;

error:
	return -1;
}

/*
 *  This function reads from the North complex port number
 */
int read_port(int port, int *port_value)
{
	static int fd = -1;
	unsigned int value;
	int ret;

	if (fd < 0)
		fd = open(PORT_DEV,  O_RDWR | O_SYNC);
	if (fd < 0) {
		printf("open CUNIT_PATH ret = %d line %d\n", fd, __LINE__);
		return -1;
	}

	ret = lseek(fd, port, SEEK_SET);
	if (ret < 0) {
		printf("lseek failed ret = %d line %d\n", fd, __LINE__);
		goto error;
	}

	ret = read(fd, &value, 4);
	if (ret < 0) {
		printf("read failed ret = %d line %d\n", fd, __LINE__);
		goto error;
	}
	*port_value = value;
	printf("Port 0x%x value is value 0x%x\n", port, value);
	return 0;

error:
	return -1;
}

/*
 *  This function reads from the North complex port number
 */
int write_port(int port, int port_value)
{
	static int fd = -1;
	int ret;

	if (fd < 0)
		fd = open(PORT_DEV,  O_RDWR | O_SYNC);
	if (fd < 0) {
		printf("open CUNIT_PATH ret = %d line %d\n", fd, __LINE__);
		return -1;
	}

	ret = lseek(fd, port, SEEK_SET);
	if (ret < 0) {
		printf("lseek failed ret = %d line %d\n", fd, __LINE__);
		goto error;
	}

	ret = write(fd, &port_value, 4);
	if (ret < 0) {
		printf("write failed ret = %d line %d\n", fd, __LINE__);
		goto error;
	}
	return 0;

error:
	return -1;
}

/*
 *  This function reads from the PCI config register
 */
int read_pci_reg(int bus, int dev, int func, int reg, int *port_value)
{
	static int fd = -1;
	int ret;
	int value;
	char buf[128];

	snprintf(buf, sizeof(buf), PCI_DEV_PATH, bus, dev, func);
	fd = open(buf, O_RDWR | O_SYNC);
	if (fd < 0) {
		printf("open pci dev %s failed ret = %d line %d\n", buf, fd, __LINE__);
		goto error;
	}

	ret = lseek(fd, reg, SEEK_SET);
	if (ret < 0) {
		printf("lseek failed ret = %d line %d\n", fd, __LINE__);
		goto error;
	}

	ret = read(fd, &value, 4);
	if (ret < 0) {
		printf("read failed ret = %d line %d\n", fd, __LINE__);
		goto error;
	}

	*port_value = value;

	printf("PCI bus 0x%x dev 0x%x func 0x%x reg value 0x%x value 0x%x\n",
											bus, dev, func, reg, value);
	return 0;

error:
	return -1;
}

/*
 *  This function writes to the PCI config register
 */
int write_pci_reg(int bus, int dev, int func, int reg, int value)
{
	static int fd = -1;
	int ret;
	char buf[128];

	snprintf(buf, sizeof(buf), PCI_DEV_PATH, bus, dev, func);

	fd = open(buf, O_RDWR | O_SYNC);
	if (fd < 0) {
		printf("open pci dev %s failed ret = %d line %d\n", buf, fd, __LINE__);
		goto error;
	}

	ret = lseek(fd, reg, SEEK_SET);
	if (ret < 0) {
		printf("lseek failed ret = %d line %d\n", fd, __LINE__);
		goto error;
	}

	ret = write(fd, &value, 4);
	if (ret < 0) {
		printf("write failed ret = %d line %d\n", fd, __LINE__);
		goto error;
	}

	read_pci_reg(bus, dev, func, reg, &ret);
	if (ret == value)
		printf("Write OK\n");
	else
		printf("Write FAILED\n");

	return 0;

error:
	return -1;
}
