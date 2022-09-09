#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void err_thr(int err, const char* info)
{
	printf("%s (%d)\n", info, err);
	exit(1);
}

pthread_mutex_t m;
int sum;
int a[100000];

void* thr_fn(void* arg)
{
	// стартовая функция потоков

	// в параметре arg удобно передавать адрес переменной,
	// содержащей начальный обрабатываемый индекс 
	// (для первого потока - 0, для второго потока - 25000 и т.д.)

	// в функции должен использоваться мьютекс m
	int startid = *(int*)arg;
        printf("beginning index is %d\n", startid);
        for(int i = startid; i < startid + 25000; ++i)
        {
                pthread_mutex_lock(&m);
                sum += a[i];
                pthread_mutex_unlock(&m);
        }
        return (void*)0;
}

int main()
{
	srand(time(0));
	for (int i = 0; i < 100000; i++)
		a[i] = rand() % 10 + 1; // генерация случайных значений для элементов массива a
	int err;
	pthread_t tid[4]; // массив идентификаторов потоков
	int num[4];       // массив начальных индексов для потоков
	err = pthread_mutex_init(&m, 0);// инициализация мьютекса
	if (err != 0)
		err_thr(err, "cannot create mutex");
	for (int i = 0; i < 4; i++)
	{
		// создание потоков
		num[i] = i * 25000;
                err = pthread_create(&tid[i], NULL, &thr_fn, &num[i]);
		if (err != 0)
			err_thr(err, "cannot create thread");
	}
	for (int i = 0; i < 4; i++)
		pthread_join(tid[i], 0);
		// ожидание завершения потоков
	printf("sum = %d\n", sum);
	// разрушение мьютекса
	pthread_mutex_destroy(&m);
	// контрольная проверка правильности параллельного алгоритма:
	// нахождение суммы в обычном цикле
	sum = 0;
	for(int i = 0; i < 100000; ++i)
		sum += a[i];
	printf("check: sum = %d\n", sum);
	return 0;
}
