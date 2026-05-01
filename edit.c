#include "mp3.h"

static unsigned int sync_safe(unsigned char *str)
{
    return ((str[3] | (str[2]<<7) | (str[1]<<14) | (str[0]<<21)));  
}

static unsigned int head_(FILE *fp)
{
    unsigned char buf[4];
    fseek(fp,6,SEEK_SET);
    fread(buf,1,4,fp);
    return sync_safe(buf);
}

void edit_mp3(int i,FILE *fp,char *change,unsigned int head_size)
{
    
    char *fid[6]={"TIT2","TPE1","TALB","TYER","TCON","TCOM"};
    
    fseek(fp,10,SEEK_SET);

    unsigned char frame_hd[5],buf[8];
        
    while(ftell(fp)<(head_size+10))
    {
        fread(buf,1,4,fp);

        buf[4]='\0';

        strcpy(frame_hd,buf);

        fread(buf,1,4,fp);

        unsigned int size=buf[3] | (buf[2]<<8) | (buf[1]<<16) | (buf[0]<<24);

        if(size<=0 || frame_hd[0]=='\0')
            break;

        fseek(fp,2,SEEK_CUR);

        if(strcmp(frame_hd,fid[i])==0)
        {
            unsigned char enc=0;
            fread(&enc,1,1,fp);
            char *arr=calloc(size,1);
            fseek(fp,-1,SEEK_CUR);
                fwrite(&enc,1,1,fp);

                strncpy(arr,change,size-1);
                arr[size-2]='\0';
                
                fwrite(arr,1,size-1,fp);
                free(arr);
                return;
        }
        else
            fseek(fp,size,SEEK_CUR);
    }

}

void print(int i)
{
    char buff[75];
    memset(buff,'=',74);
    buff[74]='\0';
    printf("\n%25s  MP3 EDIT MENU\n","");
    printf("%25s|%s|\n\n","",buff);
    
    switch(i)
    {
        case 0:
        printf("%25s  EDITED TITLE TRACK SUCCESSFULLY\n","");
        printf("\n%25s|%s|\n\n","",buff);
        break;

        case 1:
        printf("%25s  EDITED ARTIST NAME SUCCESSFULLY\n","");
        printf("\n%25s|%s|\n\n","",buff);
        break;

        case 2:
        printf("%25s  EDITED ALBUM NAME SUCCESSFULLY\n","");
        printf("\n%25s|%s|\n\n","",buff);
        break;

        case 3:
        printf("%25s  EDITED YEAR FIELD SUCCESSFULLY\n","");
        printf("\n%25s|%s|\n\n","",buff);
        break;

        case 4:
        printf("%25s  EDITED GENRE FIELD SUCCESSFULLY\n","");
        printf("\n%25s|%s|\n\n","",buff);
        break;

        case 5:
        printf("%25s  EDITED COMPOSER NAME SUCCESSFULLY\n","");
        printf("\n%25s|%s|\n\n","",buff);
        break;
    }
}

int edit(char *fname,char *option,int argc,char *change)
{
    if(argc<=4 || (strstr(fname,".mp3"))==NULL)
        return -1;

    FILE *fp=fopen(fname,"r+");

    if(fp==NULL)
        return -2;

    char buf[4];

    fread(buf,1,3,fp);

    buf[3]='\0';

    if(strcmp(buf,"ID3")!=0)
    {
        fclose(fp);
        return -2;
    }

    unsigned int head=head_(fp);

    char *str[6]={"-t","-a","-A","-y","-g","-c"};

    for(int i=0;i<6;i++)
    {
        if(strcmp(str[i],option)==0)
        {
            edit_mp3(i,fp,change,head);
            print(i);
        }
    }

    fclose(fp);
    return 1;
    
}