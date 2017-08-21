
/*
 * Copyright (C) Igor Sysoev
 */


#ifndef _NGX_BUF_H_INCLUDED_
#define _NGX_BUF_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef void *            ngx_buf_tag_t;

typedef struct ngx_buf_s  ngx_buf_t;

struct ngx_buf_s {
    /*
     pos表示数据的位置
    */
    u_char          *pos;
    /*
     last使用的内存的最后一个字节
    */
    u_char          *last;
    /*
     文件偏移的位置seek()
    */
    off_t            file_pos;
    /*
     允许读取文件的位置pos
    */
    off_t            file_last;

    /*
     缓冲区类型 ，表示哪一个module的数据bufer加载
      module可以是http_module模块
    */
    int              type;

   /*
    缓冲区开始结束位置
   */
    u_char          *start;         /* start of buffer */
    u_char          *end;           /* end of buffer */

    /*
     buffer 的类型
    */
    ngx_buf_tag_t    tag;
    /*
     buffer和文件关联
    */
    ngx_file_t      *file;
    /*
     shadowfile没有啥用
    */
    ngx_buf_t       *shadow;


    /* the buf's content could be changed */
    unsigned         temporary:1;/*表示该内存区可以修改*/

    /*
     * the buf's content is in a memory cache or in a read only memory
     * and must not be changed
     */
    unsigned         memory:1;/*表示内存区不可修改*/

   
    /* the buf's content is mmap()ed and must not be changed */
    /*表示该内存区是mmap()映射区域不可修改*/
    unsigned         mmap:1;
   
   /*
    表示buffer是否可以回收
   */
    unsigned         recycled:1;
    /*
     是否是file
    */
    unsigned         in_file:1;
    /*
     是否fflush
    */
    unsigned         flush:1;
    /*
     是否是最后的缓冲区
    */
    unsigned         last_buf:1;

    unsigned         last_shadow:1;
    /*
     是否和临时文件关联
    */
    unsigned         temp_file:1;


    unsigned         zerocopy_busy:1;
    /*size of buffer*/

    /* STUB */ int   num;
};


typedef struct ngx_chain_s       ngx_chain_t;


/*
 nginx缓存链的结构

*/

struct ngx_chain_s {
    /*数据缓存区*/
    ngx_buf_t    *buf;
    /* 
     下一个buf节点
    */
    ngx_chain_t  *next;
};


/*
ngx_bufs_t
记录num大小
记录尺寸大小size
*/
typedef struct {
     /*num表示有多少个num块*/
    ngx_int_t    num;
    /*
     size每块内存区的大小
    */
    size_t       size;
} ngx_bufs_t;


/*
过滤器的代码
ngx_output_chain_filter_pt

   |——————|       |——————————|        |——————————|
-->|      |======>|          |=======>|          |======>null
   |——————|  |    |——————————|   |    |——————————|
             |                   |
             |(filter)           |(filter)

*/
typedef int  (*ngx_output_chain_filter_pt)(void *ctx, ngx_chain_t *out);

/*
 nginx 链上下文
*/
typedef struct {

    /*output buffer area*/
    ngx_buf_t                   *buf;
   
   /*输入链*/
   /*
                    ————————————|        ———————————————
                    |           |        |              |
   ======(output)===>   (in)    |=======>|              |==========>（null）
        buffer      |           |        |              |
                    |           |        |              |
                    ————————————         |——————————————
   
   */
    ngx_chain_t                 *in;

    ngx_chain_t                 *free;
    ngx_chain_t                 *busy;

    unsigned                     sendfile;
    unsigned                     need_in_memory;
    unsigned                     need_in_temp;
    /*
     内存池
    */

    ngx_pool_t                  *pool;
    ngx_int_t                    allocated;
    /*
     记录bufs的尺寸
    */
    ngx_bufs_t                   bufs;
     
    /*
     表示当前缓存挂在模块
    */
    ngx_buf_tag_t                tag;
    
    /*
     过滤器nb
    */
    ngx_output_chain_filter_pt   output_filter;
    void                        *filter_ctx;
} ngx_output_chain_ctx_t;

/*
 输出上下文
*/

/*
 out chain输出链
*/
typedef struct {
    ngx_chain_t                 *out;
    ngx_chain_t                **last;
    /*
     已连接套接字
    */
    ngx_connection_t            *connection;

    ngx_pool_t                  *pool;

    off_t                        limit;

} ngx_chain_writer_ctx_t;


#define NGX_CHAIN_ERROR     (ngx_chain_t *) NGX_ERROR


#define ngx_buf_in_memory(b)        (b->temporary || b->memory || b->mmap)
#define ngx_buf_in_memory_only(b)   (ngx_buf_in_memory(b) && !b->in_file)
#define ngx_buf_special(b)                                                   \
        ((b->flush || b->last_buf) && !ngx_buf_in_memory(b) && !b->in_file)

#define ngx_buf_size(b)                                                      \
        (ngx_buf_in_memory(b) ? (size_t) (b->last - b->pos):                 \
                                (size_t) (b->file_last - b->file_pos))

ngx_buf_t *ngx_create_temp_buf(ngx_pool_t *pool, size_t size);
ngx_chain_t *ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs);


#define ngx_alloc_buf(pool)  ngx_palloc(pool, sizeof(ngx_buf_t))
#define ngx_calloc_buf(pool) ngx_pcalloc(pool, sizeof(ngx_buf_t))


#define ngx_alloc_chain_link(pool) ngx_palloc(pool, sizeof(ngx_chain_t))


#define ngx_alloc_link_and_set_buf(chain, b, pool, error)                    \
            do {                                                             \
                ngx_test_null(chain, ngx_alloc_chain_link(pool), error);     \
                chain->buf = b;                                              \
                chain->next = NULL;                                          \
            } while (0);


#define ngx_chain_add_link(chain, last, cl)                                  \
            if (chain) {                                                     \
                *last = cl;                                                  \
            } else {                                                         \
                chain = cl;                                                  \
            }                                                                \
            last = &cl->next


ngx_int_t ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *in);
ngx_int_t ngx_chain_writer(void *data, ngx_chain_t *in);

ngx_int_t ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain,
                             ngx_chain_t *in);
void ngx_chain_update_chains(ngx_chain_t **free, ngx_chain_t **busy,
                             ngx_chain_t **out, ngx_buf_tag_t tag);


#endif /* _NGX_BUF_H_INCLUDED_ */
