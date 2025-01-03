#include "debug.h"
#include "list.h"

List *List_create() { return calloc(1, sizeof(List)); }

void List_destroy(List *list) {
	LIST_FOREACH(list, first, next, curr) {
		if (curr->prev) free(curr->prev);
	}
	free(list->last);
	free(list);
}

void List_clear(List *list) {
	LIST_FOREACH(list, first, next, curr) {
		free(curr->value);
	}
}

void List_clear_destroy(List *list) {
	List_clear(list);
	List_destroy(list);
}

void List_push(List *list, void *value) {
	ListNode *node = calloc(1, sizeof(ListNode));
	check_mem(node);

	node->value = value;
	if (list->last == NULL) {
		list->first = node;
		list->last = node;
	} else {
		list->last->next = node;
		node->prev = list->last;
		list->last = node;
	}

	list->count++;
error:
	return;
}

void *List_remove(List *list, ListNode *node) {
	void *result = NULL;

	check(list->first && list->last, "List is empty");
	check(node, "Node cannot be null");

	if (node == list->first && node == list->last) {
		list->first = NULL;
		list->last = NULL;
	} else if (node == list->first) {
		list->first = node->next;
		check(list->first != NULL, "Invalid list, first node is NULL");
		list->first->prev = NULL;
	} else if (node == list->last) {
		list->last = node->prev;
		check(list->last != NULL, "Invalid list, last node is NULL");
		list->last->next = NULL;
	} else {
		ListNode *before = node->prev;
		ListNode *after = node->next;
		after->prev = before;
		before->next = after;
	}

	list->count--;
	result = node->value;
	free(node);

error:
	return result;
}

void *List_pop(List *list) {
	ListNode *node = list->last;
	return node != NULL ? List_remove(list, node) : NULL;
}

void List_unshift(List *list, void *value) {
	ListNode *node = calloc(1, sizeof(ListNode));
	check_mem(node);
	node->value = value;

	if (list->first == NULL) {
		list->first = node;
		list->last = node;
	} else {
		node->next = list->first;
		list->first->prev = node;
		list->first = node;
	}

	list->count++;
error:
	return;
}

void *List_shift(List *list) {
	ListNode *node = list->first;
	return node != NULL ? List_remove(list, node) : NULL;
}
