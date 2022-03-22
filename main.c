#include<stdio.h>
#include<stdlib.h>
#include<sys/inotify.h>
#include<unistd.h>

#define MAX_EVENT_MONITOR 2048
#define NAME_LEN 32
#define MONITOR_EVENT_SIZE (sizeof(struct inotify_event))
#define BUFFER_LEN MAX_EVENT_MONITOR*(MONITOR_EVENT_SIZE+NAME_LEN)

int main(int argc, char const *argv[])
{
    int i = 0;
    int fd, watch_desc;
    char buffer[BUFFER_LEN];

    fd = inotify_init();
    if(fd<0){
        perror("Notify not inilialize");
    }

    watch_desc = inotify_add_watch(fd, "/home/test", IN_CREATE|IN_MODIFY|IN_DELETE|IN_ACCESS|IN_ATTRIB|IN_MOVE);

    if(watch_desc == -1){
        printf("Couldn't add watch to %s\n", "/home/test");
    }
    else{
        printf("Monitor added to path %s\n", "/home/test");
    }

    for(;;){
        i=0;
        int total_read = read(fd, buffer, BUFFER_LEN);
        if(total_read<0){
            perror("read error");
        }
           
        while(i<total_read){
            struct inotify_event *event=(struct inotify_event * ) &buffer[i];
                if(event-> len){
                    if(event -> mask & IN_CREATE){
                        if(event -> mask & IN_ISDIR){
                            printf("Directory \"%s\" was created\n", event -> name);
                        }
                        else{
                            printf("File \"%s\" was created\n", event -> name);
                        }
                    
                    }   
                    if(event -> mask & IN_MODIFY){
                        if(event -> mask & IN_ISDIR){
                            printf("Directory \"%s\" was modified\n", event -> name);
                        }   
                        else{
                            printf("File \"%s\" was modified\n", event -> name);
                        }
                    
                    }
                    if(event -> mask & IN_DELETE){
                        if(event -> mask & IN_ISDIR){
                            printf("Directory \"%s\" was deleted\n", event -> name);
                        }
                        else{
                            printf("File \"%s\" was deleted\n", event -> name);
                        }
                    }
                    if(event -> mask & IN_ACCESS){
                        if(event -> mask & IN_ISDIR){
                            printf("Directory \"%s\" was accessed\n", event -> name);
                        }
                        else{
                            printf("File \"%s\" was accessed\n", event -> name);
                        }
                    }
                    if(event -> mask & IN_ATTRIB){
                        if(event -> mask & IN_ISDIR){
                            printf("Directory metadata \"%s\" was changed\n", event -> name);
                        }
                        else{
                            printf("Directory metadata \"%s\" was changed\n", event -> name);
                        }
                    }
                    if(event -> mask & IN_MOVE){
                        if(event -> mask & IN_ISDIR){
                            printf("Directory \"%s\" was moved\n", event -> name);
                        }
                        else{
                            printf("File \"%s\" was moved\n", event -> name);
                        }
                    }
                i += MONITOR_EVENT_SIZE + event -> len;
            }
        }    
    }
    inotify_rm_watch(fd, watch_desc);
    close(fd);

    return(EXIT_SUCCESS);
}