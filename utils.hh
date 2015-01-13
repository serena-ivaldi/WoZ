#ifndef UTILS_HH_
# define UTILS_HH_


//
#include <gtk/gtk.h>
//
//
#include <string>
#include <vector>
#include <iostream>
#include <deque>
#include <list>
#include "WoZ.hh"


using namespace std;

void add_toolTips (GtkWidget *w, const char* s);
void display_infoTab ();
string d2s(double n);
string i2s(int n);
void clear_widget (GtkWidget *w);
std::vector<string> tokenize (string s);

void clear_widget_on_switch_page (GtkNotebook *notebook, GtkNotebookPage *page, guint page_num, gpointer user_data);

GtkWidget* display_history_record (const history_record h);
const std::string currentDateTime();
const std::string DoubleTofFormatedDate(double date);

#endif //UTILS_HH_