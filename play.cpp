#include "WoZ.hh"


//=============================================
//              	DISPLAY
//=============================================
Port outPort;

void refresh_play ()
{
	display_play_list ();
}

void display_play ()
{
    GtkWidget *playLabel = gtk_label_new("Play");

    
    // Create frame
	playTabFrame_ = gtk_frame_new ("I want to play with you");
    gtk_container_set_border_width (GTK_CONTAINER (playTabFrame_), 10);
    gtk_notebook_append_page (GTK_NOTEBOOK(p_notebook_), playTabFrame_, playLabel);
    //table for display
    playTabFixed_ = gtk_fixed_new ();
    GtkWidget *playTabMenu = gtk_fixed_new (); 
    GtkWidget *table = gtk_table_new (10, 10, true);
    //gtk_widget_set_size_request (table, 290, 200); 

    playingInfoTab_ = gtk_fixed_new ();     

    gtk_container_add(GTK_CONTAINER(playTabFrame_), table);
	gtk_table_attach_defaults (GTK_TABLE (table), playTabFixed_, 0, 9, 0, 3);	
	gtk_table_attach_defaults (GTK_TABLE (table), playTabMenu, 5, 10, 0, 5);
	gtk_table_attach_defaults (GTK_TABLE (table), playingInfoTab_, 5, 10, 5, 8);
	
	display_play_list ();	
	display_play_menu (playTabMenu);
	display_playing_tab ();


	gtk_widget_show_all (playTabFixed_);
}

void display_play_menu (GtkWidget *w)
{
    //LOAD SAVE BUTTONS
    GtkWidget *menu_load_frame;
    GtkWidget *menu_load_fixed;
	menu_load_frame = gtk_frame_new ("Load/Save");
	menu_load_fixed = gtk_fixed_new ();

	gtk_container_add(GTK_CONTAINER(menu_load_frame), menu_load_fixed);
	gtk_container_add(GTK_CONTAINER(w), menu_load_frame);

	gtk_widget_set_size_request (menu_load_frame, 400, 100);
	gtk_fixed_put (GTK_FIXED(w), menu_load_frame, 0, 0);
   

    //Load playlist button
    GtkWidget *LoadPlaylistButton;
    LoadPlaylistButton = gtk_button_new_with_label ("Load Playlist");
    g_signal_connect(G_OBJECT (LoadPlaylistButton), "clicked", G_CALLBACK (open_load_playlist_dialog_box), (gpointer) &list_to_play_);
    gtk_widget_set_size_request (LoadPlaylistButton, 300, 30);
    gtk_fixed_put (GTK_FIXED(menu_load_fixed), LoadPlaylistButton, 30, 0);    
    add_toolTips (LoadPlaylistButton, "Load a playlist from a file");


    //save playlist button
    GtkWidget *savePlaylistButton;
    savePlaylistButton = gtk_button_new_with_label ("Save Playlist");
    g_signal_connect(G_OBJECT (savePlaylistButton), "clicked", G_CALLBACK (open_save_playlist_dialog_box), (gpointer) &list_to_play_);
    gtk_widget_set_size_request (savePlaylistButton, 300, 30);
    gtk_fixed_put (GTK_FIXED(menu_load_fixed), savePlaylistButton, 30, 35);       
    add_toolTips (savePlaylistButton, "Save the current playlist in a file");


    // PLAY STOP BUTTONS
    GtkWidget *menu_play_frame;
    GtkWidget *menu_play_fixed;
	menu_play_frame = gtk_frame_new ("Play");
	menu_play_fixed = gtk_fixed_new ();

	gtk_container_add(GTK_CONTAINER(menu_play_frame), menu_play_fixed);
	gtk_container_add(GTK_CONTAINER(w), menu_play_frame);

	gtk_widget_set_size_request (menu_play_frame, 400, 160);
	gtk_fixed_put (GTK_FIXED(w), menu_play_frame, 0, 100);
   

	gpointer p = &(list_to_play_[0]);
    //PLAY playlist button
    GtkWidget *playButton;
    GdkColor green;
	gdk_color_parse ("#b4ff82", &green);
    playButton = gtk_button_new_with_label ("Play");
	gtk_widget_modify_bg ( GTK_WIDGET(playButton), GTK_STATE_PRELIGHT, &green);
    g_signal_connect(G_OBJECT (playButton), "clicked", G_CALLBACK (play_playlist), p);
    gtk_widget_set_size_request (playButton, 100, 100);
    gtk_fixed_put (GTK_FIXED(menu_play_fixed), playButton, 30, 0);    
    add_toolTips (playButton, "Start playlist from the begining");


    //STOP playlist button
    GtkWidget *stopButton;
    GdkColor red;
	gdk_color_parse ("#ff9486", &red);
    stopButton = gtk_button_new_with_label ("STOP");
	gtk_widget_modify_bg ( GTK_WIDGET(stopButton), GTK_STATE_PRELIGHT, &red);
	//gtk_widget_modify_text ( GTK_WIDGET(gtk_button_get_label(GTK_BUTTON(stopButton))), GTK_STATE_NORMAL, &red);
    g_signal_connect(G_OBJECT (stopButton), "clicked", G_CALLBACK (set_stop), NULL);
    gtk_widget_set_size_request (stopButton, 140, 140);
    gtk_fixed_put (GTK_FIXED(menu_play_fixed), stopButton, 160, 0);
    add_toolTips (stopButton, "Send the 'STOP command' and stop the current playlist");

}

void display_playing_tab ()
{



	GList *children, *iter;
    if (GTK_IS_CONTAINER (playingInfoTab_))
    {

	    children = gtk_container_get_children (GTK_CONTAINER(playingInfoTab_));
	    for(iter = children; iter != NULL; iter = g_list_next(iter))
	        gtk_widget_destroy(GTK_WIDGET(iter->data));
	    g_list_free(children);
	}

	GtkWidget *playingInfoFrame = gtk_frame_new ("");
	gtk_widget_set_size_request (playingInfoFrame, 350, 100);
	gtk_fixed_put (GTK_FIXED (playingInfoTab_), playingInfoFrame, 0, 0);

	GtkWidget *playingInfoTable = gtk_table_new (4, 1, true);
	gtk_widget_set_size_request (playingInfoTable, 195, 100);
	gtk_container_add(GTK_CONTAINER(playingInfoFrame), playingInfoTable);

    GdkColor red, green;
   
    gdk_color_parse ("green", &green);
	gdk_color_parse ("red", &red);
	string label;
	if (is_playing_)
		label = "Playing";
	else
		label = "Not Playing";
	GtkWidget *playingLabel = gtk_label_new (label.c_str ());
	if (is_playing_)
		gtk_widget_modify_fg ( GTK_WIDGET(playingLabel), GTK_STATE_NORMAL, &green);
	else
		gtk_widget_modify_fg ( GTK_WIDGET(playingLabel), GTK_STATE_NORMAL, &red);

	//gtk_table_attach_defaults (GTK_TABLE (playingInfoTable), playingLabel, 0, 3, 0, 1);
	gtk_table_attach(GTK_TABLE(playingInfoTable), playingLabel, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
	label = "_________________________________________________";
	GtkWidget *playingMarkLabel = gtk_label_new (label.c_str ());
	gtk_table_attach(GTK_TABLE(playingInfoTable), playingMarkLabel, 0, 1, 3, 4, GTK_FILL, GTK_FILL, 0, 0);


	string name = "";
	string id = "0";
	if (currentPlayingItem_)
	{
		name = currentPlayingItem_->name;
		id = i2s(currentPlayingItem_->id);
	}

	label = string ("Playing: ") + id + "/" + i2s(list_to_play_.size () - 1);
	GtkWidget *playingItemLabel = gtk_label_new (label.c_str ());
	//gtk_table_attach_defaults (GTK_TABLE (playingInfoTable), playingItemLabel, 0, 3, 1, 2);
	gtk_table_attach(GTK_TABLE(playingInfoTable), playingItemLabel, 0, 1, 1, 2, GTK_FILL, GTK_FILL, 0, 0);

	label = "Name: " + name;
	GtkWidget *playingItemNameLabel = gtk_label_new (label.c_str ());
	//gtk_table_attach_defaults (GTK_TABLE (playingInfoTable), playingItemNameLabel, 0, 3, 2, 3);
	gtk_table_attach(GTK_TABLE(playingInfoTable), playingItemNameLabel, 0, 1, 2, 3, GTK_FILL, GTK_FILL, 0, 0);



	gtk_widget_show_all (playingInfoTab_);

}

void display_play_list ()
{
    //remove all child from playlistListFixed_ 
    GList *children, *iter;
    if (GTK_IS_CONTAINER (playlistListFixed_))
    {

	    children = gtk_container_get_children (GTK_CONTAINER(playlistListFixed_));
	    for(iter = children; iter != NULL; iter = g_list_next(iter))
	        gtk_widget_destroy(GTK_WIDGET(iter->data));
	    g_list_free(children);
	}

    playlistListFixed_ = gtk_fixed_new ();



    // scroll window for playlist items
    GtkWidget *scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_set_size_request (scrolledwindow, 300, 600);
    //gtk_widget_set_size_request (playlistListFixed_, 500, 200);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(playTabFixed_), scrolledwindow);
    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW(scrolledwindow), playlistListFixed_);



    // create playlist item into playlist panel
    int i = 0;
    for (unsigned int i = 0; i < list_to_play_.size (); i++)
    {
        //Container        
        GtkWidget *playlistItemFrame;
        playlistItemFrame = gtk_frame_new ((string("id: ") + i2s (list_to_play_[i].id)).c_str ());
        gtk_widget_set_size_request (playlistItemFrame, 290, 200);        
        gtk_container_set_border_width (GTK_CONTAINER (playlistItemFrame), 10);
        gtk_fixed_put(GTK_FIXED(playlistListFixed_), playlistItemFrame, 0, i * 205);

        //table for display
        GtkWidget *table = gtk_table_new (4, 4, true);
        gtk_widget_set_size_request (table, 290, 200);        
        gtk_container_add(GTK_CONTAINER(playlistItemFrame), table);

        // display history_record
        GtkWidget *table_history =  display_history_record (list_to_play_[i]);
        gtk_table_attach_defaults (GTK_TABLE (table), table_history, 0, 3, 0, 3);


        gpointer p = &(list_to_play_[i]);
        if (!is_playing_)
        {
		    GdkColor red, green;
		   
		    gdk_color_parse ("#b4ff82", &green);
    		gdk_color_parse ("#ff9486", &red);

	        // button "Delete item from playlist"
	        GtkWidget *delete_playlist_item_button = gtk_button_new_with_label ("X");
	        g_signal_connect (G_OBJECT(delete_playlist_item_button), "clicked", G_CALLBACK (delete_playlist_item), p);
	        g_object_set_data (G_OBJECT(delete_playlist_item_button), "list", (gpointer) &list_to_play_);
	        gtk_widget_set_size_request (delete_playlist_item_button, 10, 10);
	        gtk_table_attach_defaults (GTK_TABLE (table), delete_playlist_item_button, 3, 4, 0, 1);    
			add_toolTips (delete_playlist_item_button, "Remove this action");
			gtk_widget_modify_bg ( GTK_WIDGET(delete_playlist_item_button), GTK_STATE_PRELIGHT, &red);

	        //button for edit
	        GtkWidget *display_edit_item_button = gtk_button_new_with_label ("Edit");
	        g_object_set_data (G_OBJECT(display_edit_item_button), "list", (gpointer) &list_to_play_);
	        g_signal_connect (G_OBJECT(display_edit_item_button), "clicked", G_CALLBACK (display_edit_panel), p);
	        gtk_widget_set_size_request (display_edit_item_button, 10, 10);
	        gtk_table_attach_defaults (GTK_TABLE (table), display_edit_item_button, 3, 4, 1, 2);        
			add_toolTips (display_edit_item_button, "Edit this action");

	        //button for play item
	        GtkWidget *play_item_button = gtk_button_new_with_label (">");
	        g_signal_connect (G_OBJECT(play_item_button), "clicked", G_CALLBACK (play_item_from_playlist), p);
	        gtk_widget_set_size_request (play_item_button, 10, 10);
	        gtk_table_attach_defaults (GTK_TABLE (table), play_item_button, 3, 4, 2, 3);   	
			add_toolTips (play_item_button, "Play this action");
			gtk_widget_modify_bg ( GTK_WIDGET(play_item_button), GTK_STATE_PRELIGHT, &green);

	        //button for play playlist from this item
	        GtkWidget *play_playlist_from_item_button = gtk_button_new_with_label (">>");
	        g_signal_connect (G_OBJECT(play_playlist_from_item_button), "clicked", G_CALLBACK (play_playlist), p);
	        gtk_widget_set_size_request (play_playlist_from_item_button, 10, 10);
	        gtk_table_attach_defaults (GTK_TABLE (table), play_playlist_from_item_button, 3, 4, 3, 4);
			add_toolTips (play_playlist_from_item_button, "Play the playlist from here");
			gtk_widget_modify_bg ( GTK_WIDGET(play_playlist_from_item_button), GTK_STATE_PRELIGHT, &green);
 	   }
}

    gtk_widget_show_all (playTabFrame_);
}


//=============================================
//              	PLAY
//=============================================

void *play_playlist_in_thread (void *i)
{
	std::deque<history_record>::iterator *i_p = (std::deque<history_record>::iterator*) i;
	std::deque<history_record>::iterator it = *i_p;

	//gdk_threads_enter ();
	if (!list_to_play_.empty ())
	{		
		while (is_playing_ && it != list_to_play_.end ())
		{
			//g_mutex_unlock (mutex_stop_);
		 	play_item_t (&(*it));
		 	if (it->time == 0)
		 		sleep (4);
		 	else 
		 		sleep (it->time);
		 	it++;
		}
	}
	gdk_threads_enter ();
	infoTab::instance()->add_info ("Playlist ended !");
	is_playing_ = false;
	refresh_play ();
	gdk_threads_leave ();
}

void play_playlist (GtkWidget *w, gpointer i)
{
	if (is_playing_)
	{
		infoTab::instance()->add_info ("The playlist is already running !");
		return;
	}
	is_playing_ = true;
	infoTab::instance()->add_info ("Start the playlist ! ");
	history_record *h = (history_record*) i;
	
	std::deque<history_record>::iterator it = list_to_play_.begin ();
	while (it != list_to_play_.end () && it->id != h->id)
		it++;
	pthread_create (&play_thread_, NULL, play_playlist_in_thread, (void *) &it);
	refresh_play ();
}

void play_item_from_playlist (GtkWidget *w, gpointer i)
{
	history_record *h = (history_record*) i;
    play_item (h);
}

void play_item (history_record *h)
{
	currentPlayingItem_ = h;
	display_playing_tab ();
	Bottle command;
	if (h->type == "Expression")
	{
		send_expression (h);
	}
	if (h->type == "Speech")
	{
		send_speech (h);
	}
	if (h->type == "LookAt" || h->type == "Home" || h->type == "moveHand")
	{
		send_to_actionsServer (h);
	}
	if (h->type == "Action")
	{
		send_command_action (h);
	}
	

}

void play_item_t (history_record *h)
{

	gdk_threads_enter ();
	play_item (h);
	
	gdk_threads_leave ();
		
}

void send_stop ()
{
	Bottle command;
	command.addString ("action: STOP  (Not implemented yet)");
}


void set_stop (GtkWidget* w, gpointer p)
{

	send_stop ();

	is_playing_ = false;
	infoTab::instance()->add_info ("INFO: You click on STOP !");

	refresh_play ();
	return;
}
