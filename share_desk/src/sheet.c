#include "source.h"

#define SONG_NO_LENGTH              5
#define SONG_NAME_MAXIMUM           20
#define SINGER_NAME_LENGTH          10
typedef struct _SongData
{
	unsigned char SongNo[SONG_NO_LENGTH];
	unsigned char SongType;
	unsigned char Language;
	unsigned char SongName[SONG_NAME_MAXIMUM];
	unsigned char Singer[SINGER_NAME_LENGTH];
	unsigned char Country;
	unsigned char SingerWord[5];
	unsigned char SongCnt;
	unsigned char SongTone;
	unsigned char Dance;
	unsigned char SongMode;
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
    unsigned char SongError;                            // 
    unsigned char LF[2];                                //
} SongData1_t;





SongData_t *g_SongData=NULL;
SongData_t g_SongData1;
SongData1_t g_SongData2;

int g_TotalSong=0;

int main(void)
{
    char buffer[1024] = {0};
    FILE *file = NULL;
    
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
        
        g_SongData2.Tracks=g_SongData1.SongType;//7
        //unsigned char SongType;
    
        g_SongData2.Language=g_SongData1.Language;//2
        //unsigned char Language;
    
        memcpy(g_SongData2.SongName,g_SongData1.SongName,SONG_NAME_MAXIMUM);//3
        //unsigned char SongName[SONG_NAME_MAXIMUM];
    
        memcpy(g_SongData2.SongWord,g_SongData1.SongWord,10);//4
        //unsigned char SongWord[10];
        
        g_SongData2.SongCnt=g_SongData1.SongCnt;//5
        //unsigned char SongCnt;
    
        memcpy(g_SongData2.Singer,g_SongData1.Singer,SINGER_NAME_LENGTH);//6
        //unsigned char Singer[SINGER_NAME_LENGTH];
    
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