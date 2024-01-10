// gcc -Wall -Wextra object.c main.c -l pthread
#include<stdio.h>
#include"object.h"
#include <pthread.h>

pthread_t tid[3];

int main(void)
{
	int number = 42;
	initialize_monitor();
	pthread_create(&tid[0], NULL, (void *)write_string, "asd");
	pthread_create(&tid[1], NULL, (void *)write_number, &number);
	pthread_create(&tid[2], NULL, (void *)new_line, NULL);
	
	for (int i = 0; i < 3; i++) {
        pthread_join(tid[i], NULL);
    }
	write_string_and_number("dsa", 24);
	return 0;
}