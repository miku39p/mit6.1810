#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find_subset(char* path, char* pattern) {
    char buf[512], * p;
    int fd = open(path, 0);
    if (fd < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    struct dirent de;
    struct stat st;

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_DEVICE:
        break;
    case T_FILE:
        //try to find the last "/" 
        p = path;
        int last_slash_index = 0;
        for(int i = 0; i < strlen(path); i++) {
            if (path[i] == '/'){
                //get the first char after last '/'
                last_slash_index = i +1;
            }
        }
        // if find the last "/" 
        if (last_slash_index) {
            // copy the name
            p = buf;
            for(int i = last_slash_index; i < strlen(path); i++) {
                *p = path[i];
                p++;
            }
            // add the end char 0
            *p=0;
            // compare
            if(strcmp(buf, pattern) == 0) {
                printf("%s\n",path);
            }
        }
        // doesn't find the / compare all the path
        else {
            if(strcmp(path, pattern) == 0) {
                printf("%s\n", path);
            }
        }
        break; 

    case T_DIR:
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if(de.inum == 0)
                continue;
            //ignore . and .. folder
            if ((strcmp(de.name, ".") == 0) || (strcmp(de.name, "..") == 0)){
                continue;
            }
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            //recursively
            find_subset(buf, pattern);
        }
        break;

    default:
        break;
    }
    
    close(fd);
    return;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("arg number error\n");
        exit(0);
    }
    find_subset(argv[1], argv[2]);
    exit(0);
}
