// Baekjoon Problem 1005
// 위상정렬하고 동적 계획 알면 쉽댄다. 난 위상정렬 뭔지 몰라서 그냥 했다.

#include <stdio.h>
#include <stdlib.h>

int undeadsign = 0;
int *total;
int *timelist, *mlist; // timelist: 각 노드의 시간, mlist: 이전 노드번호 저장 후 불러옴
int *array; // 이전 노드 확인기
int *temp_M; // 캐시

int getlist(int bda) // 동적 할당
{
	timelist = (int *) malloc(sizeof(int) * bda);
	if(timelist == NULL)
	{
		printf("Bad Malloc!\n");
		exit(1);
	}
	
	temp_M = (int *) malloc(sizeof(int) * bda);
	if(temp_M == NULL)
	{
		printf("Bad Malloc!\n");
		exit(1);
	}
	
	mlist = (int *) malloc(sizeof(int) * bda * bda);
	if(mlist == NULL)
	{
		printf("Bad Malloc!\n");
		exit(1);
	}
	
	array = (int *) malloc(sizeof(int) * bda * bda);
	if(array == NULL)
	{
		printf("Bad Malloc!\n");
		exit(1);
	}
	
	return 0;
}

void resetarray(int bda) // Array 전체 0으로
{
	for(int i = 0; i < bda * bda; i++)
	{
		array[i] = 0;
	}
}

int addlist_times(int bda) // 각 노드별 시간 데이터 저장
{
	for(int i = 0; i < bda; i++)
	{
		scanf("%d", &timelist[i]);
	}
	
	return 0;
}


void temp_M_reset(int bda) // 캐시. 부분합 저장 후 불러오기용
{
	for(int i = 0; i < bda; i++)
	{
		temp_M[i] = -1;
	}
}

int addrules(int bda, int rules) // 규칙을 Array에 추가
{
	int prev_node, cur_node;
	for(int i = 0; i < rules; i++)
	{
		scanf("%d %d", &prev_node, &cur_node);
		array[(cur_node - 1) * bda + prev_node - 1] = 1;
	}
	
	return 0;
}

int trace(int bda, int step, int sum) // 재귀로 동작하는 Traceback Function
{
	int i = 0;
	int multipleC = 0;
	int temp = 0;
	int max = 0;
	
	for(i = 0; i < bda; i++) // 이전 노드의 개수를 확인
	{
		if(array[(step - 1) * bda + i] == 1)
		{
			multipleC++;
			mlist[(step - 1) * bda + multipleC - 1] = i + 1;
		}
	}
	if(multipleC > 1) // 이전 노드가 여러개일 경우 각 경우의 부분합 비교 필요
	{
		if(temp_M[step - 1] != -1) // 캐시. 특정 노드까지의 최대 부분합 데이터 있음
		{
			max = temp_M[step - 1];
		}
		else // 최대 부분합 데이터 없음
		{
			for(i = 0; i < multipleC; i++)
			{
				temp = timelist[step - 1] + trace(bda, mlist[(step - 1) * bda + i], sum);
				if(temp > max)
				{
					max = temp;
				}
			}
			temp_M[step - 1] = max; // 가장 큰 부분합을 저장 -> 이후 재사용
		}
		return sum + max;
	}
	else if(multipleC == 1) // 이전 노드가 하나일 경우. 그 노드의 값을 불러온다
	{
		return sum + timelist[step - 1] + trace(bda, mlist[(step - 1) * bda], sum);
	}
	else // 이전 노드가 없다 -> 리스트의 끝에 도달했으므로 리턴값 반환
	{
		return timelist[step - 1];
	}
}

void returnlist() // 동적 할당된 Array의 반환
{
	free(mlist);
	free(timelist);
	free(array);
	free(temp_M);
}



int main() // MAIN
{
	int rpt = 0, bda = 0, rules = 0;
	int finalbd = 0;
	
	scanf("%d", &rpt);
	
	total = (int *) malloc(sizeof(int) * rpt); // 최종 출력부에서 알 수 없는 문제가 있'었'기에 도입한 Array. 경로의존성 이슈로 남김.
	if(total == NULL)
	{
		printf("Bad Malloc!\n");
		exit(1);
	}
	
	for(int i = 0; i < rpt; i++)
	{
		scanf("%d %d", &bda, &rules);
		getlist(bda);
		temp_M_reset(bda);
		resetarray(bda);
		
		addlist_times(bda);
		
		addrules(bda, rules);
		
		scanf("%d", &finalbd);
		
		total[i] = trace(bda, final, 0);
		
		returnlist();
	}	
	
	for(int j = 0; j < rpt; j++)
	{
		printf("%d\n", total[j]);
	}
	
	free(total);
	
}