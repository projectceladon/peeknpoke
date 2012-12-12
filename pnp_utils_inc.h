/*
 * peeknpoke header file
 *
 * Copyright (c) 2012, Intel Corporation.
 * Hari Kanigeri <hari.k.kanigeri@intel.com>
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

#ifndef PNP_UTILS_INC_H_
#define PNP_UTILS_INC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <ctype.h>

#define SMBUS_BYTE_DATA	    		2
#define SMBUS_WORD_DATA	    		3
#define SMBUS_BLOCK_DATA	    	5
#define I2C_SMBUS_BLOCK_DATA	    	8

int read_i2c_device(char bus, int addr, int reg, int size, int *result);
int write_i2c_device(char bus, int addr, int reg, int size, int value);
int block_write_i2c_device(char bus, int addr, int reg, int size, uint8_t array_size, uint8_t *values);
int block_read_i2c_device(char bus, int addr, int reg, int size, uint8_t array_size, uint8_t *result);

int msic_program (void);
void msic_read(unsigned short target);
void msic_write(unsigned short target, unsigned short value);

int hexstring_to_int(const char* hexString, unsigned int* result);
int hexstring_to_long(const char* hexString, uint64_t* result);

int addr_range_dump(unsigned int target, unsigned int numOfWords);

int reg_write(unsigned int target, unsigned int dataBitSize, unsigned int value);
int reg_read(unsigned int target, unsigned int dataBitSize);

uint64_t msr_reg_read(int cpu, unsigned int reg_offset, int print_enabled);
int msr_reg_write(int cpu, unsigned int reg_offset, uint64_t value);

int read_nc_port(int v, int port, int *ret_val);
int write_nc_port(int v, int port, int value);
int read_port(int port, int *port_value);
int write_port(int port, int port_value);

int write_pci_reg(int bus, int dev, int func, int reg, int port_value);
int read_pci_reg(int bus, int dev, int func, int reg, int *port_value);

#endif /* PNP_UTILS_INC_H_ */
