#include "btree.h"
#include <stdlib.h>
#include <stdio.h>

struct btree
{
  int ordem;
  int n; /* Numero de chaves */
  int *k; /* k[ordem] */
  BTree **p; /* *p[N+1] */
};


static int isleaf (BTree* a)
{
  return (a->p[0] == NULL);
}

static int findpos (BTree* a, int x, int* pos)
{
  for( (*pos) = 0; (*pos) < a->n; ++(*pos))
    if (x == a->k[*pos])
      return 1;
    else if (x < a->k[*pos])
      break ;
  return 0;
}


BTree* bt_search(BTree* a, int x, int* pos)
{
  int found = findpos(a,x,pos);
  if (found)
    return a;
  else if (isleaf(a))
    return NULL;
  else
    return bt_search(a->p[*pos], x, pos);
}

int bt_key (BTree* a, int pos)
{
  return a->k[pos];
}

BTree* bt_create (int ordem)
{
  BTree* a = (BTree*) malloc(sizeof(BTree));

  a->ordem = ordem;
  a->k = (int*) malloc(sizeof(int)*ordem);
  a->p = (BTree**) malloc(sizeof(BTree*)*(ordem+1));
  a->n = 0;
  a->p[0] = NULL;
  return a;
}


void bt_destroy (BTree* a)
{
  int i;
  if (!isleaf(a))
  {
    for (i = 0; i <= a->n; ++i)
      bt_destroy(a->p[i]);
  }
  free(a);
}


static int overflow (BTree* a)
{
  return (a->n == a->ordem);
}

static BTree* split (BTree* a, int* m)
{
  int i;
  BTree* b = bt_create(a->ordem);

  int q;

  if(a->ordem%2 == 0)
    q = (a->n/2)-1;
  else
    q = a->n/2;

  b->n = a->n - q - 1;
  a->n = q;
  *m = a->k[q];
  b->p[0] = a->p[q+1];

  for (i=0; i < b->n; ++i)
  {
    b -> k[i] = a->k[q + 1 + i] ;
    b->p[i+1] = a->p[q+1+i+1];
  }
  return b;
}


static void addright (BTree* a , int pos, int k, BTree* p)
{
  int j;
  for (j = a->n; j>pos; --j)
  {
    a->k[j] = a->k[j-1];
    a -> p [j + 1] = a->p[j] ;
  }
  a->k[pos] = k;
  a->p[pos+1] = p;
  a->n++;
}

static void addleft (BTree* a, int pos, int k, BTree* p) {
  int j;
  for (j=a->n; j>pos; --j)
  {
    a->k[j] = a->k[j-1];
    a -> p [j + 1] = a -> p [j];
  }
  a->p[pos+1] = a->p[pos];
  a->k[pos] = k;
  a->p[pos] = p;
  a->n++;
}


static void insert (BTree* a, int x)
{
  int pos;
  findpos(a,x,&pos); /* insere mesmo se ja existir */
  if (isleaf(a))
  {
    addright(a,pos,x,NULL);
  }
  else
  {
    insert( a->p[pos], x );
    if (overflow(a->p[pos]))
    {
      int m;
      BTree* b = split(a->p[pos],&m);
      addright(a,pos,m,b);
    }
  }
}


BTree* bt_insert (BTree* a, int x)
{

  insert(a,x);
  if(overflow(a))
  {
    int m;
    BTree* b = split(a,&m);
    BTree* r = bt_create(a->ordem);
    r->k[0] = m;
    r->p[0] = a;
    r->p[1] = b;
    r->n = 1;
    return r;
  }
  return a;
}






#define INDENT(x) for (int j=0; j<x; ++j) printf(" ");
void bt_print (BTree* a, int indent)
{
  int i;
  if (isleaf(a))
  {
    for (i = a->n - 1; i >= 0; --i)
    {
      INDENT(indent);
      printf( "%d \n" , a->k[i]);
    }
  }
  else
  {
    bt_print(a->p[a->n],indent+2);
    for (i = a->n - 1; i>=0; --i)
    {
      INDENT(indent);
      printf("%d \n" , a->k[i] ) ;
      bt_print(a->p[i],indent+2);
    }
  }
}


unsigned long long bt_size_memory(BTree* a)
{
  int i;
  if(!isleaf(a))
  {
    long long size_no = 0;
    for (i = 0; i <= a->n; ++i)
    {
       size_no += bt_size_memory(a->p[i]);
    }
    return size_no;
  }

  return sizeof(a) + (sizeof(int) * a->ordem) + (sizeof(BTree*) * (a->ordem+1));
}




static BTree* findmax (BTree* a)
{
  while (!isleaf(a))
    a = a->p[a->n];
  return a;
}



static void delleft (BTree* a, int pos)
{
  int i;
  for (i=pos; i<a->n-1;++i)
  {
    a->k[i] = a->k[i+1];
    a->p[i] = a->p[i+1];
  }
  a->p[i] = a->p[i+1];
  a->n--;
}
static void delright (BTree* a, int pos)
{
  int i;
  for (i=pos; i<a->n-1;++i)
  {
    a->k[i] = a->k[i+1];
    a->p[i+1] = a->p[i+2];
  }
  a->n--;
}


static int underflow (BTree* a)
{
  return (a->n < a->ordem/2);
}




static void rearrange (BTree* a, int pos)
{
  if (pos==a->n) /* convert child index into key index */
    pos--;
  BTree* left = a->p[pos];
  BTree* right = a->p[pos+1];
  if (left->n + right->n + 1 >= a->ordem)
  { /* redistribute */
    while (underflow( left ))
    {
      addright(left, left->n, a->k[pos], right->p[0]);
      a->k[pos] = right->k[0];
      delleft(right, 0);
    }
    while (underflow(right))
    {
      addleft(right, 0, a->k[pos],left->p[left->n]);
      a->k[pos] = left->k[left->n-1];
      delright(left, left->n-1);
    }
  }
  else
  {
    /* concatenate */
    addright(left ,left->n,a->k[pos],right->p[0]);

    for (int i=0; i<right->n; ++i)
      addright(left, left->n, right->k[i], right->p[i+1]);
    free(right);
    delright(a, pos);
  }
}



static int rem (BTree* a, int x)
{
  if (isleaf(a))
  {
    int pos;
    if (findpos(a,x,&pos))
      delleft(a,pos); /* or delright */
    else
      return 0;
  }
  else {
    int pos;
    if (findpos(a,x,&pos))
    {
      BTree* l = findmax(a->p[pos]); a->k[pos] = l->k[l->n-1];
      l->k[l->n-1] = x;
    }
    if (!rem(a->p[pos],x))
      return 0;
    if (underflow(a->p[pos]))
      rearrange(a,pos);
  }
  return 1;
}


BTree* bt_remove (BTree* a, int x)
{
  if (rem(a,x)) {
    if (a->n == 0 & a->p[0] != NULL)
    { /* caso da raiz com um filho */
      BTree* t = a;
      a = a->p[0];
      free(t);
    }
  }
  return a;
}
