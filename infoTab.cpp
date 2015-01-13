#include "infoTab.hh"

infoTab *infoTab::infoTab_ = 0;
GtkWidget *infoTab::container_ = 0;
std::list<std::string> infoTab::infoList_;

void infoTab::display_infoTab ()
{
	clear_widget (container_);

    GtkWidget *scrollWindow = gtk_scrolled_window_new (NULL, NULL);

    GtkWidget *textView = gtk_text_view_new ();
    GtkTextBuffer *textBuff = gtk_text_buffer_new (NULL);
    gtk_container_add (GTK_CONTAINER(container_), scrollWindow);
    gtk_container_add (GTK_CONTAINER(scrollWindow), textView);
    //gtk_widget_set_size_request (GTK_WIDGET (scrollWindow), 800, 50);
    string text;
    unsigned c = infoList_.size ();
    string c_s = "";
    for (list<string>::iterator i = infoList_.begin (); i != infoList_.end (); i++)
    {
        c_s = " [" + i2s(c) + "] ";
        text += c_s + *i + "\n";
        c--;
    }
    gtk_text_buffer_set_text (textBuff, text.c_str (), text.size ());
    gtk_text_view_set_buffer (GTK_TEXT_VIEW(textView), textBuff);
    gtk_widget_show_all (container_);
    
}

void infoTab::add_info (string s)
{
    infoList_.push_front (s);
    display_infoTab ();
}

infoTab *infoTab::instance ()
{
    if (infoTab_)
        return (infoTab_);
    else
    {
        infoTab::infoTab_ = new infoTab ();
        return (infoTab::infoTab_);
    }
}


