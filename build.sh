gcc -o ioc ioc.c `pkg-config --cflags --libs gio-2.0`
gcc `pkg-config --cflags gtk+-3.0` -o destroywindow destroywindow.c `pkg-config --libs gtk+-3.0`
