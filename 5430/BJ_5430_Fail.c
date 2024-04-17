#include <stdio.h>
#include <stdlib.h>

typedef struct acvm_s
{
	int *command_set;
	int array_size;
	int *array;
	int read_direction; // 0 is forward, 1 is backward reading
	int zero_but_not_error_flag;
} acvm_t;

void bad_malloc_exit()
{
	exit(1);
}

int str_len(char *str)
{
	int length = 0;
	for(int i = 0; str[i] != 0; i++)
	{
		++length;
	}
	
	return length;
}

acvm_t *acvm_get_mem(int com_size, int arr_size)
{
	acvm_t *p;
	
	p = (acvm_t *) malloc(sizeof(acvm_t));
	
	if(p == NULL)
	{
		bad_malloc_exit();
	}
	
	p->array_size = arr_size;
	
	p->array = (int *) malloc(sizeof(int) * arr_size);
	if(p->array == NULL)
	{
		bad_malloc_exit();
	}
	
	p->command_set = (int *) malloc(sizeof(int) * com_size);
	if(p->command_set == NULL)
	{
		bad_malloc_exit();
	}	
	return p;
}

void acvm_rel_mem(acvm_t *p)
{
	free(p->command_set);
	free(p->array);
	free(p);
}

int *acvm_set_command_set(int *com_line, int com_size, char *temp_command)
{
	for(int i = 0; i < com_size; i++)
	{
		if(temp_command[i] == 'R') // Reverse = 2
		{
			com_line[i] = 2;
		}
		else if(temp_command[i] == 'D') // Delete = 1
		{
			com_line[i] = 1;
		}
	}
	
	return com_line;
}

int *acvm_set_array(int *arr, int arr_size, char *temp_array)
{
	int no_array = 0;
	int buffer = 0;
	for(int i = 1; temp_array[i] != ']'; i++)
	{
		buffer += (int)(temp_array[i] - 48);
		if(temp_array[i + 1] == ',')
		{
			arr[no_array] = buffer;
			++no_array;
			++i;
			buffer = 0;
		}
		if(temp_array[i + 1] == ']')
		{
			arr[no_array] = buffer;
			++no_array;
			++i;
			buffer = 0;
			break;
		}
		buffer *= 10;
	}
	
	return arr;
	
}

acvm_t *acvm_process_array(acvm_t *p, int com_size)
{
	// 2 is Reverse, 1 is Delete from the end
	// command_set
	// array
	// array_size
	// read_direction
	int remaining_elements = p->array_size;
	p->zero_but_not_error_flag = 0;
	
	int front_cursor = 0;
	int back_cursor = 0;
	
	p->read_direction = 0;
	
	for(int i = 0; i < com_size; i++)
	{
		if(p->command_set[i] == 2)
		{
			//printf("Read direction changed into %s\n", p->read_direction == 0 ? "forwards" : "backwards");
			p->read_direction = p->read_direction == 1 ? 0 : 1;
		}
		else if(p->command_set[i] == 1)
		{
			if(remaining_elements < 1)
			{
				p->read_direction = -1;
				return p;
			}
			if(p->read_direction == 0)
			{
				//printf("Object no.%d of the list %d -> removed\n", front_cursor, p->array[front_cursor]);
				p->array[front_cursor++] = 0;
				--remaining_elements;
			}
			else
			{
				//printf("Object no.%d of the list %d -> removed\n", p->array_size - 1 - back_cursor, p->array[p->array_size - 1 - back_cursor]);
				p->array[p->array_size - 1 - (back_cursor++)] = 0;
				--remaining_elements;
			}
		}
		
		
	}
	//printf("Interpretation Done.\n");
	if(remaining_elements == 0) p->zero_but_not_error_flag = 1;
	
	return p;
}

// str_len()

int main()
{
	acvm_t **test_case;
	int rpt;
	scanf("%d", &rpt);
	char temp_command[100001];
	char temp_array[400002];
	int size_command;
	int size_array;
	
	test_case = (acvm_t **) malloc(sizeof(acvm_t *) * rpt);
	
	for(int i = 0; i < rpt; i++)
	{
		size_command = 0;
		size_array = 0;
		
		scanf("%s", temp_command);
		scanf("%d", &size_array);
		scanf("%s", temp_array);
		
		// Due to memory limitation we couldn't separate the input phase and the process phase
		
		size_command = str_len(temp_command);
		
		test_case[i] = acvm_get_mem(size_command, size_array);
		test_case[i]->command_set = acvm_set_command_set(test_case[i]->command_set, size_command, temp_command);
		test_case[i]->array = acvm_set_array(test_case[i]->array, size_array, temp_array);
		
		//printf("%s, %s\n", temp_command, temp_array);
		
		test_case[i] = acvm_process_array(test_case[i], size_command);
	}
	
	for(int i = 0; i < rpt; i++)
	{
		if(test_case[i]->read_direction != -1) printf("[");
		if(test_case[i]->read_direction == -1)
		{
			printf("error\n");
		}
		for(int j = 0; j < test_case[i]->array_size; j++)
		{
			if(test_case[i]->read_direction == 0)
			{
				if(test_case[i]->array[j] != 0)
				{
					printf("%d,", test_case[i]->array[j]);
				}
			}
			else
			{
				if(test_case[i]->array[test_case[i]->array_size - 1 - j] != 0)
				{
					printf("%d,", test_case[i]->array[test_case[i]->array_size - 1 - j]);
				}
			}
		}
		if(test_case[i]->read_direction != -1 && test_case[i]->zero_but_not_error_flag == 0) printf("\b");
		if(test_case[i]->read_direction != -1) printf("]\n");	
	}
	for(int i = 0; i < rpt; i++)
	{
		acvm_rel_mem(test_case[i]);
	}
	free(test_case);
	return 0;
}
