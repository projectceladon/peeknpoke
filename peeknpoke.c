/*
 * peeknpoke
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

#include "pnp_utils_inc.h"

static void usage(void)
{
	printf("Usage commands: ./peeknpoke.out <arg>\n"
				"\t <arg> values are\n"
				"\t r for Register read\n"
				"\t w for Register write\n"
				"\t d for Register range dump\n"
				"\t m for MSIC accesses\n"
				"\t m r to read MSIC register\n"
				"\t m w to write MSIC register\n"
				"\t s r to read MSR register\n"
				"\t s w to write to MSR register\n"
				"\t i r to read from I2C device\n"
				"\t i w to write to I2C device\n"
				"\t n w to write to North Complex unit register\n"
				"\t n r to read from North Complex unit register\n"
				"\t p r to read from Port\n"
				"\t p w to write to the Port\n");
}

static int process_i2c_args(int argc, char **argv)
{
	unsigned int bus;
	unsigned int reg;
	unsigned int size;
	unsigned int addr;
	int result;
	unsigned int value;
	int status = 0;

	if (argc < 3) {
		printf("\n***Invalid argument list: check usage -\n");
		usage();
		return -1;
	}

	if (argv[2][0] == 'r' || argv[2][0] == 'R') {
		if (argc != 7 ) {
			printf("Usage to read:<r> <size: byte=2, word=3, data=5> <bus_no> "
					"<bus_addr in Hex without 0x prefix> "
					"<Hex registerAddress without 0x prefix>\n");
			printf("Eg: to read word data from I2C bus 1 at address 0x36 and register 0x19 use\n"
					" ./peeknpoke.out i r 3 1 36 19\n\n");
		}
		else {
			hexstring_to_int(argv[6], &reg);
			hexstring_to_int(argv[5], &addr);
			hexstring_to_int(argv[4], &bus);
			hexstring_to_int(argv[3], &size);
			status = read_i2c_device(bus, addr, reg, size, &result);
		}
	}
	else if (argv[2][0] == 'w' || argv[2][0] == 'W') {
		if (argc != 8) {
			printf("Usage to Write:<w> <size: byte=2, word=3, data=5> <bus_no> "
								"<bus_addr in Hex without 0x prefix> "
								"<Hex registerAddress without 0x prefix>\n");
			printf("Eg: to write word 0x10 to I2C bus 1 at address 0x36 and register 0x19 "
								"\n./peeknpoke.out r 3 1 36 19 10\n\n");
		}
		else {
			hexstring_to_int(argv[7], &value);
			hexstring_to_int(argv[6], &reg);
			hexstring_to_int(argv[5], &addr);
			hexstring_to_int(argv[4], &bus);
			hexstring_to_int(argv[3], &size);
			status = write_i2c_device(bus, addr, reg, size, value);
		}
	}
	else {
		printf("\n***Invalid I2C operation\n");
		status = -1;
	}

	return status;
}

static int process_nc_args(int argc, char **argv)
{
	unsigned int port;
	unsigned int reg;
	int addr;
	int result;
	unsigned int value;
	int status = 0;

	if (argc < 3) {
		printf("\n***Invalid argument list: check usage -\n");
		usage();
		return -1;
	}

	if (argv[2][0] == 'r' || argv[2][0] == 'R') {
		if (argc != 5 ) {
			printf("Usage to read:<r> <Port number in Hex without 0x prefix> "
					"<Hex registerAddress without 0x prefix>\n");
			printf("PUNIT port - 4, AUNIT - 0, HUNIT - 2 ...etc");
			printf("Eg: to read word data from Port 4 at address 0xB1 use\n"
					" ./peeknpoke.out n r 4 B1\n\n");
		}
		else {
			hexstring_to_int(argv[4], &reg);
			hexstring_to_int(argv[3], &port);
			status = read_nc_port(reg, port, &result);
		}
	}
	else if (argv[2][0] == 'w' || argv[2][0] == 'W') {
		if (argc != 6 ) {
			printf("Usage to write:<w> <Port number in Hex without 0x prefix> "
					"<Hex registerAddress without 0x prefix>\n"
					"<Hex value without 0x prefix>\n");
			printf("PUNIT port - 4, AUNIT - 0, HUNIT - 2 ...etc\n");
			printf("This works only on Secure parts !!!\n");
			printf("Eg: to write word 0xff to Port 4 at address 0x70 use\n"
					" ./peeknpoke.out n w 4 70 ff\n\n");
		}
		else {
			hexstring_to_int(argv[5], &value);
			hexstring_to_int(argv[4], &reg);
			hexstring_to_int(argv[3], &port);
			status = write_nc_port(reg, port, value);
		}
	}
	else {
		printf("\n***Invalid NC operation\n");
		status = -1;
	}
	return status;
}

static int process_port_args(int argc, char **argv)
{
	unsigned int port;
	int reg;
	int addr;
	int result;
	unsigned int value;
	int status = 0;

	if (argc < 3) {
		printf("\n***Invalid argument list: check usage -\n");
		usage();
		return -1;
	}

	if (argv[2][0] == 'r' || argv[2][0] == 'R') {
		if (argc != 4 ) {
			printf("Usage to read:<r> <Port number in Hex without 0x prefix>\n");
			printf("Eg: to read word data from Port 0x1000 use\n"
					" ./peeknpoke.out p r 1000\n\n");
		}
		else {
			hexstring_to_int(argv[3], &port);
			status = read_port(port, &result);
		}
	}
	else if (argv[2][0] == 'w' || argv[2][0] == 'W') {
		if (argc != 5 ) {
			printf("Usage to write:<w> <Port number in Hex without 0x prefix> "
					"<Hex value without 0x prefix>\n");
			printf("Eg: to write word 0xff to Port 1000 use\n"
					" ./peeknpoke.out p w 1000 ff\n\n");
		}
		else {
			hexstring_to_int(argv[4], &value);
			hexstring_to_int(argv[3], &port);
			status = write_port(port, value);
		}
	}
	else {
		printf("\n***Invalid Port operation\n");
		status = -1;
	}
	return status;
}

/*
 *  ======== process_args ========
 *  Process command line arguments for this sample
 */
static int process_args(int argc, char **argv) {
	int cmd_type;
	unsigned int reg_addr;
	unsigned int reg_val;
	uint64_t msr_val;
	int status = 0;

	if (argc < 2) {
		usage();
		return -1;
	}

	cmd_type = argv[1][0];
	switch (cmd_type) {
	case 'd':
		if (argc < 4)
			printf("Usage: <r> <Hex registerAddress without 0x prefix>"
				" <numOfWordsToDump>\n");
		else {
			hexstring_to_int(argv[2], &reg_addr);
			status = addr_range_dump(reg_addr, atoi(argv[3]));
		}
		break;
	case 'r':
		if (argc < 4)
			printf("Usage: <r> <Hex registerAddress without 0x prefix>"
				" <regAccessSize =8, 16, 32>\n");
		else {
			hexstring_to_int(argv[2], &reg_addr);
			status = reg_read(reg_addr, atoi(argv[3]));
		}
		break;
	case 'w':
		if (argc < 5)
			printf("Usage: <w> <Hex registerAddress without 0x prefix>"
				" <regAccessSize =8, 16, 32> <value>\n ");
		else {
			hexstring_to_int(argv[2], &reg_addr);
			hexstring_to_int(argv[4], &reg_val);
			status = reg_write(reg_addr, atoi(argv[3]), reg_val);
		}
		break;
	case 'm':
		if (argc == 2) {
			msic_program();
		}
		else {
			if (argv[2][0] == 'r' || argv[2][0] == 'R') {
				if (argc != 4 ) {
					printf("Usage: <r> <Hex register Address without 0x prefix>\n");
				}
				else {
					hexstring_to_int(argv[3], &reg_addr);
					msic_read((unsigned short)reg_addr);
				}
			}
			else if (argv[2][0] == 'w' || argv[2][0] == 'W') {
				if (argc != 5) {
					printf("Usage: <w> <Hex register Address without 0x prefix> "
						"<Hex Value without 0x prefix>\n");
				}
				else {
					hexstring_to_int(argv[3], &reg_addr);
					hexstring_to_int(argv[4], &reg_val);
					msic_write((unsigned short)reg_addr, (unsigned short)reg_val);
				}
			}
			else {
				printf("\n***Invalid MSIC operation\n");
			}
		}
		break;
	case 's':
		if (argv[2][0] == 'r' || argv[2][0] == 'R') {
			if (argc != 5 ) {
				printf("Usage to read:<r>  <cpu_no> <Hex registerAddress without 0x prefix>\n");
			}
			else {
				hexstring_to_int(argv[4], &reg_addr);
				status = msr_reg_read(atoi(argv[3]), reg_addr, 1);
			}
		}
		else if (argv[2][0] == 'w' || argv[2][0] == 'W') {
			if (argc != 6) {
				printf("Usage to write: <w> <cpu_no> <Hex registerAddress without"
									" 0x prefix> <hex value without 0x prefix>\n");
			}
			else {
				hexstring_to_int(argv[4], &reg_addr);
				hexstring_to_long(argv[5], &msr_val);
				status = msr_reg_write(atoi(argv[3]), reg_addr, msr_val);
			}
		}
		else {
			printf("\n***Invalid MSR operation\n");
		}
		break;

	case 'i':
		status = process_i2c_args(argc, argv);
		break;

	case 'n':
		status = process_nc_args(argc, argv);
		break;

	case 'p':
		status = process_port_args(argc, argv);
		break;

	case 'z':
		printf("PeeknPoke Architect - Hari Kanigeri\n");
		printf("Authors - Hari Kanigeri, Asutosh Pathak\n");
		break;

	default:
		usage();
		break;
	}
	return (status);
}

/*
 *  ======== main ========
 */
int main(int argc, char **argv) {
	int status = 0;

	process_args(argc, argv);
	return status;
}



