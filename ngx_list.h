
/*
 * Copyright (C) Igor Sysoev
 */


#ifndef _NGX_LIST_H_INCLUDED_
#define _NGX_LIST_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef struct ngx_list_part_s  ngx_list_part_t;

struct ngx_list_part_s {
    /*elts list首地址*/
    void             *elts;
    /*nelts list中的已分配元素个数
     就是size
    */
    ngx_uint_t        nelts;
    /*ngx_list next 典型的list*/
     ngx_list_part_t  *next;
};


typedef struct {
    
    /*list ,part 都是list对象*/
    ngx_list_part_t  *last;
    /*list part 是指向第一个数据*/
    ngx_list_part_t   part;
    /*实际list大小*/
    size_t            size;
    //
    ngx_uint_t        nalloc;
    /*每一个list对象都有一个内存池对象*/
    ngx_pool_t       *pool;

} ngx_list_t;


ngx_inline static ngx_int_t ngx_list_init(ngx_list_t *list, ngx_pool_t *pool,
                                          ngx_uint_t n, size_t size)
{
    if (!(list->part.elts = ngx_palloc(pool, n * size))) {
        return NGX_ERROR;
    }
     
    list->part.nelts = 0;
    list->part.next = NULL;
    /*
     实际上list->last存的是list->part的地址
    */
    list->last = &list->part;
    /*
      list->size =size 
    */
    list->size = size;
    list->nalloc = n;
    list->pool = pool;

    return NGX_OK;
}


/*
 *
 *  the iteration through the list:
 *
 *  part = &list.part;
 *  data = part->elts;
 *
 *  for (i = 0 ;; i++) {
 *
 *      if (i >= part->nelts) {
 *          if (part->next == NULL) {
 *              break;
 *          }
 *
 *          part = part->next;
 *          data = part->elts;
 *          i = 0;
 *      }
 *
 *      ...  data[i] ...
 *
 *  }
 */
/*/media/jackwu/DATA/sys_soft/cgwin/home/JackWU/nginx/nginx-0.1.0/src/core/ngx_list.h*/

void *ngx_list_push(ngx_list_t *list);


#endif /* _NGX_LIST_H_INCLUDED_ */
