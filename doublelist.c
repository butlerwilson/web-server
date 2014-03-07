/*
Mode: double link list
          ------<-----
prev --<--\          \
         \\   -->--  \
         \\   \    \ \  ---->---- next
	----  \   ----  \       \
	\  \  \   \  \  \
	----  \   ----  \
	\  \  \   \  \  \
	----  \   ----  \
         \    \     \   \
         -->---     -->-
*/

typedef struct double_list {
	ELEMTYPE elem;
	struct double_list *prev;
	struct double_list *next;
} double_list;

void init_double_list(double_list *dlist)
{
	dlist = (struct double_list *)malloc(sizeof(double_list));
	dlist->prev = dlist->next = NULL;
}

void destroy_double_list(double_list *dlist)
{
	struct double_list *currp;
	struct double_list *tmp;

	currp = dlist;
	while (currp->next) {
		tmp = currp->next;
		currp->next = tmp->next;
		free(tmp);
	}
	free(currp);
}
