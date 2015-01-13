#include "complex_action.hh"

Complex_actionTab *Complex_actionTab::Complex_actionTab_ = 0;
ComplexActionParams Complex_actionTab::params = ComplexActionParams ();

void Complex_actionTab::createTab (GtkWidget* notebook)
{
    GtkWidget *actionTab;
    GtkWidget *actionLabel = gtk_label_new("Complex Action");

    actionTab = gtk_frame_new ("What i do");
    gtk_container_set_border_width (GTK_CONTAINER (actionTab), 10);
    gtk_notebook_append_page (GTK_NOTEBOOK(notebook), actionTab, actionLabel);

    
    //Global display table 
    GtkWidget* displayTable = gtk_table_new (10, 10, true);
    gtk_container_add(GTK_CONTAINER(actionTab), displayTable);

	//head display table
    GtkWidget* headFrame = gtk_frame_new ("Head");

    gtk_table_attach_defaults (GTK_TABLE (displayTable), headFrame, 0, 4, 0, 4);
    create_look_tab (headFrame);

    //Move right hand display table
    GtkWidget* RAFrame = gtk_frame_new ("Left Hand");

    gtk_table_attach_defaults (GTK_TABLE (displayTable), RAFrame, 0, 4, 5, 9);
    create_move_hand_tab (RAFrame, "left");

    //Move left hand display table
    GtkWidget* LAFrame = gtk_frame_new ("Right Hand");

    gtk_table_attach_defaults (GTK_TABLE (displayTable), LAFrame, 5, 9, 5, 9);
    create_move_hand_tab (LAFrame, "right");

    //Grasp display table
    GtkWidget* GraspFrame = gtk_frame_new ("Grasp");

    gtk_table_attach_defaults (GTK_TABLE (displayTable), GraspFrame, 5, 9, 0, 4);
    create_grasp_tab (GraspFrame);


   //Home display table
    GtkWidget* HomeFrame = gtk_frame_new ("Predefined Position");

    gtk_table_attach_defaults (GTK_TABLE (displayTable), HomeFrame, 0, 10, 9, 10);
    create_home_tab (HomeFrame);
}

void Complex_actionTab::create_home_tab (GtkWidget *frame)
{
	GtkWidget* displayHomeTable = gtk_table_new (1, 7, true);
    gtk_container_add(GTK_CONTAINER(frame), displayHomeTable);
	   //HOME POSITION BUTTON

    int c = 0;

    GtkWidget* homeButton0 = gtk_button_new_with_label ("Handing");
    gtk_table_attach_defaults (GTK_TABLE (displayHomeTable), homeButton0, 0, 1, 0, 1);
    add_toolTips (homeButton0, "iCub will move to handing position");
    g_signal_connect(G_OBJECT (homeButton0), "clicked", G_CALLBACK (send_home_position), NULL);
	g_object_set_data (G_OBJECT(homeButton0), "param", (gpointer) "3");
				
    GtkWidget* homeButton1 = gtk_button_new_with_label ("Manip 1");
    gtk_table_attach_defaults (GTK_TABLE (displayHomeTable), homeButton1, 1, 2, 0, 1);
    add_toolTips (homeButton1, "iCub will move to manipulation position 1");
    g_signal_connect(G_OBJECT (homeButton1), "clicked", G_CALLBACK (send_home_position), NULL);
	g_object_set_data (G_OBJECT(homeButton1), "param", (gpointer) "4");

    GtkWidget* homeButton2 = gtk_button_new_with_label ("Manip 2");
    gtk_table_attach_defaults (GTK_TABLE (displayHomeTable), homeButton2, 2, 3, 0, 1);
    add_toolTips (homeButton2, "iCub will move to manipulation position 2");
    g_signal_connect(G_OBJECT (homeButton2), "clicked", G_CALLBACK (send_home_position), NULL);
	g_object_set_data (G_OBJECT(homeButton2), "param", (gpointer) "2");

	GtkWidget* homeButton6 = gtk_button_new_with_label ("Manip 3");
    gtk_table_attach_defaults (GTK_TABLE (displayHomeTable), homeButton6, 3, 4, 0, 1);
    add_toolTips (homeButton6, "iCub will move to manipulation position 2");
    g_signal_connect(G_OBJECT (homeButton6), "clicked", G_CALLBACK (send_home_position), NULL);
	g_object_set_data (G_OBJECT(homeButton6), "param", (gpointer) "6");

    GtkWidget* homeButton3 = gtk_button_new_with_label ("Accept Table");
    gtk_table_attach_defaults (GTK_TABLE (displayHomeTable), homeButton3, 4, 5, 0, 1);
    add_toolTips (homeButton3, "iCub will move to acceptability Position and look to ground");
    g_signal_connect(G_OBJECT (homeButton3), "clicked", G_CALLBACK (send_home_position), NULL);
	g_object_set_data (G_OBJECT(homeButton3), "param", (gpointer) "1");

    GtkWidget* homeButton4 = gtk_button_new_with_label ("Accept Wall");
    gtk_table_attach_defaults (GTK_TABLE (displayHomeTable), homeButton4, 5, 6, 0, 1);
    add_toolTips (homeButton4, "iCub will move to acceptability Position and look to wall");
    g_signal_connect(G_OBJECT (homeButton4), "clicked", G_CALLBACK (send_home_position), NULL);
	g_object_set_data (G_OBJECT(homeButton4), "param", (gpointer) "5");

    GtkWidget* homeButton5 = gtk_button_new_with_label ("Home");
    gtk_table_attach_defaults (GTK_TABLE (displayHomeTable), homeButton5, 6, 7, 0, 1);
    add_toolTips (homeButton5, "iCub will move to Home Position");
    g_signal_connect(G_OBJECT (homeButton5), "clicked", G_CALLBACK (send_home_position), NULL);
	g_object_set_data (G_OBJECT(homeButton5), "param", (gpointer) "0");
}

///////////////////////////////////////////////////////////////
//					GRASP
//////////////////////////////////////////////////////////////
void Complex_actionTab::create_grasp_tab (GtkWidget *frame)
{

    GtkWidget* displayGraspTable = gtk_table_new (8, 2, true);
    gtk_container_add(GTK_CONTAINER(frame), displayGraspTable);

    //Add button in Head Zone
    std::vector<string> button_label =  {"Skyward",
    									"Middle Rotation",
    									"To the ground",
										"Open Hand", 
										"Close bottle small",
										"Close bottle big",
										"Close cylinder construction"};
	std::vector<string> button_toolTips =  {"Position the hand Skyward",
    									"Position the hand at Middle Rotation",
    									"Position the hand To the ground",
										"Open Hand", 
										"Close Hand to grasp small bottles - mentos/white bottle",
										"Close the hand to grasp big bottles- sprite/water",
										"Close the hand to grasp cylinders for the construction task"};
	std::vector<const char*> button_params = {"skyward",
    									"middle",
    									"bottom",
										"open_hand", 
										"close_hand",
										"green_ball",
										"close_cylinders"};
	int c = 0;
	for (int i = 1; i < 8; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			GtkWidget *button = gtk_button_new_with_label (button_label[c].c_str ());
			gtk_table_attach_defaults (GTK_TABLE (displayGraspTable), button, j, j + 1, i, i + 1);
			add_toolTips (button, ("Move " + button_toolTips[c]).c_str ());
			g_signal_connect(G_OBJECT (button), "clicked", G_CALLBACK (send_grasp_on_click), (gpointer) button_params[c]);
			if (j == 0)
				g_object_set_data (G_OBJECT(button), "part", (gpointer) "left");
			else
				g_object_set_data (G_OBJECT(button), "part", (gpointer) "right");
				
		}
		c++;
	}

	GtkWidget *label = gtk_label_new ("Left Hand");
	gtk_table_attach_defaults (GTK_TABLE (displayGraspTable), label, 0, 1, 0, 1);

	label = gtk_label_new ("Right Hand");
	gtk_table_attach_defaults (GTK_TABLE (displayGraspTable), label, 1, 2, 0, 1);

}



void send_grasp_on_click (GtkWidget *w, gpointer p)
{
	string param = string((char *) p);
	gpointer data_p = g_object_get_data (G_OBJECT(w), "part");
	string part = string((char *) data_p);
	history_record h;
	h.name = "move hand";
	h.type = "moveHand";
	
	Bottle b;
	b.addString ("action");
	b.addString ("object");
	b.addString (part.c_str ());
	if (param == "open_hand" || param == "close_hand" || param == "green_ball" || param == "close_cylinders")
	{
		b.addString ("grasp");
		b.addString (param.c_str ());
	}
	else if (param == "skyward" || param == "bottom" || param == "middle")
	{
		b.addString ("rotate_hand");
		if (param == "skyward")
			if (part == "right")
			{
				b.addDouble (Complex_actionTab::params.R_HAND_SKYWARD[0]);
				b.addDouble (Complex_actionTab::params.R_HAND_SKYWARD[1]);
				b.addDouble (Complex_actionTab::params.R_HAND_SKYWARD[2]);
				b.addDouble (Complex_actionTab::params.R_HAND_SKYWARD[3]);
			}
			else
			{
				b.addDouble (Complex_actionTab::params.L_HAND_SKYWARD[0]);
				b.addDouble (Complex_actionTab::params.L_HAND_SKYWARD[1]);
				b.addDouble (Complex_actionTab::params.L_HAND_SKYWARD[2]);
				b.addDouble (Complex_actionTab::params.L_HAND_SKYWARD[3]);
			}
		else if (param == "bottom")
			if (part == "right")
			{
				b.addDouble (Complex_actionTab::params.R_HAND_GROUND[0]);
				b.addDouble (Complex_actionTab::params.R_HAND_GROUND[1]);
				b.addDouble (Complex_actionTab::params.R_HAND_GROUND[2]);
				b.addDouble (Complex_actionTab::params.R_HAND_GROUND[3]);
			}
			else
			{
				b.addDouble (Complex_actionTab::params.L_HAND_GROUND[0]);
				b.addDouble (Complex_actionTab::params.L_HAND_GROUND[1]);
				b.addDouble (Complex_actionTab::params.L_HAND_GROUND[2]);
				b.addDouble (Complex_actionTab::params.L_HAND_GROUND[3]);
			}
		else if (param == "middle")
			if (part == "right")
			{
				b.addDouble (Complex_actionTab::params.R_HAND_MIDDLE[0]);
				b.addDouble (Complex_actionTab::params.R_HAND_MIDDLE[1]);
				b.addDouble (Complex_actionTab::params.R_HAND_MIDDLE[2]);
				b.addDouble (Complex_actionTab::params.R_HAND_MIDDLE[3]);
			}
			else
			{
				b.addDouble (Complex_actionTab::params.L_HAND_MIDDLE[0]);
				b.addDouble (Complex_actionTab::params.L_HAND_MIDDLE[1]);
				b.addDouble (Complex_actionTab::params.L_HAND_MIDDLE[2]);
				b.addDouble (Complex_actionTab::params.L_HAND_MIDDLE[3]);
			}
	}
	else if (param == "pointing")
	{
		b.addString ("point");
		if (part == "right")
		{
			b.addDouble (Complex_actionTab::params.R_POINTING[0]);
			b.addDouble (Complex_actionTab::params.R_POINTING[1]);
			b.addDouble (Complex_actionTab::params.R_POINTING[2]);
		}
		else
		{
			b.addDouble (Complex_actionTab::params.L_POINTING[0]);
			b.addDouble (Complex_actionTab::params.L_POINTING[1]);
			b.addDouble (Complex_actionTab::params.L_POINTING[2]);
		}
	}
	else
	{
		cout << "Bad parameters: No command sent" << endl;
	}

	h.command = b.toString ();
	send_to_actionsServer (&h);
	return;
}



///////////////////////////////////////////////////////////////
//					MOVE HAND
//////////////////////////////////////////////////////////////
void Complex_actionTab::create_move_hand_tab (GtkWidget *frame, string part)
{

    GtkWidget* displayHandTable = gtk_table_new (8, 3, true);
    gtk_container_add(GTK_CONTAINER(frame), displayHandTable);

    //Add button in Head Zone
    std::vector<string> button_label =  {"\\", "|", "/",
										 "<--", "-->",
										 "/", "|", "\\"};
	std::vector<string> button_toolTips =  {"top/left", "top", "top/right",
										 "left", "right",
										 "bottom/left", "bottom", "bottom/right"};
	std::vector<const char*> button_params = {"0 1 1", "0 0 1", "0 -1 1",
										 "0 1 0", "0 -1 0",
										 "0 1 -1", "0 0 -1", "0 -1 -1"};
	int c = 0;

	GtkWidget *coefInteractive = gtk_entry_new ();
	gtk_table_attach_defaults (GTK_TABLE (displayHandTable), coefInteractive, 1, 2, 7, 8);
	gtk_entry_set_text (GTK_ENTRY(coefInteractive), "0.05");
	GtkWidget *metriclabel = gtk_label_new ("[m] meters");
	gtk_table_attach_defaults (GTK_TABLE (displayHandTable), metriclabel, 2, 3, 7, 8);

	string* spart = new string (part);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i != j || j != 1)
			{
				GtkWidget *button = gtk_button_new_with_label (button_label[c].c_str ());
				gtk_table_attach_defaults (GTK_TABLE (displayHandTable), button, j, j +1, i * 2, i * 2 + 2);
				add_toolTips (button, ("Move " + button_toolTips[c]).c_str ());
				g_signal_connect(G_OBJECT (button), "clicked", G_CALLBACK (send_move_hand_on_click), (gpointer) button_params[c]);
				g_object_set_data (G_OBJECT(button), "part", (gpointer) spart);
				g_object_set_data (G_OBJECT(button), "coefEntry", (gpointer) coefInteractive);
				c++;	
			}		
		}
	}
	GtkWidget *button = gtk_button_new_with_label ("Forward");
	gtk_table_attach_defaults (GTK_TABLE (displayHandTable), button, 1, 2, 2, 3);
	add_toolTips (button, "Move Forward");
	g_signal_connect(G_OBJECT (button), "clicked", G_CALLBACK (send_move_hand_on_click), (gpointer) "-1 0 0");
	g_object_set_data (G_OBJECT(button), "part", (gpointer) spart);
	g_object_set_data (G_OBJECT(button), "coefEntry", (gpointer) coefInteractive);

	button = gtk_button_new_with_label ("Back");
	gtk_table_attach_defaults (GTK_TABLE (displayHandTable), button, 1, 2, 3, 4);
	add_toolTips (button, "Move Back");
	g_signal_connect(G_OBJECT (button), "clicked", G_CALLBACK (send_move_hand_on_click), (gpointer) "1 0 0");
	g_object_set_data (G_OBJECT(button), "part", (gpointer) spart);
	g_object_set_data (G_OBJECT(button), "coefEntry", (gpointer) coefInteractive);




}



void send_move_hand_on_click (GtkWidget *w, gpointer p)
{
	string param = string((char *) p);
	gpointer part_p = g_object_get_data (G_OBJECT(w), "part");
	gpointer coefInteractive_p = g_object_get_data (G_OBJECT(w), "coefEntry");
	const char* coef_a = gtk_entry_get_text (GTK_ENTRY(coefInteractive_p));
	string *part = (string*) part_p;
	double coef = atof ((char* ) coef_a);

	if (coef > 1 || coef <= 0)
	{
		coef = 0.01;
		cout << "Warning: Coef must be in range [0, 1]. For safety, coef has been set to 0.01.";
	}

	std::vector<string> token = tokenize (param);

	if (token.size () == 3)
	{
		history_record h;
		h.name += "move ";
		h.name += *part;
		h.name += " arm";
		h.type = "moveHand";

		Bottle b;
		b.addString ("action");
		b.addString ("object");
		b.addString (part->c_str ());
		b.addString ("adjustHandPosition");
		b.addDouble (atof(token[0].c_str ()) * coef);
		b.addDouble (atof(token[1].c_str ()) * coef);
		b.addDouble (atof(token[2].c_str ()) * coef);
		h.command = b.toString ();
		send_to_actionsServer (&h);
	}
	else
	{
		cout << "ERROR: Error in command. This is a bug !" << endl;
	}
}



void send_to_actionsServer (history_record *h)
{
	// Send bottle from outPort and save the record in history
	Bottle b (h->command.c_str ());
	outPort.write (b);
	cout << "Command: " << b.toString () << endl;
	infoTab::instance ()->add_info ("Send move " + h->name);
	saveToHistory(h->type, h->name, 0, h->command);
}
///////////////////////////////////////////////////////////////
//					LOOK
//////////////////////////////////////////////////////////////



void Complex_actionTab::create_look_tab (GtkWidget *MoveHandFrame)
{

    GtkWidget* displayMoveHeadTable = gtk_table_new (4, 3, true);
    gtk_container_add(GTK_CONTAINER(MoveHandFrame), displayMoveHeadTable);

    //Add button in Head Zone
    std::vector<string> button_label =  {"\\", "|", "/",
										 "<--", "-->",
										 "/", "|", "\\"};
	std::vector<string> button_toolTips =  {"top/left", "top", "top/right",
										 "left", "right",
										 "bottom/left", "bottom", "bottom/right"};
	std::vector<const char*> button_params = {"top/left", "top", "top/right",
										 "left", "right",
										 "bottom/left", "bottom", "bottom/right",
										 "lookfront"};
	int c = 0;

	//Coef
	GtkWidget *coefInteractive = gtk_entry_new ();
	gtk_table_attach_defaults (GTK_TABLE (displayMoveHeadTable), coefInteractive, 1, 2, 3, 4);
	gtk_entry_set_text (GTK_ENTRY(coefInteractive), "0.1");

	GtkWidget *metriclabel = gtk_label_new ("[m] meters");
	gtk_table_attach_defaults (GTK_TABLE (displayMoveHeadTable), metriclabel, 2, 3, 3, 4);

	//Place button
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i != j || j != 1)
			{
				GtkWidget *button = gtk_button_new_with_label (button_label[c].c_str ());
				gtk_table_attach_defaults (GTK_TABLE (displayMoveHeadTable), button, j, j + 1, i, i + 1);
				add_toolTips (button, ("look " + button_toolTips[c]).c_str ());
				g_signal_connect(G_OBJECT (button), "clicked", G_CALLBACK (send_look_at_on_click), (gpointer) button_params[c]);
				g_object_set_data (G_OBJECT(button), "coefEntry", (gpointer) coefInteractive);
				c++;
			}
		}
	}

	GtkWidget *lookFrontButton = gtk_button_new_with_label ("Look Front");
	gtk_table_attach_defaults (GTK_TABLE (displayMoveHeadTable), lookFrontButton, 1, 2, 1, 2);
	g_object_set_data (G_OBJECT(lookFrontButton), "coefEntry", (gpointer) coefInteractive);
	g_signal_connect(G_OBJECT (lookFrontButton), "clicked", G_CALLBACK (send_look_at_on_click), (gpointer) button_params[button_params.size () - 1]);





}
void send_look_at_on_click (GtkWidget *w, gpointer p)
{
	//get data
	string param = string((char *) p);
	gpointer coefInteractive_p = g_object_get_data (G_OBJECT(w), "coefEntry");
	const char* coef_a = gtk_entry_get_text (GTK_ENTRY(coefInteractive_p));
	double coef = atof ((char* ) coef_a);

	//create history record
	history_record h;
	//write the command
	h.type = "LookAt";
	h.name = "Look at " + param;
	cout << "params: " << param <<endl;


	double x, y, z = -1;
	Bottle b;
	b.addString ("action");
	b.addString ("object");
	b.addString ("right");


    if(param == "lookfront")
    {
       b.addString ("lookfront");
    }
    else
	{

        b.addString ("lookdelta");

		if (param == "top/left")
			{x = 0; y = -1; z = 1;}
		else if (param == "top")
			{x = 0; y = 0; z = 1;}
		else if (param == "top/right")
			{x = 0; y = 1; z = 1;}

		else if (param == "left")
			{x = 0; y = -1; z = 0;}
		else if (param == "front")
			{x = 0; y = 0; z = 0;}
		else if (param == "right")
			{x = 0; y = 1; z = 0;}

		else if (param == "bottom/left")
			{x = 0; y = -1; z = -1;}
		else if (param == "bottom")
			{x = 0; y = 0; z = -1;}
		else if (param == "bottom/right")
			{x = 0; y = 1; z = -1;}
		else
		{
			cout << "Bad parameters. No action sent." << endl;
			return;
		}
		b.addDouble (x * coef);
		b.addDouble (y * coef);
		b.addDouble (z * coef);

	}

	// save the bottle in record h
	h.command = b.toString ();

	//send to actionsServer
	send_to_actionsServer (&h);
}

void send_home_position (GtkWidget* w, gpointer p)
{
	gpointer data_p = g_object_get_data (G_OBJECT(w), "param");
	string param = string((char *) data_p);
	history_record h;
	h.name = "Home" +  param;
	h.command = "action object right Home " + param;
	h.type = "Home";
	send_to_actionsServer (&h);
}





// get the singleton of complex_action
Complex_actionTab *Complex_actionTab::instance ()
{
    if (Complex_actionTab_)
        return (Complex_actionTab_);
    else
    {
        Complex_actionTab::Complex_actionTab_ = new Complex_actionTab ();
        return (Complex_actionTab::Complex_actionTab_);
    }
}


void Complex_actionTab::configure (ResourceFinder finder)
{
	Bottle &orientations = finder.findGroup("orientations");


	//recover configuration value from finder (ActionFile)
	//Hand rotation config (Quternion of hand position)
	readVector(orientations,"right_hand_skyward",params.R_HAND_SKYWARD,4);
	readVector(orientations,"right_hand_ground",params.R_HAND_GROUND,4);
	readVector(orientations,"right_hand_middle",params.R_HAND_MIDDLE,4);

	readVector(orientations,"left_hand_skyward",params.L_HAND_SKYWARD,4);
    readVector(orientations,"left_hand_ground",params.L_HAND_GROUND,4);
    readVector(orientations,"left_hand_middle",params.L_HAND_MIDDLE,4);


    // Pointing Config (position 3D of hand)
    Bottle &pointing = finder.findGroup("pointing");
    readVector(pointing,"left_pointing", params.L_POINTING, 3);
    readVector(pointing,"right_pointing", params.R_POINTING, 3);



}
