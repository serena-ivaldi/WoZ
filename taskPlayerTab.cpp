#include <sstream>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "WoZ.hh"
#include "edit.hh"

#define FROMINITPOSLABEL "From initial Pose"
#define FROMINITPOSJOINTLABEL "From initial Pose + joint"
#define RELATIVELABEL "Relative Mouvement"



#define FROMINITPOSVALUE 0
#define FROMINITPOSJOINTVALUE 1
#define RELATIVEVALUE 2


#define CTRLVELOCITYLABEL "Velocity"
#define CTRLVELSOFTLABEL "Soft Imp"
#define CTRLVELMEDIUMLABEL "Medium Imp"
#define CTRLVELHARDLABEL "Hard Imp"


#define CTRLVELOCITYVALUE 0
#define CTRLVELSOFTVALUE 1
#define CTRLVELMEDIUMVALUE 2
#define CTRLVELHARDVALUE 3

TaskPlayerTab *TaskPlayerTab::taskPlayerTab_ = 0;
string TaskPlayerTab::current_filename = "";




void TaskPlayerTab::displayAndLoadTaskRecord (GtkWidget* widget)
{
    clear_widget (widget);
    GtkWidget *table; 

    table  = gtk_table_new (10, 1, true);
    GtkWidget *listRecordFixed = gtk_fixed_new ();
    gtk_table_attach_defaults (GTK_TABLE (table), listRecordFixed, 0, 1, 0, 9);
    gtk_container_add(GTK_CONTAINER(widget), table);


    GtkWidget *scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(listRecordFixed), scrolledwindow);

    GtkWidget *armRecordExampleFixed;
    //---- example entry from conf file
    GtkWidget *armRecordExampleButton;
    armRecordExampleFixed = gtk_fixed_new ();
    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW(scrolledwindow), armRecordExampleFixed);
    gtk_widget_set_size_request (scrolledwindow, 400, 600);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_show (scrolledwindow);

    GtkWidget *reloadButton = gtk_button_new_with_label ("Reload List");
    add_toolTips (reloadButton, "Refresh list of record available and command taskPlayer to load this files");
    g_signal_connect (G_OBJECT(reloadButton), "clicked", G_CALLBACK (reloadOnClick), (gpointer) widget);
    

    gtk_table_attach_defaults (GTK_TABLE (table), reloadButton, 0, 1, 9, 10);

    

    GtkWidget *armRecordButton;
    DIR *dp;
    string *filepath;
    struct dirent *dirp;
    struct stat filestat;
    int i = 0;
    dp = opendir(taskRecordDir_.c_str ());
    if (dp == NULL)
    {
        cout << "Error(" << errno << ") opening '" << taskRecordDir_.c_str () << "'" << endl;
        return;
    }
    while ((dirp = readdir( dp )))
    {
        string filename = dirp->d_name;
        if (filename == "." || filename == "..") 
            continue;
        if (filename.size () > 7 && filename.substr (filename.size () - 7, filename.size ()) != ".record") 
            continue;
        if (filename.substr (0, 1) == ".")
            continue;


        filepath = new string (taskRecordDir_.c_str () + filename);
        armRecordButton = gtk_button_new_with_label (filename.c_str ());
        gtk_widget_set_size_request(armRecordButton, 282, 35);
        gtk_fixed_put(GTK_FIXED(armRecordExampleFixed), armRecordButton, 0, 35 * i++);
        gtk_widget_show (armRecordButton);
        g_signal_connect(G_OBJECT(armRecordButton), "clicked", G_CALLBACK(armRecordButton_click), (gpointer) filepath);

        Bottle bot;
        string cmd = "Load " + *filepath;
        bot.addString (cmd.c_str ());

        outPortTaskPlayer_.write (bot);
    }


    closedir( dp );






    gtk_widget_show_all (widget);
}

void reloadOnClick (GtkWidget* w, gpointer p)
{
    TaskPlayerTab *tab = TaskPlayerTab::instance ();
    tab->displayAndLoadTaskRecord ((GtkWidget*) p);
}

void stopOnClick ()
{
    Bottle bot;
    bot.addString ("STOP");
    outPortTaskPlayer_.write (bot);
}

void TaskPlayerTab::displayTaskPlayerOption(GtkWidget *w)
{
    GtkWidget *table = gtk_table_new (10, 10, true);
    gtk_container_add(GTK_CONTAINER(w), table);

    //add slection list Type of mouvemnent
    GtkWidget *initModeLabel = gtk_label_new ("Type of mouvement");
    init_mode_combo = gtk_combo_box_new_text();
    gtk_combo_box_append_text(GTK_COMBO_BOX(init_mode_combo), FROMINITPOSLABEL);
    gtk_combo_box_append_text(GTK_COMBO_BOX(init_mode_combo), FROMINITPOSJOINTLABEL);
    gtk_combo_box_append_text(GTK_COMBO_BOX(init_mode_combo), RELATIVELABEL);
    gtk_combo_box_set_active(GTK_COMBO_BOX(init_mode_combo), 0);
    g_signal_connect(G_OBJECT(init_mode_combo), "changed", G_CALLBACK(armRecordButton_click), (gpointer) &current_filename);

    gtk_table_attach_defaults (GTK_TABLE(table), initModeLabel, 0, 4, 0, 1);
    gtk_table_attach_defaults (GTK_TABLE(table), init_mode_combo, 4, 10, 0, 1);


    //add slectionList Type of control
    GtkWidget *controlModeLabel = gtk_label_new ("Type of control");
    control_mode_combo = gtk_combo_box_new_text();
    gtk_combo_box_append_text(GTK_COMBO_BOX(control_mode_combo), CTRLVELOCITYLABEL);
    gtk_combo_box_append_text(GTK_COMBO_BOX(control_mode_combo), CTRLVELSOFTLABEL);
    gtk_combo_box_append_text(GTK_COMBO_BOX(control_mode_combo), CTRLVELMEDIUMLABEL);
    gtk_combo_box_append_text(GTK_COMBO_BOX(control_mode_combo), CTRLVELHARDLABEL);
    gtk_combo_box_set_active(GTK_COMBO_BOX(control_mode_combo), 0);
    g_signal_connect(G_OBJECT(control_mode_combo), "changed", G_CALLBACK(armRecordButton_click), (gpointer) &current_filename);

    gtk_table_attach_defaults (GTK_TABLE(table), controlModeLabel, 0, 4, 1, 2);
    gtk_table_attach_defaults (GTK_TABLE(table), control_mode_combo, 4, 10, 1, 2);


    //set Color for button
    GdkColor green;
    gdk_color_parse ("#b4ff82", &green);

    GdkColor red;
    gdk_color_parse ("#ff9486", &red);



    GtkWidget* warningLabel = gtk_label_new ("Warning: The displayed trajectory is based on the \ncoordinates x y z recorded in the selected file and \n do NOT represent a relative mouvement even if \n you have selected this option");
    gtk_table_attach_defaults (GTK_TABLE(table), warningLabel, 0, 10, 6, 8);

    //Display Mouvement
    GtkWidget *displayButton = gtk_button_new_with_label ("Display Mouvement");
    g_signal_connect(G_OBJECT(displayButton), "clicked", G_CALLBACK(displayMouvementOnClick), (gpointer) &current_filename);
    gtk_widget_modify_bg ( GTK_WIDGET(displayButton), GTK_STATE_NORMAL, &green);
    gtk_table_attach_defaults (GTK_TABLE(table), displayButton, 4, 10, 8, 9);

    //Clear  mouvement button
    GtkWidget *clearButton = gtk_button_new_with_label ("Clear");
    g_signal_connect(G_OBJECT(clearButton), "clicked", G_CALLBACK(clearMouvementOnClick), NULL);
    gtk_widget_modify_bg ( GTK_WIDGET(clearButton), GTK_STATE_NORMAL, &red);
    gtk_table_attach_defaults (GTK_TABLE(table), clearButton, 0, 4, 8, 9);


    //Start task button
    GtkWidget *startButton = gtk_button_new_with_label ("Start Recorded Mouvement");
    g_signal_connect(G_OBJECT(startButton), "clicked", G_CALLBACK(start_on_click), (gpointer) &current_filename);
    gtk_widget_modify_bg ( GTK_WIDGET(startButton), GTK_STATE_NORMAL, &green);
    gtk_table_attach_defaults (GTK_TABLE(table), startButton, 4, 10, 9, 10);

    //STOP playlist button
    GtkWidget *stopButton = gtk_button_new_with_label ("STOP");
    g_signal_connect(G_OBJECT(stopButton), "clicked", G_CALLBACK(stopOnClick), NULL);
    gtk_widget_modify_bg ( GTK_WIDGET(stopButton), GTK_STATE_NORMAL, &red);
    gtk_table_attach_defaults (GTK_TABLE(table), stopButton, 0, 4, 9, 10);
}

void TaskPlayerTab::createTaskPlayerTab (GtkWidget *notebook)
{
    GtkWidget *taskRecordTab = gtk_frame_new ("How I move");
    GtkWidget *taskRecordLabel = gtk_label_new("TaskPlayer");


    gtk_notebook_append_page (GTK_NOTEBOOK(notebook), taskRecordTab, taskRecordLabel);

    // Add table for display
    GtkWidget *generalTable = gtk_table_new (10, 10, true);
    gtk_container_add(GTK_CONTAINER(taskRecordTab), generalTable);

    //display listRecord in table
    GtkWidget *listRecordFrame = gtk_frame_new ("List of known moves");
    gtk_table_attach_defaults (GTK_TABLE (generalTable), listRecordFrame, 0, 5, 0, 10);
    displayAndLoadTaskRecord (listRecordFrame);

    //display listRecord in table
    GtkWidget *optionFrame = gtk_frame_new ("Options");
    gtk_table_attach_defaults (GTK_TABLE (generalTable), optionFrame, 5, 10, 0, 10);
    displayTaskPlayerOption(optionFrame);


    gtk_widget_show_all(generalTable);
 
    return; 

}

void armRecordButton_click (GtkWidget *w, gpointer p)
{

    string *filename = (string*) p;
    TaskPlayerTab::current_filename = *filename;

    history_record h = create_task_history_record (*filename);

    display_show_panel (w, &h);
    return;

}

history_record create_task_history_record (string filename)
{
    TaskPlayerTab *taskPlayerTab = TaskPlayerTab::instance ();
    string initMode =  gtk_combo_box_get_active_text(GTK_COMBO_BOX(taskPlayerTab->init_mode_combo));
    string controlMode =  gtk_combo_box_get_active_text(GTK_COMBO_BOX(taskPlayerTab->control_mode_combo));

    int init_mode_value = 0;

    cout << "InitPose: " << initMode << endl;
    cout << "ControlMode: " << controlMode << endl;

    if (initMode == FROMINITPOSLABEL)
        init_mode_value = FROMINITPOSVALUE;
    if (initMode == FROMINITPOSJOINTLABEL)
        init_mode_value = FROMINITPOSJOINTVALUE;
    if (initMode == RELATIVELABEL)
        init_mode_value = RELATIVEVALUE;

    int ctrl_mode_value = 0;

    if (controlMode == CTRLVELOCITYLABEL)
        ctrl_mode_value = CTRLVELOCITYVALUE;
    if (controlMode == CTRLVELSOFTLABEL)
        ctrl_mode_value = CTRLVELSOFTVALUE;
    if (controlMode == CTRLVELMEDIUMLABEL)
        ctrl_mode_value = CTRLVELMEDIUMVALUE;
    if (controlMode == CTRLVELHARDLABEL)
        ctrl_mode_value = CTRLVELHARDVALUE;

    history_record h;
    h.type = "taskRecord";
    h.name = filename;
    h.command = "Play " + filename + " " + i2s(init_mode_value) + " " + i2s(ctrl_mode_value);

    return h;
}

void displayMouvementOnClick (GtkWidget* w, gpointer p)
{
    if (TaskPlayerTab::current_filename == "")
    {
        cout << "No task Selected" << endl;
        return;
    }
    Bottle b;
    b.addString (("Display " + TaskPlayerTab::current_filename).c_str ());
    outPortTaskPlayer_.write (b);
}

void clearMouvementOnClick (GtkWidget* w, gpointer p)
{
    if (TaskPlayerTab::current_filename == "")
    {
        cout << "No task Selected" << endl;
        return;
    }
    Bottle b;
    b.addString (("Clear " + TaskPlayerTab::current_filename).c_str ());
    outPortTaskPlayer_.write (b);
}

void start_on_click (GtkWidget* w, gpointer p)
{
    if (TaskPlayerTab::current_filename == "")
    {
        cout << "No task Selected" << endl;
        return;
    }
    history_record h = create_task_history_record (TaskPlayerTab::current_filename);

    send_ArmRecord (&h);
}

void send_ArmRecord (history_record *h)
{
    string command = h->command;

    Bottle bot;
    bot.addString (command.c_str ());
    cout << "Arm Record: " << bot.toString () << endl;
    outPortTaskPlayer_.write (bot);

    saveToHistory(h->type, h->name, 0, command);
}


TaskPlayerTab *TaskPlayerTab::instance ()
{
    if (taskPlayerTab_)
        return (taskPlayerTab_);
    else
    {
        TaskPlayerTab::taskPlayerTab_ = new TaskPlayerTab ();
        TaskPlayerTab::taskPlayerTab_->taskRecordDir_ = "";
        TaskPlayerTab::taskPlayerTab_->current_filename = "";
        return (TaskPlayerTab::taskPlayerTab_);
    }
}
