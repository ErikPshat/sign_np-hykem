// Copyright (C) 2013       tpu
// Copyright (C) 2015       Hykem <hykem@hotmail.com>
// Licensed under the terms of the GNU GPL, version 3
// http://www.gnu.org/licenses/gpl-3.0.txt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libkirk/kirk_engine.h"
#include "libkirk/psp_headers.h"
#include "utils.h"

static u8 test_k140[16] = {
	0x35, 0xfe, 0x4c, 0x96, 0x00, 0xb2, 0xf6, 0x7e, 0xf5, 0x83, 0xa6, 0x79, 0x1f, 0xa0, 0xe8, 0x86,
};
static u8 test_kirk1[32] = {
	0xca, 0x03, 0x84, 0xb1, 0xd9, 0x63, 0x47, 0x92, 0xce, 0xc7, 0x01, 0x23, 0x43, 0x72, 0x68, 0xac,
	0x77, 0xea, 0xec, 0xba, 0x6d, 0xaa, 0x97, 0xdf, 0xfe, 0x91, 0xb9, 0x39, 0x70, 0x99, 0x8b, 0x3a,
};

typedef struct {
	u32 tag;
	u8  key[16];
	u32 code;
	u32 type;
} TAG_KEY;

static TAG_KEY key_list[] = {
	{0x8004FD03, {0xF4, 0xAE, 0xF4, 0xE1, 0x86, 0xDD, 0xD2, 0x9C, 0x7C, 0xC5, 0x42, 0xA6, 0x95, 0xA0, 0x83, 0x88}, 0x5D, 2},
	{0xD91605F0, {0xB8, 0x8C, 0x45, 0x8B, 0xB6, 0xE7, 0x6E, 0xB8, 0x51, 0x59, 0xA6, 0x53, 0x7C, 0x5E, 0x86, 0x31}, 0x5D, 2},
	{0xD91606F0, {0xED, 0x10, 0xE0, 0x36, 0xC4, 0xFE, 0x83, 0xF3, 0x75, 0x70, 0x5E, 0xF6, 0xA4, 0x40, 0x05, 0xF7}, 0x5D, 2},
	{0xD91608F0, {0x5C, 0x77, 0x0C, 0xBB, 0xB4, 0xC2, 0x4F, 0xA2, 0x7E, 0x3B, 0x4E, 0xB4, 0xB4, 0xC8, 0x70, 0xAF}, 0x5D, 2},
	{0xD91609F0, {0xD0, 0x36, 0x12, 0x75, 0x80, 0x56, 0x20, 0x43, 0xC4, 0x30, 0x94, 0x3E, 0x1C, 0x75, 0xD1, 0xBF}, 0x5D, 2},
	{0xD9160AF0, {0x10, 0xA9, 0xAC, 0x16, 0xAE, 0x19, 0xC0, 0x7E, 0x3B, 0x60, 0x77, 0x86, 0x01, 0x6F, 0xF2, 0x63}, 0x5D, 2},
	{0xD9160BF0, {0x83, 0x83, 0xF1, 0x37, 0x53, 0xD0, 0xBE, 0xFC, 0x8D, 0xA7, 0x32, 0x52, 0x46, 0x0A, 0xC2, 0xC2}, 0x5D, 2},
	{0xD91610F0, {0x89, 0x07, 0x73, 0xB4, 0x09, 0x08, 0x3F, 0x54, 0x31, 0x87, 0x00, 0xF3, 0x35, 0x14, 0x55, 0xCC}, 0x5D, 2},
	{0xD91611F0, {0x61, 0xB0, 0xC0, 0x58, 0x71, 0x57, 0xD9, 0xFA, 0x74, 0x67, 0x0E, 0x5C, 0x7E, 0x6E, 0x95, 0xB9}, 0x5D, 2},
	{0xD91612F0, {0x9E, 0x20, 0xE1, 0xCD, 0xD7, 0x88, 0xDE, 0xC0, 0x31, 0x9B, 0x10, 0xAF, 0xC5, 0xB8, 0x73, 0x23}, 0x5D, 2},
	{0xD91613F0, {0xEB, 0xFF, 0x40, 0xD8, 0xB4, 0x1A, 0xE1, 0x66, 0x91, 0x3B, 0x8F, 0x64, 0xB6, 0xFC, 0xB7, 0x12}, 0x5D, 2},
	{0xD91614F0, {0xFD, 0xF7, 0xB7, 0x3C, 0x9F, 0xD1, 0x33, 0x95, 0x11, 0xB8, 0xB5, 0xBB, 0x54, 0x23, 0x73, 0x85}, 0x5D, 2},
	{0xD91615F0, {0xC8, 0x03, 0xE3, 0x44, 0x50, 0xF1, 0xE7, 0x2A, 0x6A, 0x0D, 0xC3, 0x61, 0xB6, 0x8E, 0x5F, 0x51}, 0x5D, 2},
	{0xD91616F0, {0x53, 0x03, 0xB8, 0x6A, 0x10, 0x19, 0x98, 0x49, 0x1C, 0xAF, 0x30, 0xE4, 0x25, 0x1B, 0x6B, 0x28}, 0x5D, 2},
	{0xD91617F0, {0x02, 0xFA, 0x48, 0x73, 0x75, 0xAF, 0xAE, 0x0A, 0x67, 0x89, 0x2B, 0x95, 0x4B, 0x09, 0x87, 0xA3}, 0x5D, 2},
	{0xD91618F0, {0x96, 0x96, 0x7C, 0xC3, 0xF7, 0x12, 0xDA, 0x62, 0x1B, 0xF6, 0x9A, 0x9A, 0x44, 0x44, 0xBC, 0x48}, 0x5D, 2},
	{0xD91619F0, {0xE0, 0x32, 0xA7, 0x08, 0x6B, 0x2B, 0x29, 0x2C, 0xD1, 0x4D, 0x5B, 0xEE, 0xA8, 0xC8, 0xB4, 0xE9}, 0x5D, 2},
	{0xD9161AF0, {0x27, 0xE5, 0xA7, 0x49, 0x52, 0xE1, 0x94, 0x67, 0x35, 0x66, 0x91, 0x0C, 0xE8, 0x9A, 0x25, 0x24}, 0x5D, 2},
	{0xD9161EF0, {0x5B, 0x4A, 0xD2, 0xF6, 0x49, 0xD4, 0xEB, 0x0D, 0xC0, 0x0F, 0xCB, 0xA8, 0x15, 0x2F, 0x55, 0x08}, 0x5D, 2},
	{0xD91620F0, {0x52, 0x1C, 0xB4, 0x5F, 0x40, 0x3B, 0x9A, 0xDD, 0xAC, 0xFC, 0xEA, 0x92, 0xFD, 0xDD, 0xF5, 0x90}, 0x5D, 2},
	{0xD91621F0, {0xD1, 0x91, 0x2E, 0xA6, 0x21, 0x14, 0x29, 0x62, 0xF6, 0xED, 0xAE, 0xCB, 0xDD, 0xA3, 0xBA, 0xFE}, 0x5D, 2},
	{0xD91622F0, {0x59, 0x5D, 0x78, 0x4D, 0x21, 0xB2, 0x01, 0x17, 0x6C, 0x9A, 0xB5, 0x1B, 0xDA, 0xB7, 0xF9, 0xE6}, 0x5D, 2},
	{0xD91623F0, {0xAA, 0x45, 0xEB, 0x4F, 0x62, 0xFB, 0xD1, 0x0D, 0x71, 0xD5, 0x62, 0xD2, 0xF5, 0xBF, 0xA5, 0x2F}, 0x5D, 2},
	{0xD91624F0, {0x61, 0xB7, 0x26, 0xAF, 0x8B, 0xF1, 0x41, 0x58, 0x83, 0x6A, 0xC4, 0x92, 0x12, 0xCB, 0xB1, 0xE9}, 0x5D, 2},
	{0xD91628F0, {0x49, 0xA4, 0xFC, 0x66, 0xDC, 0xE7, 0x62, 0x21, 0xDB, 0x18, 0xA7, 0x50, 0xD6, 0xA8, 0xC1, 0xB6}, 0x5D, 2},
	{0xD91680F0, {0x2C, 0x22, 0x9B, 0x12, 0x36, 0x74, 0x11, 0x67, 0x49, 0xD1, 0xD1, 0x88, 0x92, 0xF6, 0xA1, 0xD8}, 0x5D, 6},
	{0xD91681F0, {0x52, 0xB6, 0x36, 0x6C, 0x8C, 0x46, 0x7F, 0x7A, 0xCC, 0x11, 0x62, 0x99, 0xC1, 0x99, 0xBE, 0x98}, 0x5D, 6},
	{0xD91690F0, {0x42, 0x61, 0xE2, 0x57, 0x94, 0x49, 0x42, 0xB5, 0xAA, 0x6D, 0x0D, 0x08, 0x3D, 0x24, 0xF7, 0x4B}, 0x5D, 2},
};

int sign_eboot(u8 *eboot, int eboot_size, int tag, u8 *seboot);