#include "open_source.h"


#define STRIP_LENGTH 1024
#define SWAP_LENGTH 4
#define MAIN_SIGNATURE "MJ01"
#define DM_NONE 0
#define DM_MJ01 1
#define DM_MJ02 2
#define DM_MJ03 3
#define DM_MJ04 4
#define DM_IDENTIFY 0xff
#define SIGNATURE_LENGTH    4
#define SIGNATURE_KIND 4




int fd_read(struct Files *file, void *buf, size_t length)
{
    int rc=0;
    int FibonacciNumbers[] = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987};
    int JoeNumbers[] = {987, 610, 1, 1, 377, 233, 2, 3, 144, 89, 5, 8, 55, 34, 13, 21};
    char *ptr;
    char *ptr1;
    char *ptr2;
    int     BytesRead=0;
    char swap[SWAP_LENGTH];
    int i=0,j=0;
	char tmp=0;
	char *data=NULL;
    int CopyBytes=0;
    ptr = buf;
    do
    {
        if (file->CacheFullness == 0)
        {
            BytesRead = read(file->fd, file->Buffer,STRIP_LENGTH*2);
            if (BytesRead == -1)
            {
                rc = -1;
                BytesRead=0;
                break;
            }
            if (BytesRead == 0)
            {
                BytesRead=0;
                break;
            }

            if (BytesRead == (STRIP_LENGTH * 2) && file->CurrentFilePtr != 0)
            {
                switch (file->Decryptor_Enable)
                {
                case DM_MJ01:
                        /*printf("1 ==================\n");*/
                        ptr1 = file->Buffer;
                        ptr2 = ptr1 + STRIP_LENGTH + SWAP_LENGTH;
                        for (i = 0; i < (STRIP_LENGTH / (SWAP_LENGTH * 2)); i++)
                        {
                                memcpy (swap, ptr1, SWAP_LENGTH);
                                memcpy (ptr1, ptr2, SWAP_LENGTH);
                                memcpy (ptr2, swap, SWAP_LENGTH);
                                ptr1 += (SWAP_LENGTH * 2);
                                ptr2 += (SWAP_LENGTH * 2);
                        }
                        break;
                case DM_MJ02:
                        /*printf("2 ==================\n");*/
                        ptr1 = file->Buffer;
                        ptr2 = ptr1 + STRIP_LENGTH * 2 - SWAP_LENGTH;
                        for (i = 0; i < (STRIP_LENGTH / (SWAP_LENGTH * 2)); i++)
                        {
                                memcpy (swap, ptr1, SWAP_LENGTH);
                                memcpy (ptr1, ptr2, SWAP_LENGTH);
                                memcpy (ptr2, swap, SWAP_LENGTH);
                                *ptr2 ^= *ptr1;
                                ptr1 += (SWAP_LENGTH * 2);
                                ptr2 -= (SWAP_LENGTH * 2);
                        }
                        break;
                case DM_MJ03:
                        /*printf("3 ==================\n");*/
                        ptr1 = file->Buffer;
                        ptr2 = file->Buffer + STRIP_LENGTH;
                        for (j = (sizeof (FibonacciNumbers) / sizeof (FibonacciNumbers[0])) - 1; j >= 0; j--) {
                                ptr1[FibonacciNumbers[j]] ^= file->XORValue[j % 4];
                                ptr1[FibonacciNumbers[j]] ^= ptr2[JoeNumbers[j]];
                        }
                        break;
                case DM_MJ04:
                        //printf("4 ==================\n");
						data = file->Buffer;
			            for(i=0;i<64;i++)
						{
							tmp=data[i+1024];data[i+1024]=data[i+1088];data[i+1088]=tmp;
						}
					    for(i=0;i<64;i++)
						{
							tmp=data[i];data[i]=data[i+64];data[i+64]=tmp;
						}
					    for(i=0;i<1024;i++)
						{
							tmp=data[i];data[i]=data[i+1024];data[i+1024]=tmp;
						}
                        break;
                }
			}
			else if(file->Decryptor_Enable==DM_MJ04 && BytesRead == (STRIP_LENGTH * 2))
			{
				//printf("4 ==================\n");
				data = file->Buffer;
			    for(i=0;i<64;i++)
				{
					tmp=data[i+1024];data[i+1024]=data[i+1088];data[i+1088]=tmp;
				}
			    for(i=0;i<64;i++)
				{
					tmp=data[i];data[i]=data[i+64];data[i+64]=tmp;
				}
			    for(i=0;i<1024;i++)
				{
					tmp=data[i];data[i]=data[i+1024];data[i+1024]=tmp;
				}

			}
            file->CurrentFilePtr += BytesRead;
            file->CacheFullness = BytesRead;
        }
        CopyBytes = ((unsigned int) (rc + file->CacheFullness - file->CachePtr) > length) ?
                                                    ((int)length - rc) : (file->CacheFullness - file->CachePtr);
        memcpy (ptr, (file->Buffer + file->CachePtr), CopyBytes);
        file->CachePtr += CopyBytes;
        file->VirtualFilePtr += CopyBytes;
        rc += CopyBytes;
        ptr += CopyBytes;
        if (file->CachePtr == file->CacheFullness)
        {
            file->CacheFullness = 0;
        }
        file->CachePtr %= (2 * STRIP_LENGTH);
    } while ((size_t) rc < length);

    return rc;
}
int fd_open(struct Files * file,char *path)
{
    unsigned char AllSignatures[SIGNATURE_KIND][SIGNATURE_LENGTH+1] =
		    {"MJ01", "MJ02", "MJ03","cwms"};
    long long tmp=0;
    ssize_t all_count=0;
    int i=0;
    char buf[8];
    file->fd=open(path,O_RDONLY | O_LARGEFILE);
    printf("%s\n",path);
    if(file->fd==-1)
    {
        fprintf(stderr,"[%s,%d]", __FILE__, __LINE__);
        perror("");
    }
    tmp=lseek64(file->fd,-8,SEEK_END);
    if(tmp < 0)
    {
        fprintf(stderr,"[%s,%d]", __FILE__, __LINE__);
        perror("");
    }
    else
    {
        all_count=read(file->fd,buf,8);
        if(all_count != 8)
        {
            printf("[%s,%d]", __FILE__, __LINE__);
            perror("");
        }
        for(i=0;i<SIGNATURE_KIND;i++)
        {
			if(i==3) {char *m=buf+4;m[0]=m[0]+'a';m[1]=m[1]+'a';m[2]=m[2]+'a';m[3]=m[3]+'a';}
			if(memcmp(buf+4,AllSignatures[i],4) == 0)
            {
                printf("FLAGS = [%s]\n",AllSignatures[i]);
                file->Decryptor_Enable=i+1;
                if((i + 1) == DM_MJ03)
                {
                    memcpy(file->XORValue,buf,4);
                }
                break;
            }
        }

        if(i >= SIGNATURE_KIND)
        {
            printf("FLAG NONE\n");
            file->File_Size=tmp+8;
            file->Decryptor_Enable=0;
        }
        else
        {
            file->File_Size=tmp+4;
        }
        file->CachePtr = 0;
        file->CacheFullness = 0;
        file->CurrentFilePtr = 0;
        file->VirtualFilePtr = 0;
        tmp=lseek(file->fd,0,SEEK_SET);
        if(tmp < 0)
        {
            printf("[%s,%d]", __FILE__, __LINE__);
            perror("");
        }
    }
    return tmp;
}
void fd_close(struct Files *file)
{
	close(file->fd);
    return;
}
long long fd_seek(struct Files *file, long long pos, int whence)
{
    int fd = file->fd;
    long long off=0;
    long long retval = 0;
    if(whence != SEEK_SET && whence != SEEK_CUR && whence != SEEK_END)
    {
		return -1;
    }
    retval = file->VirtualFilePtr;
    switch (whence)
    {
    case SEEK_SET:
            file->VirtualFilePtr = pos;
            break;
    case SEEK_CUR:
            file->VirtualFilePtr += pos;
            break;
    case SEEK_END:
            file->VirtualFilePtr = file->File_Size + pos;
            break;
    }

    if ((file->VirtualFilePtr > file->File_Size) || (file->VirtualFilePtr < 0))
    {
        file->VirtualFilePtr = retval;
        off = -1;
    }
    else
    {
        if((file->VirtualFilePtr < file->CurrentFilePtr
			&& file->VirtualFilePtr >= (file->CurrentFilePtr - 2048)
        )
        && file->CacheFullness)
        {
            file->CachePtr = file->VirtualFilePtr % 2048;
            off = file->VirtualFilePtr;
        }
        else
        {
            file->CurrentFilePtr = ((file->VirtualFilePtr >> 11) << 11);
            file->CacheFullness = 0;
            file->CachePtr = file->VirtualFilePtr % 2048;
            off = file->VirtualFilePtr;
		    lseek(fd,file->CurrentFilePtr,SEEK_SET);
        }
    }
    return off;
}
