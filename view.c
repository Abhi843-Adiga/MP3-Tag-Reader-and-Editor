#include"mp3.h"

unsigned int head_size;

static unsigned int sync_safe(unsigned char *str)
{
    return ((str[3] | (str[2]<<7) | (str[1]<<14) | (str[0]<<21)));  //Synsafe decoded, which was encoded syncsafe to diffrentiate with audio track
}

void copy(char *select,int size,int s_size,char *temp)
{
    unsigned char enc=temp[0];

    if(enc==0 || enc==3) //1 byte encoding
    {
        int n=size-1;
        if(n>=s_size)
            n=s_size-1;
        strncpy(select,temp+1,n); 
        select[n]='\0';
    }
    else if(enc==1) //2 byte encoding
    {
        int j=0;
        for(int i=3;i<size && j<s_size-1;i+=2) 
        select[j++]=temp[i];
        select[j]='\0';
    }
}

 static void mp3_view(int audio_size,int head_size,int version,FILE *fp)
 {
    char buff[75];
    memset(buff,'=',74);
    buff[74]='\0';
    printf("\n%25s  MP3 VIEW MENU\n","");
    printf("%25s|%s|\n\n","",buff);
    printf("%25s  Version : ","");
    printf("%d\n",version);
    printf("%25s  Tag Header size : ","");
    printf("%d\n",head_size);
    printf("%25s  Audio Size : ","");
    printf("%d\n",audio_size);

    fseek(fp,10,SEEK_SET); 

    char song[150]="",artist[150]="",album[150]="",year[50]="",
    content[100]="",composer[200]="",frm_hdr[5];

    while(ftell(fp)<(head_size+10))
    {
        char buf[6];

        fread(buf,1,4,fp);

        buf[4]='\0';

        strcpy(frm_hdr,buf);

        fread(buf,1,4,fp);

        int size=(buf[3]) | (buf[2]<<8) | (buf[1]<<16) | (buf[0]<<24); //Big endian stored size

        if(size<=0 || frm_hdr[0]=='\0')
            break;

        char *temp=malloc(size);

        if(temp==NULL)
            break;

        fseek(fp,2,SEEK_CUR); //Skip flags

        fread(temp,1,size,fp);

        if((strcmp(frm_hdr,"TIT2"))==0)
            copy(song,size,sizeof(song),temp); //Clamp the max
        else if((strcmp(frm_hdr,"TPE1"))==0)
            copy(artist,size,sizeof(artist),temp);
        else if((strcmp(frm_hdr,"TALB"))==0)
            copy(album,size,sizeof(album),temp);   
        else if((strcmp(frm_hdr,"TYER"))==0)
            copy(year,size,sizeof(year),temp);
        else if((strcmp(frm_hdr,"TCON"))==0)
            copy(content,size,sizeof(content),temp);
        else if(strcmp(frm_hdr,"TCOM")==0)
            copy(composer,size,sizeof(composer),temp);
    
        free(temp);
    }

    printf("\n");
    if(song[0])
        printf("%25s  Title : %s\n","",song);
    if(artist[0])
        printf("%25s  Artist : %s\n","",artist);
    if(album[0])
        printf("%25s  Album : %s\n","",album);
    if(content[0])
        printf("%25s  Genre : %s\n","",content);
    if(year[0])
        printf("%25s  Year : %s\n","",year);
    if(composer[0])
        printf("%25s  Composer : %s\n","",composer);
    printf("\n%25s|%s|\n\n","",buff);

    
 }

int view(char* fname,int argc)
{
    if(argc<=2 || (strstr(fname,".mp3")==NULL))
        return -1;
    
    FILE *fptr=fopen(fname,"r");

    if(fptr==NULL)
        return -2;

    unsigned char str[4];
    unsigned int version,tot_size,audio_size;

    fread(str,1,3,fptr);

    str[3]='\0';

    if((strcmp(str,"ID3"))!=0)
    {
        fclose(fptr);
        return -2;
    }

    fread(str,1,1,fptr);

    version=str[0];

    fseek(fptr,6,SEEK_SET);

    fread(str,1,4,fptr);

    head_size=sync_safe(str);

    fseek(fptr,0,SEEK_END);

    tot_size=ftell(fptr);

    audio_size=tot_size-(head_size+10);

    mp3_view(audio_size,head_size,version,fptr);
    
    fclose(fptr);

    return 1;
}

