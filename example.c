#include <stdio.h>
#include "RESRC.h"
#include "UUID.h"

static RESRC res;
#define UUID_LEN 32

int main(int argc, char const *argv[])
{
    RESRC_create(&res, 1);

    RESRC_FILE_OPEN(&(res.uuid_seed), &(res.filelist[0]), "K:\\CCXXProgram\\RESRC\\out\\001", "rb");

    RESRC_FILE_cache(100, &(res.filelist[0]));
    
    printf("cache:%d data:%s\n", res.filelist[0].stat, res.filelist[0].data.data);
    RESRC_FILE_CLOSE(&(res.filelist[0]));
    printf("%d\n", (res.filelist[0]).stat);
    return 0;
}
