/**
 * Copyright (C) 2015 CODYCO Project
 * Author: Serena Ivaldi <serena.ivaldi@inria.fr>, Charles Ballarini
 * website: www.codyco.eu
 *
 * Copyright (C) 2012 MACSi Project
 * Author: Charles Ballarini
 * email:  charles.ballarini@gmail.com
 * website: www.macsi.isir.upmc.fr
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
*/

#include "action.hh"

GtkWidget *button_reset;
GtkWidget *button_send_command;
GtkWidget *button_validate;
GtkWidget *radiobutton_actions[30];
GtkWidget *radiobutton_armType[3];
radio_data rdact[30];
radio_data rdarm[3];
GtkWidget *object_edit;
GtkWidget *object2_edit;
GtkWidget *person_edit;
GtkWidget *person2_edit;
GtkWidget *Ax, *Ay, *Az, *Bx, *By, *Bz;
GtkWidget *Ao1, *Ao2, *Ao3, *Ao4, *Bo1, *Bo2, *Bo3, *Bo4;
GtkAdjustment *Ax_adj, *Ay_adj, *Az_adj, *Bx_adj, *By_adj, *Bz_adj;
GtkAdjustment *Ao1_adj, *Ao2_adj, *Ao3_adj, *Ao4_adj, *Bo1_adj, *Bo2_adj, *Bo3_adj, *Bo4_adj;
GtkWidget *comboGrasps;

// GTK
GtkWidget *labels[30];
GtkWidget *label_status;
GtkWidget *label_built_command;
GtkWidget *filename_edit;
GtkWidget *trials_edit;

// actions parameters
string action;
string object;
string object2;
string person;
string person2;
Vector xObject;
Vector oObject;
Vector xObject2;
Vector oObject2;
string direction;
string graspType;
Vector params;

// the status of the process: recording or idle
int status;
// a counter
int counterMsg;


// the command sent every time
string command;
Bottle builtCommand;

// the arm type
string armType;


void ActionTab::createTab (GtkWidget* notebook)
{
	tab_ = notebook;
	GtkWidget *actionTab;
	GtkWidget *actionFixed;
    GtkWidget *actionLabel = gtk_label_new("Action");
	actionTab = gtk_frame_new ("What i do");
	int c=0; //counter for labels
    gtk_container_set_border_width (GTK_CONTAINER (actionTab), 10);
	gtk_notebook_append_page (GTK_NOTEBOOK(tab_), actionTab, actionLabel);
	   // actions
    //-------------------------------------------------------------------------------------------
    actionFixed  = gtk_fixed_new ();
    gtk_container_add 	(GTK_CONTAINER(actionTab), actionFixed);

    // the button used to reset datas (for a new action)
    button_reset = gtk_button_new_with_label("Reset - new action");
    gtk_widget_set_size_request(button_reset, 200, 35);
    g_signal_connect(G_OBJECT(button_reset), "clicked", G_CALLBACK(button_reset_click), NULL);
    // the button to validate
    button_validate = gtk_button_new_with_label("Validate");
    gtk_widget_set_size_request(button_validate, 200, 35);
    g_signal_connect(G_OBJECT(button_validate), "clicked", G_CALLBACK(button_validate_click), NULL);   
    // the button used to send commands
    button_send_command = gtk_button_new_with_label("VALIDATION REQUIRED");
    gtk_widget_set_size_request(button_send_command, 200, 35);
    g_signal_connect(G_OBJECT(button_send_command), "clicked", G_CALLBACK(button_send_command_click), NULL);
    // the label used for status and informations
    label_status = gtk_label_new("Validate parameters before sending the command");
    label_built_command = gtk_label_new("Command: --");


    gtk_fixed_put(GTK_FIXED(actionFixed), button_reset,      10, 10);
    gtk_fixed_put(GTK_FIXED(actionFixed), button_validate,   220,10);
    gtk_fixed_put(GTK_FIXED(actionFixed), button_send_command, 10, 435);
    gtk_fixed_put(GTK_FIXED(actionFixed), label_status,  250, 430);
    gtk_fixed_put(GTK_FIXED(actionFixed), label_built_command, 250, 450 );

    xObject.resize(3,0.0);
    oObject.resize(4,0.0);
    xObject2.resize(3,0.0);
    oObject2.resize(4,0.0);
    params.resize(30,0.0);
    action="look";
    armType="right";
    counterMsg=0;
    graspType="open_hand";

    // radio buttons to choose the action
    int ac;
    //simple actions
    labels[c++] = gtk_label_new("Simple actions");
    ac=0;   radiobutton_actions[ac]=gtk_radio_button_new_with_label  (NULL, "goto");
    ac++;   radiobutton_actions[ac]=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_actions[0]),"look");
    ac++;   radiobutton_actions[ac]=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_actions[0]),"reach");
    ac++;   radiobutton_actions[ac]=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_actions[0]),"take");
    ac++;   radiobutton_actions[ac]=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_actions[0]),"point");
    ac++;   radiobutton_actions[ac]=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_actions[0]),"lift");
    ac++;   radiobutton_actions[ac]=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_actions[0]),"push");
    ac++;   radiobutton_actions[ac]=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_actions[0]),"rotate");
    ac++;   radiobutton_actions[ac]=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_actions[0]),"grasp");
    //complex actions
    labels[c++] = gtk_label_new("Complex actions");
    ac++;   radiobutton_actions[ac]=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_actions[0]),"takeliftfall");
    ac++;   radiobutton_actions[ac]=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_actions[0]),"takeobserve");
    ac++;   radiobutton_actions[ac]=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_actions[0]),"takeputinthebox");
    ac++;   radiobutton_actions[ac]=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_actions[0]),"putAonB");
    ac++;   radiobutton_actions[ac]=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_actions[0]),"takegive");

    gtk_fixed_put(GTK_FIXED(actionFixed), labels[0], 10, 60);
    for(int i=0;i<9;i++)
    {
        gtk_fixed_put(GTK_FIXED(actionFixed) , radiobutton_actions[i], 20, 60+17+i*17 );
        rdact[i].id=i;
        g_signal_connect (radiobutton_actions[i],"clicked",G_CALLBACK (radio_click_actions),  &rdact[i]);
    }

    gtk_fixed_put(GTK_FIXED(actionFixed), labels[1], 10, 60+30+9*17);
    for(int i=9;i<14;i++)
    {
        gtk_fixed_put(GTK_FIXED(actionFixed), radiobutton_actions[i], 20, 60+49+i*17 );
        rdact[i].id=i;
        g_signal_connect (radiobutton_actions[i],"clicked",G_CALLBACK (radio_click_actions),  &rdact[i]);
    }

    // parameters for actions
    int ii=c;
    labels[c] = gtk_label_new("Object A");
    c++; labels[c] = gtk_label_new("Person A");
    c++; labels[c] = gtk_label_new(" A - x");
    c++; labels[c] = gtk_label_new(" A - y");
    c++; labels[c] = gtk_label_new(" A - z");
    c++; labels[c] = gtk_label_new(" A - o1");
    c++; labels[c] = gtk_label_new(" A - o2");
    c++; labels[c] = gtk_label_new(" A - o3");
    c++; labels[c] = gtk_label_new(" A - o4");
    int iii=c;

    for(int i=ii;i<=iii;i++)
        gtk_fixed_put(GTK_FIXED(actionFixed), labels[i], 220, 70+(i-ii)*30);

    ii=c+1;
    c++; labels[c] = gtk_label_new("Object B");  
    c++; labels[c] = gtk_label_new("Person B");
    c++; labels[c] = gtk_label_new(" B - x");
    c++; labels[c] = gtk_label_new(" B - y");
    c++; labels[c] = gtk_label_new(" B - z");
    c++; labels[c] = gtk_label_new(" B - o1");
    c++; labels[c] = gtk_label_new(" B - o2");
    c++; labels[c] = gtk_label_new(" B - o3");
    c++; labels[c] = gtk_label_new(" B - o4");
    iii=c;

    for(int i=ii;i<=iii;i++)
        gtk_fixed_put(GTK_FIXED(actionFixed), labels[i], 500, 70+(i-ii)*30);


    object_edit= gtk_entry_new(); gtk_entry_set_editable(GTK_ENTRY(object_edit),true); gtk_entry_set_text(GTK_ENTRY(object_edit), "object");
    person_edit= gtk_entry_new(); gtk_entry_set_editable(GTK_ENTRY(person_edit),true);gtk_entry_set_text(GTK_ENTRY(person_edit), "mommy");
    object2_edit= gtk_entry_new();gtk_entry_set_editable(GTK_ENTRY(object2_edit),true); gtk_entry_set_text(GTK_ENTRY(object2_edit), "box");
    person2_edit= gtk_entry_new();gtk_entry_set_editable(GTK_ENTRY(person2_edit),true);gtk_entry_set_text(GTK_ENTRY(person2_edit), "friend");

    gtk_fixed_put(GTK_FIXED(actionFixed), object_edit, 300, 60);
    gtk_fixed_put(GTK_FIXED(actionFixed), person_edit, 300, 90);
    gtk_fixed_put(GTK_FIXED(actionFixed), object2_edit, 580, 60);
    gtk_fixed_put(GTK_FIXED(actionFixed), person2_edit, 580, 90);

    Ax_adj = (GtkAdjustment *) gtk_adjustment_new (0.350, -3.0, 0.0, 0.001, 0.1, 0.1);
    Bx_adj = (GtkAdjustment *) gtk_adjustment_new (0.350, -3.0, 0.0, 0.001, 0.1, 0.1);
    Ay_adj = (GtkAdjustment *) gtk_adjustment_new (0.0, -1.0, 1.0, 0.001, 0.1, 0.1);
    By_adj = (GtkAdjustment *) gtk_adjustment_new (0.0, -1.0, 1.0, 0.001, 0.1, 0.1);
    Az_adj = (GtkAdjustment *) gtk_adjustment_new (0.3, -0.2, 1.0, 0.001, 0.1, 0.1);
    Bz_adj = (GtkAdjustment *) gtk_adjustment_new (0.3, -0.2, 1.0, 0.001, 0.1, 0.1);

    Ax = gtk_spin_button_new (Ax_adj, 0.001, 3); gtk_fixed_put(GTK_FIXED(actionFixed),Ax,300,120);
    Ay = gtk_spin_button_new (Ay_adj, 0.001, 3); gtk_fixed_put(GTK_FIXED(actionFixed),Ay,300,150);
    Az = gtk_spin_button_new (Az_adj, 0.001, 3); gtk_fixed_put(GTK_FIXED(actionFixed),Az,300,180);

    Bx = gtk_spin_button_new (Bx_adj, 0.001, 3); gtk_fixed_put(GTK_FIXED(actionFixed),Bx,580,120);
    By = gtk_spin_button_new (By_adj, 0.001, 3); gtk_fixed_put(GTK_FIXED(actionFixed),By,580,150);
    Bz = gtk_spin_button_new (Bz_adj, 0.001, 3); gtk_fixed_put(GTK_FIXED(actionFixed),Bz,580,180);

    Ao1_adj = (GtkAdjustment *) gtk_adjustment_new (0.0, 0.0, 1.0, 0.001, 0.1, 0.1);
    Bo1_adj = (GtkAdjustment *) gtk_adjustment_new (0.0, 0.0, 1.0, 0.001, 0.1, 0.1);
    Ao2_adj = (GtkAdjustment *) gtk_adjustment_new (0.0, 0.0, 1.0, 0.001, 0.1, 0.1);
    Bo2_adj = (GtkAdjustment *) gtk_adjustment_new (0.0, 0.0, 1.0, 0.001, 0.1, 0.1);
    Ao3_adj = (GtkAdjustment *) gtk_adjustment_new (0.0, 0.0, 1.0, 0.001, 0.1, 0.1);
    Bo3_adj = (GtkAdjustment *) gtk_adjustment_new (0.0, 0.0, 1.0, 0.001, 0.1, 0.1);
    Ao4_adj = (GtkAdjustment *) gtk_adjustment_new (0.0, -3.15, 3.15, 0.001, 0.1, 0.1);
    Bo4_adj = (GtkAdjustment *) gtk_adjustment_new (0.0, -3.15, 3.15, 0.001, 0.1, 0.1);

    Ao1 = gtk_spin_button_new (Ao1_adj, 0.001, 3); gtk_fixed_put(GTK_FIXED(actionFixed),Ao1,300,210);
    Ao2 = gtk_spin_button_new (Ao2_adj, 0.001, 3); gtk_fixed_put(GTK_FIXED(actionFixed),Ao2,300,240);
    Ao3 = gtk_spin_button_new (Ao3_adj, 0.001, 3); gtk_fixed_put(GTK_FIXED(actionFixed),Ao3,300,270);
    Ao4 = gtk_spin_button_new (Ao4_adj, 0.001, 3); gtk_fixed_put(GTK_FIXED(actionFixed),Ao4,300,300);

    Bo1 = gtk_spin_button_new (Bo1_adj, 0.001, 3); gtk_fixed_put(GTK_FIXED(actionFixed),Bo1,580,210);
    Bo2 = gtk_spin_button_new (Bo2_adj, 0.001, 3); gtk_fixed_put(GTK_FIXED(actionFixed),Bo2,580,240);
    Bo3 = gtk_spin_button_new (Bo3_adj, 0.001, 3); gtk_fixed_put(GTK_FIXED(actionFixed),Bo3,580,270);
    Bo4 = gtk_spin_button_new (Bo4_adj, 0.001, 3); gtk_fixed_put(GTK_FIXED(actionFixed),Bo4,580,300);

    // right/left/both arms
    c++; labels[c] = gtk_label_new("Used arm:");
    gtk_fixed_put(GTK_FIXED(actionFixed), labels[c], 220, 360 );

    radiobutton_armType[0]=gtk_radio_button_new_with_label  (NULL, "right_arm");
    radiobutton_armType[1]=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_armType[0]),"left_arm");
    radiobutton_armType[2]=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_armType[0]),"both_arms");

    for(int i=0; i<3; i++)
    {
        gtk_fixed_put(GTK_FIXED(actionFixed), radiobutton_armType[i], 350+i*100, 360 );
        rdarm[i].id=i;
        g_signal_connect (radiobutton_armType[i],"clicked",G_CALLBACK (radio_click_arm),  &rdarm[i]);
    }

    // grasp types
    c++; labels[c] = gtk_label_new("Grasp type:");
    gtk_fixed_put(GTK_FIXED(actionFixed), labels[c], 220, 400 );

    comboGrasps = gtk_combo_box_new_text();
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboGrasps), "open_hand");
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboGrasps), "close_hand");
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboGrasps), "custom");
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboGrasps), "green_ball");
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboGrasps), "open_pince_2");
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboGrasps), "close_pince_2");
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboGrasps), "open_pince_1");
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboGrasps), "close_pince_1");
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboGrasps), "closer_pince_1");
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboGrasps), "open_pince_3");
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboGrasps), "close_pince_3");
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboGrasps), "pointing_hand");

    gtk_fixed_put(GTK_FIXED(actionFixed), comboGrasps, 350, 390);
    g_signal_connect(G_OBJECT(comboGrasps), "changed", G_CALLBACK(combo_selected_grasp), NULL);

    //-------------------------------------------------------------------------------------------
    // now we can plot in the window
    //-------------------------------------------------------------------------------------------

    status = VALIDATION_REQUIRED;

}

//-----------------------------------------------------------
void button_send_command_click( GtkWidget *widget)
{
    if(status==STATUS_VALIDATED)
    {
        status = STATUS_SENT;
        gtk_button_set_label(GTK_BUTTON(widget),"COMMAND SENT");
        gtk_label_set_markup(GTK_LABEL (label_status), "Command has been sent");
        counterMsg++;
        history_record h;
        h.name = "Action";
        h.type = "Action";
        h.command = builtCommand.toString().c_str ();
        h.success = 0;
        send_command_action (&h);
        //saveToHistory(builtCommand);
    }
    else if(status==VALIDATION_REQUIRED)
    {
        requireValidation();
    }
    else if(status==STATUS_SENT)
    {
        status = STATUS_VALIDATED;
        gtk_button_set_label(GTK_BUTTON(widget),"CLICK TO RE-SEND");
        gtk_label_set_markup(GTK_LABEL (label_status), "Command has been sent: do you want to re-send?");
    }
    else
    {
        // impossible status
    }
}

void send_command_action (history_record *h)
{
    outPort.write(builtCommand);
    infoTab::instance()->add_info ("Action send: " + h->name);
    saveToHistory("Action", "Action", 0, builtCommand.toString().c_str());
}

//-----------------------------------------------------------
void button_reset_click( GtkWidget *widget)
{
    // set status to validation required -> to check if everything is correct
    requireValidation();
}

//-----------------------------------------------------------
void button_validate_click( GtkWidget *widget)
{
    object = gtk_entry_get_text(GTK_ENTRY(object_edit));
    object2 = gtk_entry_get_text(GTK_ENTRY(object2_edit));
    person = gtk_entry_get_text(GTK_ENTRY(person_edit));
    person2 = gtk_entry_get_text(GTK_ENTRY(person2_edit));
    xObject[0] = gtk_spin_button_get_value ((GtkSpinButton*)Ax);
    xObject[1] = gtk_spin_button_get_value ((GtkSpinButton*)Ay);
    xObject[2] = gtk_spin_button_get_value ((GtkSpinButton*)Az);
    xObject2[0] = gtk_spin_button_get_value ((GtkSpinButton*)Bx);
    xObject2[1] = gtk_spin_button_get_value ((GtkSpinButton*)By);
    xObject2[2] = gtk_spin_button_get_value ((GtkSpinButton*)Bz);
    oObject[0] = gtk_spin_button_get_value ((GtkSpinButton*)Ao1);
    oObject[1] = gtk_spin_button_get_value ((GtkSpinButton*)Ao2);
    oObject[2] = gtk_spin_button_get_value ((GtkSpinButton*)Ao3);
    oObject[3] = gtk_spin_button_get_value ((GtkSpinButton*)Ao4);
    oObject2[0] = gtk_spin_button_get_value ((GtkSpinButton*)Bo1);
    oObject2[1] = gtk_spin_button_get_value ((GtkSpinButton*)Bo2);
    oObject2[2] = gtk_spin_button_get_value ((GtkSpinButton*)Bo3);
    oObject2[3] = gtk_spin_button_get_value ((GtkSpinButton*)Bo4);
    cout<<"A = "<<xObject.toString()<<" "<<oObject.toString()<<endl
        <<"B = "<<xObject2.toString()<<" "<<oObject2.toString()<<endl
        <<"objects = "<<object<<" "<<object2<<endl
        <<"persons = "<<person<<" "<<person2<<endl;

    //now buil the command
    buildCommand();

    //send to icub gui the preview of the pose where the object will be
    Bottle obj;

    // build the new object
    obj.clear();
    obj.addString("object"); // command to add/update an object
    obj.addString(string(object+" "+i2s(counterMsg)).c_str());
    // object dimensions in millimiters
    // (it will be displayed as an ellipsoid with the tag "my_object_name")
    obj.addDouble(20.0);
    obj.addDouble(20.0);
    obj.addDouble(20.0);
    // object position in millimiters
    // reference frame: X=fwd, Y=left, Z=up
    obj.addDouble(xObject[0]*1000.0);
    obj.addDouble(xObject[1]*1000.0);
    obj.addDouble(xObject[2]*1000.0);
    // object orientation (roll, pitch, yaw) in degrees
    obj.addDouble(0);
    obj.addDouble(0);
    obj.addDouble(0);
    // object color (0-255)
    obj.addInt(255);
    obj.addInt(0);
    obj.addInt(0);
    // transparency (0.0=invisible 1.0=solid)
    obj.addDouble(1.0);
    //send to icubGui
    outGui.write(obj);
     // end validation
    status=STATUS_VALIDATED;
    gtk_label_set_text(GTK_LABEL(label_status),"Validation successful! Now click to send the command");
    gtk_button_set_label(GTK_BUTTON(button_send_command),"READY TO SEND");
}


//-----------------------------------------------------------
void radio_click_actions( GtkWidget *widget, void* r)
{
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)))
    {
        radio_data* rd = (radio_data*)(r);

        switch (rd->id)
        {
        case 0: action="goto";
            break;
        case 1: action="look";
            break;
        case 2: action="reach";
            break;
        case 3: action="take";
            break;
        case 4: action="point";
            break;
        case 5: action="lift";
            break;
        case 6: action="push";
            break;
        case 7: action="rotate";
            break;
        case 8: action="grasp";
            break;
        case 9: action="takeliftfall";
            break;
        case 10: action="takeobserve";
            break;
        case 11: action="takeputinthebox";
            break;
        case 12: action="putAonB";
            break;
        case 13: action="takegive";
            break;
        default:
            break;
        }
        cout<<"selected action = "<<action<<endl;
    }
    requireValidation();
}

//-----------------------------------------------------------
void radio_click_arm( GtkWidget *widget, void* r)
{
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)))
    {
        radio_data* rd = (radio_data*)(r);

        switch (rd->id)
        {
        case 0: armType="right";
            break;
        case 1: armType="left";
            break;
        case 2: armType="both";
            break;
        default:
            break;
        }
        cout<<"selected arm = "<<action<<endl;
    }
    requireValidation();
}

//-----------------------------------------------------------
void combo_selected_grasp(GtkWidget *widget, void* r)
{
   gchar *text =  gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));
   graspType = text;
   cout<<"GraspType = "<<graspType<<endl;
   g_free(text);
   requireValidation();
}

//-----------------------------------------------------------
void requireValidation()
{
    status=VALIDATION_REQUIRED;
    gtk_button_set_label(GTK_BUTTON(button_send_command),"VALIDATION REQUIRED");
    gtk_label_set_markup(GTK_LABEL (label_status), "You must validate the parameters before sending the command.");
    gtk_label_set_markup(GTK_LABEL(label_built_command),string("Built command: --").c_str());
}

//-----------------------------------------------------------
void buildCommand()
{
    // the common part of all commands
    builtCommand.clear();
    builtCommand.addString("action");
    builtCommand.addString(object.c_str());
    builtCommand.addString(armType.c_str());
    builtCommand.addString(action.c_str());

    if(action=="goto")
    {
        builtCommand.addDouble(xObject[0]);
        builtCommand.addDouble(xObject[1]);
        builtCommand.addDouble(xObject[2]);
        builtCommand.addDouble(oObject[0]);
        builtCommand.addDouble(oObject[1]);
        builtCommand.addDouble(oObject[2]);
        builtCommand.addDouble(oObject[3]);

    }
    else if((action=="look")||(action=="reach")||(action=="take")||
            (action=="point")||(action=="takeliftfall")||(action=="takeobserve")||
            (action=="takeputinthebox")||(action=="rotate"))
    {
        builtCommand.addDouble(xObject[0]);
        builtCommand.addDouble(xObject[1]);
        builtCommand.addDouble(xObject[2]);
    }
    else if((action=="putAonB"))
    {
        builtCommand.addDouble(xObject[0]);
        builtCommand.addDouble(xObject[1]);
        builtCommand.addDouble(xObject[2]);
        builtCommand.addDouble(xObject2[0]);
        builtCommand.addDouble(xObject2[1]);
        builtCommand.addDouble(xObject2[2]);
        builtCommand.addString(object2.c_str());
    }
    else if((action=="push"))
    {
        builtCommand.addString(direction.c_str());
        builtCommand.addDouble(xObject[0]);
        builtCommand.addDouble(xObject[1]);
        builtCommand.addDouble(xObject[2]);
    }
    else if((action=="grasp"))
    {
        builtCommand.addString(graspType.c_str());

        if(graspType=="custom")
        {
            // TO DO
            // add fingers
        }
    }
    else if((action=="takegive"))
    {
        builtCommand.addDouble(xObject[0]);
        builtCommand.addDouble(xObject[1]);
        builtCommand.addDouble(xObject[2]);
        builtCommand.addDouble(xObject2[0]);
        builtCommand.addDouble(xObject2[1]);
        builtCommand.addDouble(xObject2[2]);
    }
    else
    {
        //no more params for actions
        // lift

    }

    string bcmd=builtCommand.toString().c_str();
    //show the built command
    gtk_label_set_markup(GTK_LABEL(label_built_command),string("Built command: "+bcmd).c_str());
}
