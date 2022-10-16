#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"

struct _list ins_list[10];
int list_cnt = 0;

struct _hash ins_hash[10];
int hash_cnt = 0;

struct _bitmap ins_bitmap[10];
int bitmap_cnt = 0;

char command[25];
char list_name[31], hash_name[31], bitmap_name[31];
char operator[10];
char name[31];char name1[50], name2[50];
int op_value;
int location, idx;

int tmp1, tmp2;
char tmp3[10];

int main(int argc, char* argv[]) {
	for (;;) {
		scanf("%s ", command);

		if (!strcmp(command,"create")) {
			
			scanf("%s", operator);
			if (!strncmp(operator,"list",4)){
				
				scanf("%s", list_name);

				ins_list[list_cnt].list_ = (struct list*)malloc(sizeof(struct list));
				list_init(ins_list[list_cnt].list_);
				strcpy(ins_list[list_cnt].name, list_name);

				++list_cnt;
			}
			
			else if (!strncmp(operator,"hash",4)) {
				
				scanf("%s", hash_name);

				ins_hash[hash_cnt].hash_ = (struct hash*)malloc(sizeof(struct hash));
				hash_init(ins_hash[hash_cnt].hash_, hash_hash, hash_less, NULL);
				strcpy(ins_hash[hash_cnt].name, hash_name);

				++hash_cnt;
			}
			else if (!strncmp(operator,"bitmap",6)) {
				int bitmap_size;
				scanf("%s %d", bitmap_name, &bitmap_size);

				ins_bitmap[bitmap_cnt].bitmap_ = bitmap_create(bitmap_size);
				strcpy(ins_bitmap[bitmap_cnt].name, bitmap_name);

				++bitmap_cnt;

			}
		}
		else if (!strcmp(command,"dumpdata")) {
			scanf("%s", name);
			if(!strncmp(name,"list",4)){
			for (int i = 0; i < list_cnt; ++i) {
				if (!strcmp(ins_list[i].name, name)) {
					struct list_elem* x;
					for (x = list_begin(ins_list[i].list_); x != list_end(ins_list[i].list_); x = list_next(x)) {
						printf("%d ", list_entry(x, struct list_item, elem)->data);
					}
					printf("\n");

					
					break;
				}
			}
			}
			
			else if(!strncmp(name,"hash",4)){
			for (int i = 0; i < hash_cnt; ++i) {
				if (!strcmp(ins_hash[i].name, name)) {
					struct hash_iterator* x = (struct hash_iterator*)malloc(sizeof(struct hash_iterator));
					hash_first(x, ins_hash[i].hash_);
					for (;;) {
						if (!hash_next(x)) break;
						printf("%d ", hash_entry(hash_cur(x), struct hash_item, elem)->data);
					}
					printf("\n");

					break;
				}
			}
			}
			else {
			for (int i = 0; i < bitmap_cnt; ++i) {
				if (!strcmp(ins_bitmap[i].name, name)) {
					int len = bitmap_size(ins_bitmap[i].bitmap_);
					for (int j = 0; j < len; ++j) {
						if (bitmap_test(ins_bitmap[i].bitmap_, j)) printf("1");
						else printf("0");
					}
					printf("\n");

					break;
				}
			}
			}
		}
		else if(!strcmp(command,"delete")){
			scanf("%s", name);
			int i; int j;
			for(i=0;i<10;i++){
				if(!strcmp(name,ins_list[i].name)){
					for(int j=i;j<9;j++){
						ins_list[j] = ins_list[j+1];
					}
					break;
				}
				else if(!strcmp(name,ins_hash[i].name)){
					hash_destroy(ins_hash[i].hash_, destructor);
					break;
				}
				else if(!strcmp(name,ins_bitmap[i].name)){
					bitmap_destroy(ins_bitmap[i].bitmap_);		
					break;
				}
			}
		}
		else if(!strcmp(command,"quit")){
			break;
		}
		else if (!strcmp(command, "list_push_front")) {
				scanf("%s %d", name, &op_value);
				for (int i = 0; i < list_cnt; ++i) {
					if (!strcmp(ins_list[i].name, name)) {
					idx = i;
					break;
					}
				}
				struct list_item* new = (struct list_item*)malloc(sizeof(struct list_item));
				new->data = op_value;
				list_push_front(ins_list[idx].list_, &new->elem);
			}
		else if (!strcmp(command, "list_push_back")) {
				scanf("%s %d", name, &op_value);
				for (int i = 0; i < list_cnt; ++i) {
					if (!strcmp(ins_list[i].name, name)) {
					idx = i;
					break;
					}
				}
				
				struct list_item* new = (struct list_item*)malloc(sizeof(struct list_item));
				new->data = op_value;
				list_push_back(ins_list[idx].list_, &new->elem);
			}
		else if (!strcmp(command, "list_front")) {
				scanf("%s", name);
				for (int i = 0; i < list_cnt; ++i) {
					if (!strcmp(ins_list[i].name, name)) {
					idx = i;
					break;
					}
				}
				struct list_elem* x = list_front(ins_list[idx].list_);
				struct list_item *temp = list_entry(x, struct list_item, elem);
				printf("%d\n", temp->data);
			}
		else if (!strcmp(command, "list_back")) {
				scanf("%s", name);
				for (int i = 0; i < list_cnt; ++i) {
					if (!strcmp(ins_list[i].name, name)) {
					idx = i;
					break;
					}
				}
				struct list_elem* x = list_back(ins_list[idx].list_);
				struct list_item *temp = list_entry(x, struct list_item, elem);
				printf("%d\n", temp->data);
			}
		
		else if (!strcmp(command, "list_pop_front")) {
				scanf("%s", name);
				for (int i = 0; i < list_cnt; ++i) {
					if (!strcmp(ins_list[i].name, name)) {
					idx = i;
					break;
					}
				}
				list_pop_front(ins_list[idx].list_);
			}
		else if (!strcmp(command, "list_pop_back")) {
				scanf("%s", name);
				for (int i = 0; i < list_cnt; ++i) {
					if (!strcmp(ins_list[i].name, name)) {
					idx = i;
					break;
					}
				}
				list_pop_back(ins_list[idx].list_);
			}
		else if (!strcmp(command, "list_insert")) {
				scanf("%s %d %d ", name, &location, &op_value);
				for (int i = 0; i < list_cnt; ++i) {
					if (!strcmp(ins_list[i].name, name)) {
					idx = i;
					break;
					}
				}
				
				struct list_elem* first = list_begin(ins_list[idx].list_);
				for (int i = 0; i < location; ++i) {
					first = list_next(first);
				}
				struct list_item* new = (struct list_item*)malloc(sizeof(struct list_item));
				new->data = op_value;
				list_insert(first, &new->elem);
			}
		else if (!strcmp(command, "list_insert_ordered")) {
				scanf("%s %d", name, &op_value);
				for (int i = 0; i < list_cnt; ++i) {
					if (!strcmp(ins_list[i].name, name)) {
					idx = i;
					break;
					}
				}

				struct list_item* new = (struct list_item*)malloc(sizeof(struct list_item));
				new->data = op_value;
				list_insert_ordered(ins_list[idx].list_, &new->elem, list_less, NULL);
			}
		
		else if (!strcmp(command, "list_max")) {
				scanf("%s", name);
				for (int i = 0; i < list_cnt; ++i) {
					if (!strcmp(ins_list[i].name, name)) {
					idx = i;
					break;
					}
				}
				printf("%d\n", list_entry(list_max(ins_list[idx].list_, list_less, NULL), struct list_item, elem)->data);
			}
		else if (!strcmp(command, "list_min")) {
				scanf("%s", name);
				for (int i = 0; i < list_cnt; ++i) {
					if (!strcmp(ins_list[i].name, name)) {
					idx = i;
					break;
					}
				}
				struct list_elem *e = list_min(ins_list[idx].list_, list_less, NULL);
				printf("%d\n", list_entry(e, struct list_item, elem)->data);
			}
		
		else if (!strcmp(command, "list_size")) {
				scanf("%s", name);
				for (int i = 0; i < list_cnt; ++i) {
					if (!strcmp(ins_list[i].name, name)) {
					idx = i;
					break;
					}
				}
				printf("%zu\n", list_size(ins_list[idx].list_));
			}
		else if (!strcmp(command, "list_empty")) {
				scanf("%s", name);
				for (int i = 0; i < list_cnt; ++i) {
					if (!strcmp(ins_list[i].name, name)) {
					idx = i;
					break;
					}
				}
				if (list_empty(ins_list[idx].list_)==true) printf("true\n");
				else printf("false\n");
			}
		else if (!strcmp(command, "list_sort")) {
				scanf("%s", name);
				for (int i = 0; i < list_cnt; ++i) {
					if (!strcmp(ins_list[i].name, name)) {
					idx = i;
					break;
					}
				}
				list_sort(ins_list[idx].list_, list_less, NULL);
			}
		else if (!strcmp(command, "list_reverse")) {
				scanf("%s", name);
				for (int i = 0; i < list_cnt; ++i) {
					if (!strcmp(ins_list[i].name, name)) {
					idx = i;
					break;
					}
				}
				list_reverse(ins_list[idx].list_);
			}
		
		else if (!strcmp(command, "list_remove")) {
				scanf("%s %d", name, &location);
				for (int i = 0; i < list_cnt; ++i) {
					if (!strcmp(ins_list[i].name, name)) {
					idx = i;
					break;
					}
				}
				

				struct list_elem* x = list_begin(ins_list[idx].list_);
				for (int i = 0; i < location; ++i) {
					x = list_next(x);
				}
				list_remove(x);
			}
		else if (!strcmp(command, "list_splice")) {
				int location, splice_start, splice_end;
				struct list_elem* dest, * start, * end;
				scanf("%s %d %s %d %d", name, &location, name1, &splice_start, &splice_end);
				for (int i = 0; i < list_cnt; ++i) {
					if (!strcmp(ins_list[i].name, name)) {
					idx = i;
					break;
					}
				}
				
				for (int i = 0; i < list_cnt; ++i) {
					if (!strcmp(ins_list[i].name, name1)) {
						
						dest = list_begin(ins_list[idx].list_);
						start = list_begin(ins_list[i].list_);
						end = list_begin(ins_list[i].list_);

						for (int i = 0; i < location; ++i) dest = list_next(dest);
						for (int i = 0; i < splice_start; ++i) start = list_next(start);
						for (int i = 0; i < splice_end; ++i) end = list_next(end);

						list_splice(dest, start, end);

						break;
					}
				}
			}
		else if (!strcmp(command, "list_swap")) {
				int swap_index1, swap_index2;
				scanf("%s %d %d", name, &swap_index1, &swap_index2);
				for (int i = 0; i < list_cnt; ++i) {
					if (!strcmp(ins_list[i].name, name)) {
					idx = i;
					break;
					}
				}

				struct list_elem* a = list_begin(ins_list[idx].list_);
				struct list_elem* * b = list_begin(ins_list[idx].list_);
				for (int i = 0; i < swap_index1; ++i) 
					a = list_next(a);
				for (int i = 0; i < swap_index2; ++i) 
					b = list_next(b);

				list_swap(a, b);
			}
		else if (!strcmp(command, "list_unique")) {
			char unique_command[100];
			int i, name1_idx;
			fgets(unique_command, 100, stdin);
			//name1 저장
			i = 0;
			while(unique_command[i] != ' ' &&  unique_command[i] != '\n'){
				name1[i] = unique_command[i];
				++i;
			}

			for (int i = 0; i < list_cnt; ++i) {
				if (!strcmp(ins_list[i].name, name1)) {
					name1_idx = i;
					break;
				}
			}

			if (unique_command[i] != ' ') {
				struct list* temp = (struct list*)malloc(sizeof(struct list));
				list_init(temp);
				list_unique(ins_list[name1_idx].list_, temp, list_less, NULL);
			}
			else {
				int j=0, name2_idx;
				//name 2 저장
				i++;
				while(unique_command[i] != ' ' &&  unique_command[i] != '\n'){
				name2[j] = unique_command[i];
				++i;
				j++;
			}
				for (j = 0; j < list_cnt; ++j) {
					if (!strcmp(ins_list[j].name, name2)) {
						name2_idx = j;
					}
				}

				list_unique(ins_list[name1_idx].list_, ins_list[name2_idx].list_, list_less, NULL);
			}
		}
		
//hash
		else if (!strcmp(command, "hash_insert")) {
				int hash_value;
				scanf("%s %d", name, &hash_value);
				for (int i = 0; i < hash_cnt; ++i) {
					if (!strcmp(ins_hash[i].name, name)) {
						idx = i;
						break;
					}
				}
							
				struct  hash_item* new = (struct hash_item*)malloc(sizeof(struct hash_item));
				new->data = hash_value;
				hash_insert(ins_hash[idx].hash_, &new->elem);
			}
		else if (!strcmp(command, "hash_clear")) {
				scanf("%s", name);
				for (int i = 0; i < hash_cnt; ++i) {
					if (!strcmp(ins_hash[i].name, name)) {
						idx = i;
						break;
					}
				}
				hash_clear(ins_hash[idx].hash_, NULL);
			}
		else if (!strcmp(command, "hash_size")) {
				scanf("%s", name);
				for (int i = 0; i < hash_cnt; ++i) {
					if (!strcmp(ins_hash[i].name, name)) {
						idx = i;
						break;
					}
				}
				printf("%zu\n", hash_size(ins_hash[idx].hash_));
			}
		else if (!strcmp(command, "hash_apply")) {
				scanf("%s %s", name, name1);
				for (int i = 0; i < hash_cnt; ++i) {
					if (!strcmp(ins_hash[i].name, name)) {
						idx = i;
						break;
					}
				}

				if (!strcmp(name1, "square")) {
					hash_apply(ins_hash[idx].hash_, hash_square);
				}
				else if (!strcmp(name1, "triple")) {
					hash_apply(ins_hash[idx].hash_, hash_triple);
				}
			}
		else if (!strcmp(command, "hash_empty")) {
				scanf("%s", name);
				for (int i = 0; i < hash_cnt; ++i) {
					if (!strcmp(ins_hash[i].name, name)) {
						idx = i;
						break;
					}
				}
				if (!hash_empty(ins_hash[idx].hash_)) 
					printf("false\n");
				else printf("true\n");
			}
		else if (!strcmp(command, "hash_delete")) {
				int hash_value;
				scanf("%s %d", name, &hash_value);
				for (int i = 0; i < hash_cnt; ++i) {
					if (!strcmp(ins_hash[i].name, name)) {
						idx = i;
						break;
					}
				}
				struct  hash_item* new = (struct hash_item*)malloc(sizeof(struct hash_item));
				new->data = hash_value;
				hash_delete(ins_hash[idx].hash_, &new->elem);
			}
		
		else if (!strcmp(command, "hash_find")) {
				int hash_value;
				scanf("%s %d", name, &hash_value);
				for (int i = 0; i < hash_cnt; ++i) {
					if (!strcmp(ins_hash[i].name, name)) {
						idx = i;
						break;
					}
				}
				struct  hash_item* new = (struct hash_item*)malloc(sizeof(struct hash_item));
				new->data = hash_value;
				struct hash_elem *hash_find_tmp= hash_find(ins_hash[idx].hash_, &new->elem);
				if (hash_find_tmp != NULL)
					printf("%d\n", new->data);
			}
		else if (!strcmp(command, "hash_replace")) {
				int hash_value;
				scanf("%s %d", name, &hash_value);
				for (int i = 0; i < hash_cnt; ++i) {
					if (!strcmp(ins_hash[i].name, name)) {
						idx = i;
						break;
					}
				}
				struct  hash_item* new = (struct hash_item*)malloc(sizeof(struct hash_item));
				new->data = hash_value;
				hash_replace(ins_hash[idx].hash_, &new->elem);
			}
	
//bitmap
		else if (!strcmp(command, "bitmap_mark")) {
				scanf("%s %d", name, &location);
				for (int i = 0; i < bitmap_cnt; ++i) {
					if (!strcmp(ins_bitmap[i].name, name)) {
						idx = i;
						break;
					}
				}
	
				bitmap_mark(ins_bitmap[idx].bitmap_, location);
			}
		else if (!strcmp(command, "bitmap_all")) {
				
				scanf("%s %d %d", name,  &tmp1, &tmp2);
				for (int i = 0; i < bitmap_cnt; ++i) {
					if (!strcmp(ins_bitmap[i].name, name)) {
						idx = i;
						break;
					}
				}
	
				if (bitmap_all(ins_bitmap[idx].bitmap_, tmp1, tmp2)) 
					printf("true\n");
				else 
					printf("false\n");
			}
		else if (!strcmp(command, "bitmap_any")) {
				
				scanf("%s %d %d", name,  &tmp1, &tmp2);
				for (int i = 0; i < bitmap_cnt; ++i) {
					if (!strcmp(ins_bitmap[i].name, name)) {
						idx = i;
						break;
					}
				}
				if (bitmap_any(ins_bitmap[idx].bitmap_, tmp1, tmp2)) 
					printf("true\n");
				else 
					printf("false\n");
			}
		else if (!strcmp(command, "bitmap_none")) {
				
				scanf("%s %d %d", name, &tmp1, &tmp2);
				for (int i = 0; i < bitmap_cnt; ++i) {
					if (!strcmp(ins_bitmap[i].name, name)) {
						idx = i;
						break;
					}
				}
				

				if (bitmap_none(ins_bitmap[idx].bitmap_, tmp1, tmp2)) 
					printf("true\n");
				else
					printf("false\n");
			}
		else if (!strcmp(command, "bitmap_contains")) {
				
				scanf("%s %d %d %s", name, &tmp1, &tmp2, tmp3);
				for (int i = 0; i < bitmap_cnt; ++i) {
					if (!strcmp(ins_bitmap[i].name, name)) {
						idx = i;
						break;
					}
				}
				

				if (strcmp(tmp3, "true")) {
					if (bitmap_contains(ins_bitmap[idx].bitmap_, tmp1, tmp2, false)) 
						printf("true\n");
					else 
						printf("false\n");
				}
				if (strcmp(tmp3, "false")) {
					if (bitmap_contains(ins_bitmap[idx].bitmap_, tmp1, tmp2, true)) 
						printf("true\n");
					else 
						printf("false\n");
				}
			}
		else if (!strcmp(command, "bitmap_count")) {
				
				scanf("%s %d %d %s", name, &tmp1, &tmp2, tmp3);
				for (int i = 0; i < bitmap_cnt; ++i) {
					if (!strcmp(ins_bitmap[i].name, name)) {
						idx = i;
						break;
					}
				}

				if (!strcmp(tmp3, "true")) 
					printf("%zu\n", bitmap_count(ins_bitmap[idx].bitmap_, tmp1, tmp2, true));
				if (!strcmp(tmp3, "false")) 
					printf("%zu\n", bitmap_count(ins_bitmap[idx].bitmap_, tmp1, tmp2, false));
			}
		else if (!strcmp(command, "bitmap_dump")) {
				scanf("%s", name);
				for (int i = 0; i < bitmap_cnt; ++i) {
					if (!strcmp(ins_bitmap[i].name, name)) {
						idx = i;
						break;
					}
				}
				bitmap_dump(ins_bitmap[idx].bitmap_);
			}
		else if (!strcmp(command, "bitmap_expand")) {
				
				scanf("%s %d", name, &tmp1);
				for (int i = 0; i < bitmap_cnt; ++i) {
					if (!strcmp(ins_bitmap[i].name, name)) {
						idx = i;
						break;
					}
				}
		
				ins_bitmap[idx].bitmap_ = bitmap_expand(ins_bitmap[idx].bitmap_, tmp1);
			}
		else if (!strcmp(command, "bitmap_flip")) {
				
				scanf("%s %d", name, &tmp1);
				for (int i = 0; i < bitmap_cnt; ++i) {
					if (!strcmp(ins_bitmap[i].name, name)) {
						idx = i;
						break;
					}
				}
				
				bitmap_flip(ins_bitmap[idx].bitmap_, tmp1);
			}
		
		else if (!strcmp(command, "bitmap_reset")) {
				
				scanf("%s %d", name, &tmp1);
				for (int i = 0; i < bitmap_cnt; ++i) {
					if (!strcmp(ins_bitmap[i].name, name)) {
						idx = i;
						break;
					}
				}
				

				bitmap_reset(ins_bitmap[idx].bitmap_, tmp1);
			}
			//////////////scan 부분 수정 필요
		else if (!strcmp(command, "bitmap_scan_and_flip")) {
				scanf("%s %d %d %s", name, &tmp1, &tmp2, tmp3);
				for (int i = 0; i < bitmap_cnt; ++i) {
					if (!strcmp(ins_bitmap[i].name, name)) {
						idx = i;
						break;
					}
				}
				
				if (!strcmp(tmp3, "true")) 
					printf("%zu\n", bitmap_scan_and_flip(ins_bitmap[idx].bitmap_, tmp1, tmp2, true));
				if (!strcmp(tmp3, "false")) 
					printf("%zu\n", bitmap_scan_and_flip(ins_bitmap[idx].bitmap_, tmp1, tmp2, false));
			}
		else if (!strcmp(command, "bitmap_scan")) {
				scanf("%s %d %d %s", name, &tmp1, &tmp2, tmp3);
				for (int i = 0; i < bitmap_cnt; ++i) {
					if (!strcmp(ins_bitmap[i].name, name)) {
						idx = i;
						break;
					}
				}
				
				if (!strcmp(tmp3, "true")) printf("%zu\n", bitmap_scan(ins_bitmap[idx].bitmap_, tmp1, tmp2, true));
				if (!strcmp(tmp3, "false")) printf("%zu\n", bitmap_scan(ins_bitmap[idx].bitmap_, tmp1, tmp2, false));
			}
			////////////////////////////////
		else if (!strcmp(command, "bitmap_set_all")) {
				
				scanf("%s %s", name, tmp3);
				for (int i = 0; i < bitmap_cnt; ++i) {
					if (!strcmp(ins_bitmap[i].name, name)) {
						idx = i;
						break;
					}
				}
				

				if (!strcmp(tmp3, "true")) bitmap_set_all(ins_bitmap[idx].bitmap_, true);
				if (!strcmp(tmp3, "false")) bitmap_set_all(ins_bitmap[idx].bitmap_, false);
			}
		else if (!strcmp(command, "bitmap_set")) {
				
				scanf("%s %d %s", name,  &tmp1, tmp3);
				for (int i = 0; i < bitmap_cnt; ++i) {
					if (!strcmp(ins_bitmap[i].name, name)) {
						idx = i;
						break;
					}
				}
			

				if (!strcmp(tmp3, "true")) bitmap_set(ins_bitmap[idx].bitmap_, tmp1, true);
				if (!strcmp(tmp3, "false")) bitmap_set(ins_bitmap[idx].bitmap_, tmp1, false);
			}
		else if (!strcmp(command, "bitmap_set_multiple")) {
				
				scanf("%s %d %d %s", name, &tmp1, &tmp2, tmp3);
				for (int i = 0; i < bitmap_cnt; ++i) {
					if (!strcmp(ins_bitmap[i].name, name)) {
						idx = i;
						break;
					}
				}
				

				if (!strcmp(tmp3, "true")) bitmap_set_multiple(ins_bitmap[idx].bitmap_, tmp1, tmp2, true);
				if (!strcmp(tmp3, "false")) bitmap_set_multiple(ins_bitmap[idx].bitmap_, tmp1, tmp2, false);
			}
		else if (!strcmp(command, "bitmap_size")) {
				scanf("%s", name);
				for (int i = 0; i < bitmap_cnt; ++i) {
					if (!strcmp(ins_bitmap[i].name, name)) {
						idx = i;
						break;
					}
				}
				printf("%zu\n", bitmap_size(ins_bitmap[idx].bitmap_));
			}
		else if (!strcmp(command, "bitmap_test")) {
				
				scanf("%s %d", name, &tmp1);
				for (int i = 0; i < bitmap_cnt; ++i) {
					if (!strcmp(ins_bitmap[i].name, name)) {
						idx = i;
						break;
					}
				}

				if (bitmap_test(ins_bitmap[idx].bitmap_, tmp1)) 
					printf("true\n");
				else 
					printf("false\n");
			}
		}
	

	return 0;
}

//list
void list_swap(struct list_elem* a, struct list_elem* b) {
	struct list_item *i1 = list_entry(a, struct list_item, elem);
	struct list_item *i2 = list_entry(b, struct list_item, elem);
	
	int tmp = i1->data;
	i1->data = i2->data;
	i2->data = tmp;
}

bool list_less(const struct list_elem* a, const struct list_elem* b, void* aux) {
	struct list_item *i1, *i2;

	i1 = list_entry(a, struct list_item, elem);
	i2 = list_entry(b, struct list_item, elem);

	if(i1->data < i2->data) 
        return true;
	else 
        return false;
}

void list_shuffle(struct list* list) {
	srand((unsigned int)time(NULL));

	struct list_elem* a, * b;

	for (int i = 0; i < list_size(list); ++i) {
		int times = rand() % list_size(list);
		a = list_begin(list);
		for (int j = 0; j < times; ++j) {
			a = list_next(a);
		}

		times = rand() % list_size(list);
		b = list_begin(list);
		for (int j = 0; j < times; ++j) {
			b = list_next(b);
		}

		list_swap(a, b);
	}
}


//hash
unsigned hash_hash(const struct hash_elem* e, void* aux) {
	return hash_int(hash_entry(e, struct hash_item, elem)->data);
}

bool hash_less(const struct hash_elem* a, const struct hash_elem* b, void* aux) {
	int a_data = hash_entry(a, struct hash_item, elem)->data,
		b_data = hash_entry(b, struct hash_item, elem)->data;

	if (a_data < b_data) return true;
	else return false;
}

void hash_square(struct hash_elem* e, void* aux) {
	int x = hash_entry(e, struct hash_item, elem)->data;
	hash_entry(e, struct hash_item, elem)->data = x * x;
}
void hash_triple(struct hash_elem* e, void* aux) {
	int x = hash_entry(e, struct hash_item, elem)->data;
	hash_entry(e, struct hash_item, elem)->data = x * x * x;
}


unsigned hash_int_2(int i) {

    return i % 10;
}

struct bitmap* bitmap_expand(struct bitmap* bitmap, int size) {
	struct bitmap* x;
	
	x = bitmap_create(size + bitmap_size(bitmap));

	for (int i = 0; i < bitmap_size(bitmap); ++i) {
		if (bitmap_test(bitmap, i)) bitmap_mark(x, i);
	}

	return x;
}

void destructor(struct hash_elem *e, void *aux){
	struct hash_item *temp;
	temp = hash_entry(e, struct hash_item, elem);
	free(temp);
}