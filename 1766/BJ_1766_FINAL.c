// Baekjoon Problem 1766
// Topology Sort + Priority Queue
// 지식이늘었다

#include <stdio.h>
#include <stdlib.h>

typedef struct node_s
{
	int done;
	int next_no;
	int prev_no;
	int *next;
} node_t;

node_t *node_get(int size)
{
	node_t *p;
	p = (node_t *) malloc(sizeof(node_t) * (size + 1));
	if(p == NULL)
	{
		printf("bdme\n");
		exit(1);
	}
	
	return p;
}

int *int_get(int size)
{
	int *p;
	p = (int *) malloc(sizeof(int) * size);
	if(p == NULL)
	{
		printf("bdme\n");
		exit(1);
	}
	
	return p;
}

int *int_res(int *p, int size)
{
	p = (int *) realloc(p, sizeof(int) * size);
	if(p == NULL)
	{
		printf("bdre\n");
		exit(1);
	}
	
	return p;
}

void prqueue_push(int *pq, int *pq_size, int data)
{
	*pq_size += 1;
	int pos = *pq_size;
	while(pos > 1 && data < pq[pos / 2])
	{
		pq[pos] = pq[pos / 2];
		pos /= 2;
	}
	pq[pos] = data;
}

int prqueue_pop(int *pq, int *pq_size)
{
	int pos = 1;
	
	int result = pq[1];
	
	if(*pq_size == 0)
	{
		printf("pr queue is empty. co\n");
		return 0;
	}
	
	
	pq[pos] = pq[*pq_size];
	
	
	
	*pq_size -= 1;
	int pq_comp_pos = 0;
	int pq_temp;
	
	while(pos * 2 <= *pq_size)
	{
		if(pos * 2 + 1 > *pq_size)
		{
			pq_comp_pos = pos * 2;
		}
		else
		{
			pq_comp_pos = pq[pos * 2] < pq[pos * 2 + 1] ? pos * 2 : pos * 2 + 1;
		}
		if(pq[pq_comp_pos] < pq[pos])
		{
			pq_temp = pq[pq_comp_pos];
			pq[pq_comp_pos] = pq[pos];
			pq[pos] = pq_temp;
			pos = pq_comp_pos;
		}
		else
		{
			break;
		}
	}
	
	
	return result;
	
}

void prqueue_print(int *pq, int *pq_size)
{
	
	printf("now heap has ");
	
	for(int i = 1; i <= *pq_size; i++)
	{
		printf("%d ", pq[i]);
	}
	
	printf("\n");
}

int main()
{
	int nodes, opts;
	scanf("%d %d", &nodes, &opts);
	
	node_t *node;
	int *prqueue;
	int prqueue_size = 0;
	
	node = node_get(nodes);
	prqueue = int_get(nodes + 1);
	
	
	for(int i = 0; i < nodes; i++)
	{
		node[i].done = 0;
		node[i].prev_no = 0;
		node[i].next_no = 0;
		node[i].next = int_get(1);
	}
	
	int from, to;
	
	for(int i = 0; i < opts; i++)
	{
		scanf("%d %d", &from, &to);
		node[from].next_no += 1;
		node[to].prev_no += 1;
		node[from].next = int_res(node[from].next, node[from].next_no);
		node[from].next[node[from].next_no - 1] = to;
	}
	
	int remaining = nodes;
	int cursor = 1;
	int result;
	
	while(remaining != 0)
	{
		if(cursor <= nodes)
		{
			if(node[cursor].prev_no == 0)
			{
				//printf("push %d\n", cursor);
				prqueue_push(prqueue, &prqueue_size, cursor);
			}
			
			cursor += 1;
		}
		else
		{
			//prqueue_print(prqueue, &prqueue_size);
			result = prqueue_pop(prqueue, &prqueue_size);
			printf("%d ", result);
			remaining -= 1;
			for(int i = 0; i < node[result].next_no; i++)
			{
				node[node[result].next[i]].prev_no -= 1;
				if(node[node[result].next[i]].prev_no == 0)
				{
					//printf("push %d\n", node[result].next[i]);
					prqueue_push(prqueue, &prqueue_size, node[result].next[i]);
				}
			}
		}
	}
	
	for(int i = 0; i < nodes; i++)
	{
		free(node[i].next);
	}
	free(node);
	free(prqueue);
	
}

