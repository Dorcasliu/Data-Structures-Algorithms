#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// the import for hash function
#include<stdio.h>
#include <stdlib.h>
#include<string.h> // memset
#define FALSE 0
#define TRUE 1

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 
// data structures representing DLList

// ******************************************************* Functions for hash list *************************************************************
typedef int STATUS;
// define the hash list and basic node
typedef struct _NODE
{
    int data;
    struct _NODE* next;
}NODE;

typedef struct _HASH_TABLE
{
    NODE* value[100];
}HASH_TABLE;

// setup hash list
HASH_TABLE* create_hash_table()
{
    HASH_TABLE* pHashTbl = (HASH_TABLE*)malloc(sizeof(HASH_TABLE));
    memset(pHashTbl, 0, sizeof(HASH_TABLE));
    return pHashTbl;
}
// find data in the hash list
NODE* find_data_in_hash(HASH_TABLE* pHashTbl, int data)
{
    NODE* pNode;
    if(NULL ==  pHashTbl)
        return NULL;

    if(NULL == (pNode = pHashTbl->value[data % 10]))
        return NULL;

    while(pNode){
        if(data == pNode->data)
            return pNode;
        pNode = pNode->next;
    }
    return NULL;
}
// insert data into hash list
STATUS insert_data_into_hash(HASH_TABLE* pHashTbl, int data)
{
    NODE* pNode;
    if(NULL == pHashTbl)
        return FALSE;

    if(NULL == pHashTbl->value[data % 10]){
        pNode = (NODE*)malloc(sizeof(NODE));
        memset(pNode, 0, sizeof(NODE));
        pNode->data = data;
        pHashTbl->value[data % 10] = pNode;
        return TRUE;
    }

    if(NULL != find_data_in_hash(pHashTbl, data))
        return FALSE;

    pNode = pHashTbl->value[data % 10];
    while(NULL != pNode->next)
        pNode = pNode->next;

    pNode->next = (NODE*)malloc(sizeof(NODE));
    memset(pNode->next, 0, sizeof(NODE));
    pNode->next->data = data;
    return TRUE;
}

// data type for nodes
typedef struct DLListNode {
	int  value;  // value (int) of this list item 
	struct DLListNode *prev;
	// pointer previous node in list
	struct DLListNode *next;
	// pointer to next node in list
} DLListNode;

//data type for doubly linked lists
typedef struct DLList{
	int  size;      // count of items in list
	DLListNode *first; // first node in list
	DLListNode *last;  // last node in list
} DLList;

// create a new DLListNode
DLListNode *newDLListNode(int it)
{
	DLListNode *new;
	new = malloc(sizeof(DLListNode));
	assert(new != NULL);
	new->value = it;
	new->prev = new->next = NULL;
	return new;
}

// create a new empty DLList
DLList *newDLList()
{
	DLList *L;
	L = malloc(sizeof (struct DLList));
	assert (L != NULL);
	L->size = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}

// ************************************************** create a DLList from a text file *********************************************************
// put your time complexity analysis for CreateDLListFromFileDlist() here
// time complexity = O(n)
// *********************************************************************************************************************************************
DLList *CreateDLListFromFileDlist(const char *filename)
{
 // put your code here
	struct DLList *L=newDLList();
	DLListNode *p_new, *p_tail;

	// for standard input
	if (strcmp(filename, "stdin") == 0){ 
		int n=0;
		char buffer[1000];
		int val[1000];
		while (n<1000){
			fgets(buffer,sizeof(buffer),stdin);//read this line
            if (sscanf(buffer, "%d", &val[n]) == 1) {
                n++;
            }

            //if got the space as input,then stop.
            else {
                break;
            }
		}

		//insert into DLList
		for (int i=0;i<n;i++){  
			p_new=newDLListNode(val[i]);
			p_tail=L->last;
			if (p_tail==NULL){
				L->first=p_new;
				L->last=p_new;
			}
			else{
				p_new->prev = p_tail;
				p_tail->next = p_new;
				L->last = L->last->next;
			}
			L->size+=1;
		}
	}

	// read from open file
	else{
		int num;
		FILE * in = fopen(filename,"r"); 
    	if (in == NULL) {
        	printf("error");
        	exit(-1);
    	}
		while(fscanf(in,"%d",&num)!=EOF){

			p_new=newDLListNode(num);
			p_tail=L->last;
			if (p_tail==NULL){
				L->first=p_new;
				L->last=p_new;
			}
			else{ // move pointer
				p_new->prev = p_tail;
				p_tail->next = p_new;
				L->last = L->last->next;
			}
			L->size+=1;
		}
    	fclose(in);
	}
	return L;
}


// *********************************************************** clone a DLList ******************************************************************
// put your time complexity analysis for cloneList() here
// time complexity = O(n)
// *********************************************************************************************************************************************
DLList *cloneList(DLList *u)
{
 // put your code here
	if (u->size<=1) // when the orignial list is empty or only got one element.
	{
		return u;
	}
	else{
		DLList *L=newDLList();
		DLListNode *p=u->first;
		while (p!=NULL){
			DLListNode *new=newDLListNode(p->value); // the node which want to insert
			DLListNode *tail=L->last; // last element in newDLList
			if (tail==NULL){
				L->first=new;
				L->last=new;
			}
			else{
				new->prev=tail;
				tail->next=new;
				L->last=L->last->next;
			}
			L->size+=1;
			p=p->next; // for traversing all elements in original DLList
		}
		return L;
	}
}


// ********************************************* compute the union of two DLLists u and v ******************************************************
// time complexity = O(n), for the reason that 2n+1 is O(n).
/*	
*	The key idea here to achieve the union function is to use hash list.
*	Firstly, make an empty hash list, traversed all elements in DLList u and put them into this hash list, 
*	aslo using cloneList function to let current union DLList equals to the DLList u. (assume there is no duplicates in DLList u ).
*	Then traversed all elements in DLList v, for each element check whether it has already in hash list.
*	if true, which means this element exists in both DLLists, then do nothing and move on to check the next element in DLList v.
*	If not in hash list, then put this element into the union DLList as well as put it into the hash list，then move on to the next element.
*/
// *********************************************************************************************************************************************
DLList *setUnion(DLList *u, DLList *v)
{
 // put your code here
    HASH_TABLE* hashtable=create_hash_table();
    DLList *union_uv; // the union list

    if (u->size>0){
        // traversed all elements in DLList u
		DLListNode *u_start=u->first;
		while (u_start != NULL){
			insert_data_into_hash(hashtable,u_start->value);
			u_start=u_start->next;
		}

	    union_uv=cloneList(u); // let DLList u==union DLList
	}
    else{ // if u is an empty DLList, then let DLList v==union DLList
	    union_uv=cloneList(v);
    }

    // traversed all elements in DLList v, check whether it's not in DLList u
    if (v->size>0){
        // traversed all elements in DLList v
		DLListNode *v_start=v->first;

		while (v_start != NULL){
            NODE* node=find_data_in_hash(hashtable,v_start->value);
            if (node==NULL){ // not in hash map
                insert_data_into_hash(hashtable,v_start->value);

                // put into union link list
                DLListNode *p_new=newDLListNode(v_start->value);
				DLListNode *p_tail=union_uv->last; // find the position of last element in DLList union
				p_new->prev = p_tail;
				p_tail->next = p_new;
				union_uv->last=union_uv->last->next;
				union_uv->size+=1;
            }
			v_start=v_start->next;
		}
	}
	return union_uv;
}


// ********************************************* compute the insection of two DLLists u and v **************************************************
// put your time complexity analysis for intersection() here
// time complexity = O(n), for the reason that 2n+1 is O(n).
/*	
*	The key idea here to achieve the intersection is to use hash list, which is similar to the union DLList.
*	Firstly, make an empty hash list, traversed all elements in DLList u and put them into this hash list.   
*	Then make an empty intersection DLList, traversed all elements in DLList v, for each element check whether it has already in hash list.
*	if true, which means this element exists in both DLLists, then put this element into the intersection DLList and move on to the next.
*	If not in hash list, then just move on to check the next element in DLList v.
*/
// *********************************************************************************************************************************************
DLList *setIntersection(DLList *u, DLList *v)
{
  // put your code here
    HASH_TABLE* hashtable=create_hash_table();
	DLList *insection_L=newDLList(); // new intersection DLList

    if (u->size>0){ 
        // traversed all elements in DLList u
		DLListNode *u_start=u->first;
		while (u_start != NULL){
			insert_data_into_hash(hashtable,u_start->value);
			u_start=u_start->next;
		}
	}

    // traversed all elements in DLList v, check whether it's already in DLList u
    if (v->size>0){
		DLListNode *v_start=v->first;

		while (v_start != NULL){
            NODE* node=find_data_in_hash(hashtable,v_start->value);
			
            if (node!=NULL){ // already in hash map

				// put into intersection DLList list
				DLListNode *p_new=newDLListNode(v_start->value);
				DLListNode *p_tail=insection_L->last;

				if (p_tail==NULL){
					insection_L->first=p_new;
					insection_L->last=p_new;
				}
				else{
					p_new->prev = p_tail;
					p_tail->next = p_new;
					insection_L->last = insection_L->last->next;
				}
				insection_L->size+=1;
            }
			v_start=v_start->next;
		}
	}
	return insection_L;
}


// *********************************************** free up all space associated with list ******************************************************
// put your time complexity analysis for freeDLList() here
// time complexity = O(n)
// *********************************************************************************************************************************************
void freeDLList(DLList *L)
{
// put your code here	
	DLListNode *p=L->first;
	DLListNode *p_next;
	while (p!=NULL){
		p_next=p->next;
		free(p);
		L->size-=1;
		p=p_next;
	}
}


// ****************************************************** display items of a DLList ************************************************************
// put your time complexity analysis for printDDList() here
// time complexity = O(n)
// *********************************************************************************************************************************************
void printDLList(DLList *u)
{
 // put your code here
	if (u->size<=0){
		printf("Empty list.\n");
	}
	else{
		DLListNode *p_start=u->first;
		while (p_start != NULL){
			printf("%d\n",p_start->value);
			p_start=p_start->next;
		}
	}
}

int main()
{
 DLList *list1, *list2, *list3, *list4;

 list1=CreateDLListFromFileDlist("File1.txt");
 printDLList(list1);

 list2=CreateDLListFromFileDlist("File2.txt");
 printDLList(list2);

 list3=setUnion(list1, list2);
 printDLList(list3);    

 list4=setIntersection(list1, list2);
 printDLList(list4);

 freeDLList(list1);
 freeDLList(list2);
 freeDLList(list3);
 freeDLList(list4);

 printf("please type all the integers of list1\n");
 list1=CreateDLListFromFileDlist("stdin");

 printf("please type all the integers of list2\n");
 list2=CreateDLListFromFileDlist("stdin");

 list3=cloneList(list1);
 printDLList(list3);
 list4=cloneList(list2);
 printDLList(list4);

 freeDLList(list1);
 freeDLList(list2);
 freeDLList(list3);
 freeDLList(list4);

 return 0; 
}
