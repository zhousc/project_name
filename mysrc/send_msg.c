#include <gtk/gtk.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#define _LARGEFILE64_SOURCE
#include <stdarg.h>
#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#define sleep(n) Sleep(1000 * (n))
#else
#include <unistd.h>
#endif 
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#ifdef WIN32
#include <Winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#define closesocket close
#endif
#include <dirent.h>
#include <pthread.h> 

GtkWidget *lable11;
GtkWidget *lable12;
GtkWidget *lable13;
GtkWidget *lable14;

GtkWidget *entry;
GtkWidget *button1;
GtkWidget *button2;
GtkWidget *combo;
GtkWidget *combo1;
GtkWidget *hseparator1;
GtkWidget *hseparator2;

GtkWidget *scrolled;

typedef struct CONTEXT_T
{
    char id[17];
    char name[60];
} Context_t;

Context_t g_context[30];
int g_context_item = 0;
char ip[20];

static GtkWidget *create_combobox_with_model()
{
    FILE *f;
    char buffer[1024]={0};
    char buffer1[1024]={0};
    char buffer2[1024]={0};
    int i = 0;
    GtkWidget *combobox = NULL;
    GtkListStore *store = NULL;
    GtkTreeIter iter;
    GtkCellRenderer *renderer = NULL;

    store = gtk_list_store_new(1, G_TYPE_STRING);
    
    strncpy(g_context[g_context_item].id,"0000000000000000",17);
    strncpy(g_context[g_context_item].name,"全部",60);
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,0,g_context[g_context_item++].name,-1);
    //g_print("===>>>>[%s %d]\n",__FUNCTION__,__LINE__);
    f=fopen("res/data.txt","rb");
    if(f)
    {
        while(fgets(buffer,1024,f))
        {
            if(strlen(buffer) > 0 && (buffer[strlen(buffer) - 1] == '\n' || buffer[strlen(buffer) - 1] == '\r'))
            {
                buffer[strlen(buffer) - 1] = 0;
            }
            if(strlen(buffer) > 0 && (buffer[strlen(buffer) - 1] == '\n' || buffer[strlen(buffer) - 1] == '\r'))
            {
                buffer[strlen(buffer) - 1] = 0;
            }
            i = sscanf(buffer,"%s %s",buffer1,buffer2);
            if(i==2)
            {
                if(strlen(buffer1)==16 && 
                    memcmp(buffer1,"0000000000000000",16))
                {
                    strncpy(g_context[g_context_item].id,buffer1,16);
                    //g_print("====>>[%s]\n",g_context[g_context_item].id);
                    strncpy(g_context[g_context_item].name,buffer2,60);
                    gtk_list_store_append(store, &iter);
                    gtk_list_store_set(store, &iter, 0, 
                    g_convert(g_context[g_context_item].name,-1,"UTF-8", "GBK", NULL, NULL, NULL), -1);
                    g_context_item++;
                    if(g_context_item >= 30)
                    {
                        break;
                    }
                }
            }
        }
        fclose(f);
    }
    //g_print("===>>>>[%s %d]\n",__FUNCTION__,__LINE__);
    combobox = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
    renderer = gtk_cell_renderer_text_new();
    g_object_set (G_OBJECT(renderer), "font", "Simsun 20","foreground", "BLACK",NULL ); 
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combobox), renderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combobox), renderer, "text", 0, NULL);
    return combobox;
}


void destroy (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();
    //g_print ("exit !!!\n");
#ifdef WIN32
    WSACleanup();
#endif
}

void button_set_text(GtkWidget * lable, gpointer data)
{
    //GdkColor color;
    PangoFontDescription *pfd;
    g_return_if_fail(lable != NULL);
    g_return_if_fail(lable != NULL);
    pfd = pango_font_description_from_string("Simsun");
    pango_font_description_set_size (pfd, 18 * PANGO_SCALE);
    gtk_widget_modify_font(GTK_WIDGET(lable),pfd);
    pango_font_description_free(pfd);
}

void button_set_text1(GtkWidget *entry, gpointer data)
{
    GdkColor color;
    PangoFontDescription *pfd;
    g_return_if_fail(entry != NULL);
    g_return_if_fail(entry != NULL);
    pfd = pango_font_description_from_string("Simsun");
    pango_font_description_set_size (pfd, 20 * PANGO_SCALE);
    gtk_widget_modify_font(GTK_WIDGET(entry),pfd);
    pango_font_description_free(pfd);
    gdk_color_parse ("RED", &color);
    gtk_widget_modify_fg (entry, GTK_STATE_NORMAL, &color);
    //gdk_color_parse ("GREEN", &color);
    //gtk_widget_modify_bg (lable, GTK_STATE_NORMAL, &color);
    //gdk_color_parse ("BLUE", &color);
    //gtk_widget_modify_base (lable, GTK_STATE_NORMAL, &color);
}

gint tag = 0;
gint loop = 0;
gint timeout_callback( gpointer data )
{
    gtk_label_set_text((GtkLabel *)lable14,"");
    loop = 0;
    return 0;
}

void send_cmd(char *cmd,int c)
{
    int sockfd=-1;
    int   len=0;
    int send_count=0;
    struct sockaddr_in cliaddr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd==-1)
    {
        g_print("create socket failue %s\n",strerror(errno));
        return;
    }

    cliaddr.sin_family      = AF_INET;
    cliaddr.sin_addr.s_addr = inet_addr (ip);
    cliaddr.sin_port        = htons(7603);
    
    len=sizeof(struct sockaddr_in);

    send_count=sendto(sockfd, (char *)cmd, c , 0, (struct sockaddr *)&cliaddr, len);
    if(send_count != c)
    {
        g_print("sendto:%s\n",strerror(errno));
    }
    else
    {
        g_print("now recv msg send_count = %d\n",send_count);
    }
    return;
}

void button_clicked(GtkWidget *widget, gpointer data)
{
    const gchar *entry_text;
    char buffer[64]={0};
    char buf[1024]={0};
    char buf1[1024]={0};
    int i = gtk_combo_box_get_active((GtkComboBox *)combo);
    GtkTextBuffer *buffer_text;
	GtkTextIter start, end;
	
	buffer_text = gtk_text_view_get_buffer(GTK_TEXT_VIEW(entry));
	gtk_text_buffer_get_bounds (GTK_TEXT_BUFFER (buffer_text), &start, &end);
    entry_text = gtk_text_buffer_get_text(buffer_text,&start,&end,TRUE);

    snprintf(buf,1024,"%s",
        g_convert(entry_text,-1,"GBK", "UTF-8", NULL, NULL, NULL));
    if(strlen(buf) > 0)
    {
        printf ("Entry contents: [%d]%s\n",strlen(buf),buf);
        g_print ("==>> active %d [%s]\n",i,g_context[i].id);
        snprintf(buf1,128,"PCCMDYJ01001%s%s",g_context[i].id,buf);
        printf("===>>>[%s]\n",buf1);
        send_cmd(buf1,strlen(buf1));
        loop++;
        snprintf(buffer,64,"信息已经送出[%d]",loop);
        gtk_label_set_text((GtkLabel *)lable14,buffer);
        if(tag)
        {
            gtk_timeout_remove(tag);
            tag = 0;
        }
        tag = gtk_timeout_add(5000,timeout_callback,NULL);
    }
}

void button_clicked1(GtkWidget *widget, gpointer data)
{
    const gchar *entry_text;
    char buf[1024]={0};
    GtkTextBuffer *buffer;
	GtkTextIter start, end;
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(entry));
	gtk_text_buffer_get_bounds (GTK_TEXT_BUFFER (buffer), &start, &end);
    entry_text = gtk_text_buffer_get_text(buffer,&start,&end,TRUE);
	snprintf(buf,1024,"%s",g_convert(entry_text,-1,"GBK", "UTF-8", NULL, NULL, NULL));
    if(strlen(buf) > 0)
    //if(0)
	{
		gtk_text_buffer_delete(buffer,&start,&end);
		//gtk_text_buffer_set_text(buffer,"",-1);
    }
	else if(0)
	{
		 gtk_text_buffer_insert(buffer,&end,"TEST\n",-1);
	}
}



void 
enter_callback( GtkWidget *widget,GtkWidget *entry )
{
    const gchar *entry_text;
    //gchar *c;
    int i;
    const char *name = gtk_widget_get_name(widget);
    entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
    g_print ("Entry contents: %s\n", entry_text);
    g_print ("name contents: %s\n", name);
    //c = gtk_combo_box_get_title((GtkComboBox *)combo);
    i = gtk_combo_box_get_active((GtkComboBox *)combo);
    g_print ("==>> active %d [%s]\n",i,g_context[i].id);
    //g_free(c);
}

void fill_cmbo(GtkWidget *cmbo)
{
    FILE *f;
    int i = 0;
    PangoFontDescription *pfd;
    GtkWidget *entry1;
    char buffer[1024]={0};
    char buffer1[1024]={0};
    char buffer2[1024]={0};
    GtkCellRenderer *renderer = NULL;
    renderer = gtk_cell_renderer_text_new();//gtk_cell_renderer_combo_new();
    g_object_set (G_OBJECT(renderer), "font", "Simsun 16","foreground", "RED",NULL );
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo), renderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo), renderer, "text", 0,NULL);
    
    f=fopen("res/data.txt","rb");
    if(f)
    {
        strncpy(g_context[g_context_item].id,"0000000000000000",17);
        strncpy(g_context[g_context_item].name,"全部",60);
        gtk_combo_box_text_append_text((GtkComboBoxText *)combo,g_context[g_context_item++].name);
        while(fgets(buffer,1024,f))
        {
            if(strlen(buffer) > 0 && (buffer[strlen(buffer) - 1] == '\n' || buffer[strlen(buffer) - 1] == '\r'))
            {
                buffer[strlen(buffer) - 1] = 0;
            }
            if(strlen(buffer) > 0 && (buffer[strlen(buffer) - 1] == '\n' || buffer[strlen(buffer) - 1] == '\r'))
            {
                buffer[strlen(buffer) - 1] = 0;
            }
            i = sscanf(buffer,"%s %s",buffer1,buffer2);
            if(i==2)
            {
                if(strlen(buffer1)==16)
                {
                    strncpy(g_context[g_context_item].id,buffer1,16);
                    g_print("====>>[%s]\n",g_context[g_context_item].id);
                    strncpy(g_context[g_context_item].name,buffer2,60);
                    gtk_combo_box_text_append_text((GtkComboBoxText *)combo,
                    g_convert(g_context[g_context_item].name,-1,"UTF-8", "GBK", NULL, NULL, NULL));
                    g_context_item++;
                    if(g_context_item >= 30)
                    {
                        break;
                    }
                }
            }
        }
        fclose(f);
    }
    entry1 = gtk_bin_get_child((GtkBin *)combo);
    gtk_editable_set_editable((GtkEditable *)entry1,FALSE);
    gtk_entry_set_text((GtkEntry *)entry1,g_convert(g_context[0].name,-1,"UTF-8", "GBK", NULL, NULL, NULL));

    pfd = pango_font_description_from_string("Simsun");
    pango_font_description_set_size (pfd, 20 * PANGO_SCALE);
    gtk_widget_modify_font(GTK_WIDGET(entry1),pfd);
    pango_font_description_free(pfd);
}

void
update_widget_bg(GtkWidget *widget, gchar *img_file)
{       
    GtkStyle *style;     
    GdkPixbuf *pixbuf;       
    GdkPixmap *pixmap;     
    gint width, height;    

    pixbuf = gdk_pixbuf_new_from_file(img_file, NULL);    
    width = gdk_pixbuf_get_width(pixbuf);   
    height = gdk_pixbuf_get_height(pixbuf);    
    pixmap = gdk_pixmap_new(NULL, width, height, 24);   
    gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, NULL, 0);
    style = gtk_style_copy(GTK_WIDGET (widget)->style);    

    if (style->bg_pixmap[GTK_STATE_NORMAL])        
    g_object_unref(style->bg_pixmap[GTK_STATE_NORMAL]);    

    style->bg_pixmap[GTK_STATE_NORMAL] = g_object_ref(pixmap);    
    style->bg_pixmap[GTK_STATE_ACTIVE] = g_object_ref(pixmap);
    style->bg_pixmap[GTK_STATE_PRELIGHT] = g_object_ref(pixmap);
    style->bg_pixmap[GTK_STATE_SELECTED] = g_object_ref(pixmap);
    style->bg_pixmap[GTK_STATE_INSENSITIVE] = g_object_ref(pixmap);
    gtk_widget_set_style(GTK_WIDGET (widget), style);
    g_object_unref(style);
}




int main(int argc, char *argv[])
{
    GError *error = NULL;
    GdkPixbuf *icon_pixbuf = NULL;
    char bar_buf[64]={0};
    char nameBuffer[32]={0};
    GtkWidget *window;
    GtkWidget *fixed;
    PangoFontDescription *pfd;
    GdkColor color;
    
    //g_setenv ("GTK_MODULES", "", TRUE);
    //g_setenv ("GTK2_RC_FILES", "/dev/null", TRUE);
    //gtk_disable_setlocale();
    
    //g_setenv (LC_ALL, "utf8", FALSE);
    //setlocale (LC_ALL, "utf8");
    //setenv(LC_ALL,"C",0);
    //gtk_rc_parse("rc/rc.txt");
#ifdef WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    if(0)
    {
        char host_name[256];
        int i = 0;
        HOSTENT *host_entry;  
        gethostname(host_name,sizeof(host_name));  //获取本地主机名称  
        host_entry=gethostbyname(host_name);   //从主机名数据库中得到对应的“主机”  
        for(i = 0; host_entry!= NULL && host_entry->h_addr_list[i]!= NULL; i++ )  
        //循环得出本地机器所有IP地址  
        {  
            const char *pszAddr=inet_ntoa (*(struct in_addr *)host_entry->h_addr_list[i]);  
            g_print("pszAddr = %s\n",pszAddr);  
        }
    }
    else
    {
        FILE *f=fopen("res/ip.txt","rb");
        if(f)
        {
            int l;
            fgets(ip,16,f);
            l = strlen(ip);
            if(l > 0 && (ip[l-1] == '\n' || ip[l-1] == '\r'))
            {
                ip[l-1] = 0;
            }
            l = strlen(ip);
            if(l > 0 && (ip[l-1] == '\n' || ip[l-1] == '\r'))
            {
                ip[l-1] = 0;
            }
            g_print("ip = %s\n",ip);
            fclose(f);
        }
    }

    gtk_init (&argc, &argv);
    setlocale (LC_ALL, "C");
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    snprintf(bar_buf,64,"版权所有 2013-08-28");
    gtk_window_set_title(GTK_WINDOW(window),bar_buf);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    
    gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
    gtk_widget_set_size_request(window,800,400);
    gtk_container_set_border_width(GTK_CONTAINER(window),10);
    
    update_widget_bg(window,"res/bg.jpg");
    
    icon_pixbuf = gdk_pixbuf_new_from_file("res/logo.jpg",&error);
    if (icon_pixbuf) 
    {
        gtk_window_set_icon(GTK_WINDOW(window), icon_pixbuf);
        g_object_unref(icon_pixbuf);
    }
    else
    {
        printf("%s\n",error->message);
        g_error_free(error);
    }
    
    g_signal_connect (G_OBJECT(window),"destroy",G_CALLBACK (destroy),NULL);
    
    

    //======================================================================
    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);
    //======================================================================
    
    
    //button1
    //======================================================================
    button1 = gtk_button_new_with_label("发送");
    gtk_widget_set_size_request(button1, 80, 50);
    gtk_fixed_put(GTK_FIXED(fixed), button1, 680, 240+80);
    gtk_widget_show(button1);
    gtk_container_foreach (GTK_CONTAINER(button1), button_set_text, NULL);
    g_signal_connect (G_OBJECT (button1), "clicked",G_CALLBACK (button_clicked),NULL);
    //======================================================================
    
    
    //button2
    //======================================================================
    button2 = gtk_button_new_with_label("清空");
    gtk_widget_set_size_request(button2, 80, 50);
    gtk_fixed_put(GTK_FIXED(fixed), button2, 560, 240+80);
    gtk_widget_show(button2);
    gtk_container_foreach (GTK_CONTAINER(button2), button_set_text, NULL);
    g_signal_connect (G_OBJECT (button2), "clicked",G_CALLBACK (button_clicked1),NULL);
    //======================================================================
    
	//scroll
	scrolled=gtk_scrolled_window_new(NULL,NULL); /*创建滚动窗口构件*/
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC/*GTK_POLICY_ALWAYS*/);
	gtk_widget_show(scrolled);
	gtk_widget_set_size_request(scrolled,760,100);
	gtk_fixed_put(GTK_FIXED(fixed),scrolled, 6, 180);
	//entry
    //======================================================================
    //entry =  gtk_entry_new();
    entry = gtk_text_view_new();
	gtk_text_view_set_wrap_mode((GtkTextView *)entry,GTK_WRAP_CHAR);
	gtk_text_view_set_accepts_tab((GtkTextView *)entry,FALSE);
	/*将文本视图构件加入滚动窗口*/
	
	//gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled),entry);
	
	gtk_container_add(GTK_CONTAINER(scrolled), entry);
	
	//gtk_widget_set_size_request(entry,760,100);
    //gtk_entry_set_max_length (GTK_ENTRY (entry), 36);
    snprintf(nameBuffer,32,"widget%d",0+1);
    gtk_widget_set_name(entry, nameBuffer);
    //g_signal_connect (G_OBJECT(entry),"activate",G_CALLBACK (enter_callback),entry);
    //gtk_fixed_put(GTK_FIXED(fixed),entry, 6, 180);
    
    pfd = pango_font_description_from_string("Simsun");
    pango_font_description_set_size (pfd, 20 * PANGO_SCALE);
    gtk_widget_modify_font(GTK_WIDGET(entry),pfd);
    pango_font_description_free(pfd);
    
    //gtk_container_foreach (GTK_CONTAINER(entry), button_set_text1, NULL);
    
    
    gdk_color_parse ("RED", &color);
    gtk_widget_modify_fg (entry, GTK_STATE_NORMAL, &color);
    gdk_color_parse ("RED", &color);
    gtk_widget_modify_bg (entry, GTK_STATE_NORMAL, &color);
    gdk_color_parse ("GRAY", &color);
    gtk_widget_modify_base (entry, GTK_STATE_NORMAL, &color);
    gtk_widget_show(entry);
    //======================================================================
    
    
    
    //combo
    //======================================================================
    if(0)
    {
        combo = gtk_combo_box_text_new_with_entry();
        gtk_widget_set_size_request(combo,600,50);
        fill_cmbo(combo);
        gtk_widget_show(combo);
        gtk_fixed_put(GTK_FIXED(fixed), combo, 160, 60);
    }
    else
    {
        combo = create_combobox_with_model();
        gtk_combo_box_set_active((GtkComboBox *)combo,0);
        gtk_widget_set_size_request(combo,600,50);
        gtk_widget_show(combo);
        gtk_fixed_put(GTK_FIXED(fixed), combo, 160, 60);
    }
    
    lable11 = gtk_label_new ("包房号:");
    //gtk_widget_set_size_request(lable11,160,50);
    gtk_fixed_put(GTK_FIXED(fixed),lable11, 6, 60+15);
    pfd = pango_font_description_from_string("Simsun");
    pango_font_description_set_size (pfd, 20 * PANGO_SCALE);
    gtk_widget_modify_font(GTK_WIDGET(lable11),pfd);
    pango_font_description_free(pfd);
    gtk_widget_show(lable11);
    
    lable12 = gtk_label_new ("信息发送:");
    //gtk_widget_set_size_request(lable12,100,50);
    gtk_fixed_put(GTK_FIXED(fixed),lable12, 6, 120+15);
    pfd = pango_font_description_from_string("Simsun");
    pango_font_description_set_size (pfd, 20 * PANGO_SCALE);
    gtk_widget_modify_font(GTK_WIDGET(lable12),pfd);
    pango_font_description_free(pfd);
    gtk_widget_show(lable12);
    
    lable13 = gtk_label_new ("管理工具");
    pfd = pango_font_description_from_string("Simsun");
    pango_font_description_set_size (pfd, 20 * PANGO_SCALE);
    gtk_widget_modify_font(GTK_WIDGET(lable13),pfd);
    pango_font_description_free(pfd);
    gtk_fixed_put(GTK_FIXED(fixed),lable13, 0, 0);
    gtk_widget_show(lable13);
    
    
    lable14 = gtk_label_new (""/*"信息已经送出"*/);
    pfd = pango_font_description_from_string("Simsun");
    pango_font_description_set_size (pfd, 20 * PANGO_SCALE);
    gtk_widget_modify_font(GTK_WIDGET(lable14),pfd);
    pango_font_description_free(pfd);
    gdk_color_parse ("RED", &color);
    gtk_widget_modify_fg (lable14, GTK_STATE_NORMAL, &color);
    //gdk_color_parse ("GREEN", &color);
    //gtk_widget_modify_bg (entry, GTK_STATE_NORMAL, &color);
    //gdk_color_parse ("RED", &color);
    //gtk_widget_modify_base (entry, GTK_STATE_NORMAL, &color);
    gtk_fixed_put(GTK_FIXED(fixed),lable14, 0, 240+10+80);
    gtk_widget_show(lable14);
    
    
    hseparator1 = gtk_hseparator_new ();
    gtk_widget_set_size_request(hseparator1,800,1);
    gtk_fixed_put(GTK_FIXED(fixed),hseparator1, 0, 120);
    gtk_widget_show(hseparator1);
    
    hseparator2 = gtk_hseparator_new ();
    gtk_widget_set_size_request(hseparator2,800,1);
    gtk_fixed_put(GTK_FIXED(fixed),hseparator2, 0, 50);
    gtk_widget_show(hseparator2);
    
    //======================================================================
    //provider = GTK_STYLE_PROVIDER (gtk_css_provider_new ());
    //g_signal_connect (provider,"parsing-error",G_CALLBACK (show_parsing_error),
    //  gtk_text_view_get_buffer (GTK_TEXT_VIEW (combo)));
    //apply_css (window, provider);
    //show
    //======================================================================
    gtk_widget_show(fixed);
    gtk_widget_show (window);
    //======================================================================
    gtk_main ();
    return 0;
}

