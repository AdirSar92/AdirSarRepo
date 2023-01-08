#include <assert.h> /* assert */
#include "fsa.h" /* Function decleration */

#define WORD_SIZE (sizeof(void *))
#define END_BLOCK ((size_t)0)
#define ALIGN_UP_TO_WORD_SIZE(a) ((a + (WORD_SIZE - 1)) & (~(WORD_SIZE - 1)))

struct fsa
{
	size_t next_avaliable;
	size_t block_size;
};

fsa_t *FsaInit(void *pool_start, size_t pool_size, size_t block_size)
{
    fsa_t *fsa = NULL;
    char *init_runner = NULL, *end = NULL;

    assert(NULL != pool_start);
    assert(pool_size > block_size);

    block_size = ALIGN_UP_TO_WORD_SIZE(block_size); 

    fsa = (fsa_t *)pool_start;
    fsa->next_avaliable = ALIGN_UP_TO_WORD_SIZE(sizeof(fsa_t));
    fsa->block_size = block_size;

    init_runner = (char *)pool_start + fsa->next_avaliable;
    end = (char *)pool_start + pool_size;

    while (end > init_runner + block_size)
    {
        *(size_t *)init_runner = init_runner - (char *)pool_start + block_size;
        init_runner += block_size;
    }
    
    *(size_t *)init_runner = END_BLOCK;
    
    return (fsa);
}

void *FsaAlloc(fsa_t *fsa)
{
	void *ret_ptr = NULL;
	
	assert(NULL != fsa);
	
	if(END_BLOCK != fsa->next_avaliable)
	{
		ret_ptr = ((char *)fsa + fsa->next_avaliable);
		fsa->next_avaliable = *((size_t *)ret_ptr);
	}
	
	return(ret_ptr);
}

void FsaFree(fsa_t *fsa, void *block_ptr)
{
	size_t offset_from_start = (size_t)block_ptr - (size_t)fsa;
	
	assert(NULL != fsa);
	assert(NULL != block_ptr);
	
	*(size_t *)block_ptr = fsa->next_avaliable;
	fsa->next_avaliable = offset_from_start;
}

size_t FsaCountFree(const fsa_t *fsa)
{
	size_t counter = 0, offset_runner = 0;
	char *fsa_byte_address = NULL;

	assert(NULL != fsa);
	fsa_byte_address = (char *)fsa;

	offset_runner = fsa->next_avaliable;
	
	while(END_BLOCK != offset_runner)
	{
		++counter;
		offset_runner = *(size_t *)(fsa_byte_address + offset_runner);
	}
	return (counter);
}

size_t FsaSuggestSize(size_t num_of_blocks, size_t block_size)
{
	assert (0 != block_size);
	
	block_size = ALIGN_UP_TO_WORD_SIZE(block_size);
  	
  	return ((block_size * num_of_blocks) + 
  			 ALIGN_UP_TO_WORD_SIZE(sizeof(fsa_t)));
}


