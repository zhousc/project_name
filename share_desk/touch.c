#include <open_source.h>
#include <linux/input.h>
#define TS_DEV "/dev/input/event3"

static int ts_fd = -1;

static int init_device()
{
    if((ts_fd = open(TS_DEV, O_RDONLY)) < 0)
    {
        print_msg("Error open %s\n", TS_DEV);
        return -1;
    }
            return ts_fd;
}

int main()
{
    int i;
    struct input_event data;
    if(init_device() < 0)
    {
        return -1;
    }
    while(1)
    {
        read(ts_fd, &data, sizeof(data));
        if (data.type == EV_KEY)
        {
            print_msg("type: EV_KEY, event = %s, value = %d\n",
                   data.code == BTN_TOUCH ? "BTN_TOUCH" : "Unkown", data.value);
        }
        else if(data.type == EV_ABS)
        {
            print_msg("type: EV_ABS, event = %s, value = %d\n",
            data.code == ABS_X ? "ABS_X" :
            data.code == ABS_Y ? "ABS_Y" :
            data.code == ABS_PRESSURE ? "ABS_PRESSURE" :
            "Unkown", data.value);
        }
        else if (data.type == EV_SYN)
        {
            print_msg("type: EV_SYN, event = %s, value = %d\n",
            data.code == SYN_REPORT ? "SYN_REPORT" : "Unkown", data.value);
        }
        else
        {
            print_msg("type: 0x%x, event = 0x%x, value = %d\n", data.type, data.code, data.value);
        }
    }
    return 0;
}