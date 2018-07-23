#include<malloc.h>
#include<stdlib.h>
#include<stdio.h>
#define PAGE_SIZE 1024
/*

three functions basically'
1. malloc
2. free
3. debug


1. malloc
constraint - should use malloc to assign only n* one kb of memory .
approach - get a block of 1024 bytes of memory as a page from the memory.
now maitain blobs ( which has metadata(metadata here is the sizeof memory block allocated +memory allocateed which user asked for )

________________
|size| the actual|
| 100| memory    |
|____|  i.e,     |
|   allocated    |
|________________|

maintain a free linked list which stored ll of the nodes.
each node consists of
from where is the free memory
how much is the free memory
pointer to next free node.

*/


struct node{
	char* freememadd;
	int freememsize;
	node* next;
	int end;
	int start;
	char* startAdddress;
	char* endAddress;
};


char* page[12];
int noOfPages;
bool b[12];
//struct Pages pages;
struct node* head[12];
//char* page[12];
void  addNode(struct node* hd, struct node* n, int i){
	struct node* prev = hd;
	/*if (hd->startAdddress == n->endAddress){
	n->endAddress = hd->endAddress;
	n->freememsize += hd->freememsize;
	n->
	}*/
	if (hd->startAdddress > n->endAddress){
		n->next = hd;
		head[i] = n;
		return;
	}

	while (hd->next){
		prev = hd;
		hd = hd->next;
		if (hd->startAdddress > n->endAddress){
			prev->next = n;
			n->next = hd;
			return;
		}
	}
	hd->next = n;
	n->next = NULL;
	return;
}


void initialise(int i){
	//if (i == 0){
	//	if (pages.noOfPages)
	//	pages.noOfPages = 0;//check if tthe first page is reallocated then??
	//}

	b[i] = true;
	noOfPages++;
	page[i] = (char*)malloc(sizeof(char) * PAGE_SIZE);
	struct node* n = (struct node*)malloc(sizeof(struct node));

	n->freememsize = PAGE_SIZE;
	n->freememadd = page[i];
	n->next = NULL;
	n->end = 1024;
	n->start = 0;
	n->startAdddress = &page[i][0];
	n->endAddress = &page[i][PAGE_SIZE - 1];
	head[i] = n;
}
void removenode(struct node* temp, int i){
	if (head[i] == temp){
		head[i] = NULL;
		// there is a case to be handled here i.e if  a page gets de allocated then again while allocating we are allocating it while 
		// checking the pages from the beggining and if head[i] iis NULL
	}
	struct node* t = head[i];
	struct node* prev = t;
	t = t->next;
	while (t != temp){
		prev = t;
		t = t->next;
	}
	prev->next = t->next;
}
int joinnode(struct node* hd, int i){
	struct node* strt = head[i];
	struct node* n1 = strt, *n2 = strt, *n3 = strt;
	while (strt->next != NULL){
		n1 = strt->next;
		if (strt->endAddress + 1 == n1->startAdddress){
			if (strt == head[i]){
				head[i]->next = n1->next;
				head[i]->freememsize += n1->freememsize;
				head[i]->endAddress = n1->endAddress;
				head[i]->end = n1->end;

			}
			else{
				strt->next = n1->next;
				strt->freememsize += n1->freememsize;
				strt->endAddress = n1->endAddress;
				strt->end = n1->end;
			}

			return 1;
		}
		strt = strt->next;
	}
	return -1;

}
void merge(struct node* hd, int i){
	//struct node* temp = head[i];

	int check = joinnode(head[i], i);
	while (check == 1){
		check = joinnode(head[i], i);
	}

}
void* getaddr(int size){
	if (size >= PAGE_SIZE - 4)return NULL;
	int i = 0;
	while (i < 12){
		if (!head[i]){
			initialise(i);
		}
		struct node * temp = head[i];
		while (temp){
			if (temp->freememsize > size + sizeof(int)){
				char* mem = &temp->freememadd[temp->start];

				temp->start += size + sizeof(int);
				temp->startAdddress = &temp->freememadd[temp->start];
				temp->end = temp->freememsize - 1;
				temp->freememsize = temp->freememsize - size - sizeof(int);
				if (temp->freememsize == 0){
					removenode(temp, i);
				}
				int* s = (int*)mem;
				*s = size;
				mem = mem + sizeof(int);
				return (void*)mem;
			}
			temp = temp->next;
		}


		i++;
	}

	return NULL;
}
void* allocate(int s){
	void* add = getaddr(s);
	return add;

}
void* hkmalloc(int size){
	if (size <= 0 || size >= 1022){
		return NULL;
	}

	void* mem = allocate(size);
	return (void*)mem;
}
int geti(void*memaddr){
	char* ptr = (char*)memaddr;
	//int count = 0;
	int i = 0;
	while (i < 12){
		if (b[i] == true){
			if ((&page[i][0] <= memaddr) && (&page[i][1023] >= memaddr)){
				return i;
			}

		}
		i++;

	}
}
void check(int i){
	if (head[i]->endAddress - head[i]->startAdddress + 1 == PAGE_SIZE){
		free(page[i]);
		b[i] = false;
	}
}
int hkfree(void* memaddr){
	if (!memaddr)return 1;
	int i = geti(memaddr);

	char* memstart = (char*)(memaddr);
	memstart = memstart - sizeof(int);
	int* size = (int*)memstart;
	struct node* n = (struct node*)malloc(sizeof(struct node));
	n->startAdddress = (char*)memstart;
	n->next = NULL;
	n->freememsize = *size + sizeof(int);
	n->endAddress = n->startAdddress + n->freememsize - 1;
	n->end = n->endAddress - &page[i][0] - 1;
	n->start = n->startAdddress - &page[i][0];
	n->freememadd = memstart;
	addNode(head[i], n, i);
	merge(head[i], i);
	check(i);
	// size now has the size of the data block.
	return 1;
}
void debug(){
	int i = 0;
	for (int i = 0; i < 12; i++){
		printf("\n PAGE NO. %d\n", i);
		if (b[i] == true){
			struct node* hd = head[i];
			while (hd){
				printf("\nmem strt address\t%d\nmem end address\t%d\nmem size = %d", (int)hd->startAdddress,
					(int)hd->endAddress, hd->freememsize);
				hd = hd->next;
			}
		}
		else{
			printf("\nPage not allocated\n");
		}
	}
}
//int main(){
//
//	int* j = (int*)hkmalloc(sizeof(int) * 15);
//	for (int i = 0; i < 15; i++){
//		j[i] = 0;
//	}
//	char* f = (char*)hkmalloc(sizeof(char) * 1000);
//	//f = "hanosh";
//	char* z = (char*)hkmalloc(sizeof(char) * 20);
//	//	address is changing when the below operation is performed*****************************.
//	//	z = "huhu";
//	char* z2 = (char*)hkmalloc(sizeof(char) * 70);
//	//z2 = "hehe";
//	hkfree(z);
//	debug();
//	hkfree(f);
//	debug();
//	hkfree(z2);
//	hkfree(j);
//	debug();
//
//
//
//	//system("pause");
//
//}