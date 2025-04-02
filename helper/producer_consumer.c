/*************************************************************************
	> File Name: producer_consumer.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Wed 02 Apr 2025 11:43:20 AM CST
 ************************************************************************/

#include"producer_consumer.h"

void sbuf_init(sbuf_t *sp,int n){
	sp->n     = n;
	sp->buf   = calloc(n,sizeof(int));
	sp->rear  = 0;
	sp->front = 0;

	sem_init(&sp->mutex,0,1);
	sem_init(&sp->slots,0,n);
	sem_init(&sp->items,0,0);
}

void sbuf_deinit(sbuf_t *sp){
	Free(sp->buf);
}

void sbuf_insert(sbuf_t *sp,int item){
	sem_wait(&sp->slots);
	sem_wait(&sp->mutex);

	sp->buf[(++sp->rear)%(sp->n)] = item;

	sem_post(&sp->mutex);
	sem_post(&sp->items);
}


int sbuf_remove(sbuf_t *sp){
	int ret;
	sem_wait(&sp->items);
	sem_wait(&sp->mutex);

	ret = sp->buf[(++sp->front)%(sp->n)];

	sem_post(&sp->mutex);
	sem_post(&sp->slots);
	return ret;
}

