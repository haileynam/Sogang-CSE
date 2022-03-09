#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"
#include "hash.h"
#include "list.h"
#include "user.h"

//extern struct bitmap** myBitmap;
struct bitmap* myBitmap[10];
struct hash* myHash[10];
struct list* myList[10];


void tok(char* str, char** arg)
{
    int tok_cnt=0;
    char* token= strtok(str, " ");

    while(token!=NULL)
    {
        arg[tok_cnt++] = token;
        token = strtok(NULL, " ");
    }
    arg[tok_cnt]= NULL;

//    for(int i=0;i<4;i++){
//        printf("%d %s\n", i,arg[i]);
//    };

    return ;
}

/* ---------------------HASH REGION-----------------------------*/

unsigned hash_value_elem(const struct hash_elem *e, void *aux) {
	struct hash_item *item = hash_entry(e, struct hash_item, elem);
	return hash_int(item->data); 
}

bool hash_less_value (const struct hash_elem *a, const struct hash_elem *b, void *aux) {
	unsigned x = hash_value_elem(a, aux);
	unsigned y = hash_value_elem(b, aux);

	return x < y;
}

void hash_free(struct hash_elem *e, void *aux) {
	struct hash_item* item = hash_entry(e, struct hash_item, elem);
	free(item);
}

void hash_square(struct hash_elem*e, void*aux)
{
	struct hash_item *item = hash_entry(e, struct hash_item, elem);
	item->data *= item->data;
}

void hash_triple(struct hash_elem *e, void *aux)
{
	struct hash_item *item = hash_entry(e, struct hash_item, elem);
	item->data = (item->data*item->data*item->data);
}

/* -------------------------------------------------------------*/

/* --------------------LIST REGION -----------------------------*/

bool list_less_value(const struct list_elem *a, const struct list_elem *b)
{
    struct list_item *item1 = list_entry(a, struct list_item, elem);
    struct list_item *item2 = list_entry(b, struct list_item, elem);

    return item1->data < item2->data;
}

/* -------------------------------------------------------------*/
void level_1(char** arg )
{   
    //printf ("level 1 \n");
    // for(int i=0;i<4;i++){
    //     printf("%d %s\n", i, arg[i]);
    // }
    char* ds_name = arg[2];

    if(strcmp(arg[0], "create")==0)
    {
        if(strcmp(arg[1], "bitmap")==0)
        {
            int idx = ds_name[2]-'0';
            size_t size = atoi(arg[3]);
            //printf("idx %d, size %zu\n", idx, size);

            myBitmap[idx]=bitmap_create(size);

        }
        if(strcmp(arg[1], "hashtable")==0)
        {
            // printf("In create hash\n");
            int idx = ds_name[4]-'0';
            // printf("%d idx\n", idx);
            // printf("size of hash: %d\n", sizeof(struct hash));
            struct hash* tmp = (struct hash*)malloc(sizeof(struct hash));

            hash_init(tmp, hash_value_elem, hash_less_value, NULL);

            myHash[idx]= tmp;
            // free(tmp);
            // printf("hash initialized!\n");
        }

        if(strcmp(arg[1], "list")==0)
        {
            // printf("In create list\n");
            int idx = ds_name[4]-'0';
            // printf("idx: %d\n", idx);
            struct list* tmp = (struct list*)malloc(sizeof(struct list));
            
            list_init(tmp);

            myList[idx]= tmp;
            
            // free(tmp);

            // printf("list initialize!\n");
        }
    }

    if (strcmp(arg[0], "dumpdata")==0)
    {   
 
        char* ds_type = arg[1];
        int idx = ds_type[strlen(arg[1])-1]-'0';
        //printf("index: %d\n", idx);
        //printf("arg[1] length: %d\n", strlen(arg[1]));
        ds_type[strlen(arg[1])-1]= '\0';
        //printf("ds_type: %s\n", ds_type);
       
        if(strcmp(ds_type, "bm")==0)
        {
            //printf("dump bm\n");
            for(size_t i =0;i<bitmap_size(myBitmap[idx]);i++)
            {
                if(bitmap_test(myBitmap[idx],i))
                    printf("1");
                else 
                    printf("0");
            }
            printf("\n");
            //printf("\nDump bm done\n");
        }

        if(strcmp(ds_type, "hash")==0)
        {
            //printf("dump hash\n");
            //printf("index: %d\n", idx);
            if(hash_empty(myHash[idx]))
            {
                //printf("Empty hash!\n");
                return;
            }
                
            else
            {
                struct hash_iterator iter;
                hash_first(&iter, myHash[idx]);
                while(hash_next(&iter))
                {
                    struct hash_item *item;
                    item = hash_entry(hash_cur(&iter), struct hash_item, elem);
                    printf("%d ", item->data);
                }
                printf("\n");

            }

        }

        if(strcmp(ds_type, "list")==0)
        {
            //printf("dump list\n");
            
            //printf("index: %d\n", idx);

            struct list_elem *cur;
            struct list_item *target;

            if(list_empty(myList[idx]))
            {
                //printf("Empty list!\n");
                return;
            }

            for(cur = list_begin(myList[idx]);cur != list_end(myList[idx]); cur = list_next(cur))
            {
                target = list_entry(cur, struct list_item, elem);
                printf("%d ", target->data);
            }
            printf("\n");

        }
    }

    if (strcmp(arg[0], "delete")==0)
    {
        char* ds_type = arg[1];
        int idx = ds_type[strlen(arg[1])-1]-'0';
        ds_type[strlen(arg[1])-1]= '\0';
        if (strcmp(ds_type, "bm") == 0)
        {
            bitmap_destroy(myBitmap[idx]);
        }

        if (strcmp(ds_type, "hash")== 0)
        {
            hash_destroy(myHash[idx], hash_free);
        }

        if (strcmp(ds_type, "list")== 0)
        {
            struct list_item *target;
            while(list_empty(myList[idx])!= NULL )
            {
                struct list_elem *e = list_pop_front(myList[idx]);

                target = list_entry(e, struct list_item, elem);
            }

        }
    }
}

 /* -------------------------OTHER COMMAND---------------------------*/ 
void level_2(char** arg)
{   
    //printf("level 2 \n");
    // for(int i=0;i<4;i++){
    //     printf("%d %s\n", i, arg[i]);
    // }
    char* ds_name = arg[1];
    int idx = ds_name[strlen(arg[1])-1]-'0';

/* ================= BITMAP ===================*/
    if(strcmp(arg[0], "bitmap_mark")==0)
    {
        // bitmap_mark bm0 15
        size_t bit_idx = (size_t)atoi(arg[2]);

        //printf("idx %d, bit_idx %zu\n", idx, bit_idx);

        bitmap_mark(myBitmap[idx], bit_idx);
        }
    else if(strcmp(arg[0], "bitmap_any")==0)
    {
        // bitmap_any bm0 1 4
        size_t start = (size_t)atoi(arg[2]);
        size_t cnt = (size_t)atoi(arg[3]);
        bool flag = bitmap_any(myBitmap[idx], start, cnt);
        printf("%s\n", flag? "true":"false");
    }

    else if(strcmp(arg[0], "bitmap_contains")==0)
    {
        // bitmap_contains bm0 7 3 true
        size_t start = (size_t)atoi(arg[2]);
        size_t cnt = (size_t)atoi(arg[3]);
        bool val = (strcmp(arg[4], "true")==0 ? true: false);
        bool res = bitmap_contains(myBitmap[idx], start, cnt, val);
        printf("%s\n", (res? "true": "false"));
    }

    else if(strcmp(arg[0], "bitmap_count")==0)
    {
        // bitmap_count bm0 2 10 false
        size_t start = (size_t)atoi(arg[2]);
        size_t cnt = (size_t)atoi(arg[3]);
        bool val = (strcmp(arg[4], "true")==0 ? true: false);
        size_t res = bitmap_count(myBitmap[idx], start, cnt, val);
        printf("%zu\n", res);
    }

    else if(strcmp(arg[0], "bitmap_dump")==0)
    {
        // bitmap_dump bm0
        bitmap_dump(myBitmap[idx]);
    }
    
    else if(strcmp(arg[0], "bitmap_expand")==0)
    {
        // bitmap_expand bm0 2
        size_t add_size = (size_t)atoi(arg[2]);
        //myBitmap[idx] = bitmap_expand(myBitmap[idx], add_size);
        bitmap_expand(myBitmap[idx], add_size);
    }

    else if(strcmp(arg[0], "bitmap_flip")==0)
    {
        // bitmap_flip bm0 10
        size_t index = (size_t)atoi(arg[2]);
        bitmap_flip(myBitmap[idx], index);
    }

    else if(strcmp(arg[0], "bitmap_none")==0)
    {
        // bitmap_none bm0 0 16
        size_t start = (size_t)atoi(arg[2]);
        size_t cnt = (size_t)atoi(arg[3]);
        bool res = bitmap_none(myBitmap[idx], start, cnt);
        printf("%s\n", (res? "true": "false"));
    }

    else if(strcmp(arg[0], "bitmap_reset")==0)
    {
        // bitmap_reset bm0 15
        size_t index = (size_t)atoi(arg[2]);
        bitmap_reset(myBitmap[idx], index);
    }

    else if(strcmp(arg[0], "bitmap_scan_and_flip")==0)
    {
        // bitmap_scan_and_flip bm0 0 5 false
        size_t start = (size_t)atoi(arg[2]);
        size_t cnt = (size_t)atoi(arg[3]);
        bool val = (strcmp(arg[4], "true")==0 ? true: false);
        size_t res = bitmap_scan_and_flip(myBitmap[idx], start, cnt, val);
        printf("%zu\n", res);
    }

    else if(strcmp(arg[0], "bitmap_scan")==0)
    {
        // bitmap_scan bm0 0 5 true
        size_t start = (size_t)atoi(arg[2]);
        size_t cnt = (size_t)atoi(arg[3]);
        bool val = (strcmp(arg[4], "true")==0 ? true: false);
        size_t res =bitmap_scan(myBitmap[idx], start, cnt, val);
        printf("%zu\n", res);
    }

    else if(strcmp(arg[0], "bitmap_set_all")==0)
    {
        // bitmap_set_all bm0 false
        bool val = (strcmp(arg[2], "true")== 0 ? true : false);
        bitmap_set_all(myBitmap[idx], val);
    }

    else if(strcmp(arg[0], "bitmap_set_multiple")==0)
    {
        // bitmap_set multiple bm0 2 9 false
        size_t start = (size_t)atoi(arg[2]);
        size_t cnt = (size_t)atoi(arg[3]);
        bool val = (strcmp(arg[4], "true")==0 ? true: false);
        bitmap_set_multiple(myBitmap[idx], start, cnt, val);
    }

    else if(strcmp(arg[0], "bitmap_set")==0)
    {
        // bitmap_set bm0 15 true
        size_t index = (size_t)atoi(arg[2]);
        bool val = (strcmp(arg[3], "true")== 0 ? true : false);
        bitmap_set(myBitmap[idx], index, val);
    }

    else if(strcmp(arg[0], "bitmap_size")==0)
    {
        // bitmap_size bm0
        size_t size = bitmap_size(myBitmap[idx]);
        printf("%zu\n", size);
    }

    else if(strcmp(arg[0], "bitmap_test")==0)
    {
        // bitmap_test bm0 10
        size_t index = (size_t)atoi(arg[2]);
        bool res = bitmap_test(myBitmap[idx], index);
        printf("%s\n", (res? "true": "false"));
    }

    else if(strcmp(arg[0], "bitmap_all")==0)
    {
        // bitmap_all bm0 7 4
        size_t start = (size_t)atoi(arg[2]);
        size_t cnt = (size_t)atoi(arg[3]);
        bool res = bitmap_all(myBitmap[idx], start, cnt);
        printf("%s\n", (res? "true": "false"));
    }

/* ================= BITMAP =============== */
/* ================= HASH ================= */

    else if(strcmp(arg[0], "hash_insert") == 0)
    {
        // hash_insert hash0 1000
        int data = atoi(arg[2]);
        struct hash_item *new_item = malloc(sizeof(struct hash_item));
        new_item->data = data;
        hash_insert(myHash[idx], &(new_item->elem));
    }

    else if(strcmp(arg[0], "hash_apply") == 0)
    {
        // hash_apply hash0 triple
        char* apply = arg[2];
        if(strcmp(apply, "square")== 0)
        {
            hash_apply(myHash[idx], hash_square);
        }

        else if(strcmp(apply, "triple") == 0)
        {
            hash_apply(myHash[idx], hash_triple);
        }
    }

    else if(strcmp(arg[0], "hash_delete") == 0)
    {
        // hash_delete hash0 10
        int data = atoi(arg[2]);
        struct hash_item *new_item = malloc(sizeof(struct hash_item));
        new_item->data = data;
        hash_delete(myHash[idx], &(new_item->elem));
    }
    else if(strcmp(arg[0], "hash_empty") == 0)
    {
        // hash_empty hash0
        // printf("In hash_empty\n");
        bool val = hash_empty(myHash[idx]);
        printf("%s\n", (val? "true": "false"));
    }
    
    else if(strcmp(arg[0], "hash_size") == 0)
    {
        // hash_size hash0
        size_t size = hash_size(myHash[idx]);
        printf("%zu\n", size);
    }
    
    else if(strcmp(arg[0], "hash_clear") == 0)
    {
        // hash_clear hash0
        hash_clear(myHash[idx], hash_free);
    }
    
    else if(strcmp(arg[0], "hash_find") == 0)
    {
        // hash_find hash0 10000
        int data = atoi(arg[2]);
        struct hash_item *new_item = malloc(sizeof(struct hash_item));
        new_item->data = data;
        struct hash_elem *e;
        e = hash_find(myHash[idx], &(new_item->elem));
        if(e == NULL) return;
        else
        {
            printf("%d\n", data);
        }

    }

    else if(strcmp(arg[0], "hash_replace") == 0)
    {
        // hash_replace hash0 1000
        int data = atoi(arg[2]);
        struct hash_item *new_item = malloc(sizeof(struct hash_item));
        new_item->data = data;
        struct hash_elem *e;
        hash_replace(myHash[idx], &(new_item->elem));
    }

/* ================== HASH ===================*/

/* =================== LIST ================== */
    
    else if(strcmp(arg[0], "list_insert") == 0)
    {
        // list_insert list0 1 2
        int before = atoi(arg[2]);
        int value = atoi(arg[3]);
        struct list_item *new_item = malloc(sizeof(struct list_item));
        new_item-> data = value; 
        struct list_elem *e_before = list_head(myList[idx]);
        int cnt=0;
        while(cnt<=before){
            e_before = e_before->next;
            cnt++;
        }

        //struct list_elem *e_new;
        //new_item = list_entry(e_new, struct list_item, elem);
        
        list_insert(e_before, &(new_item->elem));
    }

    else if (strcmp(arg[0], "list_insert_ordered") == 0)
    {
        // list_insert_ordered list0 35
        int value = atoi(arg[2]);
        struct list_item *new_item = malloc(sizeof(struct list_item));
        new_item->data = value;
        list_insert_ordered(myList[idx], &(new_item->elem), list_less_value, NULL);
    }

    else if (strcmp(arg[0], "list_push_front") == 0)
    {
        // list_push_front list0 1
        int value = atoi(arg[2]);
        struct list_item *new_item = malloc(sizeof(struct list_item));
        new_item->data = value;

        list_push_front(myList[idx], &(new_item->elem));
    }
    else if (strcmp(arg[0], "list_push_back") == 0) 
    {
        // list_push_back list0 1
        int value = atoi(arg[2]);
        struct list_item *new_item = malloc(sizeof(struct list_item));
        new_item->data = value;

        list_push_back(myList[idx], &(new_item->elem));
    }

    else if (strcmp(arg[0], "list_pop_front") == 0) 
    {
        // list_pop_front list0
        list_pop_front(myList[idx]);
    }

    else if (strcmp(arg[0], "list_pop_back") == 0) 
    {
        // list_pop_back list0
        list_pop_back(myList[idx]);
    }

    else if (strcmp(arg[0], "list_front") == 0) 
    {
        // list_front list0
        struct list_elem *front;
        struct list_item *f_item;

        front = list_front(myList[idx]);
        if(front==NULL) return;
        else{
            f_item = list_entry(front, struct list_item, elem);
        }
        printf("%d\n", f_item->data);
    }

    else if (strcmp(arg[0], "list_back") == 0) 
    {
        // list_back list0
        struct list_elem *back;
        struct list_item *b_item;

        back = list_back(myList[idx]);
        if(back==NULL) return;
        else{
            b_item = list_entry(back, struct list_item, elem);
        }
        printf("%d\n", b_item->data);
    }

    else if (strcmp(arg[0], "list_max") == 0)
    {
        // list_max list0
        struct list_elem *max_elem = list_max(myList[idx], list_less_value, NULL);
        struct list_item *max_item = list_entry(max_elem, struct list_item, elem);
        printf("%d\n", max_item->data);
    }

    else if (strcmp(arg[0], "list_min") == 0)
    {
        // list_min list0
        struct list_elem *min_elem = list_min(myList[idx], list_less_value, NULL);
        struct list_item *min_item = list_entry(min_elem, struct list_item, elem);
        printf("%d\n", min_item->data);
    }

    else if (strcmp(arg[0], "list_empty") == 0)
    {
        // list_empty list0
        bool flag = list_empty(myList[idx]);
        printf("%s\n", (flag ? "true": "false"));
    }
    
    else if (strcmp(arg[0], "list_size") == 0)
    {
        // list_size list0
        size_t size = list_size(myList[idx]);
        printf("%zu\n", size);
    }

    else if (strcmp(arg[0], "list_remove") == 0)
    {
       // list_remove list0 0 
       int index = atoi(arg[2]);
       struct list_elem *e = list_head(myList[idx]);
       struct list_item *item;
       for(int i=0;i<=index; i++)
       {
           e = list_next(e);
       }
       item = list_entry(e, struct list_item, elem);
       list_remove(e);
       free(item);
    }

    else if (strcmp(arg[0], "list_reverse") == 0)
    {
        // list_reverse list0
        list_reverse(myList[idx]);
    }

    else if (strcmp(arg[0], "list_sort") == 0)
    {
        // list_sort list0
        list_sort(myList[idx], list_less_value, NULL);
    }

    else if (strcmp(arg[0], "list_splice") == 0)
    {
        // list_splice list0 2 list1 1 4
        char* list2 = arg[3];
        //printf("%s\n", list2);
        int idx2 = (list2[4]-'0');
        //printf("%d\n", idx2);
        int before = atoi(arg[2]);
        int start = atoi(arg[4]);
        int last = atoi(arg[5]);
        //printf("before %d, start %d, last %d\n", before, start, last);

        struct list_elem *e1, *e2, *e3;
        e1= list_head(myList[idx]);
        e2=e3=list_head(myList[idx2]);
        for(int i=0;i<=before;i++){
            e1=list_next(e1);
        }
        for (int i = 0; i<= start; i++){
            e2 = list_next(e2);
        }
        for(int i=0;i<=last;i++){
            e3 = list_next(e3);
        }

        list_splice(e1, e2, e3);
    }

    else if (strcmp(arg[0], "list_swap") == 0)
    {
        // list_swap list0 1 3
        int index1 = atoi(arg[2]);
        int index2 = atoi(arg[3]);
        struct list_elem* a=list_head(myList[idx]);
		struct list_elem* b=list_head(myList[idx]);
		int count1=0;
		while(count1<=index1){
			a=a->next;
			count1++;
		}
		int count2=0;
		while(count2<=index2){
			b=b->next;
			count2++;
		}

        list_swap(a, b);
    }

    else if (strcmp(arg[0], "list_unique") == 0)
    {
        // list_unique list0 list1
        // list unique list1
        if(arg[2]==NULL){
            list_unique(myList[idx], NULL , list_less_value, NULL);
            //printf("dump in unique\n");
        }
        else{
            char* ds_name2 = arg[2];
            int idx2 = ds_name2[4]-'0';
        
        list_unique(myList[idx], myList[idx2], list_less_value, NULL);
        } 
    }

    else if (strcmp(arg[0], "list_shuffle") == 0)
    {
        // list_shuffle list1
        //printf("size: %zu\n",list_size(myList[idx]));
        list_shuffle(myList[idx]);
    }
    else
    {
        //printf("None command\n");
        return;
    }    
}