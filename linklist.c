typedef struct link_list {
 	ELEMTYPE elem;
	struct link_list *next;
} link_list;

void init_hlink_list(link_list *list)
{
	list = (struct link_list *)malloc(sizeof(struct link_list));
	list->next = NULL;
}

void init_link_list(link_list *list)
{
	list->next = NULL;
}

void desstroy_link_list(link_list *list)
{
	struct link_list *currp;
	struct link_list *tmp;

	currp = list;
	while (currp->next) {
		tmp = currp->next;
		currp->next = tmp->next;
		free(tmp);
	}
	free(currp);
}

int insert_elem_into_list_from_head(link_list *list, ELEMTYPE element)
{
	struct link_list *node;

	node = (struct link_list *)malloc(sizeof(struct link_list));
	node->elem = element;

	node->next = list->next;
	list->next = node;

	return 0;
}

int insert_elem_into_list_from_tail(link_list *list, ELEMTYPE element)
{
	struct link_list *node;
	struct link_list *currp;

	currp = list;
	node = (struct link_list *)malloc(sizeof(struct link_list));
	node->elem = element;

	while (currp->next)
		currp = currp->next;
	node->next = currp->next;
	currp->next = node;

	return 0;
}

int delete_elem_from_list(link_list *list, ELEMTPE element)
{
	struct link_list *currp;

	currp = list;
	while (currp->next) {
		if (currp->next->elem == element) {
			struct link_list tmp = currp->next;
			currp->next = tmp->next;
			free(tmp);
			return 0;
		}
		currp = currp->next;
	}

	return -1;
}

link_list *find_elem_from_list(link_list *list, ELEMTYPE element)
{
		struct link_list *currp;

	currp = list;
	while (currp->next) {
		if (currp->next->elem == element)
			return currp->next;
		currp = currp->next;
	}

	return NULL;
}
