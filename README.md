
#about
一直在注释nginx0.1源码但是没有发布到github上


今天首先发布nginx内存管理和核心数据结构的部分源代码

nginx 内存管理部分是利用了一个内存池，一猜猜到用内存池，不然内存资源如何管理都是问题；
nginx0.1内存池设计结构和前面我设计的一个内存结构相似参考mem_pool的设计


数据结构比较多，只是注释了array，其他类似，代码里添加了注释
