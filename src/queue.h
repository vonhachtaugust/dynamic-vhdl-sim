/*
 * queue.h
 *
 *  Created on: 18 Oct 2022
 *      Author: developer
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdint.h>
#include <pthread.h>

static const uint32_t QUEUE_ENTRY_SIZE = 4;
static const uint32_t QUEUE_SIZE = 4;

typedef struct queue_st Queue;

typedef struct queue_entry_st {
	uint8_t content[QUEUE_ENTRY_SIZE];
	uint32_t length;
	struct queue_entry_st* next;
} QueueEntry;

struct queue_st {
	QueueEntry fifo[QUEUE_SIZE];
	QueueEntry* free;
	pthread_mutex_t queue_lock;
};

void queue_init(Queue* queue);
QueueEntry* queue_malloc(Queue* queue);
void queue_free(Queue* queue, QueueEntry* entry);
int queue_empty();
int queue_full();
void queue_print(const char* msg, Queue* queue);

#endif /* QUEUE_H_ */
