#ifndef _HTTP_VECTOR_H_INCLUDED__
#define _HTTP_VECTOR_H_INCLUDED__
#ifdef __cplusplus
extern "c" {
#endif

#define VECTOR_DEFAULT_TOTAL 6
#define VECTOR_INCREASE_SIZE 4

typedef struct vector_st vector;

struct vector_st{
	int total;
	int current;
	void **data;
	//int (*insert)(vector *,void *data, int index);
	int (*push)(vector *, void *data);
	void* (*pop)(vector *);
	void* (*get)(vector *, int index);
	int (*update)(vector *, void *data, int index);
	int (*remove)(vector *, int index);
	int (*index)(vector *, void *data, size_t n);
	int (*len)(vector *);
	int (*Isempty)(vector *);
	void (*free)(vector *);
};

vector* vector_create();
int make_heap(vector *v, int (*compare)(void *, void *));
int heap_sort(vector *v, int (*compare)(void *, void *));
void print(vector *v);
#ifdef __cplusplus
}
#endif

#endif
