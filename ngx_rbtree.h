
/*
 * Copyright (C) Igor Sysoev
 */


#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>

/**************************************************************************
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/
/*
  nginx的红黑树实现开始比较中规中矩的，可以参考
*/

typedef struct ngx_rbtree_s  ngx_rbtree_t;

struct ngx_rbtree_s {
   ngx_int_t       key;
   ngx_rbtree_t   *left;
   ngx_rbtree_t   *right;
   ngx_rbtree_t   *parent;
   char            color;
};


void ngx_rbtree_insert(ngx_rbtree_t **root, ngx_rbtree_t *sentinel,
                       ngx_rbtree_t *node);
void ngx_rbtree_delete(ngx_rbtree_t **root, ngx_rbtree_t *sentinel,
                       ngx_rbtree_t *node);



/*找到最小的节点*/
ngx_inline static ngx_rbtree_t *ngx_rbtree_min(ngx_rbtree_t *node,
                                               ngx_rbtree_t *sentinel)
{
   while (node->left != sentinel) {
       node = node->left;
   }

   return node;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
