#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>


#define BUFFER_SIZE 2 //缓存区单元数


sem_t mutex,full,empty; //三个信号量

//缓冲区
struct DataBuffer
{
    int buffer[BUFFER_SIZE];
    int count;//当前产品数量
}dataBuffer;


//移动缓冲区
void moveDataForward(){
    int i;
    for(i=0;i<dataBuffer.count;i++)
        dataBuffer.buffer[i]=dataBuffer.buffer[i+1];
}
//产品送入缓冲区
void push(int data){
    dataBuffer.buffer[dataBuffer.count++]=data;
}
//从缓冲区取产品
int pop(){
    int data=dataBuffer.buffer[0];
    dataBuffer.count--;
    moveDataForward();
    return data;
}
//生产者线程
void *producer(void *arg){

int product;
	int a;
a=*((int*)arg);
    srand(time(NULL));
    product=rand()%1000+a;
	
sleep(1);
    printf("生产者 %d 生产一个产品%d\n",a,product);
sleep(1);
    printf("生产者 %d 申请缓冲区\n",a);
sleep(1);
   
        sem_wait(&empty);
        sem_wait(&mutex);
printf("生产者 %d 申请缓冲区成功\n",a
sleep(1);
        printf("生产者 %d 将产品 %d 放入缓冲区\n",a,product);
sleep(1);
        push(product);
        sem_post(&mutex);
        sem_post(&full);
        printf("生产者 %d 释放缓冲区,当前缓冲区共%d个产品\n",a,dataBuffer.count);


}

//消费者线程
void *customer(void *arg){
int d;
int a;
a=*((int*)arg);
sleep(1);
    printf("消费者 %d 申请缓冲区\n",a);
sleep(1);
    sem_wait(&full);
    sem_wait(&mutex);
printf("消费者 %d 申请缓冲区成功\n",a);
sleep(1);
    d=pop();
    printf("消费者 %d 从缓冲区取出产品%d\n",a,d);
sleep(1);
    sem_post(&mutex);
    sem_post(&empty);
    printf("消费者 %d 释放缓冲区，当前缓冲区共%d个产品\n",a,dataBuffer.count);

}

int main(){
    pthread_t thrd_prd1,thrd_prd2,thrd_prd3,thrd_cst1,thrd_cst2,thrd_cst3;
int i1=1,i2=2,i3=3;
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
pthread_create(&thrd_prd1, NULL, producer, (void *)&i1);
pthread_create(&thrd_prd2, NULL, producer, (void *)&i2);
pthread_create(&thrd_prd3, NULL, producer, (void *)&i3);
pthread_create(&thrd_cst1, NULL, customer, (void *)&i1);
pthread_create(&thrd_cst2, NULL, customer, (void *)&i2);
pthread_create(&thrd_cst3, NULL, customer, (void *)&i3);
    
pthread_join(thrd_prd1, NULL);
pthread_join(thrd_prd2, NULL);
pthread_join(thrd_prd3, NULL);
pthread_join(thrd_cst1, NULL);
pthread_join(thrd_cst2, NULL);  
pthread_join(thrd_cst3, NULL);
    return 0;
}
