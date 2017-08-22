
/*
 * Copyright (C) Igor Sysoev
 */


#ifndef _NGX_FILE_H_INCLUDED_
#define _NGX_FILE_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>

typedef struct ngx_path_s  ngx_path_t;

#include <ngx_garbage_collector.h>




/*注释nginx：
2017年 8-1710:17
jackwu
*/

/*
 ngx_file_s的设计
*/
struct ngx_file_s {
    /*
     文件描述符
    */
    ngx_fd_t         fd;
    /*
     字符串使用
    */
    ngx_str_t        name;
    /*
     文件信息 其实就是os.stat() python
       c里面 stat(filepath,st_struct *)
    */
    ngx_file_info_t  info;
    /*
     文件偏移位置
    */
    off_t            offset;
    /*
     系统文件偏移位置
    */
    off_t            sys_offset;
    /*log info_t*/
    ngx_log_t       *log;
    unsigned         info_valid:1;
};

#define NGX_MAX_PATH_LEVEL  3
/*
nginx路径名
*/
struct ngx_path_s {
    ngx_str_t           name;
    u_int               len;
    u_int               level[3];
    ngx_gc_handler_pt   gc_handler;
};

 /*nginx 临时文件
  各字段的的属性 和ngx_file_s的属性一样
 */
typedef struct {
    ngx_file_t   file;
    off_t        offset;
    ngx_path_t  *path;
    ngx_pool_t  *pool;
    char        *warn;

    unsigned     persistent:1;
} ngx_temp_file_t;

/*
从chain链写到temp_file文件中
*/
int ngx_write_chain_to_temp_file(ngx_temp_file_t *tf, ngx_chain_t *chain);
/*
创建临时文件
*/
int ngx_create_temp_file(ngx_file_t *file, ngx_path_t *path,
                         ngx_pool_t *pool, int persistent);
/*hash file*/
void ngx_create_hashed_filename(ngx_file_t *file, ngx_path_t *path);
int ngx_create_path(ngx_file_t *file, ngx_path_t *path);

void ngx_init_temp_number();
ngx_uint_t ngx_next_temp_number(ngx_uint_t collision);


char *ngx_conf_set_path_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

#define ngx_conf_merge_path_value(conf, prev, path, l1, l2, l3, pool)        \
    if (conf == NULL) {                                                      \
        if (prev == NULL) {                                                  \
            ngx_test_null(conf, ngx_palloc(pool, sizeof(ngx_path_t)), NULL); \
            conf->name.len = sizeof(path) - 1;                               \
            conf->name.data = (u_char *) path;                               \
            conf->level[0] = l1;                                             \
            conf->level[1] = l2;                                             \
            conf->level[2] = l3;                                             \
            conf->len = l1 + l2 + l3 + (l1 ? 1:0) + (l2 ? 1:0) + (l3 ? 1:0); \
        } else {                                                             \
            conf = prev;                                                     \
        }                                                                    \
    }



#endif /* _NGX_FILE_H_INCLUDED_ */
