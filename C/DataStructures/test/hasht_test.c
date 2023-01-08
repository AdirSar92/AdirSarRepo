#include <stdio.h>	 /* printf */
#include <stdlib.h>	 /* malloc, free */
#include <string.h>	 /* strcmp */
#include <strings.h> /* strcasecmp */

#include "hasht.h"

#define EXPECTED_CAPACITY (10)
#define NUM_OF_WORDS (100)
#define MAX_WORD_LEN (30)
#define ABC (26)

/*============================= Dictionary Funcs ============================*/
typedef enum status
{
	SUCCESS = 0,
	MEM_ERR = 1,
	FILE_ERR = 2,
	WORD_FOUND = 3,
	WORD_NOT_FOUND = 4
} status_t;

static status_t IsStringInDict(void);
static char **CreateDictionary(char **words);
static status_t FillDictionary(char **words);
static void FillHashTable(hasht_t *hash_table, char **words);
static status_t CheckWord(hasht_t *hash_table);
static void DestroyDictionary(char **words);
static size_t HashDictionaryWords(const void *key);
static int StringCompare(const void *key1, const void *key2);

/*=========================== Student Test Funcs ============================*/
static size_t HashByCourseID(const void *data);
static int CompareByGrade(const void *data1, const void *data2);
static int FactorAllStudents(void *data, void *param);

typedef struct student
{
	long course_id;
	char first_name[30];
	char last_name[30];
	int grade;
} student_t;

/*================================ Test Funcs ================================*/
static void TestAllFuncs();
static void TestCreate();
static void TestDestroy();
static void TestInsert();
static void TestRemove();
static void TestSizeEmpty();
static void TestFind();
static void TestForEach();
static void TestLoadFactor();
static void TestSpellChecker();
static void TestHashtStandartDeviation();

/*=================================== MAIN  ==================================*/
int main(void)
{
	TestAllFuncs();
	return (0);
}

static void TestAllFuncs()
{
	printf("     ~START OF TEST FUNCTION~ \n");
	TestCreate();
	TestInsert();
	TestRemove();
	TestSizeEmpty();
	TestFind();
	TestForEach();
	TestSpellChecker();
	TestLoadFactor();
	TestHashtStandartDeviation();
	TestDestroy();
	printf("      ~END OF TEST FUNCTION~ \n");
}

static void TestCreate()
{
	hasht_t *table = HashtCreate(EXPECTED_CAPACITY, CompareByGrade, HashByCourseID);
	printf("\033[1;31m");
	if (NULL != table)
	{
		printf("HashtCreate working!                                 V\n");
	}
	else
	{
		printf("HashtCreate NOT working!                             X\n");
	}

	HashtDestroy(table);
}

static void TestDestroy()
{
	printf("\033[1;37m");
	printf("*Run vlg to test HashtDestroy*\n");
}

static void TestInsert()
{
	student_t student1 = {6066, "Asaf", "Madari", 95};
	student_t student2 = {1203, "Sdadw", "Asjkd", 33};
	student_t student3 = {3389, "Adwz", "Ssmzsm", 22};

	int size_before = -1;
	int size_after = -1;

	hasht_t *table = HashtCreate(EXPECTED_CAPACITY, CompareByGrade, HashByCourseID);

	size_before = HashtSize(table);

	HashtInsert(table, &student1);
	HashtInsert(table, &student2);
	HashtInsert(table, &student3);

	size_after = HashtSize(table);
	printf("\033[1;32m");
	if (0 == size_before && 3 == size_after)
	{
		printf("HashtInsert working!                                 V\n");
	}
	else
	{
		printf("HashtInsert NOT working!                             X\n");
	}

	HashtDestroy(table);
}

static void TestRemove()
{
	student_t student1 = {6066, "Asaf", "Madari", 95};
	student_t student2 = {1203, "Sdadw", "Asjkd", 33};
	student_t student3 = {3389, "Adwz", "Ssmzsm", 22};
	student_t student4 = {3389, "Dszs", "Saaaaz", 75};
	student_t student5 = {3389, "Aswzs", "AAzaaz", 76};

	student_t grade_75 = {3389, " ", " ", 75};

	int size_before = -1;
	int size_after = -1;

	hasht_t *table = HashtCreate(EXPECTED_CAPACITY, CompareByGrade, HashByCourseID);

	HashtInsert(table, &student1);
	HashtInsert(table, &student2);
	HashtInsert(table, &student3);
	HashtInsert(table, &student4);
	HashtInsert(table, &student5);

	size_before = HashtSize(table);

	HashtRemove(table, &grade_75.course_id);

	size_after = HashtSize(table);
	printf("\033[1;33m");

	if (5 == size_before && 4 == size_after)
	{
		printf("HashtRemove working!                                 V\n");
	}
	else
	{
		printf("HashtRemove NOT working!                             X\n");
	}

	HashtDestroy(table);
}

static void TestSizeEmpty()
{
	student_t student1 = {6066, "Asaf", "Madari", 95};
	student_t student2 = {1203, "Sdadw", "Asjkd", 33};
	student_t student3 = {3389, "Adwz", "Ssmzsm", 22};
	student_t student4 = {3389, "Dszs", "Saaz", 75};

	size_t size_before = 0, size_after = 0;
	int empty = -1, not_empty = -1;

	hasht_t *table = HashtCreate(EXPECTED_CAPACITY, CompareByGrade, HashByCourseID);

	size_before = HashtSize(table);
	empty = HashtIsEmpty(table);

	HashtInsert(table, &student1);
	HashtInsert(table, &student2);
	HashtInsert(table, &student3);
	HashtInsert(table, &student4);

	size_after = HashtSize(table);
	not_empty = HashtIsEmpty(table);
	printf("\033[1;34m");
	if (0 == size_before && 4 == size_after)
	{
		printf("HashtSize working!                                   V\n");
	}
	else
	{
		printf("HashtSize NOT working!                               X\n");
	}
	printf("\033[1;35m");
	if (1 == empty && 0 == not_empty)
	{
		printf("HashtIsEmpty working!                                V\n");
	}
	else
	{
		printf("HashtIsEmpty NOT working!                            X\n");
	}

	HashtDestroy(table);
}

static void TestFind()
{
	student_t student1 = {6066, "Asaf", "Madari", 95};
	student_t student2 = {1203, "Sdadw", "Asjkd", 33};
	student_t student3 = {3389, "Adwz", "Ssmzsm", 22};
	student_t student4 = {3389, "Dszs", "Saaz", 75};

	student_t to_find = {6066, " ", " ", 95};
	student_t not_there = {3389, " ", " ", 28};

	student_t *found;
	student_t *not_found;

	hasht_t *table = HashtCreate(EXPECTED_CAPACITY, CompareByGrade, HashByCourseID);

	HashtInsert(table, &student1);
	HashtInsert(table, &student2);
	HashtInsert(table, &student3);
	HashtInsert(table, &student4);

	found = (student_t *)HashtFind(table, &to_find);
	not_found = (student_t *)HashtFind(table, &not_there);

	printf("\033[1;36m");
	if (NULL == not_found && 95 == found->grade)
	{
		printf("HashtFind working!                                   V\n");
	}
	else
	{
		printf("HashtFind NOT working!                               X\n");
	}

	HashtDestroy(table);
}

static void TestForEach()
{
	student_t student1 = {6066, "Asaf", "Madari", 95};
	student_t student2 = {1203, "Sdadw", "Asjkd", 33};
	student_t student3 = {3389, "Adwz", "Ssmzsm", 22};
	student_t student4 = {3389, "Dszs", "Saaz", 75};

	student_t to_find = {3389, " ", " ", 27};

	student_t *found;
	student_t *not_found;

	int factor_to_give = 5;

	hasht_t *table = HashtCreate(EXPECTED_CAPACITY, CompareByGrade, HashByCourseID);

	HashtInsert(table, &student1);
	HashtInsert(table, &student2);
	HashtInsert(table, &student3);
	HashtInsert(table, &student4);

	not_found = (student_t *)HashtFind(table, &to_find);

	HashtForEach(table, FactorAllStudents, &factor_to_give);

	found = (student_t *)HashtFind(table, &to_find);
	printf("\033[1;32m");
	if (27 == found->grade && NULL == not_found)
	{
		printf("HashtForEach working!                                V\n");
	}
	else
	{
		printf("HashtForEach NOT working!                            X\n");
	}

	HashtDestroy(table);
}

static void TestLoadFactor()
{
	double load_before = 0, load_after = 0;

	student_t student1 = {6066, "Asaf", "Madari", 95};
	student_t student2 = {1203, "Sdadw", "Asjkd", 33};
	student_t student3 = {3389, "Adwz", "Ssmzsm", 22};
	student_t student4 = {3389, "Dszs", "Saaz", 75};

	hasht_t *table = HashtCreate(EXPECTED_CAPACITY, CompareByGrade, HashByCourseID);
	load_before = HashtLoad(table);

	HashtInsert(table, &student1);
	HashtInsert(table, &student2);
	HashtInsert(table, &student3);
	HashtInsert(table, &student4);

	load_after = HashtLoad(table);

	printf("\033[1;31m");

	if (0 == load_before && 0.4 == load_after)
	{
		printf("HashtLoad working!                                   V\n");
	}
	else
	{
		printf("HashtLoad NOT working!                               X\n");
	}

	HashtDestroy(table);
}

static void TestHashtStandartDeviation()
{
	size_t i = 0;
	student_t students[10], students2[10];
	hasht_t *table, *table2;

	table = HashtCreate(10, CompareByGrade, HashByCourseID);
	table2 = HashtCreate(10, CompareByGrade, HashByCourseID);

	for (; i < 10; ++i)
	{
		students[i].course_id = i;
		students[i].grade = i * 15;
		HashtInsert(table, &students[i]);
	}

	for (i = 0; i < 5; ++i)
	{
		students2[i].course_id = i;
		students2[i].grade = i * 15;
		HashtInsert(table2, &students2[i]);
	}

	printf("\033[1;34m");

	if (0 == HashtStandartDeviation(table) &&
		0.5 == HashtStandartDeviation(table2))
	{
		printf("HashtStandartDeviation working!                      V\n");
	}
	else
	{
		printf("HashtStandartDeviation NOT working!                  X\n");
	}

	HashtDestroy(table);
	HashtDestroy(table2);
}

static void TestSpellChecker()
{
	printf("\033[1;35m");
	if (WORD_FOUND == IsStringInDict())
	{
		printf("SpellChecker working!                                V\n");
	}
	else
	{
		printf("SpellChecker Not Working!                            X\n");
	}
}

static size_t HashByCourseID(const void *data)
{
	return (((student_t *)data)->course_id % EXPECTED_CAPACITY);
}
static int CompareByGrade(const void *data1, const void *data2)
{
	return (((student_t *)data1)->grade == ((student_t *)data2)->grade);
}
static int FactorAllStudents(void *data, void *param)
{
	((student_t *)data)->grade += *(int *)param;
	return (SUCCESS);
}

static status_t IsStringInDict(void)
{
	status_t status = SUCCESS;
	char **words = NULL;

	hasht_t *hash_table = HashtCreate(ABC, StringCompare, HashDictionaryWords);
	if (NULL == hash_table)
	{
		return (MEM_ERR);
	}
	words = CreateDictionary(words);
	if (NULL == words)
	{
		HashtDestroy(hash_table);
		return (MEM_ERR);
	}
	if (SUCCESS != (status = FillDictionary(words)))
	{
		DestroyDictionary(words);
		HashtDestroy(hash_table);
		return (status);
	}

	FillHashTable(hash_table, words);
	status = CheckWord(hash_table);
	DestroyDictionary(words);
	HashtDestroy(hash_table);

	return (status);
}

static char **CreateDictionary(char **words)
{
	long i = 0;
	words = (char **)malloc(sizeof(char *) * NUM_OF_WORDS);
	if (NULL == words)
	{
		return (NULL);
	}

	for (; i < NUM_OF_WORDS; ++i)
	{
		words[i] = (char *)malloc(MAX_WORD_LEN);
		if (NULL == words[i])
		{
			break;
		}
	}
	if (NUM_OF_WORDS != i)
	{
		while (i >= 0)
		{
			free(words[i]);
			--i;
		}
		free(words);
		return (NULL);
	}

	return (words);
}

static status_t FillDictionary(char **words)
{
	FILE *fp = NULL;
	size_t i = 0;

	fp = fopen("american-english", "r");
	if (NULL == fp)
	{
		return (FILE_ERR);
	}
	for (; i < NUM_OF_WORDS; ++i)
	{
		fgets(words[i], MAX_WORD_LEN, fp);
	}
	if (0 != fclose(fp))
	{
		return (FILE_ERR);
	}

	return (SUCCESS);
}

static void FillHashTable(hasht_t *hash_table, char **words)
{
	size_t i = 0;
	for (; i < NUM_OF_WORDS; ++i)
	{
		HashtInsert(hash_table, words[i]);
	}
}

static status_t CheckWord(hasht_t *hash_table)
{
	char *check = "AOL\n";
	status_t is_word_exist = WORD_NOT_FOUND;

	if (NULL != HashtFind(hash_table, check))
	{
		is_word_exist = WORD_FOUND;
	}

	return (is_word_exist);
}

static void DestroyDictionary(char **words)
{
	size_t i = 0;
	for (; i < NUM_OF_WORDS; ++i)
	{
		free(words[i]);
	}
	free(words);
}

static size_t HashDictionaryWords(const void *key)
{
	return (*(char *)key % ABC);
}

static int StringCompare(const void *key1, const void *key2)
{
	return (0 == strcasecmp((char *)key1, (char *)key2));
}