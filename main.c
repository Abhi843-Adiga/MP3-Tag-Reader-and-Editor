#include "mp3.h"

int check_arg(char *str,int argc)
{
    char *arr[3]={"-v","-e","-h"};

    for(int i=0;i<3;i++)
    {
        if((strcmp(arr[i],str))==0)
            return i;
    }

    return  -1;
}

void usage(char *str)
{
    char buf[75];
    memset(buf,'=',74);
    buf[74]='\0';
    printf("\n%25s  USAGE","");
    printf("\n%25s|%s|\n\n","",buf);
    printf("%25s  View: %s -v <mp3_file_name.mp3>\n","",str);
    printf("%25s  Edit: %s -e <-t/-a/-A/-y/-g/-c> <changing_text> <mp3_file_name.mp3>\n","",str);
    printf("%25s  Help: %s -h\n","",str);
    printf("\n%25s|%s|\n\n","",buf);
}

void help(void)
{
    char buf[75];
    memset(buf,'=',74);
    buf[74]='\0';
    printf("\n%25s  HELP MENU\n","");
    printf("%25s|%s|\n\n","",buf);
    printf("%25s  -v : ","");
    printf("%s\n"," To view MP3 contents ");
    printf("%25s  -e : ","");
    printf("%s\n"," To edit MP3 contents ");
    printf("%25s  -t : ","");
    printf("%s\n"," To edit MP3 song title ");
    printf("%25s  -a : ","");
    printf("%s\n"," To edit MP3 artist name ");
    printf("%25s  -A : ","");
    printf("%s\n"," To edit MP3 album name");
    printf("%25s  -y : ","");
    printf("%s\n"," To edit MP3 year content ");
    printf("%25s  -g : ","");
    printf("%s\n"," To edit genre ");
    printf("%25s  -c : ","");
    printf("%s\n"," To edit composer ");
    printf("\n%25s|%s|\n\n","",buf);

}

int main(int argc,char *argv[])
{
    //CLA Validation
    if(argc<=1)
    {
        usage(argv[0]);
        return 1;
    }

    //Checks the argument passed -v,-e or --help
    int op=check_arg(argv[1],argc);

    switch(op)
    {
        case 0:
            int ret = view(argv[2],argc);
            if(ret==-1)
            {
                printf("Operation failed!!\n");
                usage(argv[0]);
                return 1;
            }
            else if(ret==-2)
            {
                printf("Invalid MP3 file\n");
                printf("Please enter appropirate file\n");
                return 1;
            }
            break;

        case 1:
            int re=edit(argv[4],argv[2],argc,argv[3]);
            if(re==-1)
            {
                printf("Operation failed!!\n");
                usage(argv[0]);
                return 1;
            }
            else if(re==-2)
            {
                printf("Invalid MP3 file\n");
                printf("Please enter appropirate file\n");
                return 1;
            }
                
            break;

        case 2:
            help();
            break;
        
        case -1:
            printf("Invalid argument\n");
            usage(argv[0]);
    }

    return 0;
}