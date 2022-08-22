#include <stdio.h>
#include "RESRC.h"
#include "UUID.h"
#include <regex.h>

static RESRC res;
#define UUID_LEN 32

void *func(void *arg)
{
    RESRC *r = (RESRC *)arg;
    RESRC_FILE *p = RESRC_select_path(r, "K:\\CCXXProgram\\RESRC\\out\\001");
    printf("%s cache:%d data:%s\n", __func__, p->stat, p->data.data);
    return NULL;
}
int main(int argc, char const *argv[])
{

    RESRC_create(&res, 1);

    RESRC_FILE_OPEN(&(res.uuid_seed), &(res.filelist[0]), "K:\\CCXXProgram\\RESRC\\out\\001", "rb");
    RESRC_FILE_cache(10, &(res.filelist[0]));

    pthread_t th;
    pthread_create(&th, NULL, func, &res);
    pthread_join(th,NULL);

    printf("%s cache:%d data:%s\n", __func__, res.filelist[0].stat, res.filelist[0].data.data);

    RESRC_FILE_CLOSE(&(res.filelist[0]));
    printf("%d\n", (res.filelist[0]).stat);

    return 0;
}
