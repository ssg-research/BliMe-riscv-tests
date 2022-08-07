#include <stdio.h>

#define ARRAY_SIZE (__UINT16_MAX__ << 4)
unsigned int g_array[ARRAY_SIZE];

void set_array(unsigned int val)
{
	for (unsigned long long int i = 0; i < ARRAY_SIZE; ++i)
		g_array[i] = val;
}

int check_array(unsigned int val)
{
	for (unsigned long long int i = 0; i < ARRAY_SIZE; ++i)
		if (g_array[i] != val)
			return 1;
	
	return 0;
}

int main( int argc, char* argv[] )
{
	set_array(0);
	int check_result = check_array(0);
	if (check_result != 0)
	{
		printf("failed 0 check\n");
		return check_result;
	}
	
	set_array(0xdeadbeef);
	check_result = check_array(0xdeadbeef);
	if(check_result != 0)
	{
		printf("failed constant check\n");
		return check_result;
	}

	printf("passed\n");
}