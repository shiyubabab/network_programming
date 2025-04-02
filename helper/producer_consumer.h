/*************************************************************************
	> File Name: producer_consumer.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 02 Apr 2025 11:43:34 AM CST
 ************************************************************************/

#ifndef __PRODUCER_CONSURMER_H__
#define __PRODUCER_CONSURMER_H__

#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include"csapp.h"

typedef struct sbuf{
	int *buf;
	int n;

	int rear;
	int front;

	sem_t mutex;
	sem_t slots;
	sem_t items;
}sbuf_t;

void sbuf_init(sbuf_t *sp,int n);
void sbuf_deinit(sbuf_t *sp);

void sbuf_insert(sbuf_t *sp, int item);
int sbuf_remove(sbuf_t *sp);

#endif
