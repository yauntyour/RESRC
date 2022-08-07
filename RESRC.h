#ifndef __APP_RES__H__
#define __APP_RES__H__
#include <stdio.h>
#include <pthread.h>

#include "sm3.h"
#include "FIO/Asfio.h"
#include "UUID.h"

#define NULL_STAT 0
#define ONLINE_STAT 1
#define UNDERLINE_STAT -1
#define FREE_STAT -2
#define CLOSE_STAT -3

#define ERROR_RESRC ~0
static pthread_t *Thread_queue;

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

    typedef struct _FILE_PATH
    {
        char *path, *mode;
    } FILE_PATH;
    
    /*File的内容*/
    typedef struct _RESRC_FILE
    {
        FILE_PATH path;
        unsigned char hash[32],UUID[32];
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

    typedef struct _TIMEOUT
    {
        size_t ms;
        RESRC_FILE *file;
    } TIMEOUT;

    int RESRC_TIMEOUT(size_t ms, RESRC_FILE *file)
    {
#ifdef _WIN32
        _sleep(ms);
#else
    sleep(ms);
#endif
        free(file->data.data);
        file->data.length = 0;
        file->stat = UNDERLINE_STAT;
        return UNDERLINE_STAT;
    }

    int RESRC_init(size_t Thread_queue_MAX)
    {
        Thread_queue = (pthread_t *)calloc(Thread_queue_MAX, sizeof(pthread_t));
        memset(Thread_queue, 0, Thread_queue_MAX);
    }
    size_t RESRC_create(RESRC *p, size_t _NumOfRESRC)
    {
        (*p).filelist = (RESRC_FILE *)calloc(_NumOfRESRC, sizeof(RESRC_FILE));
        if ((*p).filelist != NULL)
        {
            memset((*p).filelist, 0, _NumOfRESRC);
            (*p).Number = _NumOfRESRC;
            (*p).uuid_seed = 0;
            for (size_t i = 0; i < _NumOfRESRC; i++)
            {
                (*p).filelist[i].stat = NULL_STAT;
            }
            return _NumOfRESRC;
        }
        return -1;
    }

    /*file为指向对应RESRC_FILE的指针，并非数组*/
    int RESRC_FILE_OPEN(UUID_t *p,RESRC_FILE *file, char *filepath, char *mode)
    {
        file->asp = Asfio_create(filepath, mode);
        file->path.path = filepath;
        file->path.mode = mode;
        file->stat = NULL_STAT;
        UUID(file->UUID,32,p);
        return 0;
    }
    int RESRC_FILE_CLOSE(RESRC_FILE *file)
    {
        Asfio_close(&file->asp, 0);
        file->stat = CLOSE_STAT;
        return 0;
    }

    void *func_cache(void *arg)
    {
        TIMEOUT *t = (TIMEOUT *)arg;
        RESRC_TIMEOUT(t->ms,t->file);

    }

    int RESRC_FILE_cache(size_t timeout, RESRC_FILE *file)
    {
        if (file->stat == FREE_STAT)
        {
            return FREE_STAT;
        }
        else
        {
            if (Asfio_readcall(&(file->asp), &(file->data)) != 0)
            {
                return file->data.length;
                file->stat = ONLINE_STAT;
            }
            else
            {
                return ERROR_RESRC;
            };
        }
        pthread_t th;
        TIMEOUT t;
        t.ms = timeout;
        t.file = file;
        pthread_create(&th, NULL,func_cache,&t);
        return ONLINE_STAT;
    }

    int RESRC_free(RESRC_FILE *file)
    {
        if (file->stat == FREE_STAT)
        {
            return FREE_STAT;
        }
        else
        {
            free(file->data.data);
            file->data.length = 0;
            file->stat = FREE_STAT;
            return file->stat;
        }
    }
#ifdef __cplusplus
}
#endif //__cplusplus
#endif //!__APP_RES__H__