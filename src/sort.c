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

#include <assert.h>
#include "sort_private.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t upo_quick_sort_partition(void *base, size_t lo, size_t hi, size_t size, upo_sort_comparator_t cmp);
void upo_merge_sort_rec(void *base, size_t lo, size_t hi, size_t size, upo_sort_comparator_t cmp);
void upo_quick_sort_rec(void *base,size_t lo, size_t hi, size_t size, upo_sort_comparator_t cmp);
void upo_merge_sort_merge(void *base, size_t lo, size_t mid, size_t hi, size_t size, upo_sort_comparator_t cmp);

void any_copy(void *dest, const void *src, size_t n)
{
    assert( dest != NULL );
    assert( src != NULL );
    unsigned char* el1 = dest;
    unsigned char* el2 = (unsigned char*) src;
    

    for (unsigned int i = 0; i < n; ++i)
        el1[i] = el2[i];
}

void upo_insertion_sort(void *base, size_t n, size_t size, upo_sort_comparator_t cmp)
{
    if(base == NULL || cmp == NULL)
        return;

    unsigned char* temp = (unsigned char*) base;

    for (unsigned int i = 0; i < (size * n) - 1; i += size)
    {
        int j = i;
        while(j > 0 && cmp(&temp[j],&temp[j - size]) < 0)
        {
            unsigned char* t = malloc(size);
            any_copy(t,&temp[j],size);
            any_copy(&temp[j],&temp[j - size],size);
            any_copy(&temp[j - size],t,size);
            free(t);
            j-= size;
        }
    }
}

void upo_merge_sort(void *base, size_t n, size_t size, upo_sort_comparator_t cmp)
{
    if(base == NULL || cmp == NULL)
        return;
    
    upo_merge_sort_rec(base, 0, n-1 , size, cmp);
}

void upo_merge_sort_rec(void *base, size_t lo, size_t hi, size_t size, upo_sort_comparator_t cmp)
{
    if(lo >= hi)
        return;

    size_t mid = (lo + hi) / 2;
    upo_merge_sort_rec(base, lo, mid, size, cmp);
    upo_merge_sort_rec(base, mid + 1, hi, size, cmp);
    upo_merge_sort_merge(base, lo, mid, hi, size, cmp);
}

void upo_merge_sort_merge(void *base, size_t lo, size_t mid, size_t hi, size_t size, upo_sort_comparator_t cmp)
{
    
    unsigned char* temp = (unsigned char*)base;
    size_t i = 0;
    size_t j = (mid + 1) - lo;
    unsigned char* arr = malloc((hi-lo + 1) * size);
    memmove(arr, &temp[lo*size], size * (hi-lo + 1));
    for (size_t k = lo; k <= hi; k++)
    {
        if( i > (mid - lo) )
        {
            any_copy(&temp[k*size], &arr[(j)*size], size);
            j++;
        }
        else if(j > (hi - lo))
        {
            any_copy(&temp[k*size], &arr[(i)*size], size);
            i++;
        }
        else if( cmp(&arr[(j)*size], &arr[(i)*size]) < 0 )
        {
            any_copy(&temp[k*size], &arr[(j)*size], size);
            j++;
        }
        else
        {
            any_copy(&temp[k*size], &arr[(i)*size], size);
            i++;
        }
    }
    free(arr);
    
    /*
    unsigned char* base = (unsigned char*)t;
    size_t i = lo;
    size_t j = mid + 1;
    size_t k = 0;
    unsigned char* b = malloc((hi - lo)*size);
    memmove(b, &base[lo*size],(hi-lo)*size);
    while ((i <= mid) && (j <= hi))
    {
        if (cmp(&base[i*size], &base[j*size]) < 0 ) 
        {
            any_copy(&b[k*size], &base[i*size],size);
            i++;
        }
        else
        {
            any_copy(&b[k*size], &base[j*size],size);
            j++; 
        }
        k++;
    }
    
    while ((i <= mid))
    {
        any_copy(&b[k*size], &base[i*size],size);
        i++;
        k++;
    }

    while ((j <= hi))
    {
        any_copy(&b[k*size], &base[j*size],size);
        j++;
        k++;
    }
 
    for(k = lo; k < hi; k++)
        any_copy(&base[k*size], &b[(k-lo)*size],size);

    free(b);*/
}

void upo_quick_sort(void *base, size_t n, size_t size, upo_sort_comparator_t cmp)
{
    if(base == NULL || cmp == NULL)
        return;
    upo_quick_sort_rec(base, 0, n - 1, size, cmp);
}

void upo_quick_sort_rec(void *base,size_t lo, size_t hi, size_t size, upo_sort_comparator_t cmp)
{
    if(lo >= hi)
        return;

    size_t j = upo_quick_sort_partition(base, lo , hi, size, cmp);
    
    if(j > 0) upo_quick_sort_rec(base, lo , j-1, size, cmp);

    upo_quick_sort_rec(base, j+1 , hi, size , cmp);
}

size_t upo_quick_sort_partition(void *base, size_t lo, size_t hi, size_t size, upo_sort_comparator_t cmp)
{
    unsigned char* temp = (unsigned char*) base;
    size_t p = lo;
    size_t i = lo;
    size_t j = hi + 1;
    while(1)
    {
        do{
            i++;
        } while(i < hi && cmp(&temp[i * size],&temp[p * size]) < 0);

        do{
            j--;
        } while(j > lo && cmp(&temp[j * size],&temp[p * size]) > 0);

        if(i >= j)
            break;

        unsigned char* t = malloc(size);
        any_copy(t,&temp[j*size],size);
        any_copy(&temp[j*size],&temp[i*size],size);
        any_copy(&temp[i*size],t,size);
        free(t);
    }

    unsigned char* t = malloc(size);
    any_copy(t,&temp[p*size],size);
    any_copy(&temp[p*size],&temp[j*size],size);
    any_copy(&temp[j*size],t,size);
    free(t);

    return j;
}
