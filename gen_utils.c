/*
 * gen utils file
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

/*
 *  This function converst a hex string to integer
 */
int hexstring_to_int(const char* hexString, unsigned int* result)
{
	int i;
	int temp;
	int factor;
	int length;
	int status = 0;

	length = strlen(hexString);
	if ((length == 0) || (length > 8))
		return -1;
	*result = 0;
	factor = 1;
	/* Convert the characters to hex number */
	for (i = length - 1; i >= 0; i--) {
		if (isxdigit(*(hexString + i))) {
			if (*(hexString + i) >= 97) {
				temp = (*(hexString + i) - 97) + 10;
			} else if (*(hexString + i) >= 65) {
				temp = (*(hexString + i) - 65) + 10;
			} else {
				temp = *(hexString + i) - 48;
			}
			*result += (temp * factor);
			factor *= 16;
		} else {
			/* Conversion was abnormally terminated by
			 non hexadecimal digit */
			return -1;
		}
	}
	return status;
}

/*
 *  This function converst a hex string to integer
 */
int hexstring_to_long(const char* hexString, uint64_t* result)
{
	int i;
	int temp;
	uint64_t factor;
	int length;
	int status = 0;

	length = strlen(hexString);
	if ((length == 0) || (length > 16))
		return -1;
	*result = 0;
	factor = 1;
	/* Convert the characters to hex number */
	for (i = length - 1; i >= 0; i--) {
		if (isxdigit(*(hexString + i))) {
			if (*(hexString + i) >= 97) {
				temp = (*(hexString + i) - 97) + 10;
			} else if (*(hexString + i) >= 65) {
				temp = (*(hexString + i) - 65) + 10;
			} else {
				temp = *(hexString + i) - 48;
			}
			*result += ((uint64_t)temp * factor);
			factor *= 16ULL;
		} else {
			/* Conversion was abnormally terminated by
			 non hexadecimal digit */
			return -1;
		}
	}
	return status;
}


