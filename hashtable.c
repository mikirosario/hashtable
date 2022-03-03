#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100000 //max number of storable elements without collision

typedef struct	entry_s
{
	char			*key;
	char 			*value;
	struct entry_s	*next;
}				entry_t;

typedef struct	hashtable_s
{
	entry_t	*	entries[TABLE_SIZE];
}				hashtable_t;

/*!
** @brief	Hashes key to return a value between 0 and TABLE_SIZE - 1.
**
** @param	key	The key to hash.
** @return	The hashed position.
*/
size_t	hash(const char * key)
{
	size_t			value = 0;
	unsigned int	key_len = strlen(key);

	for (size_t i = 0; i < key_len; ++i)
		value = value * 37 + key[i];
	value = value % TABLE_SIZE;
	return value;
}

/*!
** @brief	Initializes a hashtable by setting all its entry pointers to NULL.
**
** @param	hashtable	A pointer to the hashtable to initialize.
*/
void		ht_init(hashtable_t * hashtable)
{
	//initialize hashtable entry pointer array with NULL pointers
	for (size_t i = 0; i < TABLE_SIZE; ++i)
		hashtable->entries[i] = NULL;
}

/*!
** @brief	Allocates memory to create a node or 'entry' in a hashtable and save
**			the associated key-value data.
**
** @details	Each entry node contains a key-value pair and a pointer to next. If
**			different keys hash to the same slot ('collide'), they are saved in
**			a NULL-terminated linked list of entry nodes accessible via
**			entry->next.
**
** @param	key		The key to save in the entry.
** @param	value	The value associated with the key.
** @return	A pointer to the entry, or NULL if any allocation failed.
*/
entry_t *	ht_create_entry(const char *key, const char *value)
{
	//allocate memory for entry
	entry_t *	entry = malloc(sizeof(entry_t));

	entry->key = malloc(strlen(key) + 1);
	entry->value = malloc(strlen(value) + 1);
	if (entry != NULL && entry->key != NULL && entry->value != NULL)
	{
		strcpy(entry->key, key);
		strcpy(entry->value, value);
		entry->next = NULL;
	}
	else
	{
		if (entry != NULL)
		{
			if (entry->key != NULL)
				free(entry->key);
			if (entry->value != NULL)
				free(entry->value);
			free(entry);
			entry = NULL;
		}
	}
	return entry;
}

/*!
** @brief	Creates a key-value entry in the hashtable, or updates the value of
**			an already-existing key. If memory allocation fails, the hashtable
**			remains unmodified.
**
** @param	hashtable	The hashtable in which to store the key-value pair.
** @param	key			The key to store in the hashtable.
** @param	value		The value associated with @a key.
** @return	A pointer to the key-value entry in the hashtable, or NULL if memory
**			allocation failed.
*/
entry_t *		ht_set(hashtable_t * hashtable, const char * key, const char *value)
{
	size_t		slot = hash(key);
	entry_t *	entry = hashtable->entries[slot];

	if (entry == NULL)	//slot is free; save key-value here
	{
		hashtable->entries[slot] = ht_create_entry(key, value);
		return hashtable->entries[slot];
	}
	else				//slot is occupied; either key already exists, or key hashes to same slot as other key (collision)
	{
		entry_t *	prev;
		do
		{
			if (strcmp(entry->key, key) == 0) //key already exists, replace value
			{
				char * tmp = malloc(strlen(value) + 1);
				if (tmp != NULL)
				{
					free(entry->value);
					entry->value = tmp;
					strcpy(entry->value, value);
					return entry;
				}
				else
					return NULL;
		 	}
			prev = entry;
			entry = entry->next;
		} while (entry != NULL);
		prev->next = ht_create_entry(key, value);
		return prev->next;
	}
}

/*!
** @brief	Gets a pointer to the value for @a key in @a hashtable, or NULL if
**			it does not exist.
**
** @param	hashtable	The hashtable to search.
** @param	key			The key to search the hashtable for.
** @return	A pointer to the value associated with the key, or NULL if no such
**			key is in the hashtable.
*/
char *		ht_get(hashtable_t * hashtable, const char * key)
{
	size_t		slot = hash(key);
	
	for (entry_t * entry = hashtable->entries[slot]; entry != NULL; entry = entry->next)
		if (strcmp(entry->key, key) == 0)
			return entry->value;
	return NULL;
}

/*!
** @brief	Traverses the entire hashtable and prints out its key-value contents
**			to stdout level-by-level.
**
** @param	hashtable	A pointer to the hashtable to print.
*/
void	print_hashtable(hashtable_t * hashtable)
{
	for (size_t i = 0; i < TABLE_SIZE; ++i)
	{
		entry_t *	entry = hashtable->entries[i];
		if (entry != NULL)
		{
			printf("slot[%4zu]: ", i);
			for ( ; entry != NULL; entry = entry->next)
				printf("%s=%s ", entry->key, entry->value);
			printf("\n");
		}
	}
}

int	main(void)
{
	hashtable_t	hashtable;
	ht_init(&hashtable);
	ht_set(&hashtable, "madrid", "madrid");
	ht_set(&hashtable, "cataluña", "barcelona");
	ht_set(&hashtable, "valencia", "valencia");
	ht_set(&hashtable, "euskadi", "vitoria-gasteiz");
	ht_set(&hashtable, "navarra", "pamplona");
	ht_set(&hashtable, "aragón", "zaragoza");
	ht_set(&hashtable, "la rioja", "logroño");
	ht_set(&hashtable, "asturias", "oviedo");
	ht_set(&hashtable, "cantabria", "santander");
	ht_set(&hashtable, "galicia", "santiago de compostela");
	ht_set(&hashtable, "castilla y león", "burgos");
	ht_set(&hashtable, "castilla la mancha", "toledo");
	ht_set(&hashtable, "andalucía", "sevilla");
	ht_set(&hashtable, "extremadura", "mérida");
	ht_set(&hashtable, "murcia", "murcia");
	ht_set(&hashtable, "canarias", "las palmas");
	ht_set(&hashtable, "baleares", "palma");
	ht_set(&hashtable, "ceuta", "ceuta");
	ht_set(&hashtable, "melilla", "melilla");
	print_hashtable(&hashtable);
	return 0;
}
