
/*
 * Copyright (C) Igor Sysoev
 */


#ifndef _NGX_ARRAY_H_INCLUDED_
#define _NGX_ARRAY_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>

/*注释array */
struct ngx_array_s {
    /*当前array的首地址*/
    void        *elts;
    /*总共有多少的元素个数*/

    /*
    nelts表示array当前实际的元素
    */

    ngx_uint_t   nelts;
    /*实际大小*/
    size_t       size;
    /*元素个数*/
    ngx_uint_t   nalloc;
    /*内存池的*/
    ngx_pool_t  *pool;
};


ngx_array_t *ngx_create_array(ngx_pool_t *p, ngx_uint_t n, size_t size);
void ngx_destroy_array(ngx_array_t *a);
void *ngx_push_array(ngx_array_t *a);


ngx_inline static ngx_int_t ngx_array_init(ngx_array_t *array, ngx_pool_t *pool,
                                           ngx_uint_t n, size_t size)
{    
    /*ngx_palloc*/
    if (!(array->elts = ngx_palloc(pool, n * size))) {
        return NGX_ERROR;
    }
    
    /*当前元素个数*/
    array->nelts = 0;
    /*实际尺寸*/
    array->size = size;
    /*已分配array块*/
    array->nalloc = n;
    /*内存池对象*/
    array->pool = pool;

    return NGX_OK;
}



#define ngx_init_array(a, p, n, s, rc)                                       \
    ngx_test_null(a.elts, ngx_palloc(p, n * s), rc);                         \
    a.nelts = 0; a.size = s; a.nalloc = n; a.pool = p;

#define ngx_array_create  ngx_create_array
#define ngx_array_push    ngx_push_array


#endif /* _NGX_ARRAY_H_INCLUDED_ */
