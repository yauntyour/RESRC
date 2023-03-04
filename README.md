# RESRC 资源池

集中性管理资源，一次加载在各处调用

基本使用:

```c++
#include <stdio.h>
#include "RESRC.h"
#include "UUID.h"
#include <regex.h>

static RESRC res;
#define UUID_LEN 32
//文件的UUID
UUID_ptr u;

void *func(void *arg)
{
    RESRC *r = (RESRC *)arg;
    
    //使用路径搜索
    RESRC_FILE *p = RESRC_select_path(r, "K:\\CCXXProgram\\RESRC\\out\\001");
    printf("%s cache:%d data:%s\n", __func__, p->stat, p->data.data);
    
    //使用UUID搜索
    RESRC_FILE *p2 = RESRC_select_UUID(r,u);
    printf("%s cache:%d data:%s\n", __func__, p2->stat, p2->data.data);
    
    return NULL;
}
int main(int argc, char const *argv[])
{
     //创建一个资源池并设置大小+初始化
    RESRC_create(&res, 1);

    //打开文件
    u =  RESRC_FILE_OPEN(&(res.uuid_seed), &(res.filelist[0]), "K:\\CCXXProgram\\RESRC\\out\\001", "rb");
    //缓存文件内容（全加载）
    RESRC_FILE_cache(&(res.filelist[0]));

    pthread_t th;
    pthread_create(&th, NULL, func, &res);
    pthread_join(th,NULL);

    //关闭文件指针，不会释放缓存
    RESRC_FILE_CLOSE(&(res.filelist[0]));
    printf("%s cache:%d data:%s\n", __func__, res.filelist[0].stat, res.filelist[0].data.data);

    //释放缓存的文件
    RESRC_FILE_free(&(res.filelist[0]));
    printf("%s cache:%d data:%s\n", __func__, res.filelist[0].stat, res.filelist[0].data.data);

    RESRC_free(&res);
    return 0;
}

 ```

RESRC_FILE的结构:

```c++

    typedef struct _FILE_PATH
    {
        char *path, *mode;
    } FILE_PATH;

    /*File的内容*/
    typedef struct _RESRC_FILE
    {
        FILE_PATH path;
        unsigned char hash[32], UUID[32];
        bytes data;
        Asfio asp;
        int stat;
    } RESRC_FILE;

    typedef struct _RESRC
    {
        RESRC_FILE *filelist;
        size_t Number;
        UUID_t uuid_seed;
    } RESRC;

```
