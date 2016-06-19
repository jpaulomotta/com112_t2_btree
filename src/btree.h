#ifndef BTREEH
#define BTREEH

typedef struct btree BTree;

BTree* bt_create (int ordem);
void bt_destroy (BTree* a);
BTree* bt_search (BTree* a, int x, int* pos);
int bt_key (BTree* a, int pos);
BTree* bt_insert (BTree* a, int x);
BTree* bt_remove (BTree* a, int x);
void bt_print (BTree* a, int indent);

unsigned long long bt_size_memory(BTree* a);

BTree* bt_remove (BTree* a, int x);

#endif
