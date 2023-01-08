#include <assert.h> /* assert */
#include "vsa.h" /* Function declerations */

/*=============================== FUNC MACROS ===============================*/
#define STRUCT_SIZE (ALIGN_UP_TO_WORD_SIZE(sizeof(vsa_t)))
#define WORD_SIZE (sizeof(void *))
#define ALIGN_UP_TO_WORD_SIZE(a) ((a + (WORD_SIZE - 1)) & (~(WORD_SIZE - 1)))
#define ABS_VALUE(x) (x < 0 ? x * -1 : x)
#define MAX(a,b) (a > b ? a : b)

/*================================== MACROS =================================*/
#define FULL ((long)-1)
#define END_BLOCK ((long)0)
#define DUMMIES (2)
#define VALIDATOR ((long) 0xDEADBEEF)
#define NOT_VALID ((long) 0xBAADF00D)

struct block_header
{
	long block_size;
	#ifndef NDEBUG
	long validator;
	#endif /* __NDEBUG__ */
};

typedef struct block_header block_t;

/*=============================== Static Funcs ==============================*/

__inline__ static void InitValidator (vsa_t *vsa, long validator);
static void FixDefrag(vsa_t *vsa);
static void AllocateBlockHeader(vsa_t *memory_pool, size_t requested_size);
static vsa_t *FindBlockToAllocate(vsa_t *memory_pool, size_t requested_size);

/*=========================== Function Definitions ===========================*/

vsa_t *VsaInit(void *pool_start, size_t pool_size)
{
	vsa_t *vsa_start;
	block_t *vsa_end;

	assert(0 != pool_size);
	assert(NULL != pool_start);
	
	vsa_start = (vsa_t *)pool_start;
	vsa_start->block_size = pool_size - DUMMIES * STRUCT_SIZE;
	InitValidator(vsa_start, VALIDATOR);

	vsa_end = (vsa_t *)((char *)pool_start + pool_size - STRUCT_SIZE);
	vsa_end->block_size = END_BLOCK;
	InitValidator(vsa_end, VALIDATOR);
	
	return (vsa_start);
}

void *VsaAlloc(vsa_t *memory_pool, size_t requested_size)
{
	void *to_return = NULL;
	block_t *vsa_start = memory_pool;

	assert (requested_size > 0);
	assert (NULL != memory_pool);
	
	requested_size = ALIGN_UP_TO_WORD_SIZE(requested_size);
	memory_pool = FindBlockToAllocate(memory_pool, requested_size);
	
	if (END_BLOCK == memory_pool->block_size)
	{
		FixDefrag(vsa_start);
		memory_pool = FindBlockToAllocate(vsa_start, requested_size);
	}
	
	if (END_BLOCK != memory_pool->block_size)
	{
		if (memory_pool->block_size > (long)(STRUCT_SIZE + requested_size))
		{
			AllocateBlockHeader(memory_pool, requested_size);
		}
		memory_pool->block_size *= FULL;
		to_return = (char *)memory_pool + STRUCT_SIZE;
	}
		
	return (to_return);

}

void VsaFree(void *block_ptr)
{
	block_t *to_free = NULL; 
	assert (NULL != block_ptr);
	
	to_free = (vsa_t *)((char *)block_ptr - STRUCT_SIZE);
	assert(VALIDATOR == to_free->validator);

	to_free->block_size = ABS_VALUE(to_free->block_size);
	
}

size_t VsaLargestChunkAvaliable(vsa_t *memory_pool)
{
	size_t max = 0;
	long this_block_size = 0;

	assert (NULL != memory_pool);
	
	FixDefrag(memory_pool);

	do 
	{
		this_block_size = memory_pool->block_size;
		max = MAX(this_block_size, (long)max);
		memory_pool = (vsa_t *)((char *)memory_pool + STRUCT_SIZE + ABS_VALUE(this_block_size));
	} while (END_BLOCK != this_block_size);

	return (max);
}

static void FixDefrag(vsa_t *vsa)
{
	block_t *next_vsa = vsa;
	long this_block_size = vsa->block_size;
	long next_block_size = 0;

	do
	{
		next_vsa = (vsa_t *)((char *)vsa + STRUCT_SIZE + ABS_VALUE(this_block_size));
		next_block_size = next_vsa->block_size;

		if (this_block_size > 0 && next_block_size > 0)
		{
			vsa->block_size += next_block_size + STRUCT_SIZE;
			InitValidator(next_vsa, NOT_VALID);
			this_block_size = vsa->block_size;
			continue;
		}
		vsa = next_vsa;
		this_block_size = vsa->block_size;
		
	} while (END_BLOCK != next_block_size);
}

__inline__ static void InitValidator(vsa_t *vsa, long validator)
{
	assert (NULL != vsa);
	(void)vsa;
	#ifndef NDEBUG
	vsa->validator = validator;
	#endif
	(void)validator;
}

static vsa_t *FindBlockToAllocate(vsa_t *memory_pool, size_t requested_size)
{
	long this_block_size = 0;
	this_block_size = memory_pool->block_size;

	while (END_BLOCK != this_block_size && 
			this_block_size < (long)requested_size)
	{
		memory_pool = (vsa_t *)((char *)memory_pool + STRUCT_SIZE + 
								 ABS_VALUE(this_block_size));
		this_block_size = memory_pool->block_size;
	}

	return (memory_pool);
}

static void AllocateBlockHeader(vsa_t *memory_pool, size_t requested_size)
{
	vsa_t *new_vsa;

	new_vsa = (vsa_t *)((char *)memory_pool + STRUCT_SIZE + requested_size);
	new_vsa->block_size = memory_pool->block_size - 
							requested_size - STRUCT_SIZE;
	InitValidator(new_vsa, VALIDATOR);
	memory_pool->block_size = requested_size;
}