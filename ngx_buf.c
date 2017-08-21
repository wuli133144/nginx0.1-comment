
/*
 * Copyright (C) Igor Sysoev
 */


#include <ngx_config.h>
#include <ngx_core.h>

/*创建临时缓冲区

 
*/
ngx_buf_t *ngx_create_temp_buf(ngx_pool_t *pool, size_t size)
{
    ngx_buf_t *b;
     /*
        ngx_calloc_buf（）
        创建buf对象
      */
    if (!(b = ngx_calloc_buf(pool))) {
        return NULL;
    }
    /* 
      创建缓冲区开始位置
    */
    if (!(b->start = ngx_palloc(pool, size))) {
        return NULL;
    }

    b->pos = b->start;
    b->last = b->start;
    b->end = b->last + size;
    b->temporary = 1;/*临时的内存区*/

    /*

    b->file_pos = 0;
    b->file_last = 0;

    b->file = NULL;
    b->shadow = NULL;

    b->tag = 0;

     */

    return b;
}

/*
 创建连续的数组
 ngx_pool_t pool ngx_bufs_t
 并且将数组挂在chain_link上
*/
ngx_chain_t *ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    /* 
     ngx_chain实际就是一个链表结构 
      {
          ngx_buf_t a;
          ngx_chain *next;
      }
    */

    ngx_chain_t  *chain, *cl, **ll;
    
    /*
     在内存池pool申请num*size个空间大小

    */
    if (!(p = ngx_palloc(pool, bufs->num * bufs->size))) {
        return NULL;
    }

    ll = &chain;

    for (i = 0; i < bufs->num; i++) {
        if (!(b = ngx_calloc_buf(pool))) {
            return NULL;
        }
        /*
         典型的将p挂在到b->pos上
         设置操作的首地址为p
        */
        b->pos = p;
        b->last = p;
        b->temporary = 1;
        b->start = p;

        p += bufs->size; 
        /*p要后移几个位置*/

        b->end = p;
        /* 
         创建一个chain_link
        */
        if (!(cl = ngx_alloc_chain_link(pool))) {
            return NULL;
        }
        /* 
         将一个数组挂到chain上
        */
        cl->buf = b;
        /* 
         *ll=c1
        */
        *ll = cl;
        ll = &cl->next;
    }

    *ll = NULL;

    return chain;
}

/*
 chain 拷贝数据

 功能：将chain中的数据直接加到chain in数组中
*/
ngx_int_t ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain,
                             ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;
    /*其中chain是一个二级指针
     chain 
    */
    /*轮循找到具体的
     chain节点
    */
    for (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }
    
    /*
      将in中的数据逐一的拷贝到cl中
      注意：chain是一个链结构
    */
    while (in) {
        ngx_test_null(cl, ngx_alloc_chain_link(pool), NGX_ERROR);

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->next;
        in = in->next;
    }

    *ll = NULL;

    return NGX_OK;
}


/*
 释放节点

 将busy链中的节点放到free节点中释放掉
*/
void ngx_chain_update_chains(ngx_chain_t **free, ngx_chain_t **busy,
                             ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *tl;
   
    if (*busy == NULL) {
        *busy = *out;

    } else {
        for (tl = *busy; /* void */ ; tl = tl->next) {
            if (tl->next == NULL) {
                tl->next = *out;
                break;
            }
        }
    }

    *out = NULL;

    while (*busy) {
        if (ngx_buf_size((*busy)->buf) != 0) {
            break;
        }

#if (HAVE_WRITE_ZEROCOPY)
        if ((*busy)->buf->zerocopy_busy) {
            break;
        }
#endif
       /*类型不同*/

        if ((*busy)->buf->tag != tag) {
            *busy = (*busy)->next;
            continue;
        }

        (*busy)->buf->pos = (*busy)->buf->last = (*busy)->buf->start;
        
        tl = *busy;
        *busy = (*busy)->next;
        tl->next = *free;
        *free = tl;
    }
}
