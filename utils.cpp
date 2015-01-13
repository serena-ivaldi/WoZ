#include "utils.hh"

string i2s(int n)
{
    char buff [10];
    sprintf(buff,"%d",n);
    return string(buff);
}

string d2s(double n)
{
    char buff [20];
    sprintf(buff,"%lf",n);
    char ret [3];
    int i = 0;
    /* partial copy (only 5 chars): */
    strncpy (ret, buff, 3);
    ret[3] = '\0';   /* null character manually added */
    return string(ret);
}



// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}



// Get format YYYY-MM-DD.HH:mm:ss from date as double
const std::string DoubleTofFormatedDate(double date) {
    time_t     dateApprox = (time_t) date;
    struct tm  tstruct;
    char       buf[80];
    double     delta = (double) (date - (double) dateApprox);

    tstruct = *localtime(&dateApprox);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d-%R:%S", &tstruct);
    sprintf (buf, "%s:%d", buf, (int) (delta*1000));

    return buf;
}

GtkWidget* display_history_record (const history_record h)
{

    //table for display
    GtkWidget *table = gtk_table_new (3, 3, true);
    gtk_widget_set_size_request (table, 290, 200);        

    //label
    GtkWidget *playlistLabel;
    string type = h.type;
    playlistLabel = gtk_label_new (type.c_str ());
    gtk_table_attach_defaults (GTK_TABLE (table), playlistLabel, 0, 2, 0, 1);

    //time
    GtkWidget *historyTime;
    string tim = d2s(h.time);
    historyTime = gtk_label_new (tim.c_str ());
    gtk_table_attach_defaults (GTK_TABLE (table), historyTime, 2, 3, 0, 1);

    //name
    GtkWidget *playlistName;
    string name = h.name;
    playlistName = gtk_label_new (name.c_str ());
    gtk_table_attach_defaults (GTK_TABLE (table), playlistName, 0, 2, 1, 2);

    gtk_widget_show_all (table);
    return table;
}


void clear_widget_on_switch_page (GtkNotebook *notebook, GtkNotebookPage *page, guint page_num, gpointer p)
{
    GtkWidget *wtoclear = (GtkWidget*) p;
    clear_widget (wtoclear);
}



void add_toolTips (GtkWidget *w, const char* s)
{
    GtkTooltips *toolTips = gtk_tooltips_new();
    gtk_tooltips_set_tip(toolTips, w, s, NULL);
}

void clear_widget (GtkWidget *w)
{

    GList *children, *iter;

    children = gtk_container_get_children (GTK_CONTAINER(w));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);
}


 vector<string> tokenize(string  theString)
{
    size_t  start = 0, end = 0;
    std::vector<string> res;

    while ( end != string::npos)
    {
        end = theString.find(" ", start);

        // If at end, use length=maxLength.  Else use length=end-start.
        res.push_back( theString.substr( start,
                       (end == string::npos) ? string::npos : end - start));

        // If at end, use start=maxSize.  Else use start=end+delimiter.
        start = (   ( end > (string::npos - 1) )
                  ?  string::npos  :  end + 1);
    }
    return res;
}

