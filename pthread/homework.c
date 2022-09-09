#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct interval
{
	int left, right;
};

/*output error*/
void err_report(int err, const char* info)
{
	printf(info, err);
	exit(1);
}

/*funtion for finding prime*/
void* findPrime(void* arg)
{
	int cnt = *((int*)arg);
	int n = 2;
	int isprime = 1;
	while(cnt > 0)
	{
		isprime = 1;
		for(int i = 2; i*i <= n; ++i)
		{
			if(n % i == 0)
			{
				isprime = 0;
				break;
			}
		}
		if(isprime) --cnt;
		++n;
	}
	n -= 1;
	return (void*)n;
}
/*function for calculating intergral of differencial x^4*/
void* calInterg(void* arg)
{
	struct interval *intv = (struct interval*)arg;
	float a = intv->left;
	float b = intv->right;
	float ans = 0;
	float len = (b - a) / 1000.0;
	for(int i = 0; i < 1000; ++i)
	{
		ans += (a*a*a*a) * len;
		a += len;
	}
	printf("The intergral of x^4 in interval(%d, %d) is %f\n",intv->left, intv->right, ans);
}


/*main function*/
int main()
{
	int which_prime = 500;
	int prime = 0;
	float ans = 0;
	int err;
	struct interval intv = {0, 5};
	/*anounce two thread id*/
	pthread_t thread1_id, thread2_id;
	/*create new threads*/
	err = pthread_create(&thread1_id, NULL, &findPrime, (void*)&which_prime);
	if(err != 0)
		err_report(err, "cannot create thraed1 with returned error %d");
	err = pthread_create(&thread2_id, NULL, &calInterg, (void*)&intv);
	if(err != 0)
		err_report(err, "cannot create thread2 with returned error %d");
	/*To wait the threads get finished*/
	pthread_join(thread1_id, (void*)&prime);
	pthread_join(thread2_id, NULL);
	printf("The %dth prime number is %d\n", which_prime, prime);
	//printf("The intergral in interval(%d, %d) is %f\n", intv.left, intv.right, ans);
	return 0;
}
