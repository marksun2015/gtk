#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define true TRUE
#define false FALSE

#define DOES_NOTHING_BUTTON 1
#define QUIT_BUTTON 2

GtkWidget *parent;

static void button_click_cb(GtkWidget * widget, gpointer data)
{
   gint which;

   g_print("Button Pressed\n");

   which = GPOINTER_TO_INT(data);

   switch (which) {
   case QUIT_BUTTON:
      g_signal_emit_by_name(parent,"delete_event");
      break;
   }
}

static gboolean delete_event(GtkWidget * widget, GdkEvent * event, gpointer data)
{
   gint result;

  GtkWidget *mbox;

   mbox =
       gtk_dialog_new_with_buttons("Quit?", GTK_WINDOW(widget),
               GTK_DIALOG_DESTROY_WITH_PARENT, "_Quit", 0, "_Cancel", 1, NULL);

   gtk_dialog_set_default_response(GTK_DIALOG(mbox), 1);

   result = gtk_dialog_run(GTK_DIALOG(mbox));

   gtk_widget_destroy(mbox);

   g_print("delete event occurred\n");
   /* Change TRUE to FALSE and the main window will be destroyed with
    * a "delete-event". */


   if (result == 0)
      return (false);
   else
      return (true);
}

static int destroy(GtkWidget * widget, gpointer data)
{

   return (FALSE);
}

static void activate(GtkApplication * app, gpointer user_data)
{
   /* GtkWidget is the storage type for widgets */
   GtkWidget *window;
   GtkWidget *button;
   GtkWidget *container;
   GtkWidget *topContainer;

   /* create a new window */
   window = gtk_application_window_new(app);
   parent = window;  // save this as a meaningful pointer, could so renamed window as parent...
   g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
   g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);

   /* Sets the border width of the window. */
   gtk_container_set_border_width(GTK_CONTAINER(window), 10);
   gtk_window_set_title(GTK_WINDOW(window), "Simple test emit");

   /* Start building first column */
   topContainer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
   container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);

   // build buttons column
   button = gtk_button_new_with_label("Does Nothing");
   gtk_box_pack_start(GTK_BOX(container), button, true, true, 1);
   g_signal_connect(button, "clicked", G_CALLBACK(button_click_cb), GINT_TO_POINTER(DOES_NOTHING_BUTTON));
   button = gtk_button_new_with_label("Quit");
   gtk_box_pack_start(GTK_BOX(container), button, true, true, 1);
   g_signal_connect(button, "clicked", G_CALLBACK(button_click_cb), GINT_TO_POINTER(QUIT_BUTTON));

// put column in to top container
   gtk_box_pack_start(GTK_BOX(topContainer), container, false, false, 1);


   /* put the composite container in to the top level window */
   gtk_container_add(GTK_CONTAINER(window), topContainer);

// show all widgets
   gtk_widget_show_all(window);

}

int main(int argc, char **argv)
{
   GtkApplication *app;
   int status;

   tzset();    // call to set timezone

   app = gtk_application_new("org.gmail.mipi.gareth.simple", G_APPLICATION_FLAGS_NONE);
   g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
   status = g_application_run(G_APPLICATION(app), argc, argv);
   g_object_unref(app);

   return status;
}
