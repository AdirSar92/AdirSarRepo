#include <stdint.h> /* uint64_t */
#include <stdio.h> /* printf */
#include "bitarray.h"

#define BIT_ARRAY_SIZE 65

void PrintBits(bitarray_t arr);

int main(void)
{

	int get_val = 0;
	size_t count = 0;
	uint64_t check = 0;
	char to_write_into[BIT_ARRAY_SIZE];

	/* ********** Test SetAll ********** */
	printf ("/* ********** Test SetAll ********** */\n");
	check = BitArraySetAll(check);
	PrintBits(check);

	/* ********** Test ResetAll ********** */
	printf ("/* ********** Test ResetAll ********** */\n");
	check = BitArrayResetAll(check);
	PrintBits(check);

	/* ********** Test GetVal ********** */
	printf ("/* ********** Test GetVal ********** */\n");
	printf ("/* ********** 6th bit: ********** */\n");
	check = 255;
	get_val = BitArrayGetVal(check, 5);
	PrintBits(check);
	printf ("6th bit is : %d\n", get_val);

	/* ********** Test CountOn ********** */
	printf ("/* ********** Test CountOn ********** */\n");
	count = BitArrayCountOn(check);
	PrintBits(check);
	printf ("number of on bits: %ld\n", count);

	/* ********** Test CountOff ********** */
	printf ("/* ********** Test CountOff ********** */\n");
	count = BitArrayCountOff(check);
	PrintBits(check);
	printf ("number of off bits: %ld\n", count);

	/* ********** Test SetOff ********** */
	printf ("/* ********** Test SetOff ********** */\n");
	printf ("/* ********** Set 5th bit off ********** */\n");
	check = BitArraySetOff(check, 4);
	PrintBits(check);	

	/* ********** Test SetOn ********** */
	printf ("/* ********** Test SetOn ********** */\n");
	printf ("/* ********** Set 5th bit on ********** */\n");
	check = BitArraySetOn(check, 4);
	PrintBits(check);	
	
	/* ********** Test BitArrayFlip ********** */
	printf ("/* ********** Test BitArrayFlip ********** */\n");
	printf ("/* ********** Flip 42th index ********** */\n");
	check = BitArrayFlip(check, 42);
	PrintBits(check);	

	/* ********** Test BitArraySetBit ********** */
	check = BitArrayResetAll(check);
	printf ("/* ********** Test BitArraySetBit ********** */\n");
	printf ("/* ********** Set 43th index to 1 ********** */\n");
	check = BitArraySetBit(check, 43, 1);
	PrintBits(check);	


	/* ********** Test BitArrayToString ********** */
	check = 284724682;
	printf ("/* ********** Test BitArrayToString ********** */\n");
	BitArrayToString(check, to_write_into);
	printf ("%s\n", to_write_into);
	

	printf ("/* ********** Test BitArrayMirror ********** */\n");
	PrintBits(check);
	check = BitArrayMirrorLUT(check);
	PrintBits(check);


	printf ("/* ********** Test RotateLeft 5 times ********** */\n");
	PrintBits(check);
	check = BitArrayRotL(check, 5);
	PrintBits(check);

	printf ("/* ********** Test RotateRight 5 times ********** */\n");
	PrintBits(check);
	check = BitArrayRotR(check, 5);
	PrintBits(check);
	


	return 0;
}
