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
	unsigned char Language; //"1.国语歌曲", "2.粤语歌曲", "3.闽南歌曲", 
                            //"4.英语歌曲", "5.日文歌曲", "6.韩文歌曲", 
                            //"7.泰文歌曲", "8.柬埔寨歌", "1.印度歌曲", 
                            //"2.马来歌曲", "3.印尼歌曲", "9.其它语别"
	unsigned char SongName[SONG_NAME_MAXIMUM];
	unsigned char Singer[SINGER_NAME_LENGTH];
	unsigned char Country;
	unsigned char SingerWord[5];
	unsigned char SongCnt;
	unsigned char SongTone;
	unsigned char Dance;    //"1.串  烧", "2.迪士高", "3.恰  恰", 
                            //"4.普鲁士", "5.探  戈", "6.华尔兹", 
                            //"7.吉特巴", "8.慢  摇"
	unsigned char SongMode; //"1.合唱歌曲X", "2.生日歌曲X", "3.相声小品X",
                            //"4.革命歌曲X", "5.怀旧歌曲",  "6.喜庆歌曲X", 
                            //"7.戏曲欣赏X", "8.儿童歌曲X", "电影"
	unsigned char SongWord[10];
	unsigned char MP3Background[3];
	unsigned char Reserved[2];	// Reserved[0] 作為音量設定值
	unsigned char VideoType;	// 影片類型，如DVD、演唱會、MTV等
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
    unsigned char Tracks;//音轨信息                      //SongType
    unsigned char Movies;//1.mtv 2.演唱会 3.风景 4.电影  //
    unsigned char VolumeL[3];                           //
    unsigned char VolumeR[3];                           //
    unsigned char SongType;//歌曲分类                    //
                            //1.生日歌曲X.      2.中外民歌.
                            //3.交谊舞曲.       4.京剧.(相声小品)X
                            //5.儿童歌曲X.      6.合唱歌曲X.
                            //7.喜庆歌曲X.      8.戏曲X.
                            //9.流行歌曲.       0.革命歌曲X.
                            //a.怀旧歌曲.
    unsigned char Dance;    
                            //"1.串  烧", "2.迪士高", "3.恰  恰", 
                            //"4.普鲁士", "5.探  戈", "6.华尔兹", 
                            //"7.吉特巴", "8.慢  摇"
    unsigned char Film;     //动作.言情.惊悚.爱情.喜剧.
                            //轻喜剧.黑色.纪录片.古装历史.
                            //科幻.家庭.乡村.旅游电影.
                            //还有戏剧电影.战争电影
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
            g_SongData2.Tracks = '2'; //標準(右聲道有人聲)
        }
        else
        {
            g_SongData2.Tracks = '1';  //相反(左聲道有人聲)
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
        //"1.合唱歌曲X", "2.生日歌曲X", "4.相声小品X",
        //"5.革命歌曲X", "6.怀旧歌曲",  "7.喜庆歌曲X", 
        //"8.戏曲欣赏X", "9.儿童歌曲X", "3.电影"
        
        //歌曲分类                    //
        //1.生日歌曲X.      2.中外民歌.
        //3.交谊舞曲.       4.京剧.(相声小品)X
        //5.儿童歌曲X.      6.合唱歌曲X.
        //7.喜庆歌曲X.      8.戏曲X.
        //9.流行歌曲.       0.革命歌曲X.
        //a.怀旧歌曲.
        
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
