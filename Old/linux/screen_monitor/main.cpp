#include <iostream>
#include <pthread.h>
#include<unistd.h>

#define TIME 1000
using namespace std;

pthread_t tid[5];

class Screen{
    private:
        pthread_mutex_t mutex;
    public:
        Screen(void) {
            pthread_mutex_init(&mutex, NULL);
        }
        void write_string(string s){
            pthread_mutex_lock(&mutex);
            cout << s;
            pthread_mutex_unlock(&mutex);
        }
        void write_number(int n){
            pthread_mutex_lock(&mutex);
            cout << n;
            pthread_mutex_unlock(&mutex);
        }
        void new_line(void){
            pthread_mutex_lock(&mutex);
            cout << endl;
            pthread_mutex_unlock(&mutex);
        }
        void write_string_and_number(string s, int n){
            pthread_mutex_lock(&mutex);
            cout << s << n;
            pthread_mutex_unlock(&mutex);
        }
};

void* print_x(void* unused);


int main(void)
{
    Screen myScreen;

    for(int i = 0; i < 5; i++){
        pthread_create(&(tid[i]), 
                        NULL, 
                        &myScreen.write_number(1),
                        NULL);  
    }
    return 0;    
}