#include <libc.h>
#include <pthread.h>


int main()
{
    pthread_mutex_t *l;
    pthread_mutex_init(l, NULL);
    pthread_mutex_destroy(l);
    
}