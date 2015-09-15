#include<stdio.h>
#include<stdlib.h>
#include "stack.h"

stack InitStack(stack S)
{
	S=(stack)malloc(sizeof(struct Stack));
	S->base =(void **)malloc(STACK_INIT_SIZE*sizeof(void **));
	if(!S->base) exit(1);
	S->top=S->base;
	S->stacksize =STACK_INIT_SIZE;
	return S;
}

void* GetTop(stack S)
{
	void **p;
	void *e;
	if(NULL == S || S->top == S->base) {
		return NULL;
	} 

	p = S->top;
	p=p-1;
	e=*p;
	return e;

}

stack Push(stack S, void *e)
{
	if((S->top)-(S->base)>=(S->stacksize)) {
		S->base =(void**)realloc(S->base,(S->stacksize + STACKINCREMENT)*sizeof(void**));

		S->top=S->base+S->stacksize;
		S->stacksize+=STACKINCREMENT;
	}
	*(S->top)=e;
	S->top++;
	return S;
}


void* Pop(stack S)
{
	void *e;
	if(NULL == S || S->top ==S->base) {
		return NULL;
	}
	S->top=S->top-1;
	e=*S->top;
	return e;
}
int StackEmpty(stack S)
{
	return (NULL == S || (S->base == S->top));
}


#if 0
void PreOrderTraverse(BiTree T)
{
	stack S=NULL;
	S=InitStack(S);
	BiTree p=NULL;
	p=T;
	while(p||!StackEmpty(S))
	{
		if(p)
		{
			printf("%4c",p->data);
			S=Push(S,p);
			p=p->lchild;
		}
		else
		{
			p=Pop(S);
			p=p->rchild;
		}
	}
}
void InOrderTraveerse(BiTree T)
{
		stack S=NULL;
		S=InitStack(S);
		BiTree p=NULL;
		p=T;
		while(p||!StackEmpty(S))
		{
			if(p){S=Push(S,p); p=p->lchild;}
			else
			{
				p=Pop(S);
				printf("%4c",p->data);
				p=p->rchild;
			}
		}
}
//
void PostOrderTravaerse(BiTree T)
{
	stack S=NULL;
	S=InitStack(S);
	BiTree p=NULL;
	BiTree tmp = NULL;
	p = T;
  Push(S, p);
	while(!StackEmpty(S))
	{

		while(p){
			if(p->lflag != 1 )
			{
				if(GetTop(S) != p) {
					Push(S, p);	
				}
				p->lflag = 1;
				if(p->lchild){
                    Push(S, p->lchild);
                    p = p->lchild;
				}
			}else if(p->rflag != 1 )
			{
				if(GetTop(S) != p) {
					Push(S, p);	
				}
				p->rflag = 1;
                if(p->rchild){
                    Push(S, p->rchild);
                    p = p->rchild;
                }

			}
			if(p->lflag == 1 && p->rflag == 1){
                break;
			}
		}
		p = Pop(S);
		printf("%4c", p->data);
		if(StackEmpty(S)) {
			return;
		}
		tmp = GetTop(S);
		if(tmp->lflag == 1 && tmp->rflag == 1) {
			p = tmp;
		}else if(tmp->lflag != 1){
			tmp->lflag = 1;
			p = tmp->lchild;
		}else if(tmp->rflag != 1){
			tmp->rflag = 1;
			p = tmp->rchild;
		}
	}
}
#endif



