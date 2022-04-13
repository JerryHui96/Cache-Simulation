//#include "pch.h"
#include <iostream>
using namespace std;

#define WAY2 2
#define WAY4 4
#define WAY8 8
#define WAY16 16
#define WAY32 32

#define ADDR_NUM 5000

void cm(unsigned int currentAddress[1], int addr_trace, int cache_size, char set_associativity[2], int block_size, int output[3]);
//	output [0] = total number of simulation address
//	output [1] = total number of cache hit
//	output [2] = cache hit ratio

void directMap(unsigned int currentAddress[1], int cache_size, int block_size);
void two_Way(unsigned int currentAddress[1], int cache_size, int block_size);
void four_Way(unsigned int currentAddress[1], int cache_size, int block_size);
void eight_Way(unsigned int currentAddress[1], int cache_size, int block_size);
void sixteen_Way(unsigned int currentAddress[1], int cache_size, int block_size);
void thirtyTwo_Way(unsigned int currentAddress[1], int cache_size, int block_size);
void Fully_Associative(unsigned int currentAddress[1], int cache_size, int block_size);

int max(int arr[], int set_number);

int cToi(char set_associativity[2]);

unsigned int currentAddress[1] = { 0 };
int miss[1];
int hit[1];
int *cache_memory;
int **set_LRU;
int *set_LRU_F;

int main()
{
	int addr_num = 0;
	int cache_size = 0;
	int block_size = 0;
	char set_associativity[] = { "00" };
	int err = 0;
	int output[3] = { 0,0,0 };
	float hit_ratio[1] = { 0 };
	int block_number;
	int set_number;

	int index = 0;

	miss[0] = 0;
	hit[0] = 0;

	int stop = 1;

	FILE *fp;

	int addr_trace = 0;

	err = fopen_s(&fp, "..\addr_trace.txt", "r");
	if (err)
	{
		printf("Did not open the file\n");
		printf("Error = %d\n\n", err);
		exit(1);
	}
	else if(err == 0)
	{
		printf("Successful reading the file\n");
		printf("Error = %d\n\n", err);
	}

	if (fp == NULL)
	{
		printf("fp is null\n\n");
		exit(1);
	}

	printf("What is the cache size in kilobyte(KB)? (Enter 1, 2, 4, 8, 16, 32, or 64)\n");
	scanf_s("%d", &cache_size);

	printf("What is the block size in byte(B)? (Enter 1, 2, 4, 8, 16, 32, or 64)\n");
	scanf_s("%d", &block_size);

	printf("What is the set associativity of this cache memory? (1, 2, 4, 8, 16, 32, or F (Fully Associative))\n");
	scanf_s("%2s", set_associativity, 2);

	printf("\ncache_size = %d,\tblock_size = %d,\tset_associativity = %s\n\n", cache_size, block_size, set_associativity);

	block_number = cache_size * 1024 / block_size;

	cache_memory = new (nothrow) int[block_number];	//	Declare an array of size = cache block size

	if (set_associativity[0] != 'F')
	{
		set_number = block_number / cToi(set_associativity);
		set_LRU = new(nothrow) int*[set_number];

		for (int i = 0; i < set_number; i++)
		{
			set_LRU[i] = new(nothrow) int[cToi(set_associativity)];
		}

		for (int i = 0; i < set_number; i++)
		{
			for (int j = 0; j < cToi(set_associativity); j++)
			{
				set_LRU[i][j] = -1;
			}
		}
	}
	else if (set_associativity[0] == 'F')
	{
		set_number = block_number;
		set_LRU_F = new(nothrow)int[block_number];
		for (int i = 0; i < block_number; i++)
		{
			set_LRU_F[i] = -1;
		}
	}

	for (int i = 0; i < block_number; i++)
	{
		cache_memory[i] = -1;
	}

	for (int k = 0; k < ADDR_NUM; k++)
	{
		fscanf_s(fp, "%d", &addr_trace);

		/*
		if (addr_trace == -220888)
		{
			if (stop != 0)
			{
				stop = 0;
			}
			else
			{
				break;
			}
		*/
		cm(currentAddress, addr_trace, cache_size, set_associativity, block_size, output);

		hit_ratio[0] = float(output[1]) / float(output[0]); //	Total Hit / Total Address #
		//printf("trace Address: %d,\t\tCurrent Address: %u,\tTotal Address Fetched: %d,\tTotal Hit: %d,\tTotal Miss: %d,\tHit Ratio: %f\tnext_replace_index: %d\n", addr_trace, currentAddress[0], output[0], output[1], output[2], hit_ratio[0], max(set_LRU_F, set_number));
		printf("trace Address: %d,\t\tCurrent Address: %u,\tTotal Address Fetched: %d,\tTotal Hit: %d,\tTotal Miss: %d,\tHit Ratio: %f\t\n", addr_trace, currentAddress[0], output[0], output[1], output[2], hit_ratio[0]);

		//if (k == 28 || k == 22 || k == 23|| k == 21)
		{/*
			cout << endl;
			for (int w = 0; w < block_number; w+=WAY4)
			{
				for (int ww = 0; ww < WAY4; ww++)
				{
					printf("\tSet Index = %d, %d, LRU: %d\n", w/WAY4, cache_memory[w+ww], set_LRU[w/WAY4][ww]);
					if (w == 8 && ww == 1)
					{
						printf("index output: %d\n\n", max(set_LRU[4], WAY4));
					}
				}
			}
			cout << endl;
			cout << endl;*/
		}

		{/*
			for (int w = 0; w < block_number; w += WAY4)
			{
				for (int ww = 0; ww < WAY4; ww++)
				{
					printf("\tSet Index = %d, %d, LRU: %d\n", w / WAY4, cache_memory[w + ww], set_LRU[w / WAY4][ww]);
				}
			}
			cout << endl;
			cout << endl;*/
		}

	}

	for (int i = 0; i < block_number; i++)
	{
		printf("index: %d\tstored memory: %d\n", i, cache_memory[i]);
	}

	//hit_ratio[0] = output[1] / output[0];
	//printf("\nTotal Address Fetched: %d,\tTotal Hit: %d,\tTotal Miss: %d,\tHit Ratio: %f\n", output[0], output[1], output[2], hit_ratio[0]);
	fclose(fp);
	return 0;
}

void cm(unsigned int currentAddress[1], int addr_trace, int cache_size, char set_associativity[2], int block_size, int output[3])
{
	output[0]++;
	currentAddress[0] += addr_trace;

	switch (set_associativity[0])
	{
	case '1':
	{
		if (set_associativity[1] == '6')
		{
			sixteen_Way(currentAddress, cache_size, block_size);
		}
		else
		{
			directMap(currentAddress, cache_size, block_size);
		}
		break;
	}
	case '2':
	{
		two_Way(currentAddress, cache_size, block_size);
		break;
	}
	case '4':
	{
		four_Way(currentAddress, cache_size, block_size);
		break;
	}
	case '8':
	{
		eight_Way(currentAddress, cache_size, block_size);
		break;
	}
	case '3':
	{
		thirtyTwo_Way(currentAddress, cache_size, block_size);
		break;
	}
	case 'F':
	{
		Fully_Associative(currentAddress, cache_size, block_size);
		break;
	}
	}
	output[1] = hit[0];
	output[2] = miss[0];
}

int cToi(char set_associativity[2])
{
	switch (set_associativity[0])
	{
	case '1':
		if (set_associativity[1] == '6')
		{
			return 16;
		}
		else
		{
			return 1;
		}
	case '2':
		return 2;
	case '4':
		return 4;
	case '8':
		return 8;
	case '3':
		return 32;
	}
}

int max(int arr[], int set_number)
{
	int max_value = 0;
	int max_value_index = 0;

	for (int p = 0; p < set_number; p++)
	{
		if (arr[p] > max_value)
		{
			max_value = arr[p];
			max_value_index = p;
		}
	}
	return max_value_index;
}

void directMap(unsigned int currentAddress[1], int cache_size, int block_size)
{
	int block_select = 0;
	int block_number = cache_size * 1024 / block_size;
	block_select = int((currentAddress[0] / block_size)) % block_number; //ca = 512, bs = 16, cs = 8*1024 => 
	int lowLimit;

	lowLimit = currentAddress[0] - (currentAddress[0] % block_size); //ca = 8192, bs = 16 => 8192 - (8192%16) = ;

	if (cache_memory[block_select] != lowLimit)
	{
		miss[0]++;
		cache_memory[block_select] = lowLimit;
	}
	else if (cache_memory[block_select] == lowLimit)
	{
		hit[0]++;
	}
}

void two_Way(unsigned int currentAddress[1], int cache_size, int block_size)
{
	int block_number = cache_size * 1024 / block_size;
	int set_number = block_number / WAY2;
	int set_select = int((currentAddress[0] / block_size)) % set_number;
	int block_select = set_select * WAY2;

	int lowLimit = currentAddress[0] - (currentAddress[0] % block_size);

	for (int i = 0; i < WAY2; i++)
	{
		//block_select += i;
		if (cache_memory[block_select + i] == -1 || cache_memory[block_select + i] == lowLimit)
		{
			if (cache_memory[block_select + i] == lowLimit)
			{
				hit[0]++;
				set_LRU[set_select][i] = 0;
			}
			else if (cache_memory[block_select + i] == -1)
			{
				miss[0]++;
				cache_memory[block_select + i] = lowLimit;
				set_LRU[set_select][i] = 0;
			}

			for (int j = 0; j < WAY2; j++)
			{
				if (set_LRU[set_select][j] != -1)
				{
					set_LRU[set_select][j]++;
				}
			}
			break;
		}
		else
		{
			if (i == WAY2 - 1 && cache_memory[block_select + i] != -1)
			{
				miss[0]++;
				cache_memory[block_select + max(set_LRU[set_select], WAY2)] = lowLimit;
				set_LRU[set_select][max(set_LRU[set_select], WAY2)] = 0;

				for (int j = 0; j < WAY2; j++)
				{
					if (set_LRU[set_select][j] != -1)
					{
						set_LRU[set_select][j]++;
					}
				}
				break;
			}
		}
	}

}

void four_Way(unsigned int currentAddress[1], int cache_size, int block_size)
{
	int block_number = cache_size * 1024 / block_size;
	int set_number = block_number / WAY4;
	int set_select = int((currentAddress[0] / block_size)) % set_number;
	int block_select = set_select * WAY4;

	int lowLimit = currentAddress[0] - (currentAddress[0] % block_size);

	for (int i = 0; i < WAY4; i++)
	{
		//block_select += i;
		if (cache_memory[block_select + i] == -1 || cache_memory[block_select + i] == lowLimit)
		{
			if (cache_memory[block_select + i] == lowLimit)
			{
				hit[0]++;
				set_LRU[set_select][i] = 0;
			}
			else if (cache_memory[block_select + i] == -1)
			{
				miss[0]++;
				cache_memory[block_select + i] = lowLimit;
				set_LRU[set_select][i] = 0;
			}

			for (int j = 0; j < WAY4; j++)
			{
				if (set_LRU[set_select][j] != -1)
				{
					set_LRU[set_select][j]++;
				}
			}
			break;
		}
		else
		{
			if (i == WAY4 - 1 && cache_memory[block_select + i] != -1)
			{
				miss[0]++;
				cache_memory[block_select + max(set_LRU[set_select], WAY4)] = lowLimit;
				set_LRU[set_select][max(set_LRU[set_select], WAY4)] = 0;

				for (int j = 0; j < WAY4; j++)
				{
					if (set_LRU[set_select][j] != -1)
					{
						set_LRU[set_select][j]++;
					}
				}
				break;
			}
		}
	}
}

void eight_Way(unsigned int currentAddress[1], int cache_size, int block_size)
{
	int block_number = cache_size * 1024 / block_size;
	int set_number = block_number / WAY8;
	int set_select = int((currentAddress[0] / block_size)) % set_number;
	int block_select = set_select * WAY8;

	int lowLimit = currentAddress[0] - (currentAddress[0] % block_size);

	for (int i = 0; i < WAY8; i++)
	{
		//block_select += i;
		if (cache_memory[block_select + i] == -1 || cache_memory[block_select + i] == lowLimit)
		{
			if (cache_memory[block_select + i] == lowLimit)
			{
				hit[0]++;
				set_LRU[set_select][i] = 0;
			}
			else if (cache_memory[block_select + i] == -1)
			{
				miss[0]++;
				cache_memory[block_select + i] = lowLimit;
				set_LRU[set_select][i] = 0;
			}

			for (int j = 0; j < WAY8; j++)
			{
				if (set_LRU[set_select][j] != -1)
				{
					set_LRU[set_select][j]++;
				}
			}
			break;
		}
		else
		{
			if (i == WAY8 - 1 && cache_memory[block_select + i] != -1)
			{
				miss[0]++;
				cache_memory[block_select + max(set_LRU[set_select], WAY8)] = lowLimit;
				set_LRU[set_select][max(set_LRU[set_select], WAY8)] = 0;

				for (int j = 0; j < WAY8; j++)
				{
					if (set_LRU[set_select][j] != -1)
					{
						set_LRU[set_select][j]++;
					}
				}
				break;
			}
		}
	}
}

void sixteen_Way(unsigned int currentAddress[1], int cache_size, int block_size)
{
	int block_number = cache_size * 1024 / block_size;
	int set_number = block_number / WAY16;
	int set_select = int((currentAddress[0] / block_size)) % set_number;
	int block_select = set_select * WAY16;

	int lowLimit = currentAddress[0] - (currentAddress[0] % block_size);

	for (int i = 0; i < WAY16; i++)
	{
		//block_select += i;
		if (cache_memory[block_select + i] == -1 || cache_memory[block_select + i] == lowLimit)
		{
			if (cache_memory[block_select + i] == lowLimit)
			{
				hit[0]++;
				set_LRU[set_select][i] = 0;
			}
			else if (cache_memory[block_select + i] == -1)
			{
				miss[0]++;
				cache_memory[block_select + i] = lowLimit;
				set_LRU[set_select][i] = 0;
			}

			for (int j = 0; j < WAY16; j++)
			{
				if (set_LRU[set_select][j] != -1)
				{
					set_LRU[set_select][j]++;
				}
			}
			break;
		}
		else
		{
			if (i == WAY16 - 1 && cache_memory[block_select + i] != -1)
			{
				miss[0]++;
				cache_memory[block_select + max(set_LRU[set_select], WAY16)] = lowLimit;
				set_LRU[set_select][max(set_LRU[set_select], WAY16)] = 0;

				for (int j = 0; j < WAY16; j++)
				{
					if (set_LRU[set_select][j] != -1)
					{
						set_LRU[set_select][j]++;
					}
				}
				break;
			}
		}
	}
}

void thirtyTwo_Way(unsigned int currentAddress[1], int cache_size, int block_size)
{
	int block_number = cache_size * 1024 / block_size;
	int set_number = block_number / WAY32;
	int set_select = int((currentAddress[0] / block_size)) % set_number;
	int block_select = set_select * WAY32;

	int lowLimit = currentAddress[0] - (currentAddress[0] % block_size);

	for (int i = 0; i < WAY32; i++)
	{
		//block_select += i;
		if (cache_memory[block_select + i] == -1 || cache_memory[block_select + i] == lowLimit)
		{
			if (cache_memory[block_select + i] == lowLimit)
			{
				hit[0]++;
				set_LRU[set_select][i] = 0;
			}
			else if (cache_memory[block_select + i] == -1)
			{
				miss[0]++;
				cache_memory[block_select + i] = lowLimit;
				set_LRU[set_select][i] = 0;
			}

			for (int j = 0; j < WAY32; j++)
			{
				if (set_LRU[set_select][j] != -1)
				{
					set_LRU[set_select][j]++;
				}
			}
			break;
		}
		else
		{
			if (i == WAY32 - 1 && cache_memory[block_select + i] != -1)
			{
				miss[0]++;
				cache_memory[block_select + max(set_LRU[set_select], WAY32)] = lowLimit;
				set_LRU[set_select][max(set_LRU[set_select], WAY32)] = 0;

				for (int j = 0; j < WAY32; j++)
				{
					if (set_LRU[set_select][j] != -1)
					{
						set_LRU[set_select][j]++;
					}
				}
				break;
			}
		}
	}
}

void Fully_Associative(unsigned int currentAddress[1], int cache_size, int block_size)
{
	int block_number = cache_size * 1024 / block_size;
	int block_select = int((currentAddress[0] / block_size)) % block_number; //ca = 512, bs = 16, cs = 8*1024 => 
	int lowLimit = currentAddress[0] - currentAddress[0] % block_size;

	for (int i = 0; i < block_number; i++)
	{
		if (cache_memory[i] == lowLimit)
		{
			hit[0]++;
			set_LRU_F[i] = 0;
			for (int i = 0; i < block_number; i++)
			{
				if (set_LRU_F[i] != -1)
				{
					set_LRU_F[i]++;
				}
			}
			break;
		}
		else if (cache_memory[i] == -1)
		{
			miss[0]++;
			cache_memory[i] = lowLimit;
			set_LRU_F[i] = 0;
			for (int i = 0; i < block_number; i++)
			{
				if (set_LRU_F[i] != -1)
				{
					set_LRU_F[i]++;
				}
			}
			break;
		}
		else if (i == block_number - 1 && cache_memory[i] != -1)
		{
			miss[0]++;
			cache_memory[max(set_LRU_F, block_number)] = lowLimit;
			set_LRU_F[max(set_LRU_F, block_number)] = 0;
			for (int i = 0; i < block_number; i++)
			{
				if (set_LRU_F[i] != -1)
				{
					set_LRU_F[i]++;
				}
			}
			break;
		}
	}
}
