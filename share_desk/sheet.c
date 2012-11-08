#include "open_source.h"

#define     SONG_TYPE_MPEG1_RC_VOCAL    '1'
#define     SONG_TYPE_MPEG1_LC_VOCAL    '2'
#define     SONG_TYPE_MPEG2_RC_VOCAL    '3'
#define     SONG_TYPE_MPEG2_LC_VOCAL    '4'
#define     SONG_TYPE_MPEG12_0X81_VOCAL '5'
#define     SONG_TYPE_MPEG12_0X80_VOCAL '6'
#define     SONG_TYPE_MPEG12_0XC1_VOCAL '7'
#define     SONG_TYPE_MPEG12_0XC0_VOCAL '8'
#define     SONG_TYPE_MPEG4_RC_VOCAL    '9'
#define     SONG_TYPE_MPEG4_LC_VOCAL    'A'
#define     SONG_TYPE_MPEG12_0X89_VOCAL 'F'
#define     SONG_TYPE_MPEG12_0X88_VOCAL 'G'
#define     SONG_TYPE_MPEG12_0XA1_VOCAL 'H'
#define     SONG_TYPE_MPEG12_0XA0_VOCAL 'I'
#define     SONG_TYPE_MP3           'J'

#define SONG_NO_LENGTH              5
#define SONG_NAME_MAXIMUM           20
#define SINGER_NAME_LENGTH          10
typedef struct _SongData
{
	unsigned char SongNo[SONG_NO_LENGTH];
	unsigned char SongType;
	unsigned char Language; //"1.�������", "2.�������", "3.���ϸ���", 
                            //"4.Ӣ�����", "5.���ĸ���", "6.���ĸ���", 
                            //"7.̩�ĸ���", "8.����կ��", "1.ӡ�ȸ���", 
                            //"2.��������", "3.ӡ�����", "9.�������"
	unsigned char SongName[SONG_NAME_MAXIMUM];
	unsigned char Singer[SINGER_NAME_LENGTH];
	unsigned char Country;
	unsigned char SingerWord[5];
	unsigned char SongCnt;
	unsigned char SongTone;
	unsigned char Dance;    //"1.��  ��", "2.��ʿ��", "3.ǡ  ǡ", 
                            //"4.��³ʿ", "5.̽  ��", "6.������", 
                            //"7.���ذ�", "8.��  ҡ"
	unsigned char SongMode; //"1.�ϳ�����X", "2.���ո���X", "3.����СƷX",
                            //"4.��������X", "5.���ɸ���",  "6.ϲ�����X", 
                            //"7.Ϸ������X", "8.��ͯ����X", "��Ӱ"
	unsigned char SongWord[10];
	unsigned char MP3Background[3];
	unsigned char Reserved[2];	// Reserved[0] ���������O��ֵ
	unsigned char VideoType;	// ӰƬ��ͣ���DVD���ݳ�����MTV��
	unsigned char LF[2];
} SongData_t;


typedef struct _SongData1
{
    unsigned char SongNo[SONG_NO_LENGTH];               //SongNo
    unsigned char Language;                             //Language
    unsigned char SongName[SONG_NAME_MAXIMUM];          //SongName
    unsigned char SongWord[10];                         //SongWord
    unsigned char SongCnt;                              //SongCnt
    unsigned char Singer[SINGER_NAME_LENGTH];           //Singer
    unsigned char Singer2[SINGER_NAME_LENGTH];          //
    unsigned char Tracks;//������Ϣ                      //SongType
    unsigned char Movies;//1.mtv 2.�ݳ��� 3.�羰 4.��Ӱ  //
    unsigned char VolumeL[3];                           //
    unsigned char VolumeR[3];                           //
    unsigned char SongType;//��������                    //
                            //1.���ո���X.      2.�������.
                            //3.��������.       4.����.(����СƷ)X
                            //5.��ͯ����X.      6.�ϳ�����X.
                            //7.ϲ�����X.      8.Ϸ��X.
                            //9.���и���.       0.��������X.
                            //a.���ɸ���.
    unsigned char Dance;    
                            //"1.��  ��", "2.��ʿ��", "3.ǡ  ǡ", 
                            //"4.��³ʿ", "5.̽  ��", "6.������", 
                            //"7.���ذ�", "8.��  ҡ"
    unsigned char Film;     //����.����.���.����.ϲ��.
                            //��ϲ��.��ɫ.��¼Ƭ.��װ��ʷ.
                            //�ƻ�.��ͥ.���.���ε�Ӱ.
                            //����Ϸ���Ӱ.ս����Ӱ
    unsigned char SongError;                            // 
    unsigned char LF[2];                                //
} SongData1_t;

int do_save(int fd, void *buf, size_t length,long long int all)
{
	all = all;
	char *data=buf;
	char tmp=0;
	if(0 && length==2048)
	{
		int i=0;
		for(i=0;i<1024;i++)
		{
			tmp=data[i];data[i]=data[i+1024];data[i+1024]=tmp;
		}
		for(i=0;i<64;i++)
		{
			tmp=data[i];data[i]=data[i+64];data[i+64]=tmp;
		}
		for(i=0;i<64;i++)
		{
			tmp=data[i+1024];data[i+1024]=data[i+1088];data[i+1088]=tmp;
		}
	}
	write(fd,buf,length);
    return 0;
}


int init_save(char *file)
{
    int fd=-1;
    fd=open(file,O_RDWR | O_CREAT | O_TRUNC | O_LARGEFILE);
    if(fd==-1)
    {
        print_msg("[%s,%d]", __FILE__, __LINE__);
        exit(-1);
    }
    print_msg("open write file ok\n");
    return fd;
}
void exit_save(int fd)
{
    close(fd);
}



void read_files(char *path)
{
	char buf[2048]={0};
	int count=0;
	long long all=0;
	int save=0;
	struct Files file;
	save=init_save("zhoustest.mpg");
	fd_open(&file,path);
	while(1)
    {
		count=fd_read(&file,buf,2048);
        all+=count;
        if(all>file.File_Size)
        {
            count-=4;
        }
        do_save(save,buf,count,all);
        if(count < 2048)
        {
            break;
        }
    }
    exit_save(save);
	fd_close(&file);
}


SongData_t *g_SongData=NULL;
SongData_t g_SongData1;
SongData1_t g_SongData2;

int g_TotalSong=0;

int main(void)
{
    char buffer[1024] = {0};
    FILE *file = NULL;
	int port=1001;
	int count=0,count1=0;
    int g_song_type = 0;
	//read_files("/mnt/ntfs/file_bak/songs/9/07/95009.mpg");
	//return;

	//recv_broadcast(&port);
	//g_external_data_flag=1;

	while(0)
	{
		if(count1 % 3 == 0)
			send_broadcast("eth0",port,"TEST BROADCAST",10);
		count1++;
		clear_data();

		if(g_external_data_flag != 0)
		{
			count++;
			g_external_data_flag=0;
			print_msg("hava data=%d\n",count);
		}
		else
		{
			//print_msg("no data\n");
		}
		//if(count == 10) break;
	}
	//return;
    //print_msg("%s\n",__FILE__);
    int i = 0;
    g_SongData=(SongData_t *)malloc(65535*sizeof(SongData_t));
    g_TotalSong = 0;
    file = fopen ("song.txt", "rb");
    if (file)
    {
        while (1)
        {
            memset (buffer, 0, 1024);
            if ((fgets (buffer, 1024, file)) == NULL)
            {   
                break;
            }   
            else
            {   
                if(strlen(buffer)==65 && 
                    isdigit(buffer[0]) && 
                    isdigit(buffer[1]) && 
                    isdigit(buffer[2]) && 
                    isdigit(buffer[3]) && 
                    isdigit(buffer[4]))
                {
                    memcpy (&(g_SongData[g_TotalSong]), buffer, 65);
                    g_TotalSong++;   
                }   
            }   
            if (g_TotalSong >= 65535)
            {   
                break;
            }   
        }
        fclose (file);
    }
    print_msg("g_TotalSong=%d %d\n",g_TotalSong,sizeof(SongData_t));
	print_msg("g_TotalSong=%d %d\n",g_TotalSong,sizeof(SongData1_t));
	
    file = fopen ("song1.txt", "w+");
    for(i=0;i<g_TotalSong;i++)
    {
        g_SongData1=g_SongData[i];
        memset(&g_SongData2,0x20,sizeof(SongData1_t));
        
        memcpy(g_SongData2.SongNo,g_SongData1.SongNo,SONG_NO_LENGTH);//1
        //unsigned char SongNo[SONG_NO_LENGTH];
        
        //g_SongData2.Tracks=g_SongData1.SongType;
        if ((g_song_type == SONG_TYPE_MPEG1_RC_VOCAL) || 
        (g_song_type == SONG_TYPE_MPEG2_RC_VOCAL) || 
        (g_song_type == SONG_TYPE_MPEG12_0X81_VOCAL) || 
        (g_song_type == SONG_TYPE_MPEG12_0XC1_VOCAL) || 
        (g_song_type == SONG_TYPE_MPEG12_0X89_VOCAL) || 
        (g_song_type == SONG_TYPE_MPEG12_0XA1_VOCAL) || 
        (g_song_type == SONG_TYPE_MPEG4_RC_VOCAL) || 
        (g_song_type == SONG_TYPE_MP3))
        {
            g_SongData2.Tracks = '2'; //�˜�(��������)
        }
        else
        {
            g_SongData2.Tracks = '1';  //�෴(��������)
        }
        //unsigned char SongType;
    
        g_SongData2.Language=g_SongData1.Language;//2
        //unsigned char Language;
    
        memcpy(g_SongData2.SongName,g_SongData1.SongName,SONG_NAME_MAXIMUM);//3
        //unsigned char SongName[SONG_NAME_MAXIMUM];
    
        memcpy(g_SongData2.SongWord,g_SongData1.SongWord,10);//4
        //unsigned char SongWord[10];
        
        g_SongData2.SongCnt=g_SongData1.SongCnt;//5
        //unsigned char SongCnt;
    
        g_SongData2.Dance=g_SongData1.Dance;
        
        memcpy(g_SongData2.Singer,g_SongData1.Singer,SINGER_NAME_LENGTH);//6
        //unsigned char Singer[SINGER_NAME_LENGTH];
    
        
        if(g_SongData1.SongMode=='1')//6140a785 239
        {
            g_SongData2.SongType='6';
        }
        if(g_SongData1.SongMode=='2')
        {
            g_SongData2.SongType='1';
        }
        else if(g_SongData1.SongMode=='3')
        {
            g_SongData2.Film='1';
        }
        else if(g_SongData1.SongMode=='4')
        {
            g_SongData2.SongType='4';
        }
        else if(g_SongData1.SongMode=='5')
        {
            g_SongData2.SongType='0';
        }
        else if(g_SongData1.SongMode=='6')
        {
            g_SongData2.SongType='a';
        }
        else if(g_SongData1.SongMode=='7')
        {
            g_SongData2.SongType='7';
        }
        else if(g_SongData1.SongMode=='8')
        {
            g_SongData2.SongType='8';
        }
        else if(g_SongData1.SongMode=='9')
        {
            g_SongData2.SongType='5';
        }
        //"1.�ϳ�����X", "2.���ո���X", "4.����СƷX",
        //"5.��������X", "6.���ɸ���",  "7.ϲ�����X", 
        //"8.Ϸ������X", "9.��ͯ����X", "3.��Ӱ"
        
        //��������                    //
        //1.���ո���X.      2.�������.
        //3.��������.       4.����.(����СƷ)X
        //5.��ͯ����X.      6.�ϳ�����X.
        //7.ϲ�����X.      8.Ϸ��X.
        //9.���и���.       0.��������X.
        //a.���ɸ���.
        
        //unsigned char Singer2[SINGER_NAME_LENGTH];
        g_SongData2.LF[0]='\r';
        g_SongData2.LF[1]='\n';
        //unsigned char LF[2];
        fwrite(&g_SongData2,1,sizeof(SongData1_t),file);
    }
    print_msg("g_TotalSong=%d %d\n",g_TotalSong,i);
    if(file) fclose(file);
    return 0;
}
