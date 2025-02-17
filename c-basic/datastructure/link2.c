

struct list_head {
    struct list_head *next, *prev;
};


/*
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 */
#define container_of(ptr, type, member) ({               \
    const typeof(((type *)0)->member) * __mptr = (ptr);  \
    (type *)((char *)__mptr - m_offsetof(type, member)); \
})

/* refer to linux source code: include/linux/list.h */

/*
 * list_entry - get the struct for this entry
 * @ptr:        the &struct list_head pointer.
 * @type:       the type of the struct this is embedded in.
 * @member:     the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) container_of(ptr, type, member)

/*
 * list_for_each_entry  -       iterate over list of given type
 * @pos:        the type * to use as a loop cursor.
 * @head:       the head for your list.
 * @member:     the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)                   \
    for ((pos) = list_entry((head)->next, typeof(*(pos)), member);   \
         &(pos)->member != (head);                                 \
         (pos) = list_entry((pos)->member.next, typeof(*(pos)), member))

/*
 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:    the type * to use as a loop counter.
 * @n:      another type * to use as temporary storage
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define list_for_each_entry_safe(pos, n, head, member)           \
    for ((pos) = list_entry((head)->next, typeof(*(pos)), member),   \
         (n) = list_entry((pos)->member.next, typeof(*(pos)), member); \
         &(pos)->member != (head);                                 \
         (pos) = (n), (n) = list_entry((n)->member.next, typeof(*(n)), member))

static inline void list_add(struct list_head *new, struct list_head *head)
{
    head->next->prev = new;
    new->next = head->next;
    new->prev = head;
    head->next = new;
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void _list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

/* delete a list entry */
static inline void list_del(struct list_head *entry)
{
    _list_del(entry->prev, entry->next);
}

static inline int list_empty(struct list_head *head)
{
    return head->next == head;
}
