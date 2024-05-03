// Baekjoon Problem 2568
// LIS

#include <stdio.h>
#include <stdlib.h>

typedef struct index_s
{
	int prev_node;
	int node;
	int priority;
	int isAbandoned;
	
} index_t;

typedef struct cache_s
{
	int index;
	int priority;
	int node;
} cache_t;

cache_t **cache;

struct cache_out_s
{
	int priority;
	int abandoned_node_index;
};

void bad_malloc()
{
	printf("Malloc Error go fix rn :/\n");
	exit(1);
}

void bad_realloc()
{
	printf("Realloc Error go fix rn :/\n");
	exit(1);
}




int cache_size = 1;

int cache_malloc()
{
	cache = (cache_t **) malloc(sizeof(cache_t *));
	if(cache == NULL)
	{
		bad_malloc();
	}
	
	cache[0] = (cache_t *) malloc(sizeof(cache_t));
	if(cache[0] == NULL)
	{
		bad_malloc();
	}
	
	return 0;
}

int cache_realloc(int cache_size)
{
	cache = (cache_t **) realloc(cache, sizeof(cache_t *) * cache_size);
	if(cache == NULL)
	{
		bad_realloc();
	}
	
	cache[cache_size - 1] = (cache_t *) malloc(sizeof(cache_t));
	if(cache[cache_size - 1] == NULL)
	{
		bad_malloc();
	}
	
	return 0;
}

int binsort(int start, int end, int node)
{
	if(cache[start]->node == node) return start;
	if(cache[start]->node > node) return start;
	if(cache[end]->node == node) return end;
	if(cache[end]->node < node) return end + 1;
	
	int m = (start + end) / 2;
	//printf("%6d %6d %6d\n", start, end, m);
	
	if(cache[m]->node == node)
	{
		return m;
	}
	else if(cache[m]->node < node) 
	{
		//printf("%d < %d\n", cache[m]->node, node);
		return binsort(m + 1, end, node);
	}
	else 
	{
		//printf("%d > %d\n", cache[m]->node, node);
		return binsort(start, m - 1, node);
	}
	
}

struct cache_out_s check_cache_index(int node, int index)
{
	struct cache_out_s cache_output;
	
	int position;
	
	position = binsort(0, cache_size - 1, node);
	
	//printf("Cache_Size = %d\n", cache_size);

	if(position == cache_size)
	{
		//printf("New_Cache_Data\n");
		cache_size += 1;
		cache_realloc(cache_size);
		cache[position]->node = node;
		cache[position]->index = index;
		cache[position]->priority = position;
		cache_output.priority = position;
		cache_output.abandoned_node_index = -1;

	}
	else
	{
		//printf("Old_Data_Access\n");
		cache_output.abandoned_node_index = cache[position]->index;
		cache[position]->node = node;
		cache_output.priority = cache[position]->priority;
		cache[position]->index = index;
	}
	/*
	for(int i = 0; i < cache_size; i++)
	{
		printf("%6d %6d %6d\n", cache[i]->node, cache[i]->priority, cache[i]->index);
	}
	printf("\n");
	*/
#if 0
#endif
	return cache_output;
	
}

index_t **index_order_check(index_t **p, int node, int cache_size, int index)
{
	struct cache_out_s cache_output;
	p[index]->node = node;
	//printf("!\n");
	cache_output = check_cache_index(node, index);


	p[index]->priority = cache_output.priority;
	if(cache_output.abandoned_node_index != -1) p[cache_output.abandoned_node_index]->isAbandoned = 1;
	/*
	for(int i = 0; i < index + 1; i++)
	{
		printf("%6d %6d %6d\n", p[i]->node, p[i]->priority, p[i]->isAbandoned);
	}
	
	printf("\n");
	*/
	return p;
	
}

int main()
{
	int bin_search_order[500000];
	int lines_num;
	int node[500000] = {};
	int next_node[500000] = {};
	
	cache_malloc();
	
	cache[0]->priority = 0;
	cache[0]->node = 0;
	cache[0]->index = 0;
	
	int n1, n2;
	
	scanf("%d", &lines_num);
	
	for(int i = 0; i < lines_num; i++)
	{
		scanf("%d %d", &n1, &n2);
		node[n1 - 1] = n1;
		next_node[n1 - 1] = n2;
	}
	
	index_t **table;
	table = (index_t **) malloc(sizeof(index_t *) * lines_num);
	
	for(int i = 0; i < lines_num; i++)
	{
		table[i] = (index_t *) malloc(sizeof(index_t));
	}
	
	int index_no = 0;
	
	//printf("\n");

	for(int i = 0; i < 500000; i++)
	{
		if(node[i] != 0)
		{
			//printf("?\n");
			table[index_no]->prev_node = node[i];
			table = index_order_check(table, next_node[i], cache_size, index_no);
			//printf("%d %d\n", table[index_no]->prev_node, table[index_no]->node);
			index_no++;
		}
	}	
	
	int temp = cache_size - 1;
	
	for(int i = lines_num - 1; i >= 0; i--)
	{
		if(table[i]->priority == temp)
		{
			table[i]->isAbandoned = 0;
			temp--;
		}
		else
		{
			table[i]->isAbandoned = 1;
		}
	}
	/*
	for(int i = 0; i < lines_num; i++)
	{
		printf("%6d %6d %6d\n", table[i]->node, table[i]->priority, table[i]->isAbandoned);
	}
	
	printf("\n");
	*/
	printf("%d\n", lines_num - cache_size + 1);

	for(int i = 0; i < lines_num; i++)
	{
		if(table[i]->isAbandoned == 1)
		{
			printf("%d\n", table[i]->prev_node);
		}
	}
	
	for(int i = 0; i < lines_num; i++)
	{
		free(table[i]);
	}
	
	for(int i = 0; i < cache_size; i++)
	{
		free(cache[i]);
	}
	
	free(cache);

	return 0;
	
}
