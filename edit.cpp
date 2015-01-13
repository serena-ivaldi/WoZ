#include "WoZ.hh"

void undisplay_edit_tab ()
{
    paramTab::clear ();
    return;
}


void display_edit_panel (GtkWidget *w, gpointer p)
{
	// Start to clear panel
	paramTab::clear ();

    //cast gpointer to history_record
    history_record *h = (history_record*) p;

    //cast list
    gpointer data_p = g_object_get_data (G_OBJECT(w), "list");
    std::deque<history_record> *list = (std::deque<history_record>*) data_p; 

    // create table for display

    //table for display
    GtkWidget *table = gtk_table_new (6, 2, true);
    gtk_widget_set_size_request (table, 500, 200);        
    gtk_container_add(GTK_CONTAINER(paramTab::container_), table);

    GdkColor red, green;
    gdk_color_parse ("#b4ff82", &green);
    gdk_color_parse ("#ff9486", &red);
    //button modify 
    GtkWidget *modify_item_button = gtk_button_new_with_label ("Modify");
    gtk_widget_set_size_request (modify_item_button, 10, 10);
	g_object_set_data (G_OBJECT(modify_item_button), "list", (gpointer) list);
	g_object_set_data (G_OBJECT(modify_item_button), "item", p);
    gtk_table_attach_defaults (GTK_TABLE (table), modify_item_button, 1, 2, 5, 6);
	g_signal_connect (G_OBJECT(modify_item_button), "clicked", G_CALLBACK (modify_item_from_edit_tab), NULL);
	add_toolTips (modify_item_button, "Modify action (This action do not modify the playlist file)");
    gtk_widget_modify_bg ( GTK_WIDGET(modify_item_button), GTK_STATE_PRELIGHT, &green);

	//button cancel
    GtkWidget *cancel_item_button = gtk_button_new_with_label ("Close");
    g_signal_connect (G_OBJECT(cancel_item_button), "clicked", G_CALLBACK (undisplay_edit_tab), NULL);
    gtk_widget_set_size_request (cancel_item_button, 10, 10);
    gtk_table_attach_defaults (GTK_TABLE (table), cancel_item_button, 0, 1, 5, 6);
    add_toolTips (cancel_item_button, "Close this panel whithout modification");
    gtk_widget_modify_bg ( GTK_WIDGET(cancel_item_button), GTK_STATE_PRELIGHT, &red);

    //Type
    GtkWidget *labelType;
    labelType = gtk_label_new ("Type:");
    string type = h->type;
    GtkWidget *typeEdit = gtk_entry_new ();
    gtk_entry_set_editable(GTK_ENTRY(typeEdit),false);
    gtk_entry_set_text(GTK_ENTRY(typeEdit), type.c_str ());
    gtk_table_attach_defaults (GTK_TABLE (table), labelType, 0, 1, 0, 1);
    gtk_table_attach_defaults (GTK_TABLE (table), typeEdit, 1, 2, 0, 1);

    //Name
    GtkWidget *labelName;
    labelName = gtk_label_new ("Name:");
    string name = h->name;
    GtkWidget *nameEdit = gtk_entry_new ();
    gtk_entry_set_editable(GTK_ENTRY(nameEdit),true);
    gtk_entry_set_text(GTK_ENTRY(nameEdit), name.c_str ());
    gtk_table_attach_defaults (GTK_TABLE (table), labelName, 0, 1, 1, 2);
    gtk_table_attach_defaults (GTK_TABLE (table), nameEdit, 1, 2, 1, 2);
    g_object_set_data (G_OBJECT(modify_item_button), "entry_name", (gpointer) nameEdit);

    //TIME
    GtkWidget *labelTime;
    labelTime = gtk_label_new ("Time:");
    double tim = h->time;
    char tim_c[20];
    sprintf (tim_c , "%lf", tim);
    GtkWidget *timeEdit = gtk_entry_new ();
    gtk_entry_set_editable(GTK_ENTRY(timeEdit), true);
    gtk_entry_set_text(GTK_ENTRY(timeEdit), tim_c);
    gtk_table_attach_defaults (GTK_TABLE (table), labelTime, 0, 1, 2, 3);
    gtk_table_attach_defaults (GTK_TABLE (table), timeEdit, 1, 2, 2, 3);
    g_object_set_data (G_OBJECT(modify_item_button), "entry_time", (gpointer) timeEdit);


    //Command
    GtkWidget *labelCommand;
    labelCommand = gtk_label_new ("Command:");
    string command = h->command;
    GtkWidget *commandEdit = gtk_entry_new ();
    gtk_entry_set_editable(GTK_ENTRY(commandEdit), true);
    gtk_entry_set_text(GTK_ENTRY(commandEdit), command.c_str ());
    gtk_table_attach_defaults (GTK_TABLE (table), labelCommand, 0, 1, 3, 4);
    gtk_table_attach_defaults (GTK_TABLE (table), commandEdit, 1, 2, 3, 4);
    g_object_set_data (G_OBJECT(modify_item_button), "entry_command", (gpointer) commandEdit);

    




    gtk_widget_show_all (paramTab::container_);

}


void display_show_panel (GtkWidget *w, gpointer p)
{
    // Start to clear panel
    paramTab::clear ();

    //cast gpointer to history_record
    history_record *h = (history_record*) p;

    //cast list
    gpointer data_p = g_object_get_data (G_OBJECT(w), "list");
    std::deque<history_record> *list = (std::deque<history_record>*) data_p; 

    // create table for display

    //table for display
    GtkWidget *table = gtk_table_new (6, 2, true);
    gtk_widget_set_size_request (table, 500, 200);        
    gtk_container_add(GTK_CONTAINER(paramTab::container_), table);


    //Type
    GtkWidget *labelType;
    labelType = gtk_label_new ("Type:");
    string type = h->type;
    GtkWidget *typeEdit = gtk_entry_new ();
    gtk_entry_set_editable(GTK_ENTRY(typeEdit),false);
    gtk_entry_set_text(GTK_ENTRY(typeEdit), type.c_str ());
    gtk_table_attach_defaults (GTK_TABLE (table), labelType, 0, 1, 0, 1);
    gtk_table_attach_defaults (GTK_TABLE (table), typeEdit, 1, 2, 0, 1);

    //Name
    GtkWidget *labelName;
    labelName = gtk_label_new ("Name:");
    string name = h->name;
    GtkWidget *nameEdit = gtk_entry_new ();
    gtk_entry_set_editable(GTK_ENTRY(nameEdit),true);
    gtk_entry_set_text(GTK_ENTRY(nameEdit), name.c_str ());
    gtk_table_attach_defaults (GTK_TABLE (table), labelName, 0, 1, 1, 2);
    gtk_table_attach_defaults (GTK_TABLE (table), nameEdit, 1, 2, 1, 2);
    
    //TIME
    GtkWidget *labelTime;
    labelTime = gtk_label_new ("Time:");
    double tim = h->time;
    char tim_c[20];
    sprintf (tim_c , "%lf", tim);
    GtkWidget *timeEdit = gtk_entry_new ();
    gtk_entry_set_editable(GTK_ENTRY(timeEdit), true);
    gtk_entry_set_text(GTK_ENTRY(timeEdit), tim_c);
    gtk_table_attach_defaults (GTK_TABLE (table), labelTime, 0, 1, 2, 3);
    gtk_table_attach_defaults (GTK_TABLE (table), timeEdit, 1, 2, 2, 3);
   

    //Command
    GtkWidget *labelCommand;
    labelCommand = gtk_label_new ("Command:");
    string command = h->command;
    GtkWidget *commandEdit = gtk_entry_new ();
    gtk_entry_set_editable(GTK_ENTRY(commandEdit), true);
    gtk_entry_set_text(GTK_ENTRY(commandEdit), command.c_str ());
    gtk_table_attach_defaults (GTK_TABLE (table), labelCommand, 0, 1, 3, 4);
    gtk_table_attach_defaults (GTK_TABLE (table), commandEdit, 1, 2, 3, 4);
   




    gtk_widget_show_all (paramTab::container_);

}