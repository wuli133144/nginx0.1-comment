
/*
 * Copyright (C) Igor Sysoev
 */


#ifndef _NGX_CYCLE_H_INCLUDED_
#define _NGX_CYCLE_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>

/*2017-8-11 注释ngx_cycle_s
 这是ngx的核心数据结构，我从核心数据结构，向外讲解

*/
struct ngx_cycle_s {
    /*
     变态设计****conf_ctx
    */
     /*
      核心变量记录环境的参数
     */
    void           ****conf_ctx;
    /*
     内存池数据
    */
    ngx_pool_t        *pool;

    /*
     日志文件
    */
    ngx_log_t         *log;
    ngx_log_t         *new_log;
    /*
     ngx 核心数据结构的设计，我单独拿出来将
    */
   /*
    监听数组
   */
    ngx_array_t        listening;
    /*
    ngx_array_t
     路径数组
    */
    ngx_array_t        pathes;
    /*
     ngx_list_t打开文件的链表
    */
    ngx_list_t         open_files;

   /*
    连接池的总量
   */ 
    ngx_uint_t         connection_n;
   
    /*
    连接池的设计
    */
    ngx_connection_t  *connections;
    /*
    读写事件
    */
    ngx_event_t       *read_events;
    /*
    事件写事件
    */
    ngx_event_t       *write_events;
   
    ngx_cycle_t       *old_cycle;
    /*
    配置文件
    */
    ngx_str_t          conf_file;
    /*
     用户名
    */
    ngx_str_t          root;
};

/*
 nginx核心配置文件结构
 对应nginx.conf文件
*/
typedef struct {
     ngx_flag_t  daemon;
     ngx_flag_t  master;
      
    /*工作进程*/
     ngx_int_t   worker_processes;
    
    /*用户
    */
     ngx_uid_t   user;
     /*
      组
     */
     ngx_gid_t   group;
        /*进程的pid*/
     ngx_str_t   pid;

     ngx_str_t   newpid;

#if (NGX_THREADS)
     ngx_int_t   worker_threads;
     size_t      thread_stack_size;
#endif

} ngx_core_conf_t;


typedef struct {
     ngx_pool_t  *pool;   /* pcre's malloc() pool */
} ngx_core_tls_t;


ngx_cycle_t *ngx_init_cycle(ngx_cycle_t *old_cycle);

ngx_int_t ngx_create_pidfile(ngx_cycle_t *cycle, ngx_cycle_t *old_cycle);

void ngx_delete_pidfile(ngx_cycle_t *cycle);

void ngx_reopen_files(ngx_cycle_t *cycle, ngx_uid_t user);

ngx_pid_t ngx_exec_new_binary(ngx_cycle_t *cycle, char *const *argv);


extern volatile ngx_cycle_t  *ngx_cycle;
extern ngx_array_t            ngx_old_cycles;
extern ngx_module_t           ngx_core_module;
extern ngx_uint_t             ngx_test_config;
#if (NGX_THREADS)
extern ngx_tls_key_t          ngx_core_tls_key;
#endif


#endif /* _NGX_CYCLE_H_INCLUDED_ */
