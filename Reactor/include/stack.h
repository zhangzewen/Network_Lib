
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10
typedef struct Stack{
	void **top;
	void **base;
	int stacksize;
}*stack;

stack InitStack(stack S);

int StackEmpty(stack S);

void* GetTop(stack S);

stack Push(stack S,void * node);

void *Pop(stack S);

void Print(stack S);
