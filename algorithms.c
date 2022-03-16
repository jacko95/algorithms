#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*upo_bst_comparator_t)(const void*, const void*); //Si dichiara un puntatore alla funzione upo_bst_comparator_t che riceve due puntatori void che non possono essere modificati (ma copiati sì)
int (*upo_bst_comparator_t)(const void*, const void*) = cmp; //Si assegna la funzione cmp al puntatore a funzione upo_bst_comparator_t

//Compara le chiavi da inserire nella hashtable
int (*upo_ht_comparator_t)(const void*, const void*) = compare;

int cmp(const void* a, const void* b){
    assert(a != NULL && b != NULL);
    unsigned char* pa = (unsigned char*)a;
    unsigned char* pb = (unsigned char*)b;
    if(*pa > *pb)
        return 1;
    if(*pa < *pb)
        return -1;
    else
        return 0;
}

//Non da imparare perchè esistono memcpy() e memmove()
void any_copy(void* dest, void* src, size_t n){
    assert(dest != NULL && src != NULL);
    unsigned char* b = (unsigned char*)dest;
    unsigned char* a = (unsigned char*)src;
    int i = 0;
    for(; i < n; i++)
        b[i] = a[i];
}

void upo_merge_sort(void *base, size_t n, size_t size, upo_sort_comparator_t cmp){
    if(base == NULL || cmp == NULL)
        return;
    upo_merge_sort_rec(base, 0, n-1, size, cmp); //L'array base va da 0 a n-1
}

void upo_merge_sort_rec(void *base, size_t lo, size_t hi, size_t size, upo_sort_comparator_t cmp){
    if(lo >= hi)
        return;
    if(hi <= lo+3-1) { //if(hi <= lo + CUTOFF - 1)
        upo_insertion_sort(base, hi+1, size, cmp);//hi+1 perchè la funz. precedente gli passa n-1
        return;
    }
    size_t mid = (lo+hi)/2;
    upo_merge_sort_rec(base, lo, mid, size, cmp);
    upo_merge_sort_rec(base, mid+1, hi, size, cmp);
    upo_merge_sort_merge(base, lo, mid, hi, size, cmp);
}

void upo_merge_sort_merge(void *base, size_t lo, size_t mid, size_t hi, size_t size, upo_sort_comparator_t cmp){
    unsigned char* a = (unsigned char*)base;
    size_t i = 0; //puntatore da lo in temp
    size_t j = mid+1-lo; //puntatore da mid+1 in temp
    size_t k = 0; //puntatore in a
    unsigned char* temp = malloc((hi+1-lo)*size); //nuovo array da ordinare che conterrà i due array
    memmove(temp, &a[lo*size], (hi+1-lo)*size); //copia tutti gli elementi da a(base) a temp
    //seleziona l'elemento minore dei due sottoarray ordinati
    for(k = lo; k <= hi; k++){ //k parte da lo e arriva ad hi
        if(i > (mid-lo)){//i > mid
            any_copy(&a[k*size], &temp[j*size], size);
            j++;
        }
        else if(j > (hi-lo)){//j > hi
            any_copy(&a[k*size], &temp[i*size], size);
            i++;
        }
        else if(cmp(&temp[j*size], &temp[i*size]) < 0){//se elemento j-esimo < elemento i-esimo
            any_copy(&a[k*size], &temp[j*size], size);
            j++;
        }
        else{//if(cmp(&temp[j*size], &temp[i*size]) >= 0)//se elemento j-esimo >= elemento i-esimo
            any_copy(&a[k*size], &temp[i*size], size);
            i++;
        }
    }
    free(temp);
}

void upo_insertion_sort(void *base, size_t n, size_t size, upo_sort_comparator_t cmp){
    if(base == NULL || cmp == NULL)
        return;
    unsigned char* a = (unsigned char*)base;
    int i = 0;
    for(i = 0; i < (n*size)-1; i += size){
        int j = i;
        while(j > 0 && cmp(&a[j], &a[j-size]) < 0){
            unsigned char* temp = (unsigned char*)malloc(sizeof(size));
            any_copy(temp, &a[j], size);
            any_copy(&a[j], &a[j-size], size);
            any_copy(&a[j-size], temp, size);
            free(temp);
            j -= size;
        }
    }
}

void bubble_sort(void* base, size_t n, size_t size, upo_sort_comparator_t cmp){
    if(base == NULL || cmp == NULL)
        return;
    unsigned char* a = (unsigned char*)base;
    int j = 0;
    int i = 0;
    for(i = 0; i < n-1; i++){
        for(j = 0; j < n-1; j++){
            if(cmp(&a[j*size], &a[(j+1)*size]) > 0){
                unsigned char* temp = (unsigned char*)malloc(sizeof(size));
                any_copy(temp, &a[j*size], size);
                any_copy(&a[j*size], &a[(j+1)*size], size);
                any_copy(&a[(j+1)*size], temp, size);
                free(temp);
            }
        }
    }
}

void bidirectional_sort(void* base, size_t n, size_t size, upo_sort_comparator_t cmp){ 
    if(base == NULL || cmp == NULL)
        return;
    unsigned char* a = (unsigned char*)base;
    int j = 0;
    int swapped = 0;
    while(1){ //oppure while(swapped) senza il return;
        swapped = 0;
        for(j = 0; j < n-1; j++){
            if(cmp(&a[j*size], &a[(j+1)*size]) > 0){
                unsigned char* temp = (unsigned char*)malloc(sizeof(size));
                any_copy(temp, &a[j*size], size);
                any_copy(&a[j*size], &a[(j+1)*size], size);
                any_copy(&a[(j+1)*size], temp, size);
                free(temp);
                swapped = 1;
            } 
        } 
        if(!swapped) 
            return;
        for(j = n-1; j > 0; j--){
            if(cmp(&a[j*size], &a[(j+1)*size]) > 0){
                unsigned char* temp = (unsigned char*)malloc(sizeof(size));
                any_copy(temp, &a[j*size], size);
                any_copy(&a[j*size], &a[(j+1)*size], size);
                any_copy(&a[(j+1)*size], temp, size);
                free(temp);
                swapped = 1;
            } 
        }
        //if(!swapped) return;
    } 
} 

void odd_even_sort(void* base, size_t n, size_t size, upo_sort_comparator_t cmp){ 
    if(base == NULL || cmp == NULL || n == 1)//n == 1 caso base
        return;
    unsigned char* a = (unsigned char*)base;
    int sorted = 0; //Initially array is unsorted
    int j = 0;
    while(!sorted){ 
        sorted = 1; 
        for(j = 1; j < n-1; j+=2){ //Perform Bubble sort on odd indexed element 
            if(cmp(&a[j*size], &a[(j+1)*size]) > 0){
                unsigned char* temp = (unsigned char*)malloc(sizeof(size));
                any_copy(temp, &a[j*size], size);
                any_copy(&a[j*size], &a[(j+1)*size], size);
                any_copy(&a[(j+1)*size], temp, size);
                free(temp);
                sorted = 0; 
            } 
        } 
        for(j = 0; j < n-1; j+=2){ //Perform Bubble sort on even indexed element 
            if(cmp(&a[j*size], &a[(j+1)*size]) > 0){
                unsigned char* temp = (unsigned char*)malloc(sizeof(size));
                any_copy(temp, &a[j*size], size);
                any_copy(&a[j*size], &a[(j+1)*size], size);
                any_copy(&a[(j+1)*size], temp, size);
                free(temp);
                sorted = 0; 
            } 
        } 
    } 
}

void* upo_ht_linprob_put(upo_ht_linprob_t ht, void *key, void *value){
    if(ht == NULL || key == NULL || value == NULL) 
        return NULL;
    void *old_value = NULL;
    if(upo_ht_linprob_load_factor(ht) > 0.5)
        upo_ht_linprob_resize(ht, ht->capacity * 2);
    size_t h = ht->key_hash(key, ht->capacity);
    int found_tomb = 0;
    size_t h_tomb;
    while((ht->slots[h].key != NULL && ht->key_cmp(key, ht->slots[h].key) != 0) || ht->slots[h].tombstone == 1){
        if(ht->slots[h].tombstone == 1 && !found_tomb){
            found_tomb = 1;
            h_tomb = h;
        }
        h = (h + 1) % ht->capacity;
    }
    if(ht->slots[h].key == NULL){
        if(found_tomb == 1)
            h = h_tomb;
        ht->slots[h].key = key;
        ht->slots[h].value = value;
        ht->slots[h].tombstone = 0;
        ht->size++;
    }
    else{
        old_value = ht->slots[h].value;
        ht->slots[h].value = value;
    }
    return old_value;
}

void upo_ht_linprob_insert(upo_ht_linprob_t ht, void *key, void *value){
    if(ht == NULL || key == NULL || value == NULL) 
        return;
    if(upo_ht_linprob_load_factor(ht) > 0.5)
        upo_ht_linprob_resize(ht, ht->capacity * 2);
    size_t h = ht->key_hash(key, ht->capacity);
    int found_tomb = 0;
    size_t h_tomb;
    while((ht->slots[h].key != NULL && ht->key_cmp(ht->slots[h].key, key) != 0) || ht->slots[h].tombstone == 1){
        if(ht->slots[h].tombstone == 1 && !found_tomb){
            found_tomb = 1;
            h_tomb = h;
        }
        h = (h + 1) % ht->capacity;
    }
    if(ht->slots[h].key == NULL){
        if(found_tomb == 1)
            h = h_tomb;
        ht->slots[h].key = key;
        ht->slots[h].value = value;
        ht->slots[h].tombstone = 0;
        ht->size++;
    }
}

void* upo_ht_linprob_get(upo_ht_linprob_t ht, void* key){
    if(ht == NULL || key == NULL)
        return NULL;
    size_t h = ht->key_hash(key, ht->capacity);
    while((ht->slots[h].key != NULL && ht->key_cmp(ht->slots[h].key, key) != 0) || ht->slots[h].tombstone == 1)
        h = (h + 1) % ht->capacity;
    if(ht->slots[h].key != NULL)
        return ht->slots[h].value;
    else
        return NULL;
}

int upo_ht_linprob_contains(const upo_ht_linprob_t ht, const void *key){
    if(ht == NULL || key == NULL) 
        return 0;
    size_t h = ht->key_hash(key, ht->capacity);
    while((ht->slots[h].key != NULL && ht->key_cmp(ht->slots[h].key, key) != 0) || ht->slots[h].tombstone == 1)
        h = (h + 1) % ht->capacity;
    if(ht->slots[h].key != NULL)
        return 1;
    else
        return 0;
}

void upo_ht_linprob_delete(upo_ht_linprob_t ht, const void *key, int destroy_data){
    if(ht == NULL || key == NULL) 
        return;
    size_t h = ht->key_hash(key, ht->capacity);
    while((ht->slots[h].key != NULL && ht->key_cmp(ht->slots[h].key, key) != 0) || ht->slots[h].tombstone == 1)
        h = (h + 1) % ht->capacity;
    if(ht->slots[h].key != NULL){
        ht->slots[h].key = NULL;
        ht->slots[h].value = NULL;
        ht->slots[h].tombstone = 1;
        if(destroy_data == 1){
            free(ht->slots[h].key);
            free(ht->slots[h].value);
        }
        ht->size--;
        if(upo_ht_linprob_load_factor(ht) < 0.125)
            upo_ht_linprob_resize(ht, ht->capacity/2);
    }
}

void* upo_ht_sepchain_get(const upo_ht_sepchain_t ht, const void *key){
    if(ht == NULL || key == NULL) 
        return NULL;
    size_t h = ht->key_hash(key, ht->capacity);
    upo_ht_sepchain_list_node_t* n = ht->slots[h].head; //head puntatore alla testa della lista di collisione dello slot in posizione h
    while(n != NULL && ht->key_cmp(key, n->key) != 0)
        n = n->next;
    if(n != NULL)
        return n->value;
    else
        return NULL;
}

void* upo_ht_sepchain_olist_get(const upo_ht_sepchain_t ht, const void *key){
    if(ht == NULL || key == NULL) 
        return NULL;
    size_t h = ht->key_hash(key, ht->capacity);
    upo_ht_sepchain_list_node_t *n = ht->slots[h].head;
    while(n != NULL && ht->key_cmp(key, n->key) > 0)
        n = n->next;
    if(n != NULL && ht->key_cmp(key, n->key) == 0)
        return n->value;
    else
        return NULL;
}

int upo_ht_sepchain_contains(const upo_ht_sepchain_t ht, const void *key){
    if(ht == NULL || key == NULL) 
        return 0;
    size_t h = ht->key_hash(key, ht->capacity);
    upo_ht_sepchain_list_node_t *n = ht->slots[h].head;
    while(n != NULL && ht->key_cmp(key, n->key) != 0)//Per key != n->key si dovrebbe usare
        n = n->next;
    if(n != NULL)
        return 1;
    else
        return 0;
}

int upo_ht_sepchain_olist_contains(const upo_ht_sepchain_t ht, const void *key){
    if(ht == NULL || key == NULL) 
        return 0;
    size_t h = ht->key_hash(key, ht->capacity);
    upo_ht_sepchain_list_node_t *n = ht->slots[h].head;
    while(n != NULL && ht->key_cmp(key, n->key) > 0)
        n = n->next;
    if(n != NULL && ht->key_cmp(key, n->key) == 0)
        return 1;
    else
        return 0;
}

void* upo_ht_sepchain_put(upo_ht_sepchain_t ht, void *key, void *value){
    if(ht == NULL || key == NULL || value == NULL) 
        return NULL;
    size_t h = ht->key_hash(key, ht->capacity);
    upo_ht_sepchain_list_node_t* n = ht->slots[h].head;
    void* old_value = NULL;
    while(n != NULL && ht->key_cmp(key, n->key) != 0)//while(n != NULL && key < n->key)
        n = n->next; 
    if(n == NULL){
        n = malloc(sizeof(upo_ht_sepchain_list_node_t));
        if(n == NULL){
            perror("Unable to allocate memory for upo_ht_sepchain_list_node_t");
            abort();
        }
        n->key = key;
        n->value = value;
        n->next = ht->slots[h].head;
        ht->slots[h].head = n;
        ht->size++;
    }
    else{
        old_value = n->value;
        n->value = value;
    }
    return old_value;
}

//da inviare
void* upo_ht_sepchain_olist_put(upo_ht_sepchain_olist_t ht, void *key, void *value){
    if(ht == NULL || key == NULL || value == NULL) 
        return NULL;
    void *old_value = NULL;
    size_t h = ht->key_hash(key, ht->capacity);
    upo_ht_sepchain_list_node_t *n = ht->slots[h].head;
    upo_ht_sepchain_list_node_t *p = NULL;
    
    upo_ht_sepchain_list_node_t *new = malloc(sizeof(upo_ht_sepchain_list_node_t));
    if(new == NULL){
        perror("Unable to allocate memory for upo_ht_sepchain_list_node_t");
        abort();
    }
    new->key = key;
    new->value = value;
    new->next = NULL;
    
    while(n != NULL && ht->key_cmp(key, n->key) > 0){
        p = n;
        n = n->next;
    }
    
    if(p == NULL){//Inserisce new in testa
        new->next = ht->slots[h].head;
        ht->slots[h].head = new;
        ht->size++;
    }
    else{//if(p != NULL)
        if(n == NULL){//Inserimento new in coda
            p->next = new;
            ht->size++;
        }
        else if(ht->key_cmp(key, n->key) == 0){//sostituzione del valore
            old_value = n->value;
            n->value = value;
            free(new);
        }
        else if(ht->key_cmp(key, n->key) < 0){//Inserimento new in mezzo
            new->next = n;
            p->next = new;
            ht->size++;
        }
    }
    return old_value;
}

void upo_ht_sepchain_insert(upo_ht_sepchain_t ht, void* key, void* value){
    if(ht == NULL || key == NULL || value == NULL) 
        return;
    size_t h = ht->key_hash(key, ht->capacity);
    upo_ht_sepchain_list_node_t *n = ht->slots[h].head;
    while(n != NULL && ht->key_cmp(key, n->key) != 0)
        n = n->next;
    if(n == NULL){
        n = malloc(sizeof(upo_ht_sepchain_list_node_t));
        if(n == NULL){
            perror("Unable to allocate memory for upo_ht_sepchain_list_node_t");
            abort();
        }
        n->key = key;
        n->value = value;
        n->next = ht->slots[h].head;
        ht->slots[h].head = n;
        ht->size++;
    }
}

void upo_ht_sepchain_olist_insert(upo_ht_sepchain_olist_t ht, void *key, void *value){
    if(ht == NULL || key == NULL || value == NULL) 
        return;
    
    size_t h = ht->key_hash(key, ht->capacity);
    upo_ht_sepchain_list_node_t *n = ht->slots[h].head;
    upo_ht_sepchain_list_node_t *p = NULL;
        
    while(n != NULL && ht->key_cmp(key, n->key) > 0){
        p = n;
        n = n->next;
    }
    
    if(n == NULL || ht->key_cmp(key, n->key) < 0){ // Chiave non trovata -> Inserimento
        upo_ht_sepchain_list_node_t *new = malloc(sizeof(upo_ht_sepchain_list_node_t));
        if(new == NULL){
            perror("Unable to allocate memory for upo_ht_sepchain_list_node_t");
            abort();
        }
        new->key = key;
        new->value = value;
        new->next = n; // Nell'inserimento in coda n = NULL.
        
        if(p == NULL) // Inserisce new in testa
            ht->slots[h].head = new;
        else // Inserisce new in mezzo/coda
            p->next = new;
        
        ht->size++;
    }
}

void upo_ht_sepchain_delete(upo_ht_sepchain_t ht, const void *key, int destroy_data){
    if(ht == NULL || key == NULL) 
        return;
    size_t h = ht->key_hash(key, ht->capacity);
    upo_ht_sepchain_list_node_t *n = ht->slots[h].head;
    upo_ht_sepchain_list_node_t *p = NULL;
    while(n != NULL && ht->key_cmp(key, n->key) != 0){
        p = n;
        n = n->next;
    }
    if(n != NULL){
        if(p == NULL)//Cancellazione in testa
            ht->slots[h].head = n->next;//il nuovo ht->slots[h].head è/contiene n->next (si salta n)
        else
            p->next = n->next;//Cancellazione in mezzo ed in coda (si salta/cancella n)
        if(destroy_data == 1){
            free(n->key);
            free(n->value);
        }
        free(n);
        ht->size--;
    }
}

//Non vista dal prof?
void upo_ht_sepchain_olist_delete(upo_ht_sepchain_olist_t ht, const void *key, int destroy_data){
    if(ht == NULL || key == NULL) 
        return;

    size_t h = ht->key_hash(key, ht->capacity);
    upo_ht_sepchain_list_node_t *n = ht->slots[h].head;
    upo_ht_sepchain_list_node_t *p = NULL;

    while(n != NULL && ht->key_cmp(key, n->key) > 0){
        p = n;
        n = n->next;
    }

    if(n != NULL && ht->key_cmp(key, n->key) == 0){ // Chiave trovata -> Cancellazione
        if(p == NULL) // Cancellazione in testa (scollegamento)
            ht->slots[h].head = n->next;
        else // Cancellazione in mezzo/coda (scollegamento) 
            p->next = n->next;
        if(destroy_data == 1){
            free(n->key);
            free(n->value);
        }
        free(n);
        ht->size--;
    }
}

size_t upo_bst_size(const upo_bst_t tree){
    if(tree == NULL)
        return 0;
    return upo_bst_size_impl(tree->root);
}

int upo_bst_size_impl(upo_bst_node_t* n){
    if(n == NULL) //caso base
        return 0;
    return 1 + upo_bst_size_impl(n->left) + upo_bst_size_impl(n->right);
}

int upo_bst_is_empty(const upo_bst_t tree){
    if(tree == NULL || tree->root == NULL)
        return 1;
    else
        return 0;
}

void upo_bst_insert(upo_bst_t tree, void* key, void* value){
    tree->root = upo_bst_insert_impl(tree->root, key, value, tree->key_cmp);
}

upo_bst_node_t* upo_bst_insert_impl(upo_bst_node_t* n, void* key, void* value, upo_bst_comparator_t cmp){
    if(n == NULL)
        return upo_bst_node_create(key, value);
    else if(cmp(key, n->key) < 0)
        n->left = upo_bst_insert_impl(n->left, key, value, cmp);
    else if(cmp(key, n->key) > 0)
        n->right = upo_bst_insert_impl(n->right, key, value, cmp);
    //else{} //?????
    return n;
}

void* upo_bst_put(upo_bst_t tree, void *key, void *value){
    void* old_value = NULL;
    tree->root = upo_bst_put_impl(tree->root, key, value, old_value, tree->key_cmp);
    return old_value;
}

upo_bst_node_t* upo_bst_put_impl(upo_bst_node_t *n, void *key, void *value, void* old_value, upo_bst_comparator_t cmp){
    old_value = NULL;
    if(n == NULL)
        return upo_bst_node_create(key, value);
    else if(key_cmp(key, n->key) < 0)//Se k è minore di n->key, metti k a come figlio sinistro
        n->left = upo_bst_put_impl(n->left, key, value, old_value, cmp);   
    else if(key_cmp(key, n->key) > 0)//Se k è maggiore di n->key, metti k a come figlio destro
        n->right = upo_bst_put_impl(n->right, key, value, old_value, cmp);
    else{
        old_value = n->value;
        n->value = value;
    }
    return n;
}

upo_bst_node_t* upo_bst_node_create(void* key, void* value){
    upo_bst_node_t* n = malloc(sizeof(upo_bst_node_t));
    n->key = key;
    n->value = value;
    n->left = NULL;
    n->right = NULL;
    return n;
}

void* upo_bst_get(const upo_bst_t tree, const void *key){
    upo_bst_node_t *n = upo_bst_get_impl(tree->root, key, tree->key_cmp);
    if(n != NULL)
        return n->value; 
    return NULL;
}

upo_bst_node_t* upo_bst_get_impl(upo_bst_node_t* n, void* key, upo_bst_comparator_t cmp){
    if(n == NULL || cmp == NULL)
        return NULL;
    if(cmp(n->key, key) > 0)
        return upo_bst_get_impl(n->left, key, cmp);
    else if(cmp(n->key, key) < 0)
        return upo_bst_get_impl(n->right, key, cmp);
    else
        return n;
}

int upo_bst_contains(const upo_bst_t tree, const void *key){
    if(upo_bst_get_impl(tree->root, key, tree->key_cmp) != NULL)
        return 1;
    else
        return 0;
}

int upo_bst_is_leaf(upo_bst_node_t* n){
    assert(n != NULL);
    if(n->left == NULL && n->right == NULL)
        return 1;
    else
        return 0;
}

size_t upo_bst_height(const upo_bst_t tree){
    return upo_bst_height_impl(tree->root);
}

int upo_bst_height_impl(upo_bst_node_t* n){
    if(n == NULL) 
        return 0;
    if(upo_bst_is_leaf(n))
        return 0;
    else if(upo_bst_height_impl(n->left) >= upo_bst_height_impl(n->right))
        return 1 + upo_bst_height_impl(n->left);
    else if(upo_bst_height_impl(n->left) <= upo_bst_height_impl(n->right))
        return 1 + upo_bst_height_impl(n->right);
}

/*
upo_bst_node_t* max_impl(upo_bst_node_t* n){
    if(n == NULL)
        return NULL;
    if(n->right == NULL)
        return n;
    else
        return max_impl(n->right);
}*/
upo_bst_node_t* upo_bst_max_impl(upo_bst_node_t* n){
    if(n == NULL)
        return NULL;
    else if(n->right != NULL)
        upo_bst_max_impl(n->right);
    else
        return n;
}

void* upo_bst_max(const upo_bst_t tree){
    upo_bst_node_t* n = upo_bst_max_impl(tree->root);
    if(n == NULL)
        return NULL;
    return n->key;
}

size_t conta_foglie(upo_bst_node_t* n){
    if(n == NULL)
        return 0;
    if(is_leaf_impl(n))
        return 1;
    else
        return conta_foglie(n->left) + conta_foglie(n->right);
}

size_t upo_bst_rank(const upo_bst_t tree, const void* key){
    if(tree == NULL || key == NULL)
        return 0;
    return upo_bst_rank_impl(tree->root, key, tree->key_cmp);
}

size_t upo_bst_rank_impl(upo_bst_node_t *n, const void* key, upo_bst_comparator_t cmp){
    if(n == NULL)
        return 0;
    if(cmp(n->key, key) < 0)
        return 1 + upo_bst_rank_impl(n->left, key, cmp) + upo_bst_rank_impl(n->right, key, cmp);
    else//if(cmp(key, n->key) <= 0)
        return upo_bst_rank_impl(n->left, key, cmp) + upo_bst_rank_impl(n->right, key, cmp);
}

int upo_bst_range(upo_bst_t tree, const void* low_key, const void* high_key){
    if(tree == NULL || tree-> root == NULL) 
        return 0;
    return range_impl(tree->root, low_key, high_key, tree->key_cmp);
}

int upo_bst_range_impl(upo_bst_node_t *n, const void* low_key, const void* high_key, upo_bst_comparator_t cmp){
    if(cmp(high_key, low_key) < 0)
        return -1;
    if(n == NULL) //caso base
        return 0;
    if(cmp(low_key, n->key) > 0)//*(int*)(n->key) < *(int*)(low_key))
        return upo_bst_range_impl(n->right, low_key, high_key, cmp);
    else if(cmp(high_key, n->key) < 0)//*(int*)(n->key) > *(int*)(high_key))
        return upo_bst_range_impl(n->left, low_key, high_key, cmp);
    else if((cmp(high_key, n->key) >= 0) && (cmp(low_key, n->key) <= 0))
        return 1 + upo_bst_range_impl(n->left, low_key, high_key, cmp) + upo_bst_range_impl(n->right, low_key, high_key, cmp);
}

int foglie_in_range(upo_bst_node_t *n, const void* low_key, const void* high_key, upo_bst_comparator_t cmp){
    if(cmp(high_key, low_key) < 0)
        return -1;
    if(n == NULL)
        return 0;
    if((cmp(high_key, n->key) >= 0) && (cmp(low_key, n->key) <= 0) && is_leaf_impl(n) == 1)
        return 1 + foglie_in_range(n->left, low_key, high_key, cmp) + foglie_in_range(n->right, low_key, high_key, cmp);
    else
        return foglie_in_range(n->left, low_key, high_key, cmp) + foglie_in_range(n->right, low_key, high_key, cmp);
}

//Conta le foglie del sottoalbero con radice in key nell'albero tree
int foglie(const upo_bst_t tree, void* key){
    if(tree == NULL || tree->root == NULL)
        return 0;
    if(upo_bst_contains(tree, key) == 0)
        return 0;
    upo_bst_node_t* n = tree->root;//(upo_bst_node_t*)malloc(sizeof(upo_bst_node_t)); ???
    while(n != NULL){
        if(tree->key_cmp(n->key, key) > 0)
            n = n->left;
        else if(tree->key_cmp(n->key, key) < 0)
            n = n->right;
        else//if(tree->key_cmp(n->key, key) == 0)
            return conta_foglie(n);
    }
    free(n);
}

int foglie(upo_bst_node_t* n, void* key, upo_bst_comparator_t cmp){
    if(n == NULL)
        return -1;
    if(cmp(key, n->key) < 0)
        return foglie(n->left, key, cmp);
    else if(cmp(key, n->key) > 0)
        return foglie(n->right, key, cmp);
    else
        return conta_foglie(n);
}

upo_bst_node_t* delete_C1(upo_bst_node_t* n, const void* k, int destroy_data){
    upo_bst_node_t* x = n;
    if(n->left != NULL)
        n = n->left;
    else
        n = n->right;
    if(destroy_data){
        free(x->value);
        free(x->key);
    }
    free(x);
    return n;
}

upo_bst_node_t* delete_C2(upo_bst_comparator_t key_cmp, upo_bst_node_t* n, const void* k, int destroy_data){
    upo_bst_node_t* m;
    m = max_impl(n->left);
    n->key = m->key;
    n->value = m->value;
    n->left = delete_impl(n->left, m->key, key_cmp, destroy_data);
    return n;
}

upo_bst_node_t* delete_impl(upo_bst_node_t* n, const void *k, upo_bst_comparator_t key_cmp, int destroy_data){
    if(n == NULL)
        return NULL;
    if(key_cmp(k, n->key) == -1)
        n->left = delete_impl(n->left, k, key_cmp, destroy_data);
    else if(key_cmp(k, n->key) == 1)
        n->right = delete_impl(n->right, k, key_cmp, destroy_data);
    else if(n->left != NULL && n->right != NULL)
        n = delete_C2(key_cmp, n, k, destroy_data);
    else
        n = delete_C1(n, k, destroy_data);
    return n;
}

void* upo_bst_get_value_depth(const upo_bst_t tree, const void *key, long *depth){
    if(tree == NULL || tree->root == NULL)
        return NULL;
    if(upo_bst_contains(tree, key) == 0)
        return NULL;
    if(tree->key_cmp(key, tree->root->key) == 0)
        return tree->root->value;
    upo_bst_node_t* n = tree->root;
    int alt = 0;
    while(tree->key_cmp(key, n->key) != 0){ //chiave != *(int*)(n->key)
        if(tree->key_cmp(key, n->key) < 0){
            alt++;
            n = n->left;
        }
        else if(tree->key_cmp(key, n->key) > 0){
            alt++;
            n = n->right;
        }
        else if(tree->key_cmp(key, n->key) == 0){
            depth = (long*)&alt;
            return n->value;
        }
    }
}

void* upo_bst_predecessor(const upo_bst_t tree, const void* key){
    if(tree == NULL || tree->root == NULL || key == NULL)
        return NULL;
    upo_bst_node_t* pred = upo_bst_predecessor_impl(tree->root, key, tree->key_cmp);
    if(pred != NULL)
        return pred->key;
    else
        return NULL;
}

upo_bst_node_t* upo_bst_predecessor_impl(upo_bst_node_t* n, const void* key, upo_bst_comparator_t cmp){
    if(n == NULL)
        return NULL;
    else if(cmp(key, n->key) <= 0)//Quando sarà uguale saremo sul nodo di cui vogliamo trovare il predecessore
        return upo_bst_predecessor_impl(n->left, key, cmp);
    else{//Abbiamo superato il nodo di cui vogliamo trovare il predecessore che sarà il maggiore del sottoalbero sinistro
        upo_bst_node_t* pred = upo_bst_predecessor_impl(n->right, key, cmp);
        if(pred != NULL)
            return pred;
        else//Se supera una foglia
            return n;
    }
}

void* upo_bst_successor(const upo_bst_t tree, const void* key){
    if(tree == NULL || tree->root == NULL || key == NULL)
        return NULL;
    upo_bst_node_t* succ = upo_bst_successor_impl(tree->root, key, tree->key_cmp);
    if(succ != NULL)
        return succ->key;
    else
        return NULL;
}

upo_bst_node_t* upo_bst_successor_impl(upo_bst_node_t* n, const void* key, upo_bst_comparator_t cmp){
    if(n == NULL)
        return NULL;
    else if(cmp(key, n->key) >= 0)
        return upo_bst_successor_impl(n->right, key, cmp);
    else{
        upo_bst_node_t* succ = upo_bst_successor_impl(n->left, key, cmp);
        if(succ != NULL)
            return succ;
        else
            return n;
    }
}

