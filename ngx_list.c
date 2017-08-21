
/*
 * Copyright (C) Igor Sysoev
 */


#include <ngx_config.h>
#include <ngx_core.h>


void *ngx_list_push(ngx_list_t *l)
{
    void             *elt;
    ngx_list_part_t  *last;

    last = l->last;
     
     /*l->nalloc
      表示实际元素个数插槽
     last->nelts表示当前元素个数
     */
    if (last->nelts == l->nalloc) {
         
         /*实际元素已经满了
           这时候开始扩充源list大小
          */

        /* the last part is full, allocate a new list part */

        if (!(last = ngx_palloc(l->pool, sizeof(ngx_list_part_t)))) {
            return NULL;
        }

        if (!(last->elts = ngx_palloc(l->pool, l->nalloc * l->size))) {
            return NULL;
        }

        last->nelts = 0;
        last->next = NULL;

        l->last->next = last;
        l->last = last;
    }

    elt = (char *) last->elts + l->size * last->nelts;
    last->nelts++;

    return elt;
}
