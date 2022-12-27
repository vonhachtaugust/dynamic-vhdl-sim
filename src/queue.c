/*
 * queue.c
 *
 *  Created on: 18 Oct 2022
 *      Author: developer
 */
#include "queue.h"
#include <stdio.h>
#include <string.h>

static QueueEntry* queue_search(Queue* queue, QueueEntry* entry);

/***
 * queue_init > initialize queue by setting all values to known state and initialize the first memeory allocatable entry
 */
void queue_init(Queue* queue) {
	for (uint32_t i = 0; i < QUEUE_SIZE; i++) {
		memset(&queue->fifo[i], 0, sizeof(QueueEntry));

		if (i < QUEUE_SIZE-1) {
			queue->fifo[i].next = &queue->fifo[i+1];
		} else {
			/* This defines queue full */
			queue->fifo[QUEUE_SIZE-1].next = NULL;
		}
	}
	queue->free = &queue->fifo[0];
}

/***
 * queue_alloc > take free and set free equal to free.next
 */
QueueEntry* queue_malloc(Queue* queue) {
	QueueEntry* entry = NULL;

	pthread_mutex_lock(&queue->queue_lock);

	/* Check for full */
	if (queue->free != NULL) {
		// Take write (free slot in array).
		entry = queue->free;

		// Redirect write to the taken slots next.
		queue->free = entry->next;
	}

	if (entry != NULL) {
		entry->next = NULL;
	}

	pthread_mutex_unlock(&queue->queue_lock);

	return entry;
}

/***
 * queue_free > allow entry to me allocated again, redirect free pointer to this array position
 */
void queue_free(Queue* queue, QueueEntry* entry) {
	pthread_mutex_lock(&queue->queue_lock);

	entry = queue_search(queue, entry);

	if (entry != NULL) {
		/* Entry is missing its next pointer */
		QueueEntry* tmp = queue->free;

		/* The newly freed entry is available for malloc again */
		queue->free = entry;

		memset(queue->free, 0, sizeof(QueueEntry));

		/* Entry next is now the previous free entry */
		entry->next = tmp;
	}

	pthread_mutex_unlock(&queue->queue_lock);
}

/***
 * queue_search > ensure entry is actually in the queue, NULL otherwise
 */
static QueueEntry* queue_search(Queue* queue, QueueEntry* entry) {
	/* TODO: Check memory start and end instead since we know its a continuous block */
	for (uint32_t i = 0; i < QUEUE_SIZE; i++) {
		if (&queue->fifo[i] == entry) {
			return &queue->fifo[i];
		}
	}
	return NULL;
}

int queue_empty(Queue* queue) {
	return (queue->free == &queue->fifo[0]);
}

int queue_full(Queue* queue) {
	return (queue->free == NULL);
}

void queue_print(const char* msg, Queue* queue) {
	printf("%s\n", msg);
	printf("<queue> %p <free> %p\n", queue, queue->free);

	for (uint32_t i = 0; i < QUEUE_SIZE; i++) {
		printf("<id> %d <address> %p <length> %d <next> %p", i, &queue->fifo[i], queue->fifo[i].length, queue->fifo[i].next);
		printf(" <content> ");
		for (uint32_t j = 0; j < QUEUE_ENTRY_SIZE; j++) {
			printf(" 0x%02hhx ", queue->fifo[i].content[j]);
		}
		printf("\n");
	}
}


