#include <glib.h>
#include <gio/gio.h>
 
/*
gcc ioc.c `pkg-config --cflags --libs gio-2.0`
*/
 
static gboolean
io_func_cb(GIOChannel *source, GIOCondition condition, gpointer app) {
 
    GError *error = NULL;
    GNotification *notification;
    gchar *text;
    gchar *s;
 
    g_io_channel_read_line(source, &text, NULL, NULL, &error);
    if (g_strcmp0(text, "exit\n") == 0) {
        /* Decriment */
        g_application_release(app);
        g_free(text);
        return FALSE;
    }
    s = g_strdup_printf("notify-send -i gtk-ok %s", text);
    g_spawn_command_line_async(s, NULL);
    g_free(s);
    g_free(text);
     
    return TRUE;
}
 
static void
activate_cb (GApplication *app, gpointer user_data) {
 
    GIOChannel *channel;
 
    /* 0 == stdin */
    channel = g_io_channel_unix_new(0);
    g_io_add_watch (channel, G_IO_IN, io_func_cb, app);
    g_io_channel_unref(channel);
    /* Inclement */
    g_application_hold(app);
}
 
int
main (int argc, char* argv[]) {
 
    GApplication *app;
/*
    g_type_init(); // < 2.36
    g_thread_init(argc, argv); // < 2.32
*/
    app = g_application_new("org.suzuki.hayabusa", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate_cb), NULL);
    g_printf("Type 'exit' to exit\n");
    /* MainLoop run */
    g_application_run(app, argc, argv);
 
    g_application_quit(app);
    return 0;
}
