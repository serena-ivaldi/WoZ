#include "status.hh"
#include "WoZ.hh"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;
using namespace std;



GtkWidget *radiobutton_mode_pos[3];
GtkWidget *radiobutton_mode_trq[3];
GtkWidget *radiobutton_mode_imp_s[3];
GtkWidget *radiobutton_mode_imp_m[3];
GtkWidget *radiobutton_mode_imp_h[3];
GtkWidget *stiffnessInfoLabel[3];


robot_interfaces *Irobot;


string StatusTab::robotName_ = "iCubSim_";
StatusTab *StatusTab::StatusTab_ = 0;
std::map<string, string> StatusTab::paramsList_ = std::map<string, string> ();


void StatusTab::createTab (GtkWidget* notebook)
{
	//creation of Tab
	GtkWidget *generalTab;
    GtkWidget *generalLabel = gtk_label_new("Status");

    generalTab = gtk_frame_new ("How i am");
    gtk_container_set_border_width (GTK_CONTAINER (generalTab), 10);
    gtk_notebook_append_page (GTK_NOTEBOOK(notebook), generalTab, generalLabel);

    // Add table for display
    GtkWidget *generalTable = gtk_table_new (20, 20, true);
    gtk_container_add(GTK_CONTAINER(generalTab), generalTable);

    //display traking option in table
    GtkWidget *OptionsFrame = gtk_frame_new ("Options");
    gtk_table_attach_defaults (GTK_TABLE (generalTable), OptionsFrame, 0, 5, 0, 2);
    displayOptions (OptionsFrame);

    //display marker in table
    GtkWidget *MarkerFrame = gtk_frame_new ("Markers");
    gtk_table_attach_defaults (GTK_TABLE (generalTable), MarkerFrame, 5, 20, 0, 20);
    displayMarker (MarkerFrame);

    //display swiftness option in table
    //GtkWidget *swiftnessOptionFrame = gtk_frame_new ("Stiffness");
    //gtk_table_attach_defaults (GTK_TABLE (generalTable), swiftnessOptionFrame, 5, 20, 0, 20);
    //displayStiffnessAndImpedance (swiftnessOptionFrame);

    gtk_widget_show_all (generalTab);
}

void StatusTab::displayMarker (GtkWidget* w)
{
    GtkWidget* generalTable = gtk_table_new (2, 10, false);
    gtk_container_add(GTK_CONTAINER(w), generalTable);

    //task1
    GtkWidget* Marker1Button = gtk_button_new_with_label ("Manipulation Task 1");
    gtk_table_attach_defaults (GTK_TABLE (generalTable), Marker1Button, 0, 1, 0, 1);
    g_signal_connect (G_OBJECT (Marker1Button), "clicked", G_CALLBACK (send_marker), Marker1Button);

    //task2
    GtkWidget* Marker2Button = gtk_button_new_with_label ("Social Task 2");
    gtk_table_attach_defaults (GTK_TABLE (generalTable), Marker2Button, 0, 1, 1, 2);
    g_signal_connect (G_OBJECT (Marker2Button), "clicked", G_CALLBACK (send_marker), Marker2Button);

    //task3
    GtkWidget* Marker3Button = gtk_button_new_with_label ("Fonctional Task 3");
    gtk_table_attach_defaults (GTK_TABLE (generalTable), Marker3Button, 0, 1, 2, 3);
    g_signal_connect (G_OBJECT (Marker3Button), "clicked", G_CALLBACK (send_marker), Marker3Button);

    //Demo
    GtkWidget* Marker7Button = gtk_button_new_with_label ("Demo");
    gtk_table_attach_defaults (GTK_TABLE (generalTable), Marker7Button, 0, 1, 3, 4);
    g_signal_connect (G_OBJECT (Marker7Button), "clicked", G_CALLBACK (send_marker), Marker7Button);


    //task1
    GtkWidget* Marker4Button = gtk_button_new_with_label ("Manipulation END");
    gtk_table_attach_defaults (GTK_TABLE (generalTable), Marker4Button, 1, 2, 0, 1);
    g_signal_connect (G_OBJECT (Marker4Button), "clicked", G_CALLBACK (send_marker), Marker4Button);

    //task2
    GtkWidget* Marker5Button = gtk_button_new_with_label ("Social END");
    gtk_table_attach_defaults (GTK_TABLE (generalTable), Marker5Button, 1, 2, 1, 2);
    g_signal_connect (G_OBJECT (Marker5Button), "clicked", G_CALLBACK (send_marker), Marker5Button);

    //task3
    GtkWidget* Marker6Button = gtk_button_new_with_label ("Fonctional END");
    gtk_table_attach_defaults (GTK_TABLE (generalTable), Marker6Button, 1, 2, 2, 3);
    g_signal_connect (G_OBJECT (Marker6Button), "clicked", G_CALLBACK (send_marker), Marker6Button);


    //Demoend
    GtkWidget* Marker8Button = gtk_button_new_with_label ("Demo END");
    gtk_table_attach_defaults (GTK_TABLE (generalTable), Marker8Button, 1, 2, 3, 4);
    g_signal_connect (G_OBJECT (Marker8Button), "clicked", G_CALLBACK (send_marker), Marker8Button);





}


void send_marker (GtkButton* w)
{
    const gchar *text = gtk_button_get_label (w);

    saveToHistory("Marker", text, 0, "");

}

void StatusTab::displayOptions (GtkWidget* w)
{
    //create tracking mode table for diplay
	GtkWidget* generalTable = gtk_table_new (2, 2, false);
    gtk_container_add(GTK_CONTAINER(w), generalTable);

    //GENERAL
    GtkWidget* showParamsButton = gtk_button_new_with_label ("Show Params");
    gtk_table_attach_defaults (GTK_TABLE (generalTable), showParamsButton, 0, 2, 0, 1);
    g_signal_connect (G_OBJECT (showParamsButton), "clicked", G_CALLBACK (displayRobotParam), NULL);

    //TRACKING OPTION
    // Create Frame
    GtkWidget* trackingFrame = gtk_frame_new ("Tracking Mode");
    GtkWidget* trakingTable = gtk_table_new (3, 3, false);
    gtk_container_add (GTK_CONTAINER (trackingFrame), trakingTable);
    gtk_table_attach_defaults (GTK_TABLE(generalTable), trackingFrame, 0, 1, 1, 2);
    // Create Button
    GtkWidget *trakingModeToOnButton = gtk_button_new_with_label ("On");
    GtkWidget *trakingModeToOffButton = gtk_button_new_with_label ("Off");
    // display Button
    gtk_table_attach_defaults (GTK_TABLE (trakingTable), trakingModeToOnButton, 0, 1, 0, 1);
    gtk_table_attach_defaults (GTK_TABLE (trakingTable), trakingModeToOffButton, 1, 2, 0, 1);

    // Add tooltips 
    add_toolTips (trakingModeToOnButton, ("Switch Tracking Mode to On"));
    add_toolTips (trakingModeToOffButton, ("Switch Tracking Mode to Off"));

    // attach signal
    string* true_s = new string ("true");
    string* false_s = new string ("false");
    g_signal_connect(G_OBJECT (trakingModeToOffButton), "clicked", G_CALLBACK (send_change_tracking_mode_on_click), (gpointer) false_s);
	g_signal_connect(G_OBJECT (trakingModeToOnButton), "clicked", G_CALLBACK (send_change_tracking_mode_on_click), (gpointer) true_s);

    // Enable Torso Option
    // Create Frame
    GtkWidget* torsoFrame = gtk_frame_new ("Torso");
    GtkWidget* torsoTable = gtk_table_new (3, 3, false);
    gtk_container_add (GTK_CONTAINER (torsoFrame), torsoTable);
    gtk_table_attach_defaults (GTK_TABLE(generalTable), torsoFrame, 0, 1, 2, 3);
    // Create Button
    GtkWidget *enableTorsoButton = gtk_button_new_with_label ("On");
    GtkWidget *disableTorsoButton = gtk_button_new_with_label ("Off");
    // display Button
    gtk_table_attach_defaults (GTK_TABLE (torsoTable), enableTorsoButton, 0, 1, 0, 1);
    gtk_table_attach_defaults (GTK_TABLE (torsoTable), disableTorsoButton, 1, 2, 0, 1);

    // Add tooltips 
    add_toolTips (enableTorsoButton, ("Enable Torso"));
    add_toolTips (disableTorsoButton, ("Disable Torso"));

    // attach signal
    g_signal_connect(G_OBJECT (enableTorsoButton), "clicked", G_CALLBACK (send_torso_option_on_click), (gpointer) true_s);
    g_signal_connect(G_OBJECT (disableTorsoButton), "clicked", G_CALLBACK (send_torso_option_on_click), (gpointer) false_s);

    // Enable Speak Option
    // Create Frame
    GtkWidget* speakFrame = gtk_frame_new ("Debug Speech");
    GtkWidget* speakTable = gtk_table_new (3, 3, false);
    gtk_container_add (GTK_CONTAINER (speakFrame), speakTable);
    gtk_table_attach_defaults (GTK_TABLE(generalTable), speakFrame, 0, 1, 3, 4);
    // Create Button
    GtkWidget *enableSpeakButton = gtk_button_new_with_label ("On");
    GtkWidget *disableSpeakButton = gtk_button_new_with_label ("Off");
    // display Button
    gtk_table_attach_defaults (GTK_TABLE (speakTable), enableSpeakButton, 0, 1, 0, 1);
    gtk_table_attach_defaults (GTK_TABLE (speakTable), disableSpeakButton, 1, 2, 0, 1);

    // Add tooltips 
    add_toolTips (enableSpeakButton, ("Enable Debug Speech"));
    add_toolTips (disableSpeakButton, ("Disable Debug Speech"));

    // attach signal
    g_signal_connect(G_OBJECT (enableSpeakButton), "clicked", G_CALLBACK (send_speech_option_on_click), (gpointer) true_s);
    g_signal_connect(G_OBJECT (disableSpeakButton), "clicked", G_CALLBACK (send_speech_option_on_click), (gpointer) false_s);

	gtk_widget_show_all (generalTable);
}

void send_speech_option_on_click (GtkWidget* w, gpointer command)
{
    //get command
    string* value = (string*)command;
    //create bottle and set option values
    Bottle b;
    b.addString ("param");
    if (*value != "true")
    {
        StatusTab::add_param ("Speech Enabled", "False");
        infoTab::instance()->add_info ("Set Speech to False");
        b.addString ("disable_speech");
    }
    else
    {
        StatusTab::add_param ("Speech Enabled", "True");
        infoTab::instance()->add_info ("Set Speech to True");
        b.addString ("enable_speech");
    }
    //redraw option tab
    displayRobotParam ();
    //send command
    outPort.write (b);
}

void send_torso_option_on_click (GtkWidget* w, gpointer command)
{
    //get command
    string* value = (string*)command;
    //create bottle and set option values
    Bottle b;
    b.addString ("param");
    if (*value != "true")
    {
        StatusTab::add_param ("Torso Enabled", "False");
        infoTab::instance()->add_info ("Enable Torso");
        b.addString ("disable_torso");
    }
    else
    {
        StatusTab::add_param ("Torso Enabled", "True");
        infoTab::instance()->add_info ("Disable Torso");
        b.addString ("enable_torso");
    }
    //redraw option tab
    displayRobotParam ();
    //send command
    outPort.write (b);
}

// Extern Send on click
void send_change_tracking_mode_on_click (GtkWidget* w, gpointer command)
{
    //get command
    string* value = (string*)command;
    //create bottle and set option values
	Bottle b;
	b.addString ("param");
	b.addString ("change_tracking_mode");

	if (*value != "true")
	{
        StatusTab::add_param ("Tracking Mode", "Off");
        infoTab::instance()->add_info ("Disable Tracking");
        b.addInt (0);
    }
	else
    {
        StatusTab::add_param ("Tracking Mode", "On");
        infoTab::instance()->add_info ("Enable Tracking");
		b.addInt (1);
	}
    
    //redraw option tab
    displayRobotParam ();
    //send command
	outPort.write (b);
}







void put_everything_in_position()
{
    for (int i=0; i<3; i++)
    {
        int jmax=0;
        switch (i)
        { 
            case LEFT_ARM:
            case RIGHT_ARM:
                jmax=5;
            break;
            case TORSO:
                jmax=3;
            break;
        }
        for (int j=0; j<jmax; j++)
        {
            if (Irobot->icmd[i]) Irobot->icmd[i]->setPositionMode(j);
        }
    }
}

void update_radio_buttons()
{
    for (int i=0; i<3; i++)
    {
        int jmax=0;
        switch (i)
        { 
            case LEFT_ARM:
            case RIGHT_ARM:
                jmax=5;
            break;
            case TORSO:
                jmax=3;
            break;
        }
        for (int j=0; j<jmax; j++)
        {
            int mode=0;
            if (Irobot->icmd[i]) Irobot->icmd[i]->getControlMode(j,&mode);
            switch (mode)
            {
                case VOCAB_CM_POSITION:
                    //gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton_mode_pos[i]),true);
                break;

                case VOCAB_CM_TORQUE:
                    //gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton_mode_trq[i]),true);
                break;

                case VOCAB_CM_IMPEDANCE_POS:
                    //gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton_mode_imp_s[i]),true);
                break;

                default:
                case VOCAB_CM_IDLE:
                case VOCAB_CM_VELOCITY:
                case VOCAB_CM_IMPEDANCE_VEL:
                break;
            }
        }
    }
}

void update_labels()
{
    char buff [255];
    for (int i=0; i<3; i++)
    {
        int jmax=0;
        string txt="<small><span font_desc=\"Courier 8\">";
        switch (i)
        { 
            case LEFT_ARM:
            case RIGHT_ARM:
                jmax=5;
            break;
            case TORSO:
                jmax=3;
            break;
        }
        for (int j=0; j<jmax; j++)
        {    
            double stiff=0;
            double damp=0;
            sprintf (buff, "\n \nJ%d:\n", j); txt+= string(buff);
            if (Irobot->iimp[i]) Irobot->iimp[i]->getImpedance(j,&stiff,&damp);
            sprintf (buff, "stiff: %3.3f Nm/deg\n", stiff); txt+= string(buff);
            sprintf (buff, "damp:  %3.3f Nm/(deg/s)\n", damp); txt+= string(buff);
        }
        txt+="</span></small>";
        //gtk_label_set_text(GTK_LABEL(stiffnessInfoLabel[i]), txt.c_str());
        gtk_label_set_markup (GTK_LABEL (stiffnessInfoLabel[i]), txt.c_str());
    }
}

void radio_click_p( GtkWidget *widget, void* r)
{
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)))
    {
        radio_data_option* rd = (radio_data_option*)(r);
        //printf("%d %d\n",rd->id,rd->mode);
        if (!Irobot->icmd[rd->id]) return;
        switch (rd->id)
        {
            case LEFT_ARM:
            case RIGHT_ARM:
                Irobot->icmd[rd->id]->setPositionMode(0);
                Irobot->icmd[rd->id]->setPositionMode(1);
                Irobot->icmd[rd->id]->setPositionMode(2);
                Irobot->icmd[rd->id]->setPositionMode(3);
                Irobot->icmd[rd->id]->setPositionMode(4);
            break;
            case TORSO:
                Irobot->icmd[rd->id]->setPositionMode(0);
                Irobot->icmd[rd->id]->setPositionMode(1);
                Irobot->icmd[rd->id]->setPositionMode(2);
            break;
        }
        update_labels();
    }
}
void radio_click_t(GtkWidget *widget, void* r)
{
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)))
    {
        radio_data_option* rd = (radio_data_option*)(r);
        //printf("%d %d\n",rd->id,rd->mode);
        if (!Irobot) return;
        if (!Irobot->icmd[rd->id]) return;
        if (!Irobot->iimp[rd->id]) return;
        switch (rd->id)
        {
            case LEFT_ARM:
            case RIGHT_ARM:
                Irobot->iimp[rd->id]->setImpedance(0,0.0,0.0);
                Irobot->iimp[rd->id]->setImpedance(1,0.0,0.0);
                Irobot->iimp[rd->id]->setImpedance(2,0.0,0.0);
                Irobot->iimp[rd->id]->setImpedance(3,0.0,0.0);
                Irobot->iimp[rd->id]->setImpedance(4,0.0,0.0);
                Irobot->icmd[rd->id]->setTorqueMode(0);
                Irobot->icmd[rd->id]->setTorqueMode(1);
                Irobot->icmd[rd->id]->setTorqueMode(2);
                Irobot->icmd[rd->id]->setTorqueMode(3);
                Irobot->icmd[rd->id]->setTorqueMode(4);
           break;
            case TORSO:
                Irobot->iimp[rd->id]->setImpedance(0,0.0,0.0);
                Irobot->iimp[rd->id]->setImpedance(1,0.0,0.0);
                Irobot->icmd[rd->id]->setTorqueMode(0);
                Irobot->icmd[rd->id]->setTorqueMode(1);
                //Irobot->icmd[rd->id]->setTorqueMode(2);
                Irobot->iimp[rd->id]->setImpedance(2,0.1,0.0);
                Irobot->icmd[rd->id]->setImpedancePositionMode(2);
            break;
        }

        update_labels();

    }
}
void radio_click_is( GtkWidget *widget, void* r)
{
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)))
    {
        radio_data_option* rd = (radio_data_option*)(r);
        //printf("%d %d\n",rd->id,rd->mode);
        if (!Irobot->icmd[rd->id]) return;
        switch (rd->id)
        {
            case LEFT_ARM:
            case RIGHT_ARM:
                Irobot->iimp[rd->id]->setImpedance(0,0.2,0.0);
                Irobot->iimp[rd->id]->setImpedance(1,0.2,0.0);
                Irobot->iimp[rd->id]->setImpedance(2,0.2,0.0);
                Irobot->iimp[rd->id]->setImpedance(3,0.2,0.0);
                Irobot->iimp[rd->id]->setImpedance(4,0.1,0.0);
                Irobot->icmd[rd->id]->setImpedancePositionMode(0);
                Irobot->icmd[rd->id]->setImpedancePositionMode(1);
                Irobot->icmd[rd->id]->setImpedancePositionMode(2);
                Irobot->icmd[rd->id]->setImpedancePositionMode(3);
                Irobot->icmd[rd->id]->setImpedancePositionMode(4);
            break;
            case TORSO:
                Irobot->iimp[rd->id]->setImpedance(0,0.1,0.0);
                Irobot->iimp[rd->id]->setImpedance(1,0.1,0.0);
                Irobot->iimp[rd->id]->setImpedance(2,0.1,0.0);
                Irobot->icmd[rd->id]->setImpedancePositionMode(0);
                Irobot->icmd[rd->id]->setImpedancePositionMode(1);
                Irobot->icmd[rd->id]->setImpedancePositionMode(2);
            break;
        }
        update_labels();
    }
}
void radio_click_im( GtkWidget *widget, void* r)
{
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)))
    {
        radio_data_option* rd = (radio_data_option*)(r);
        //printf("%d %d\n",rd->id,rd->mode);
        if (!Irobot->icmd[rd->id]) return;
        switch (rd->id)
        {
            case LEFT_ARM:
            case RIGHT_ARM:
                Irobot->iimp[rd->id]->setImpedance(0,0.4,0.03);
                Irobot->iimp[rd->id]->setImpedance(1,0.4,0.03);
                Irobot->iimp[rd->id]->setImpedance(2,0.4,0.03);
                Irobot->iimp[rd->id]->setImpedance(3,0.2,0.01);
                Irobot->iimp[rd->id]->setImpedance(4,0.2,0.00);
                Irobot->icmd[rd->id]->setImpedancePositionMode(0);
                Irobot->icmd[rd->id]->setImpedancePositionMode(1);
                Irobot->icmd[rd->id]->setImpedancePositionMode(2);
                Irobot->icmd[rd->id]->setImpedancePositionMode(3);
                Irobot->icmd[rd->id]->setImpedancePositionMode(4);
            break;
            case TORSO:
                Irobot->iimp[rd->id]->setImpedance(0,0.3,0.0);
                Irobot->iimp[rd->id]->setImpedance(1,0.3,0.0);
                Irobot->iimp[rd->id]->setImpedance(2,0.3,0.0);
                Irobot->icmd[rd->id]->setImpedancePositionMode(0);
                Irobot->icmd[rd->id]->setImpedancePositionMode(1);
                Irobot->icmd[rd->id]->setImpedancePositionMode(2);
            break;
        }
        update_labels();
    }
}
void radio_click_ih( GtkWidget *widget, void* r)
{
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)))
    {
        radio_data_option* rd = (radio_data_option*)(r);
        //printf("%d %d\n",rd->id,rd->mode);
        if (!Irobot->icmd[rd->id]) return;
        switch (rd->id)
        {
            case LEFT_ARM:
            case RIGHT_ARM:
                Irobot->iimp[rd->id]->setImpedance(0,0.6,0.06);
                Irobot->iimp[rd->id]->setImpedance(1,0.6,0.06);
                Irobot->iimp[rd->id]->setImpedance(2,0.6,0.06);
                Irobot->iimp[rd->id]->setImpedance(3,0.3,0.02);
                Irobot->iimp[rd->id]->setImpedance(4,0.2,0.00);
                Irobot->icmd[rd->id]->setImpedancePositionMode(0);
                Irobot->icmd[rd->id]->setImpedancePositionMode(1);
                Irobot->icmd[rd->id]->setImpedancePositionMode(2);
                Irobot->icmd[rd->id]->setImpedancePositionMode(3);
                Irobot->icmd[rd->id]->setImpedancePositionMode(4);
            break;
            case TORSO:
                Irobot->iimp[rd->id]->setImpedance(0,0.7,0.015);
                Irobot->iimp[rd->id]->setImpedance(1,0.7,0.015);
                Irobot->iimp[rd->id]->setImpedance(2,0.7,0.015);
                Irobot->icmd[rd->id]->setImpedancePositionMode(0);
                Irobot->icmd[rd->id]->setImpedancePositionMode(1);
                Irobot->icmd[rd->id]->setImpedancePositionMode(2);
            break;
        }
        update_labels();
    }
}

void StatusTab::displayStiffnessAndImpedance (GtkWidget *w)
{

    Irobot = new robot_interfaces();
    Irobot->init(robotName_);
    cout << "RobotName: " << robotName_ << endl;
    
    GtkWidget *fixed;
    GtkWidget *part_frame [3];
    GtkWidget *fixed_inside_part_frame [3];
 
    fixed  = gtk_fixed_new ();
    gtk_container_add (GTK_CONTAINER(w), fixed);


    part_frame[LEFT_ARM] = gtk_frame_new ("Left Arm");
    part_frame[RIGHT_ARM] = gtk_frame_new ("Right Arm");
    part_frame[TORSO] = gtk_frame_new ("Torso");

    gtk_widget_set_size_request    (part_frame[LEFT_ARM],   150, 550    );
    gtk_widget_set_size_request    (part_frame[RIGHT_ARM],  150, 550    );
    gtk_widget_set_size_request    (part_frame[TORSO],      150, 550    );

    gtk_fixed_put    (GTK_FIXED(fixed), part_frame[LEFT_ARM],   30+150*0, 30    );
    gtk_fixed_put    (GTK_FIXED(fixed), part_frame[RIGHT_ARM],  30+150*1, 30    );
    gtk_fixed_put    (GTK_FIXED(fixed), part_frame[TORSO],      30+150*2, 30    );

    radio_data_option r[5][5];
    for (int i=0; i<5; i++)
        for (int j=0; j<5; j++)
        {
            r[i][j].id=j;
            r[i][j].mode=i;
        }
 
    for (int i=0; i<3; i++)
    {
        //create radio buttons
        fixed_inside_part_frame[i]  = gtk_fixed_new ();
        stiffnessInfoLabel[i] = gtk_label_new("");
        gtk_container_add     (GTK_CONTAINER(part_frame[i]), fixed_inside_part_frame[i]    );

        radiobutton_mode_pos[i]    = gtk_radio_button_new_with_label  (NULL, "position");
        radiobutton_mode_trq[i]    = gtk_radio_button_new_with_label_from_widget  (GTK_RADIO_BUTTON (radiobutton_mode_pos[i]), "zero torque");
        radiobutton_mode_imp_s[i]  = gtk_radio_button_new_with_label_from_widget  (GTK_RADIO_BUTTON (radiobutton_mode_pos[i]), "soft spring");
        radiobutton_mode_imp_m[i]  = gtk_radio_button_new_with_label_from_widget  (GTK_RADIO_BUTTON (radiobutton_mode_pos[i]), "medium spring");
        radiobutton_mode_imp_h[i]  = gtk_radio_button_new_with_label_from_widget  (GTK_RADIO_BUTTON (radiobutton_mode_pos[i]), "hard spring");        
        gtk_fixed_put    (GTK_FIXED(fixed_inside_part_frame[i] ), radiobutton_mode_pos[i],     20, 30     );
        gtk_fixed_put    (GTK_FIXED(fixed_inside_part_frame[i] ), radiobutton_mode_trq[i],     20, 60     );
        gtk_fixed_put    (GTK_FIXED(fixed_inside_part_frame[i] ), radiobutton_mode_imp_s[i],   20, 90     );
        gtk_fixed_put    (GTK_FIXED(fixed_inside_part_frame[i] ), radiobutton_mode_imp_m[i],   20, 120    );
        gtk_fixed_put    (GTK_FIXED(fixed_inside_part_frame[i] ), radiobutton_mode_imp_h[i],   20, 150    );
        gtk_fixed_put    (GTK_FIXED(fixed_inside_part_frame[i] ), stiffnessInfoLabel[i],               20, 200    );
        //turns off widgets of missing parts
        if (Irobot->dd[i]==0)
        {
            gtk_widget_set_sensitive(part_frame[i],false);
            gtk_widget_set_sensitive(fixed_inside_part_frame[i], false);
            gtk_widget_set_sensitive(radiobutton_mode_pos[i],    false);
            gtk_widget_set_sensitive(radiobutton_mode_trq[i],    false);
            gtk_widget_set_sensitive(radiobutton_mode_imp_s[i],  false);
            gtk_widget_set_sensitive(radiobutton_mode_imp_m[i],  false);
            gtk_widget_set_sensitive(radiobutton_mode_imp_h[i],  false);
        }
        //connects callbacks
        g_signal_connect (radiobutton_mode_pos[i],    "clicked",G_CALLBACK (radio_click_p),  &r[0][i]);
        g_signal_connect (radiobutton_mode_trq[i],    "clicked",G_CALLBACK (radio_click_t),  &r[1][i]);
        g_signal_connect (radiobutton_mode_imp_s[i],  "clicked",G_CALLBACK (radio_click_is), &r[2][i]);
        g_signal_connect (radiobutton_mode_imp_m[i],  "clicked",G_CALLBACK (radio_click_im), &r[3][i]);
        g_signal_connect (radiobutton_mode_imp_h[i],  "clicked",G_CALLBACK (radio_click_ih), &r[4][i]);
    }

    //update_radio_buttons();
    //gtk_widget_set_sensitive(part_frame[TORSO],false);

    update_labels();
    put_everything_in_position();
        
    gtk_widget_show_all (w);
}



void displayRobotParam ()
{
    GtkWidget* generalTable = gtk_table_new (20, 1, true);
    paramTab::clear ();
    gtk_container_add (GTK_CONTAINER (paramTab::container_), generalTable);


    std::map<string, string>::iterator it = StatusTab::paramsList_.begin ();
    int i = 0;
    for (; it != StatusTab::paramsList_.end (); it++, i++)
    {
        GtkWidget *frame = gtk_frame_new (it->first.c_str ());
        GtkWidget *value = gtk_label_new (it->second.c_str ());

        gtk_table_attach_defaults (GTK_TABLE (generalTable), frame, 0, 1, i, i+1);
        gtk_container_add (GTK_CONTAINER(frame), value);
    }

    gtk_widget_show_all (paramTab::container_);
}

void StatusTab::add_param (string key, string param)
{
    paramsList_[key] = param;
}


StatusTab *StatusTab::instance ()
{
    if (StatusTab_)
        return (StatusTab_);
    else
    {
        StatusTab::StatusTab_ = new StatusTab ();
        return (StatusTab::StatusTab_);
    }
}


