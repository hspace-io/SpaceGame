//gcc -o secret_file main.c -lfuse -D_FILE_OFFSET_BITS=64 -fstack-protector
#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/statvfs.h>
#include <pthread.h>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <linux/stat.h>

struct virtual_file{
    char* path;
    mode_t mode;
    size_t size;
    off_t offset;
    void* data;
    int opened;
} typedef virtual_file;

virtual_file* file_list[20];
char flag[] = "hspace{fUse_iS_$imp13_AnD_PoweRFul1!}";

static virtual_file* get_file(const char* path){
    virtual_file* file = NULL;
    for (int i = 0; i < 20; i++){
        if (file_list[i] && !strcmp(file_list[i]->path, path)){
            file = file_list[i];
            break;
        }
    }
    return file;
}

static int virtual_getattr(const char* path, struct stat *stbuf){        
    memset(stbuf, 0, sizeof(struct stat));
    if (!strcmp(path, "/")){
            stbuf->st_nlink = 0; 
            stbuf->st_uid = 1000;
            stbuf->st_gid = 1000;
            stbuf->st_mode = __S_IFDIR | 0755;      
            return 0;      
    }
    for (int i = 0; i < 20; i++){
        if (file_list[i] == NULL)
            continue;  
        if (!strcmp(path, file_list[i]->path)){
            stbuf->st_mode = __S_IFREG | file_list[i]->mode;
            stbuf->st_size = file_list[i]->size;
            stbuf->st_nlink = 1;             
            stbuf->st_uid = 1000;
            stbuf->st_atime = time(NULL);            
            stbuf->st_gid = 1000;               
            return 0;
        }    
    }    
    return -ENOENT;
}

static int virtual_create(const char* path, mode_t mode, struct fuse_file_info *fi){
    for (int i = 0; i < 20; i++){
        if (file_list[i] && !strcmp(file_list[i]->path, path))
            return -EEXIST;
    }    
    virtual_file* file = (virtual_file*)malloc(sizeof(virtual_file));
    file->path = malloc(strlen(path) + 1);
    strncpy(file->path, path, strlen(path) + 1);
    file->mode = 0666;
    file->size = 0x100;
    file->offset = 0;
    file->opened = 1;
    file->data = malloc(file->size);
    for (int i = 0; i < 20; i++){
        if (!file_list[i]){
            file_list[i] = file;            
            return 0;
        }
    }
    return -ENOMEM;
}

static int virtual_readdir(const char* path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){    
    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    if (!strcmp(path, "/")){
        for (int i = 0; i < 20; i++){
            if (file_list[i]){
                puts(file_list[i]->path);
                filler(buf, file_list[i]->path + 1, NULL, 0);
            }
        }
        return 0;
    }
    else{
        return -ENOTDIR;
    }    
}

static int virtual_write(const char* path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
    virtual_file* file = get_file(path);
    if (!file || !file->opened)
        return -ENOENT;
    if (size > file->size - file->offset){
        size_t origin = file->size;
        file->size = size;
        if (file->size >= 0x5000){
            file->size = origin;
            return -ENOMEM;
        }                
        file->data = realloc(file->data, file->size);
    }    
    memcpy(file->data + file->offset, buf, size);
    file->offset += size;
    return size;
}

static int virtual_read(const char* path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
    virtual_file* file = get_file(path);
    if (!file || !file->opened)
        return -ENOENT;
    memcpy(buf, file->data + file->offset, file->size - file->offset > size ? size : file->size - file->offset);
    int result =  file->size - file->offset > size ? size : file->size - file->offset;
    file->offset += result;
    return result;
}

static int virtual_close(const char* path, struct fuse_file_info *fi){
    virtual_file* file = get_file(path);
    if (!file || !file->opened)
        return -ENOENT;
    file->offset = 0;  
    file->opened = 0;  
    return 0;
}

static int virtual_truncate(const char* path, off_t off){
    virtual_file* file = get_file(path);
    if (!file)
        return -ENOENT;
    else if (!(file->mode & 0400))
        return -EPERM;
    file->data = realloc(file->data, off); 
    file->size = off;
    if (off == 0){
        for (int i = 0; i < 20; i++){
            if (file_list[i] && !strcmp(file_list[i]->path, path)){
                free(file_list[i]);
                file_list[i] = NULL;
                break;
            }
        }
    }
    return 0;
}

static int virtual_unlink(const char* path){
     for (int i = 0; i < 20; i++){
        if (file_list[i] && !strcmp(file_list[i]->path, path)){
            if (!(file_list[i]->mode & 0400)){
                return -EACCES;
            }
            free(file_list[i]->data);
            free(file_list[i]);
            file_list[i] = NULL;            
            return 0;
        }
    }
}

static int virtual_open(const char* path, struct fuse_file_info *fi){
    virtual_file* file = get_file(path);
    if (!file)
        return -ENOENT;    
    if (!(file->mode & 0400)){
        return -EACCES;
    }
    file->opened = 1;
    return 0;
}

static int virtual_untimes(const char*path, const struct timespec tv[2]){
    return 0;
}

static struct fuse_operations oper = {
    .getattr = virtual_getattr,      
    .readdir = virtual_readdir,
    .write = virtual_write,
    .create = virtual_create,
    .open = virtual_open,
    .read = virtual_read,
    .release = virtual_close,
    .truncate = virtual_truncate,        
    .unlink = virtual_unlink,    
    .utimens = virtual_untimes,
};

int main(int argc, char *argv[])
{
    setbuf(stdout, 0);
    virtual_file* file = (virtual_file*)malloc(sizeof(virtual_file));
    file->data = flag;
    file->size = 0x30;
    file->opened = 0;
    file->mode = 0;
    file->path = "/flag";
    file_list[0] = file;
    return fuse_main(argc, argv, &oper, NULL);    
}
