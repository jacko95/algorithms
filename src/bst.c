/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/*
 * Copyright 2015 University of Piemonte Orientale, Computer Science Institute
 *
 * This file is part of UPOalglib.
 *
 * UPOalglib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UPOalglib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UPOalglib.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bst_private.h"
#include <stdio.h>
#include <stdlib.h>

//Per eseguirlo senza librerie del prof 
/*
typedef int(*upo_bst_comparator_t)(const void*, const void*); //Si dichiara un puntatore alla funzione upo_bst_comparator_t che riceve due puntatori void che non possono essere modificati (ma copiati sì)

typedef void (*upo_bst_visitor_t)(void*, void*, void*);//??????

typedef struct upo_bst_node_s upo_bst_node_t;
struct upo_bst_node_s{ //Struttura che definisce un nodo
	void* key; //Puntatore alla chiave
    void* value; //Puntatore al valore associato alla chiave
	upo_bst_node_t* left; //Puntatore al figlio sinistro
	upo_bst_node_t* right; //Puntatore al figlio destro
};

struct upo_bst_s{ //Struttura che definisce un albero
    upo_bst_node_t* root; //Puntatore alla radice dell' albero
    upo_bst_comparator_t key_cmp; //Puntatore alla funzione di comparazione
};
typedef struct upo_bst_s* upo_bst_t;

struct upo_bst_key_list_node_s {
    void *key; //Puntatore alla chiave????
    struct upo_bst_key_list_node_s *next; //Puntatore al nodo successivo nella lista???
};
typedef struct upo_bst_key_list_node_s upo_bst_key_list_node_t;
typedef upo_bst_key_list_node_t* upo_bst_key_list_t;
*/
upo_bst_node_t* put_impl( upo_bst_node_t *, void *, void *, void* , upo_bst_comparator_t );
upo_bst_node_t* node_create(void *, void *);
upo_bst_node_t* get_impl(upo_bst_node_t* ,const void*, upo_bst_comparator_t);
upo_bst_node_t* delete_impl(upo_bst_node_t* ,const void *, upo_bst_comparator_t, int);
upo_bst_node_t* delete_C1(upo_bst_node_t*, int);
upo_bst_node_t* delete_C2(upo_bst_comparator_t, upo_bst_node_t*, int);
upo_bst_node_t* max_impl(upo_bst_node_t*);
void upo_bst_visita_nodi(upo_bst_key_list_t, upo_bst_node_t*);
upo_bst_key_list_t upo_bst_add_to_tail(upo_bst_key_list_t, upo_bst_node_t*);
upo_bst_key_list_t upo_bst_keys(upo_bst_t);
void upo_bst_keys_range_impl(upo_bst_key_list_t*, const void*, const void*, upo_bst_comparator_t);
void upo_bst_clear(upo_bst_t tree, int destroy_data);
upo_bst_comparator_t upo_bst_get_comparator(const upo_bst_t tree);


/**** EXERCISE #1 - BEGIN of FUNDAMENTAL OPERATIONS ****/


upo_bst_t upo_bst_create(upo_bst_comparator_t key_cmp)
{
    upo_bst_t tree = malloc(sizeof(struct upo_bst_s));
    if (tree == NULL)
    {
        perror("Unable to create a binary search tree");
        abort();
    }

    tree->root = NULL;
    tree->key_cmp = key_cmp;

    return tree;
}

void upo_bst_destroy(upo_bst_t tree, int destroy_data)
{
    if (tree != NULL)
    {
        upo_bst_clear(tree, destroy_data);
        free(tree);
    }
}

void upo_bst_clear_impl(upo_bst_node_t *node, int destroy_data)
{
    if (node != NULL)
    {
        upo_bst_clear_impl(node->left, destroy_data);
        upo_bst_clear_impl(node->right, destroy_data);

        if (destroy_data)
        {
            free(node->key);
            free(node->value);
        }

        free(node);
    }
}

void upo_bst_clear(upo_bst_t tree, int destroy_data)
{
    if (tree != NULL)
    {
        upo_bst_clear_impl(tree->root, destroy_data);
        tree->root = NULL;
    }
}


upo_bst_node_t* node_create(void *k, void *v){
    upo_bst_node_t *node = malloc(sizeof(upo_bst_node_t));
    node->key=k;
    node->value=v;
    node->left=NULL;
    node->right=NULL;
    return node;
}

upo_bst_node_t* put_impl(upo_bst_node_t *n, void *k, void *v, void* vold, upo_bst_comparator_t key_cmp)
{
    vold = NULL;
    if(n==NULL){
        return node_create(k, v);
    }
    else if(key_cmp(k, n->key)==-1)
     n->left = put_impl(n->left, k, v, vold, key_cmp);   
    
    else if(key_cmp(k, n->key)==1)
        n->right = put_impl( n->right, k, v, vold, key_cmp);
    else{
        vold = n->value;
        n->value = v;
    }
    return n;
}


void* upo_bst_put(upo_bst_t tree, void *key, void *value)
{
    void *vold = NULL;
    tree->root = put_impl(tree->root, key, value, vold, tree->key_cmp);
    return vold;
}

upo_bst_node_t* insert_impl(upo_bst_node_t* n, void* k, void* v, upo_bst_comparator_t key_cmp){
    if(n==NULL)
        return node_create(k, v);
    else if(key_cmp(k, n->key)==-1)
        n->left = insert_impl(n->left, k, v, key_cmp);
    else if(key_cmp(k, n->key)==1)
        n->right = insert_impl(n->right, k, v, key_cmp);
    else
    {}
    return n;
}

void upo_bst_insert(upo_bst_t tree, void *key, void *value)
{
  tree->root = insert_impl(tree->root, key, value, tree->key_cmp);
}

upo_bst_node_t* get_impl(upo_bst_node_t* n,const void *k, upo_bst_comparator_t key_cmp){
    if(n==NULL)
        return NULL;
    if(key_cmp(k, n->key)==-1)
        return get_impl(n->left, k, key_cmp);
    else if(key_cmp(k, n->key)==1)
        return get_impl(n->right, k, key_cmp);
    else
        return n;
}

void* upo_bst_get(const upo_bst_t tree, const void *key)
{
  upo_bst_node_t *n = get_impl(tree->root, key, tree->key_cmp);
  if(n!=NULL)
      return n->value; 
  return NULL;
}

int upo_bst_contains(const upo_bst_t tree, const void *key)
{
  if(get_impl(tree->root, key, tree->key_cmp) != NULL)
      return 1;
  else
      return 0;
}

upo_bst_node_t* delete_C1(upo_bst_node_t* n, int destroy_data){
    upo_bst_node_t *x = n;
    if(n->left!=NULL)
        n=n->left;
    else
        n=n->right;
    if(destroy_data){
        free(x->value);
        free(x->key);
    }
    free(x);
    return n;
}

upo_bst_node_t* delete_C2(upo_bst_comparator_t key_cmp, upo_bst_node_t* n, int destroy_data){
    upo_bst_node_t *m;
    m=max_impl(n->left);
    n->key=m->key;
    n->value=m->value;
    n->left= delete_impl(n->left, m->key, key_cmp, destroy_data);
    return n;
}

upo_bst_node_t* max_impl(upo_bst_node_t* n){  //PUNTATORE A NODO
    if(n==NULL)
        return NULL;
    else if(n->right!=NULL)
        return max_impl(n->right);
    else 
        return n;
}

upo_bst_node_t* delete_impl( upo_bst_node_t* n,const void *k, upo_bst_comparator_t key_cmp, int destroy_data){
    if(n==NULL)
        return NULL;
    if(key_cmp(k, n->key)==-1)
        n->left=delete_impl( n->left, k, key_cmp, destroy_data);
    else if(key_cmp(k, n->key)==1)
        n->right=delete_impl( n->right, k, key_cmp, destroy_data);
    else if(n->left!=NULL && n->right!=NULL)
        n=delete_C2(key_cmp, n, destroy_data);
    else
        n=delete_C1(n, destroy_data);
    return n;
}

void upo_bst_delete(upo_bst_t tree, const void *key, int destroy_data)
{
  tree->root=delete_impl(tree->root, key, upo_bst_get_comparator(tree), destroy_data);
}

size_t size_impl(upo_bst_node_t* n){
 if(n==NULL)
     return 0;
 return 1 + size_impl(n->left) + size_impl(n->right);
}

size_t upo_bst_size(const upo_bst_t tree)
{
    if(tree==NULL)
        return 0;
    return size_impl(tree->root);
}

int is_leaf_impl(upo_bst_node_t* n){
    if(n->left == NULL && n->right == NULL)
        return 1;
    else
        return 0;
}

int height_impl(upo_bst_node_t* n, upo_bst_comparator_t key_cmp){
    if(n==NULL || is_leaf_impl(n))
        return 0;
    int left=0, right=0;
    left =  height_impl(n->left, key_cmp);
    right = height_impl(n->right, key_cmp);
    if(left>right)
        return 1+left;
    else
        return 1+right;
}

size_t upo_bst_height(const upo_bst_t tree)
{
   return height_impl(tree->root, tree->key_cmp);
}

void traverse_in_order_impl(upo_bst_node_t* n, upo_bst_visitor_t v, void* visit_arg){
 if(n != NULL){
     traverse_in_order_impl(n->left, v, visit_arg);
     v(n->key, n->value, visit_arg);
     traverse_in_order_impl(n->right, v, visit_arg);
 }
}

void upo_bst_traverse_in_order(const upo_bst_t tree, upo_bst_visitor_t visit, void *visit_arg)
{
   traverse_in_order_impl(tree->root, visit, visit_arg);
}

int upo_bst_is_empty(const upo_bst_t tree)
{
   if(tree==NULL || tree->root==NULL)
       return 1;
   else
       return 0;
}


/**** EXERCISE #1 - END of FUNDAMENTAL OPERATIONS ****/


/**** EXERCISE #2 - BEGIN of EXTRA OPERATIONS ****/

upo_bst_node_t* min_impl(upo_bst_node_t* n){
    if(n==NULL)
        return NULL;
    else if(n->left==NULL)
        return n;
    else
        return min_impl(n->left);
}

void* upo_bst_min(const upo_bst_t tree)
{
    upo_bst_node_t* n=min_impl(tree->root);
    if(n==NULL)
        return NULL;
    return n->key;
}

upo_bst_node_t* max_impl2(upo_bst_node_t* n){
 if(n==NULL)
     return NULL;
 else if(n->right==NULL)
     return n;
 else
     return max_impl(n->right);
}

void* upo_bst_max(const upo_bst_t tree)
{
    upo_bst_node_t* n = max_impl2(tree->root);
    if(n==NULL)
        return NULL;
    return n->key;
}

void upo_bst_delete_min(upo_bst_t tree, int destroy_data)
{
  upo_bst_node_t *n = min_impl(tree->root);
  if(n!=NULL)
    tree->root = delete_impl(tree->root, n->key,upo_bst_get_comparator(tree), destroy_data); 
}

void upo_bst_delete_max(upo_bst_t tree, int destroy_data)
{
  upo_bst_node_t *n = max_impl2(tree->root);
  if(n!=NULL)
    tree->root = delete_impl(tree->root, n->key,upo_bst_get_comparator(tree), destroy_data); 
}

upo_bst_node_t * floor_impl(upo_bst_node_t* n, const void* key, upo_bst_comparator_t key_cmp){
    if(n==NULL)
        return NULL;
    else if(key_cmp(key, n->key)==-1)
        return floor_impl(n->left, key, key_cmp);
    else if(key_cmp(key, n->key)>=0){
        upo_bst_node_t *x = floor_impl(n->right, key, key_cmp);
        if(x!=NULL)
            return x;
    }
    return n;
    
}

void* upo_bst_floor(const upo_bst_t tree, const void *key)
{
    upo_bst_node_t *n = floor_impl(tree->root, key, upo_bst_get_comparator(tree));
    if(n!=NULL)
        return n->key;
    return NULL;
}

upo_bst_node_t * ceiling_impl(upo_bst_node_t* n, const void* key, upo_bst_comparator_t key_cmp){
    if(n==NULL)
        return NULL;
    else if(key_cmp(key, n->key)==1)
        return ceiling_impl(n->right, key, key_cmp);
    else if(key_cmp(key, n->key)<=0){
        upo_bst_node_t *x = ceiling_impl(n->left, key, key_cmp);
        if(x!=NULL)
            return x;
    }
    return n;
}

void* upo_bst_ceiling(const upo_bst_t tree, const void *key)
{
  upo_bst_node_t* n=ceiling_impl(tree->root, key, upo_bst_get_comparator(tree));
  if(n!=NULL)
      return n->key;
  return NULL;
}

upo_bst_key_list_t upo_bst_keys_range(const upo_bst_t tree, const void *low_key, const void *high_key)
{
    if(tree == NULL) return NULL;
    if(tree->root == NULL) return NULL;
    upo_bst_key_list_t list = upo_bst_keys(tree);
    upo_bst_keys_range_impl(&list, low_key, high_key, tree->key_cmp);
    return list;
}

void upo_bst_keys_range_impl(upo_bst_key_list_t *list, const void *low_key, const void *high_key, upo_bst_comparator_t cmp )
{
    if( (*list) == NULL ) return;

    upo_bst_key_list_t temp;

    while( *list != NULL )
    {
        if(cmp((*list)->key,low_key) < 0 || cmp((*list)->key,high_key) > 0)
        {
            temp = *list;
            (*list) = (*list)->next;
            free(temp);
        }
        else break;
    }

    upo_bst_key_list_t current = (*list);
    while( current->next != NULL )
    {
        if( cmp((current)->next->key, low_key) < 0 || cmp((current)->next->key, high_key) > 0)
        {
            temp = current->next;
            current->next = temp->next;
            free(temp);
        }
        else
            current = current->next;
    }
}

upo_bst_key_list_t upo_bst_keys(const upo_bst_t tree)
{
    if(tree == NULL) return NULL;

    upo_bst_key_list_t list = NULL, result;
    if(tree->root != NULL)
    {
        list = malloc(sizeof(upo_bst_key_list_node_t));
        list->key = tree->root->key;
        list->next = NULL;
        result = list;
        upo_bst_visita_nodi(list, tree->root);
    }
    else result = NULL;

    return result;
}

void upo_bst_visita_nodi(upo_bst_key_list_t elem, upo_bst_node_t *node)
{
    if(node == NULL) return;

    upo_bst_visita_nodi(elem, node->left);
    elem = upo_bst_add_to_tail(elem, node);
    upo_bst_visita_nodi(elem, node->right);
}

upo_bst_key_list_t upo_bst_add_to_tail(upo_bst_key_list_t last, upo_bst_node_t *node)
{
    upo_bst_key_list_t temp = malloc(sizeof(upo_bst_key_list_node_t));
    (last)->next = temp;
    (last) = temp;
    (last)->key = node->key;
    (last)->next = NULL;
    return last;
}

int is_bst_impl(upo_bst_node_t*n, const void *min_key, const void *max_key, upo_bst_comparator_t key_cmp){
    if(n==NULL)
        return 1;
    int b = 0;
    b=is_bst_impl(n->left, min_key, n->key, key_cmp);
    if(b==0)
        return 0;
    b=is_bst_impl(n->right, n->key, max_key, key_cmp);
    if(b==0)
        return 0;
    if(key_cmp(n->key, min_key)<0 || key_cmp(n->key, max_key)>0)
        return 0;
    return 1;
}

int upo_bst_is_bst(const upo_bst_t tree, const void *min_key, const void *max_key)
{
  return is_bst_impl(tree->root, min_key, max_key, tree->key_cmp);
}


/**** EXERCISE #2 - END of EXTRA OPERATIONS ****/


upo_bst_comparator_t upo_bst_get_comparator(const upo_bst_t tree)
{
    if (tree == NULL)
    {
        return NULL;
    }

    return tree->key_cmp;
}

//Per eseguirlo senza il test del prof
/*
void main()
{
    upo_bst_t tree = upo_bst_create();//cosa deve prendere in ingresso????
    
    int chiave = 6;
    int valore = 98;
    int* pchiave = &chiave;
    int* pvalore = &valore;
     
    upo_bst_insert(tree, &chiave, &valore);
	//upo_bst_insert(tree, 1);
	//upo_bst_insert(tree, 10);
	
	if(upo_bst_is_empty(tree) == 1)
		printf("\nL'albero e' vuoto");
	else
		printf("\nL'albero non e' vuoto");
		
	printf("\nI nodi presenti nell' albero sono %d", size_impl(tree->root));//vuole la radice dell' albero
    
    printf("\nL' altezza dell' albero e' %d", upo_bst_height(tree));
    
}*/
