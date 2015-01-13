#include "playlistTab.hh"
#include <limits>
#include <iomanip> 

PlaylistTab *PlaylistTab::playlistTab_ = 0;;

std::deque<history_record> PlaylistTab::history_ = std::deque<history_record> ();
std::deque<history_record> PlaylistTab::playlist_ = std::deque<history_record> ();

//PlaylistTab
GtkWidget *PlaylistTab::playlistFixed_ = 0;
//HistoryTab
GtkWidget *PlaylistTab::historyFixed_ = 0;

//=================================================================
//                     UTILS
//=================================================================
//-----------------------------------------------------------
void saveToHistory(string type, string name, double tim, string command)
{
    //create a record and add it informations
    history_record h;
    h.name = name;
    h.time = tim;
    h.command = command;
    h.id = PlaylistTab::history_.size ();
    h.type = type;
    h.success = 0;
    h.lastTimePlayed = yarp::os::Time::now ();
    //cout <<  std::setprecision (15)  << (int) yarp::os::Time::now () << endl;
    //cout <<  DoubleTofFormatedDate(yarp::os::Time::now ()) << endl;
    PlaylistTab::history_.push_back (h);
    //redraw the hisotry panel
    PlaylistTab::display_history_tab ();
    //add info in info panel
    infoTab::instance()->add_info (string("==> : Action Sent: ") +h.type +": "+ h.name);
}

void delete_playlist_item (GtkWidget *w, gpointer p)
{
    //cast playlist
    gpointer list_p = g_object_get_data (G_OBJECT(w), "list");
    std::deque<history_record> *list = (std::deque<history_record>*) list_p; 

    history_record *r = (history_record*) p;
    unsigned int id = r->id;
    deque<history_record>::iterator i = list->begin ();
    unsigned int c = 0;
    // Delete element ID
    while (i != list->end ())
    { 
        if (i->id == id)
        {
            infoTab::instance()->add_info (string ("X : Delete item from playlist: ") + i->name);
            list->erase (i);
            break;
        }
        i++;
    }
    // Update id from playlist
    i = list->begin ();
    while (i != list->end ())
    {
        i->id = c;
        c++;
        i++;
    }
    paramTab::clear ();  
    PlaylistTab::display_playlist_tab ();
    display_play_list ();
    PlaylistTab::display_history_tab ();

}

void add_from_history_to_playlist (GtkWidget *w, gpointer p)
{
    
    history_record *r = (history_record*) p;
    infoTab::instance()->add_info (string ("--> : Add action from history to playlist: ") + r->name);
    // add_record (&playlist_, *r);
    add_record (&PlaylistTab::playlist_, PlaylistTab::playlist_.size (), r->type, r->name, r->time, r->command);

    PlaylistTab::display_playlist_tab ();
}


void add_record (deque<history_record> *list, int id, string type, string name, double tim, string command)
{
    // create new history_record and add it to history
    history_record h;
    h.command = command;      
    h.type = type;
    h.name = name;
    h.success = 0;
    h.time  = tim;
    h.id = id;
    h.lastTimePlayed = 0;
    list->push_back (h);
}

void add_record (deque<history_record> *list, const history_record h)
{
    history_record rec;

    rec.type = h.type;
    rec.name = h.name;
    rec.success = h.success;
    rec.time  = h.time;
    rec.command = h.command;
    rec.id = list->size ();
    rec.lastTimePlayed = 0;
    list->push_back (rec);

}

void modify_item_from_edit_tab(GtkWidget *w, gpointer p)
{

    //====Get informations
    //cast list
    gpointer list_p = g_object_get_data (G_OBJECT(w), "list");
    std::deque<history_record> *list = (std::deque<history_record>*) list_p; 
    
    cout << "size: " << list->size () << endl;


    //cast item
    gpointer item_p = g_object_get_data (G_OBJECT(w), "item");
    history_record *h = (history_record*) item_p; 
    cout << "id: " << h->id << endl;
    
    //cast entry_name
    gpointer name_p = g_object_get_data (G_OBJECT(w), "entry_name");
    const char *name = gtk_entry_get_text(GTK_ENTRY(name_p));
    
    //cast entry_time
    gpointer time_p = g_object_get_data (G_OBJECT(w), "entry_time");
    const char *tim_s = gtk_entry_get_text(GTK_ENTRY(time_p));
    double tim = atof (tim_s);

        //cast entry_time
    gpointer command_p = g_object_get_data (G_OBJECT(w), "entry_command");
    const char *command = gtk_entry_get_text(GTK_ENTRY(command_p));

    std::deque<history_record>::iterator it = list->begin ();
    while (it != list->end ())
    {
        if (it->id == h->id)
        {
            it->name = name;
            it->time = tim;
            it->command = command;
            break;
        }
        it++;
    }
    infoTab::instance()->add_info (string ("C : Change item id = ") + i2s(it->id));
    PlaylistTab::display_playlist_tab ();
    PlaylistTab::display_history_tab ();
    display_play_list ();
}



//=================================================================
//                     PLAYLIST TAB
//=================================================================
void PlaylistTab::display_playlist_tab ()
{
    GtkWidget *playlistListFixed;
    GtkWidget *LoadPlaylistButton;
    GtkWidget *savePlaylistButton;
    playlistListFixed = gtk_fixed_new ();

    //remove all child from playlistFixed_ 
    GList *children, *iter;

    children = gtk_container_get_children (GTK_CONTAINER(PlaylistTab::playlistFixed_));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);


    //Load playlist button
    LoadPlaylistButton = gtk_button_new_with_label ("Load Playlist");
    g_signal_connect(G_OBJECT (LoadPlaylistButton), "clicked", G_CALLBACK (open_load_playlist_dialog_box), (gpointer) &playlist_);
    gtk_widget_set_size_request (LoadPlaylistButton, 300, 30);
    gtk_fixed_put (GTK_FIXED(PlaylistTab::playlistFixed_), LoadPlaylistButton, 0, 0);
    add_toolTips (LoadPlaylistButton, "Load a playlist from a file");


    //save playlist button
    savePlaylistButton = gtk_button_new_with_label ("Save Playlist");
    g_signal_connect(G_OBJECT (savePlaylistButton), "clicked", G_CALLBACK (open_save_playlist_dialog_box), (gpointer) &playlist_);
    gtk_widget_set_size_request (savePlaylistButton, 300, 30);
    gtk_fixed_put (GTK_FIXED(PlaylistTab::playlistFixed_), savePlaylistButton, 0, 620);
    add_toolTips (savePlaylistButton, "Save the current playlist in a file");

    // scroll window for playlist item
    GtkWidget *scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_set_size_request (scrolledwindow, 300, 500);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_show (scrolledwindow);
    //gtk_container_add(GTK_CONTAINER(playlistFixed_), scrolledwindow);
    gtk_fixed_put (GTK_FIXED(PlaylistTab::playlistFixed_), scrolledwindow, 0, 60);
    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW(scrolledwindow), playlistListFixed);
    


    // create playlist item into playlist panel
    int i = 0;
    for (unsigned int i = 0; i < playlist_.size (); i++)
    {

        //Container        
        GtkWidget *playlistItemFrame;
        playlistItemFrame = gtk_frame_new ((string("id: ") + i2s (playlist_[i].id)).c_str ());
        gtk_widget_set_size_request (playlistItemFrame, 290, 200);        
        gtk_container_set_border_width (GTK_CONTAINER (playlistItemFrame), 10);
        gtk_fixed_put(GTK_FIXED(playlistListFixed), playlistItemFrame, 0, i * 205);

        //table for display
        GtkWidget *table = gtk_table_new (4, 4, true);
        gtk_widget_set_size_request (table, 290, 200);        
        gtk_container_add(GTK_CONTAINER(playlistItemFrame), table);

        // display history_record
        GtkWidget *table_history =  display_history_record (playlist_[i]);
        gtk_table_attach_defaults (GTK_TABLE (table), table_history, 0, 3, 0, 3);



        gpointer p = &(playlist_[i]);
        GdkColor red, green;
        gdk_color_parse ("#ff9486", &red);
        // button "Delete item from playlist"
        GtkWidget *delete_playlist_item_button = gtk_button_new_with_label ("X");
        g_signal_connect (G_OBJECT(delete_playlist_item_button), "clicked", G_CALLBACK (delete_playlist_item), p);
        g_object_set_data (G_OBJECT(delete_playlist_item_button), "list", (gpointer) &playlist_);
        gtk_widget_set_size_request (delete_playlist_item_button, 10, 10);
        gtk_table_attach_defaults (GTK_TABLE (table), delete_playlist_item_button, 3, 4, 0, 1);
        add_toolTips (delete_playlist_item_button, "Delete action from playlist");
        gtk_widget_modify_bg ( GTK_WIDGET(delete_playlist_item_button), GTK_STATE_PRELIGHT, &red);

        //button for edit
        GtkWidget *display_edit_item_button = gtk_button_new_with_label ("Edit");
        g_object_set_data (G_OBJECT(display_edit_item_button), "list", (gpointer) &playlist_);
        g_signal_connect (G_OBJECT(display_edit_item_button), "clicked", G_CALLBACK (display_edit_panel), p);
        gtk_widget_set_size_request (display_edit_item_button, 10, 10);
        gtk_table_attach_defaults (GTK_TABLE (table), display_edit_item_button, 3, 4, 1, 2); 
        add_toolTips (display_edit_item_button, "Edit this action");       

    }

    gtk_widget_show_all (PlaylistTab::playlistFixed_);
}


void PlaylistTab::display_history_tab ()
{
    //------ HISTORY
    // create history panel
    GtkWidget *historyListFixed;
    GtkWidget *historySaveButton;
    GtkWidget *historyClearButton;

    historyListFixed = gtk_fixed_new ();

    //remove all child from historyFixed_ 
    GList *children, *iter;

    children = gtk_container_get_children (GTK_CONTAINER(PlaylistTab::historyFixed_));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    //save button
    historySaveButton = gtk_button_new_with_label ("Save history As");
    g_signal_connect(G_OBJECT(historySaveButton), "clicked", G_CALLBACK(open_save_history_dialog_box), NULL);
    gtk_widget_set_size_request (historySaveButton, 150, 30);
    gtk_fixed_put (GTK_FIXED(PlaylistTab::historyFixed_), historySaveButton, 0, 620);
    add_toolTips (historySaveButton, "Save history in a file");

    //save button
    historyClearButton = gtk_button_new_with_label ("Clear");
    g_signal_connect(G_OBJECT(historyClearButton), "clicked", G_CALLBACK(clear_history), NULL);
    gtk_widget_set_size_request (historyClearButton, 140, 30);
    gtk_fixed_put (GTK_FIXED(PlaylistTab::historyFixed_), historyClearButton, 160, 620);
    add_toolTips (historyClearButton, "Clear History");
    
    // scroll window for history item
    GtkWidget *scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request (scrolledwindow, 300, 600);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_show (scrolledwindow);
    gtk_container_add(GTK_CONTAINER(PlaylistTab::historyFixed_), scrolledwindow);
    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW(scrolledwindow), historyListFixed);





    // create history item into history panel
    // display history from history_records deque history_
    for (unsigned int i = 0; i < history_.size (); i++)
    {

        // Container
        
        GtkWidget *historyItemFrame;
        historyItemFrame = gtk_frame_new ((string("id: ") + i2s (history_[i].id)).c_str ());
        gtk_widget_set_size_request (historyItemFrame, 290, 200);        
        gtk_container_set_border_width (GTK_CONTAINER (historyItemFrame), 10);
        gtk_fixed_put(GTK_FIXED(historyListFixed), historyItemFrame, 0, i * 205);

        //table for display
        GtkWidget *table = gtk_table_new (4, 4, true);
        gtk_widget_set_size_request (table, 290, 200);        
        gtk_container_add(GTK_CONTAINER(historyItemFrame), table);

        GdkColor red, green;
        gdk_color_parse ("#ff9486", &red);
        gdk_color_parse ("#b4ff82", &green);
        // display history_record
        GtkWidget *table_history =  display_history_record (history_[i]);
        gtk_table_attach_defaults (GTK_TABLE (table), table_history, 0, 3, 0, 3);
       
        gpointer p = &(history_[i]);
        // button "add to playlist"
        GtkWidget *add_to_playlist_button = gtk_button_new_with_label ("-->");
        g_signal_connect (G_OBJECT(add_to_playlist_button), "clicked", G_CALLBACK (add_from_history_to_playlist), p);
        gtk_widget_set_size_request (add_to_playlist_button, 10, 10);
        gtk_table_attach_defaults (GTK_TABLE (table), add_to_playlist_button, 3, 4, 0, 1);
        add_toolTips (add_to_playlist_button, "Add this action to the current playlist");
        gtk_widget_modify_bg ( GTK_WIDGET(add_to_playlist_button), GTK_STATE_PRELIGHT, &green);

        //button for edit
        GtkWidget *display_edit_item_button = gtk_button_new_with_label ("Edit");
        g_object_set_data (G_OBJECT(display_edit_item_button), "list", (gpointer) &history_);
        g_signal_connect (G_OBJECT(display_edit_item_button), "clicked", G_CALLBACK (display_edit_panel), p);
        gtk_widget_set_size_request (display_edit_item_button, 10, 10);
        gtk_table_attach_defaults (GTK_TABLE (table), display_edit_item_button, 3, 4, 1, 2);
        add_toolTips (display_edit_item_button, "Edit this action");

        // button "Delete item from playlist"
        GtkWidget *delete_playlist_item_button = gtk_button_new_with_label ("X");
        g_signal_connect (G_OBJECT(delete_playlist_item_button), "clicked", G_CALLBACK (delete_playlist_item), p);
        g_object_set_data (G_OBJECT(delete_playlist_item_button), "list", (gpointer) &history_);
        gtk_widget_set_size_request (delete_playlist_item_button, 10, 10);
        gtk_table_attach_defaults (GTK_TABLE (table), delete_playlist_item_button, 3, 4, 2, 3);
        add_toolTips (delete_playlist_item_button, "Delete action from playlist");
        gtk_widget_modify_bg ( GTK_WIDGET(delete_playlist_item_button), GTK_STATE_PRELIGHT, &red);

    }

    gtk_widget_show_all (PlaylistTab::historyFixed_);
}

void PlaylistTab::createPlaylistTab (GtkWidget* notebook)
{

	GtkWidget *playlistTab;	
    GtkWidget *playlistPanelFixed;    
    GtkWidget *playlistLabel = gtk_label_new("Playlist");

    playlistPanelFixed  = gtk_fixed_new ();
	playlistTab = gtk_frame_new ("What i did");
    gtk_container_set_border_width (GTK_CONTAINER (playlistTab), 10);
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook), playlistTab, playlistLabel);
    gtk_container_add (GTK_CONTAINER (playlistTab), playlistPanelFixed);

    //------ HISTORY
    PlaylistTab::historyFixed_  = gtk_fixed_new ();
    gtk_fixed_put (GTK_FIXED(playlistPanelFixed), historyFixed_, 0, 0);
    gtk_widget_set_size_request (historyFixed_, 400, 800);

    display_history_tab ();



    
    // ------ PLAYLIST
    // create playlist panel on the right of the page
    
    PlaylistTab::playlistFixed_  = gtk_fixed_new ();
    gtk_fixed_put (GTK_FIXED(playlistPanelFixed), playlistFixed_, 500, 0);

    display_playlist_tab ();
    //gtk_widget_queue_draw (playlistFixed);

    gtk_widget_show_all (playlistTab);

    
}



//=================================================================
//                     HISTORY LOAD SAVE
//=================================================================
void load_history_record ()
{
    int i = 0;

    // Load record form file set in RFhistory_
    while (true)
    {
        char buff[100];
        sprintf (buff , "%d", i);
        Bottle &grp = RFhistory_.findGroup(buff);
        if (grp.isNull ())
            break;
        else
        {
            string type = grp.find ("type").asString ().c_str();
            string name = grp.find ("name").asString ().c_str();
            string command = grp.find ("command").asString ().c_str();
            //p.success = grp.find ("success").asInt ();
            double tim  = grp.find ("time").asDouble ();
            int id = i;
           
            add_record (&PlaylistTab::history_, id, type, name, tim, command);
        }
        i++;
    }
}

void saveHistoryToFile(char* file)
{
    int totalRec = PlaylistTab::history_.size();
    history_record tmp;

    cout<<"saving to: "<< file << endl;

    ofstream out;
    out.open(file);
    if(!out)
    {
        cout << "GUI can't open in write mode the trajectory file: " << file << endl
            << "Aborting." << endl;
        return;
    }
    else
        cout << "GUI: saving " << totalRec << " items on " << file << endl;

    std::deque<history_record>::iterator i = PlaylistTab::history_.begin ();
    int j = 0;
    for(; i < PlaylistTab::history_.end (); i++, j++)
    {
        out << "[" << j << "]" << endl;
        out << "name " << '"' << i->name <<'"' << endl;
        out << "type " << '"' << i->type << '"'<< endl;
        //out << "success " << i->success << endl;
        out << "command " << '"' << i->command << '"' << endl;
        out << "time " << i->time << endl;
        out << "date " << std::setprecision (15) << i->lastTimePlayed << endl;
        out << endl;
    }
    out.close();
}


void open_save_history_dialog_box ()
{
    GtkWidget *dialog;
     
    dialog = gtk_file_chooser_dialog_new ("Save History",
                          GTK_WINDOW(window_),
                          GTK_FILE_CHOOSER_ACTION_SAVE,
                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                          GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                          NULL);
    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
    

    gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), string ("ID_" + currentDateTime () + "_WoZ.history").c_str ());
     
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
     
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        saveHistoryToFile (filename);
        g_free (filename);
    }
     
    gtk_widget_destroy (dialog);
}

//=================================================================
//                     PLAYLIST LOAD SAVE
//=================================================================
void load_playlist_from_file (char* filename, deque<history_record>* list)
{
    ResourceFinder RFPLaylist;
    //retrieve information for the list of parts
    RFPLaylist.setDefaultConfigFile(filename);
    RFPLaylist.configure("MACSI_ROOT", 0, NULL);

    list->clear ();
    int i = 0;
    while (true)
    {
        char buff[100];
        sprintf (buff , "%d", i);
        Bottle &grp = RFPLaylist.findGroup(buff);
        if (grp.isNull ())
            break;
        else
        {
                     
            string type = grp.find ("type").asString ().c_str();
            string name = grp.find ("name").asString ().c_str();
            string command = grp.find ("command").asString ().c_str();
            //p.success = grp.find ("success").asInt ();
            double tim  = grp.find ("time").asDouble ();
            int id = i;
           
            add_record (list, id, type, name, tim, command);
        }
        i++;
    }
    cout << "INFO: Load playlist from " << filename << " : " << list->size () << " records loaded" << endl;
    string info = "Playlist loaded from " + string (filename);
    infoTab::instance()->add_info (info);
    PlaylistTab::display_playlist_tab ();
    display_play_list ();
}

void open_load_playlist_dialog_box (GtkWidget *w, deque<history_record> *list)
{
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new ("Choose a playlist to load",
                          GTK_WINDOW(window_),
                          GTK_FILE_CHOOSER_ACTION_OPEN,
                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                          NULL);
     
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
     
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        load_playlist_from_file (filename, list);
        g_free (filename);
    }
     
    gtk_widget_destroy (dialog);
}
void open_save_playlist_dialog_box (GtkWidget *w, deque<history_record> *list)
{
    GtkWidget *dialog;
     
    dialog = gtk_file_chooser_dialog_new ("Save Playlist",
                          GTK_WINDOW(window_),
                          GTK_FILE_CHOOSER_ACTION_SAVE,
                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                          GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                          NULL);
    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
    

    gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), "my_playlist");
     
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
     
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        savePlaylistToFile (filename, list);
        g_free (filename);
    }
     
    gtk_widget_destroy (dialog);
}

void savePlaylistToFile(char* file, deque<history_record>* list)
{
    int totalRec = list->size();
    history_record tmp;

    cout<<"saving to: "<< file << endl;

    ofstream out;
    out.open(file);
    if(!out)
    {
        cout<<"GUI can't open in write mode the trajectory file: " << file << endl
            << "Aborting." << endl;
        return;
    }
    else
        cout << "GUI: saving " << totalRec << " items on " << file << endl;
    std::deque<history_record>::iterator i = list->begin ();
    int j = 0;
    for(; i != list->end (); i++, j++)
    {
        out << "[" << j << "]" << endl;
        out << "name " << '"'<< i->name << '"'<< endl;
        out << "type " << '"' << i->type << '"'<< endl;
        //out << "success " << i->success << endl;
        out << "time " << i->time << endl;
        out << "command " << '"' << i->command << '"' << endl;
        out << endl;
    }
    out.close();
    infoTab::instance()->add_info (string ("Save playlist to ") + file);
}

void clear_history (GtkWidget w, gpointer p)
{
    PlaylistTab::history_.clear ();
    infoTab::instance()->add_info ("History Clear !");
    paramTab::clear ();  
    PlaylistTab::display_playlist_tab ();
    display_play_list ();
    PlaylistTab::display_history_tab ();
}

PlaylistTab *PlaylistTab::instance ()
{
    if (playlistTab_)
        return (playlistTab_);
    else
    {
        playlistTab_ = new PlaylistTab ();
        return (playlistTab_);
    }
}

