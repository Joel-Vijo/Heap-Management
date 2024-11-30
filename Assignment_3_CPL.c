#include <stdio.h>
#include <stdlib.h>
enum status{Free, Allocated};
typedef struct Node{
    int size;
    enum status tag;
    struct Node *next;
    struct Node *prev;
    struct Node *next_free;//used for maintaining free list
    struct Node *prev_free;
}Node;

typedef struct Node2{
    Node *p;
    struct Node2 *next;
}Node2;

void insert(Node2 **pointers,Node *p){
    if(*pointers==NULL){
        *pointers=(Node2 *)malloc(sizeof(Node2));
        (*pointers)->p=p;
        (*pointers)->next=NULL;
    }
    else{
        Node2 *q=*pointers;
        while(q->next!=NULL){
            q=q->next;
        }
        Node2 *t=(Node2 *)malloc(sizeof(Node2));
        t->p=p;
        q->next=t;
        t->next=NULL;
    }
}

void remove_node(Node2 **pointers,Node *p){
    Node2 *q=*pointers;
    Node2 *r=NULL;
    while(q!=NULL && (q->p)!=p){
        r=q;
        q=q->next;
    }
    if(r!=NULL){
        r->next=q->next;
    }
    else{
        *pointers=q->next;
    }
    free(q);
}

void initialize(Node *Heap){
    int s;
    printf("Enter size of heap in bits: ");
    scanf("%d",&s);
    Heap->size=s;
    Heap->next=NULL;
    Heap->prev=NULL;
    Heap->next_free=NULL;
    Heap->prev_free=NULL;
    Heap->tag=Free;
}

int closest_value(int a){
    int s=8;
    while(s<a){
        s=s+8;
    }
    return s;
}

Node *First_fit_Allocate(Node *Heap,int size){
    Node *p=Heap;
    Node *q=p;
    while(q!=NULL && q->tag==1){
        q=q->next;
    }
    if(q==NULL){
        printf("Heap is completely utilized, Cannot allocate memory\n");
        return NULL;
    }
    while(q!=NULL && q->size<size){
        q=q->next_free;
    }
    if(q==NULL){
        printf("Cannot allocate memory of required size\n");
        return NULL;
    }
    else{
        int s=closest_value(size);
        Node *t=(Node *)malloc(sizeof(Node));
        if(s<q->size){
            t->size=s;
            t->tag=Allocated;
            t->next=q->next;
            if(q->next!=NULL){
                (q->next)->prev=t;
            }
            t->prev_free=NULL;
            t->next_free=NULL;
            t->prev=q;
            q->next=t;
            q->size=(q->size)-s;
            return t;
        }
        else{
            if(q->prev_free!=NULL){
                (q->prev_free)->next_free=q->next_free;
            }
            if(q->next_free!=NULL){
                (q->next_free)->prev_free=q->prev_free;
            }
            q->tag=Allocated;
            q->prev_free=NULL;
            q->next_free=NULL;
            free(t);
            return q;
        }
    }
}

Node *Best_fit_Allocate(Node *Heap, int size){
    Node *p=Heap;
    Node *q=p;
    while(q!=NULL && q->tag==1){
        q=q->next;
    }
    if(q==NULL){
        printf("Heap is completely utilized, Cannot allocate memory\n");
        return NULL;
    }
    Node *best;
    while(q!=NULL && q->size<size){
        q=q->next_free;
    }
    if(q==NULL){
        printf("Cannot allocate memory of required size\n");
        return NULL;
    }
    else{
        best=q;
        while(q!=NULL){
            if(q->size>=size && q->size<best->size){
                best=q;
            }
            q=q->next_free;
        }
        q=best;
        int s=closest_value(size);
        Node *t=(Node *)malloc(sizeof(Node));
        if(s<q->size){
            t->size=s;
            t->tag=Allocated;
            t->next=q->next;
            if(q->next!=NULL){
                (q->next)->prev=t;
            }
            t->prev_free=NULL;
            t->next_free=NULL;
            t->prev=q;
            q->next=t;
            q->size=(q->size)-s;
            return t;
        }
        else{
            if(q->prev_free!=NULL){
                (q->prev_free)->next_free=q->next_free;
            }
            if(q->next_free!=NULL){
                (q->next_free)->prev_free=q->prev_free;
            }
            q->tag=Allocated;
            q->prev_free=NULL;
            q->next_free=NULL;
            free(t);
            return q;
        }
    }
}

void Delete(Node **Heap, Node *p){
    if(p->prev!=NULL && (p->prev)->tag==0){
        if(p->next!=NULL && (p->next)->tag==0){
            (p->prev)->size+=p->size+(p->next)->size;
            (p->prev)->next=(p->next)->next;
            if((p->next)->next!=NULL){
                ((p->next)->next)->prev=p->prev;
            }
            (p->prev)->next_free=(p->next)->next_free;
            if((p->next)->next_free!=NULL){
                ((p->next)->next_free)->prev_free=p->prev;                
            }
            free(p);
            free(p->next);
            p=NULL;
        }
        else{
            (p->prev)->size+=p->size;
            (p->prev)->next=p->next;
            if(p->next!=NULL){
                (p->next)->prev=p->prev;
            }
            free(p);
            p=NULL;
        }
    }
    else if(p->next!=NULL && (p->next)->tag==0){
        (p->next)->size+=p->size;
        if(p->prev!=NULL){
            (p->prev)->next=p->next;
        }
        else{
            *Heap=p->next;
        }
        (p->next)->prev=p->prev;
        free(p);
        p=NULL;
    }
    else{
        p->tag=Free;
        Node *q=*Heap;
        Node *r=NULL;
        while(q!=p){
            if(q->tag==0){
                r=q;
            }
            q=q->next;
        }
        p->prev_free=r;
        if(r!=NULL){
            p->next_free=r->next_free;
            r->next_free=p;
        }
        if(p->next_free!=NULL){
            (p->next_free)->prev_free=p;
        }
    }

}

int calculate_free_space(Node *Heap){
    Node *p=Heap;
    while(p!=NULL && p->tag==1){
        p=p->next;
    }
    int count=0;
    while(p!=NULL){
        count=count+p->size;
        p=p->next_free;
    }
    return count;
}

void print_free_list(Node *Heap){
    Node *p=Heap;
    while(p!=NULL && p->tag==1){
        p=p->next;
    }
    int i=0;
    while(p!=NULL){
        printf("Free Block %d: %d\n",i+1,p->size);
        p=p->next_free;
        i++;
    }
    printf("\n");
}

void print_heap(Node *Heap){
    Node *p=Heap;
    int i=0;
    while(p!=NULL){
        printf("Block %d: %d",i+1,p->size);
        if(p->tag==0){
            printf(" Free\n");
        }
        else{
            printf(" Allocated\n");
        }
        p=p->next;
        i++;
    }
    printf("\n");
}

void print_pointers(Node2 *pointers){
    Node2 *q=pointers;
    while(q!=NULL){
        printf("%x: %d\n",q->p,(q->p)->size);
        q=q->next;
    }
}

void delete_heap(Node *Heap){
    Node *p=Heap;
    Node *q=p->next;
    while(p!=NULL){
        free(p);
        p=q;
        if(q!=NULL){
            q=q->next;
        }
    }
}

void delete_pointers_list(Node2 *pointers){
    Node2 *p=pointers;
    Node2 *q=pointers->next;
    while(p!=NULL){
        free(p);
        p=q;
        if(q!=NULL){
            q=q->next;
        }
    }
}
int main(){
    Node *Heap=(Node *)malloc(sizeof(Node));
    initialize(Heap);
    printf("------Heap Management------\n");
    printf("\n");
    Node2 *pointers=NULL;
    int run=1;
    while(run){
        int x;
        printf("Options: 1)First fit allocation  2)Best fit allocation  3)Free  4)Print heap  5)Print Free_list  6)Print pointers  7)Print free space  8)exit\n");
        printf("Enter your choice: ");
        printf("\n");
        scanf("%d",&x);
        Node *p;
        switch(x){
            case 1:;
                int size;
                printf("Enter size to allocate: ");
                scanf("%d",&size);
                p=First_fit_Allocate(Heap,size);
                if(p!=NULL){
                    printf("%x\n",p);
                    insert(&pointers,p);
                }
                break;
            case 2:;
                int s;
                printf("Enter size to allocate: ");
                scanf("%d",&s);
                p=Best_fit_Allocate(Heap,s);
                if(p!=NULL){
                    printf("%x\n",p);
                    insert(&pointers,p);
                }
                break;
            case 3:;
                printf("Enter address of Node to be freed: ");
                scanf("%p",&p);
                remove_node(&pointers,p);
                Delete(&Heap,p);
                break;
            case 4:
                print_heap(Heap);
                break;
            case 5:
                print_free_list(Heap);
                break;
            case 6:
                print_pointers(pointers);
                break;
            case 7:
                printf("%d\n",calculate_free_space(Heap));
                break;
            case 8:
                delete_pointers_list(pointers);//to delete the list we have created
                delete_heap(Heap);//to delete the heap we have created
                run=0;
                break;
        }   
    }
    return 0;
} 