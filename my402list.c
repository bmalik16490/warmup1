/* Author : Bhavya Malik */

#include <stdio.h>
#include <stdlib.h>
#include "my402list.h"

//1. Length
int My402ListLength(My402List* myList)
{
	return myList->num_members; 
}

//2. Empty
int My402ListEmpty(My402List* myList)
{
	if(myList->num_members == 0) 
		return TRUE;
	else
		return FALSE;
}


//3. Append
int My402ListAppend(My402List* myList, void* obj)
{
	
	if(My402ListEmpty(myList))
	{
		My402ListElem* newElem=NULL;//recheck
		if(!(newElem=(My402ListElem*)malloc(sizeof(My402ListElem))))
		{	
			return FALSE;		
		}
	
		My402ListElem* anchorElem=&myList->anchor;
		newElem->obj=obj;

		anchorElem->next=newElem;
		anchorElem->prev=newElem;
		newElem->next=anchorElem;
		newElem->prev=anchorElem;
		myList->num_members++;
		
		return TRUE;

	}
	
	else 
		return  My402ListInsertAfter(myList, obj, My402ListLast(myList));
}

//4. Insert after a given element
int My402ListInsertAfter(My402List* myList, void* obj, My402ListElem* myListElem)
{
	My402ListElem* newElem=NULL;
	//if list empty append
	if(myListElem == NULL)
	{	
		return My402ListAppend(myList,obj);
	}
	else
	{
		if(!(newElem=(My402ListElem*)malloc(sizeof(My402ListElem))))
		{	
			return FALSE;		
		}
		newElem->obj=obj; //assign object to new element
		newElem->next=myListElem->next; //make new elements next point to next of the element after which the new element is added
		newElem->prev=myListElem; // make new element's previous to point to the element after which it is added		
		myListElem->next->prev=newElem;// make list's element's next's prev to point to new element
		myListElem->next=newElem; // make list's element's next point to new element
		
		myList->num_members++; //increment number of elements in list by 1

		return TRUE; 
	}
	return FALSE;
}

//5. Prepend: appends before anchor node
int My402ListPrepend(My402List* myList, void* obj)
{
	return My402ListInsertAfter(myList, obj, My402ListFirst(myList)->prev); 
}

//6. InsertBefore
int My402ListInsertBefore(My402List* myList, void* obj, My402ListElem* myListElem)
{
	if(myListElem == NULL)
	{
		return My402ListPrepend(myList, obj);
	}
	
	return My402ListInsertAfter(myList, obj, myListElem->prev);
}

//7. Unlink
void My402ListUnlink(My402List* myList, My402ListElem* myListElem)
{
	 
	myListElem->prev->next=myListElem->next; // current's make previous element to point to current's next elemnt
	myListElem->next->prev=myListElem->prev;// make current's next element to point to current's previous element
	myList->num_members--; //decrement length of list	
	free(myListElem); //free the element
	
	
} 

//8. Unlink All
void My402ListUnlinkAll(My402List* myList)
{
	int i=0;
	My402ListElem* element= NULL;

	//till there are elements in the list run the for loop for unlink	
	for(;i<myList->num_members;i++)
 	{	element=My402ListFirst(myList);
		My402ListUnlink(myList,element);
		
	}
	
}


//9. First 
My402ListElem *My402ListFirst(My402List* myList) 
{
	return myList->anchor.next;
}


//10. Last
My402ListElem *My402ListLast(My402List* myList) 
{
	return myList->anchor.prev;
}

//11. Next
My402ListElem* My402ListNext(My402List* myList, My402ListElem* myListElem)
{
	if(My402ListLast(myList) == myListElem)
		return NULL;
	else
		return myListElem->next;
} 

//12. Prev
My402ListElem* My402ListPrev(My402List* myList, My402ListElem* myListElem)
{
	if(My402ListFirst(myList) == myListElem)
		return NULL;
	else
		return myListElem->prev;
} 



//13. find
My402ListElem* My402ListFind(My402List* myList, void* obj)
{
	My402ListElem* current = My402ListFirst(myList);//get first element
	do
	{
		if(current->obj == obj) 
			return current;
	}
	while((current=My402ListNext(myList, current))); 
	
	return NULL;
}



//14. init: intializes the list to empty list. Returns TRUE if all is well else returns NULL on error
int My402ListInit(My402List* myList)
{
	if(myList)
	{
		myList->num_members=0; //number of elements in list=0
		void* objVal=NULL; 
		myList->anchor.obj=&objVal; //make anchor's obj point to NULL
		myList->anchor.next = &myList->anchor;
		myList->anchor.prev = &myList->anchor;
		return TRUE;	
	}	
	return FALSE;
}


