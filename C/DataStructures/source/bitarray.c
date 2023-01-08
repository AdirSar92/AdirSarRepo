#include <assert.h> /* assert */
#include <limits.h> /* CHAR_BIT */
#include <stdio.h>	/* printf */

#include "bitarray.h" /* Functions declerations */

#define ARRAY_SIZE (CHAR_BIT * sizeof(size_t))
#define ASCII_NUMBER_TO_CHAR 48
#define OFFSET 1

/* LUT for count set bits */
#define B2(n) n, n + 1, n + 1, n + 2
#define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)
static const unsigned char bit_lut[256] = {B6(0), B6(1), B6(1), B6(2)};

/* LUT for byte mirror */
#define R2(n) n, n + 2 * ARRAY_SIZE, n + 1 * ARRAY_SIZE, n + 3 * ARRAY_SIZE
#define R4(n) R2(n), R2(n + 2 * 16), R2(n + 1 * 16), R2(n + 3 * 16)
#define R6(n) R4(n), R4(n + 2 * 4), R4(n + 1 * 4), R4(n + 3 * 4)
#define REVERSE_BITS R6(0), R6(2), R6(1), R6(3)
static const unsigned char reversed_number_LUT[256] = {REVERSE_BITS};

/* Checked by Omer */
bitarray_t BitArraySetAll(bitarray_t arr)
{
	(void)arr;
	return ~((bitarray_t)0);
}

bitarray_t BitArraySetOn(bitarray_t arr, size_t index)
{
	assert(index <= (ARRAY_SIZE - 1));
	return (((bitarray_t)1 << index) | arr);
}

int BitArrayGetVal(bitarray_t arr, size_t index)
{
	assert(ARRAY_SIZE >= index);
	return ((arr >> (index)) & (bitarray_t)1);
}

size_t BitArrayCountOn(bitarray_t arr)
{
	size_t count = 0;

	count = bit_lut[arr & 0xFF];
	count += bit_lut[(arr >> 8) & 0xFF];
	count += bit_lut[(arr >> 16) & 0xFF];
	count += bit_lut[(arr >> 24) & 0xFF];
	count += bit_lut[(arr >> 32) & 0xFF];
	count += bit_lut[(arr >> 40) & 0xFF];
	count += bit_lut[(arr >> 48) & 0xFF];
	count += bit_lut[(arr >> 56) & 0xFF];

	return (count);
}

bitarray_t BitArrayResetAll(bitarray_t arr)
{
	return (arr & (bitarray_t)0);
}

bitarray_t BitArraySetOff(bitarray_t arr, size_t index)
{
	assert(index <= (ARRAY_SIZE - 1));

	return ((arr & ~((bitarray_t)1 << index)));
}

bitarray_t BitArrayFlip(bitarray_t arr, size_t index)
{
	assert(index <= (ARRAY_SIZE - 1));
	return (((bitarray_t)1 << index) ^ arr);
}

bitarray_t BitArrayRotL(bitarray_t arr, size_t num_of_rotations)
{
	num_of_rotations %= ARRAY_SIZE;
	return ((arr << num_of_rotations) |
			(arr >> (ARRAY_SIZE - num_of_rotations)));
}

bitarray_t BitArrayRotR(bitarray_t arr, size_t num_of_rotations)
{
	num_of_rotations %= ARRAY_SIZE;
	return ((arr >> num_of_rotations) |
			(arr << (ARRAY_SIZE - num_of_rotations)));
}

size_t BitArrayCountOff(bitarray_t arr)
{
	size_t count = 0;
	bitarray_t flipped_arr = ~(arr);

	count = BitArrayCountOn(flipped_arr);

	return (count);
}

char *BitArrayToString(bitarray_t arr, char *to_write_into)
{
	size_t count = ARRAY_SIZE;
	bitarray_t one = 1;

	assert(to_write_into);

	while (count > (bitarray_t)0)
	{
		to_write_into[count - OFFSET] = (arr & one) + ASCII_NUMBER_TO_CHAR;
		arr >>= one;
		--count;
	}

	to_write_into[ARRAY_SIZE] = '\0';

	return (to_write_into);
}

bitarray_t BitArraySetBit(bitarray_t arr, size_t index, int value)
{

	assert(0 == value || 1 == value);
	assert(index <= ARRAY_SIZE - 1);

	if (1 == value)
	{
		return (BitArraySetOn(arr, index));
	}

	return (BitArraySetOff(arr, index));
}

bitarray_t BitArrayMirror(bitarray_t arr)
{
	arr = ((arr) >> 32) |
		  ((arr) << 32);

	arr = ((arr & 0xFFFF0000FFFF0000) >> 16) |
		  ((arr & 0x0000FFFF0000FFFF) << 16);

	arr = ((arr & 0xFF00FF00FF00FF00) >> 8) |
		  ((arr & 0x00FF00FF00FF00FF) << 8);

	arr = ((arr & 0xF0F0F0F0F0F0F0F0) >> 4) |
		  ((arr & 0x0F0F0F0F0F0F0F0F) << 4);

	arr = ((arr & 0xCCCCCCCCCCCCCCCC) >> 2) |
		  ((arr & 0x3333333333333333) << 2);

	arr = ((arr & 0xAAAAAAAAAAAAAAAA) >> 1) |
		  ((arr & 0x5555555555555555) << 1);

	return (arr);
}

bitarray_t BitArrayMirrorLUT(bitarray_t arr)
{
	bitarray_t reverse_number = 0;
	reverse_number =
		(bitarray_t)reversed_number_LUT[arr & 0xff] << 56 |
		(bitarray_t)reversed_number_LUT[(arr >> 8) & 0xff] << 48 |
		(bitarray_t)reversed_number_LUT[(arr >> 16) & 0xff] << 40 |
		(bitarray_t)reversed_number_LUT[(arr >> 24) & 0xff] << 32 |
		(bitarray_t)reversed_number_LUT[(arr >> 32) & 0xff] << 24 |
		(bitarray_t)reversed_number_LUT[(arr >> 40) & 0xff] << 16 |
		(bitarray_t)reversed_number_LUT[(arr >> 48) & 0xff] << 8 |
		(bitarray_t)reversed_number_LUT[(arr >> 56) & 0xff];

	return (reverse_number);
}

void PrintBits(bitarray_t arr)
{
	bitarray_t index = (bitarray_t)1 << (ARRAY_SIZE - 1);

	while (0 < index)
	{
		if (arr & index)
		{
			printf("1");
		}
		else
		{
			printf("0");
		}

		index >>= 1;
	}

	printf("\n");
}
