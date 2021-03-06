#include "open_source.h"
typedef enum _VINPUT_RT_VALUE__
{
	RET_OK=0,
	RET_FAIL=1,
	RET_NOT_INIT=2,
}VINPUT_RT_VALUE;

#define IOCTL_MOUSE_STATUS  0
#define IOCTK_KBD_STATUS    1
#define IOCTK_TC_STATUS     3

typedef enum  _TC_MOUSE_CLICKTYPE_E
{
    MOUSE_ACTION_MOVE =0x100, /*�ƶ�*/
	MOUSE_RIGHT_SINGLE_CLICK =0x201,
	MOUSE_RIGHT_DOUBLE_CLICK=0x202,
	MOUSE_RIGHT_DOWN=0x203,
	MOUSE_RIGHT_UP=0x204,
	MOUSE_RGIHT_DOWN_MOVE=0x205,
	MOUSE_LEFT_SINGLE_CLICK =0x301,
	MOUSE_LEFT_DOUBLE_CLICK=0x302,
	MOUSE_LEFT_DOWN=0x303,
	MOUSE_LEFT_UP=0x304,
	MOUSE_LEFT_DOWN_MOVE=0x305,
	MOUSE_WHEEL=0x306,
	TC_DOWN=0x00,
	TC_UP  =0x01,
	TC_MOVE=0x02
}TC_MOUSE_CLICKTYPE_E ;

#pragma pack(1)
typedef short	S16;
typedef struct stUIMsgHead
{
    S16 s16sndModuleName;
    S16 s16rcvModuleName;
    S16 s16msgType;
    S16 s16usMsglen;
    S16 s16usRsv;
    S16 s16usMsgSeq;
}UIMsgHead;

typedef struct stUI_TC_MOUSE_MSG
{
    UIMsgHead stMsgHead;
    short clickType;
	int dx;
	int dy;
}UI_TC_MOUSE_MSG;

int get_mouse(UI_TC_MOUSE_MSG *mouse_data)
{
	static int file = -1;
	static unsigned char mdata[3];
	int count = 0;
	int tmp=0;
	if(file==-1)
	{
		file=open("/dev/input/mice",O_RDONLY);
		if(file==-1)
		{
			usleep(10);
		}
	}

	if(file != -1)
	{
		count=read(file,mdata,3);
		if(count != 3)
		{
			close(file);
			file=-1;
		}
		else
		{
			//print_msg("%02x %02x %02x\n",mdata[0],mdata[1],mdata[2]);
			if((mdata[0] & 0x09) == 0x09)
			{
				mouse_data->clickType=MOUSE_LEFT_SINGLE_CLICK;
			}
			else if ((mdata[0] & 0x0f) == 0x0a)
			{
				mouse_data->clickType=MOUSE_RIGHT_SINGLE_CLICK;
			}
			else
			{
				mouse_data->clickType=MOUSE_ACTION_MOVE;
			}
			if ((mdata[0] & 0x10) == 0x10)
			{
				tmp = -((0xff - mdata[1] + 1) & 0x7f);
			}
			else
			{
				tmp = +(mdata[1] & 0x7f);
			}
			mouse_data->dx=tmp;

			if ((mdata[0] & 0x20) == 0x20)
			{
				tmp = ((0xff - mdata[2] + 1) & 0x7f);
			}
			else
			{
				tmp = -(mdata[2] & 0x7f);
			}
			mouse_data->dy=tmp;
			print_msg("%d %d\n",(int)mouse_data->dx,(int)mouse_data->dy);
		}
	}
}
int search(int code);
int getch(void);
int main(void)
{
	int port=8822;
	int count=0;
	UI_TC_MOUSE_MSG mouse_data;
	int input=0,kkk=0;
if(0)
{
	printf("\033[K");
	printf("\033[2J");
	printf("\033[1m");
	printf("\033[1;1H");
	printf("\033[?25l");
	print_msg("Please Input Keyboad\r");
	while(1){
		char data []={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		input=getch();
		printf("\033[K");
		kkk=search(input);
		if(isprint(input))
		{
			print_msg("getch = [%x %c %d]\r",input,input,kkk);
		}
		else
		{
			print_msg("getch = [%x %d]\r",input,kkk);
		}
		if(input == 1)
		{
			break;
		}
		if(kkk){
			data[12]=0x09;
			data[13]=0x03;

			data[14]=0x05;
			send_broadcast("eth0",port,data,sizeof(UI_TC_MOUSE_MSG));
		}
	}
	printf("\033[2J");
	printf("\033[0m");
	printf("\33[?25h");
	print_msg("Quit ...\n");
	return 0;
}
	print_msg("sizeof(UIMsgHead) = %d\n",sizeof(UIMsgHead));
	print_msg("sizeof(UI_TC_MOUSE_MSG) = %d\n",sizeof(UI_TC_MOUSE_MSG));
	while(1)
	{
		memset(&mouse_data,0,sizeof(UI_TC_MOUSE_MSG));
		get_mouse(&mouse_data);
		send_broadcast("eth0",port,(char *)&mouse_data,sizeof(UI_TC_MOUSE_MSG));
		//count++;
		//print_msg("count = %d\n",count);
		//sleep(1);
	}
    return 0;
}


int getch(void)
{
    struct termios tm, tm_old;
    int fd = STDIN_FILENO, c;
    if(tcgetattr(fd, &tm) < 0)
	{
        return -1;
	}
    tm_old = tm;
    cfmakeraw(&tm);
    if(tcsetattr(fd, TCSANOW, &tm) < 0)
        return -1;
    c = fgetc(stdin);
    if(tcsetattr(fd, TCSANOW, &tm_old) < 0)
        return -1;
    return c;
}

int table[115][2]={
{0x7f,1},     //BackSpace
{0x31,2},     //1
{0x32,3},     //2
{0x33,4},     //3
{0x34,5},     //4
{0x35,6},     //5
{0x36,7},     //6
{0x37,8},     //7
{0x38,9},     //8
{0x39,10},    //9
{0x30,11},    //0
{'-' ,12},    //MINUS
{0x00,13},    //EQUALS
{0x7e,14},    //DEL
{0x09,15},    //TAB
{'Q' ,16},    //Q
{'W' ,17},    //W
{'E' ,18},    //E
{'R' ,19},    //R
{'T' ,20},    //T
{'Y' ,21},    //Y
{'U' ,22},    //U
{'I' ,23},    //I
{'O' ,24},    //O
{'P' ,25},    //P
{'(' ,26},    //LEFT_BRACKET
{')' ,27},    //RIGHT_BRACKET
{0x00,28},    //DPAD_CENTER
{'A' ,30},    //A
{'S' ,31},    //S
{'D' ,32},    //D
{'F' ,33},    //F
{'G' ,34},    //G
{'H' ,35},    //H
{'J' ,36},    //J
{'K' ,37},    //K
{'L' ,38},    //L
{':' ,39},    //SEMICOLON
{0x00,40},    //APOSTROPHE
{0x00,41},    //GRAVE
{0x00,42},    //SHIFT_LEFT
{0x00,43},    //BACKSLASH
{'Z' ,44},    //Z
{'X' ,45},    //X
{'C' ,46},    //C
{'V' ,47},    //V
{'B' ,48},    //B
{'N' ,49},    //N
{'M' ,50},    //M
{0x00,51},    //COMMA
{0x00,52},    //PERIOD
{0x00,53},    //SLASH
{0x2f,54},    //SHIFT_RIGHT
{0x00,56},    //ALT_LEFT
{' ' ,57},    //SPACE
{0x00,59},    //F1
{0x00,60},    //F2
{0x00,61},    //F3
{0x00,62},    //F4
{0x00,88},    //POWER             WAKE
{0x00,100},   //ALT_RIGHT
{0x00,102},   //HOME              WAKE
{0x00,103},   //DPAD_UP           WAKE_DROPPED
{0x00,104},   //PAGE_UP
{0x00,105},   //DPAD_LEFT         WAKE_DROPPED
{0x00,106},   //DPAD_RIGHT        WAKE_DROPPED
{0x00,108},   //DPAD_DOWN         WAKE_DROPPED
{0x00,109},   //PAGE_DOWN
{0x00,111},   //DEL
{0x00,113},   //MUTE
{0x00,114},   //VOLUME_DOWN
{0x00,115},   //VOLUME_UP
{0x00,116},   //POWER             WAKE
{0x00,127},   //SEARCH            WAKE_DROPPED
{0x00,128},   //MEDIA_STOP
{0x00,138},   //HELP
{0x00,139},   //MENU              WAKE_DROPPED
{0x00,141},   //SETTINGS
{0x00,150},   //BROWSER
{0x00,155},   //ENVELOPE
{0x00,157},   //MYAPP
{0x00,158},   //BACK              WAKE_DROPPED
{0x00,159},   //MEDIA_FAST_FORWARD
{0x00,162},   //EJECTCLOSECD
{0x00,164},   //MEDIA_PLAY_PAUSE
{0x00,168},   //MEDIA_REWIND
{0x00,182},   //REPEAT
{0x00,212},   //CAMERA
{0x00,215},   //AT
{0x00,217},   //SEARCH            WAKE_DROPPED
{0x00,226},   //MOVIE
{0x00,227},   //STAR
{0x00,228},   //POUND
{0x00,229},   //MENU              WAKE_DROPPED
{0x00,230},   //SOFT_RIGHT        WAKE
{0x00,232},   //DPAD_CENTER       WAKE_DROPPED
{0x00,63},    //MOUSE
{0x00,354},   //GOTO
{0x00,358},   //INFO
{0x00,364},   //FAV
{0x00,370},   //SUB
{0x00,377},   //TV
{0x00,392},   //AUDIO
{0x00,402},   //CHANNEL_UP
{0x00,403},   //CHANNEL_DOWN
{0x00,406},   //AB
{0x00,418},   //ZOOM_IN
{0x00,419},   //ZOOM_OUT
{0x00,478},   //IME
{0x00,479},   //MORE
{0x00,480},   //BTV
{0x00,481},   //VOD
{0x00,482},   //NVOD
{0x00,483},   //NPVR
{0x00,484},   //SEEK
};



int search(int code)
{
	int i=0;
	int ret=0;
	for(i=0;i<114;i++)
	{
		if(table[i][0]==code)
		{
			ret=table[i][1];
			break;
		}
	}
	return ret;
}
