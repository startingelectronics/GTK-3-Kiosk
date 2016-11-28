//---------------------------------------------------------------------
//  Program:      rpi_gtk_kiosk
//
//  Description:  Raspberry PI Shopping Mall Information Kiosk
//                - Displays a main window with mall map and buttons
//                - Clicking any button opens a corresponding window
//                  with information on a shop in the mall
//                - Each shop window has a back button that closes the
//                  window reverting back to the main mall window
//
//  Build info:   Built using Glade and GTK+ 3
//                Compiled using GCC
//
//  Date:         25 November 2016      Author: W.A. Smith
//                http://startingelectronics.org
//---------------------------------------------------------------------
#include <gtk/gtk.h>
#include <X11/Xlib.h>

GtkWidget* win_new_glade(char *glade_file, char *win_name, char *image_shop);
void HideCursor(GtkWidget *window);
void MoveCursor(int x, int y);

int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_main.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);

	gtk_window_fullscreen(GTK_WINDOW(window));
	
	MoveCursor(800, 480);	// move cursor off screen
    gtk_widget_show(window);                

	HideCursor(window);
	
    
    gtk_main();

    return 0;
}

// button click handlers for the shops
void on_btn_shop1_clicked()
{
	win_new_glade("glade/shop.glade", "window_shop", "res/shop1.png");
}

void on_btn_shop2_clicked()
{
	win_new_glade("glade/shop.glade", "window_shop", "res/shop2.png");
}

void on_btn_shop3_clicked()
{
	win_new_glade("glade/shop.glade", "window_shop", "res/shop3.png");
}

void on_btn_shop4_clicked()
{
	win_new_glade("glade/shop.glade", "window_shop", "res/shop4.png");
}

// close window when back button is clicked in any shop window
void on_btn_back_clicked(GtkWidget *widget, gpointer data)
{
	gtk_widget_destroy(data);
}

// close the main window when Alt + F4 pressed - needed during development
void on_window_main_destroy()
{
    gtk_main_quit();
}

// window creation utility function
// creates a window from a glade file and displays the selected image in the window
GtkWidget* win_new_glade(char *glade_file, char *win_name, char *image_shop)
{
	GtkBuilder      *builder;
    GtkWidget       *window;
    GtkWidget		*image;
    
	builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, glade_file, NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, win_name));
    gtk_builder_connect_signals(builder, NULL);
    
    image = GTK_WIDGET(gtk_builder_get_object(builder, "img_shop"));
    gtk_image_set_from_file(GTK_IMAGE(image), image_shop);

    g_object_unref(builder);

	// prevent cursor from briefly appearing between screen changes
	MoveCursor(800, 480);	// move cursor off screen
    gtk_widget_show(window);
    
    HideCursor(window);
    
    return window;
}

void HideCursor(GtkWidget *window)
{
	GdkDisplay		*display;
    GdkCursor		*hideCursor = NULL;
    GdkWindow		*gdk_window;
    
	// hide cursor
	display = gdk_display_get_default();
	hideCursor = gdk_cursor_new_for_display(display, GDK_BLANK_CURSOR);
	gdk_window = gtk_widget_get_window(window);
	gdk_window_set_cursor(gdk_window, hideCursor);
}

void MoveCursor(int x, int y)
{
	Display *dpy;
	Window root_window;
	
	dpy = XOpenDisplay(0);
	root_window = XRootWindow(dpy, 0);
	XSelectInput(dpy, root_window, KeyReleaseMask);
	XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, x, y);
	XFlush(dpy);
}
