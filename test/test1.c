#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define N 100

struct pos{
	int x;
	int y;
};

int main()
{
	struct pos pos[N];
	double magic = 3.5e-2;
	srand(time(NULL));
	for(int i =0;i<N;i++){
		pos[i].x = rand();
		pos[i].y = rand();
	}
	for(int i=0;i<N;i++){
		printf("(%d,%d)\n",pos[i].x,pos[i].y);
	}
    return 0;
}