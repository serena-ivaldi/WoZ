/**
 * Copyright (C) 2015 CODYCO Project
 * Author: Charles Ballarini, Serena Ivaldi <serena.ivaldi@inria.fr>
 * website: www.codyco.eu
 *
 * Copyright (C) 2014 EDHHI Project
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

/**
@ingroup macsi_guis

\biref Description of WoZ module and interaction
WoZ is the GUI used in EDHHI experience to control the robot mouvement.
You can find in WoZ/doc a schema that describe interaction wih WoZ and other modules.
- WoZ is the Gui.
    You can move arms and hands, play recorded trajectories, move the head, change some settings, speak, display emotion, make playlist of commands.
- actionsServer2 is the main module to control the robot: Arm, head, torso
- ispeak_all is a module used to speak in french and english
- taskrecorder is not directly in interaction with WoZ but is used to record file played by taskPlayer 
- taskPlayer is the module used to replay a mouvement recorded.



\section tested_os_sec Tested OS
Linux

\author Charles Ballarini
*/



#include "WoZ.hh"


using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;
//using namespace macsi::modHelp;
//using namespace macsi::objects;
using namespace std;



// a struct used for radiobuttons




// the name of the module (for ports)
string moduleName = "WoZ";
// the robot name
string robotName_;


// the port used to send previews to the icubgui
Port outGui;



// the file where to save the history and the path
string fileName, pathFiles;

// feedback
GtkWidget *button_feedback_positive;
GtkWidget *button_feedback_negative;



GtkWidget *label_command;

// GLOBAL

// configuration files
ResourceFinder RFspeechConf_;
ResourceFinder RFhistory_;
ResourceFinder RFface_;
ResourceFinder RFaction_;

//global interface
//GTK window
GtkWidget *window_;
GtkWidget *p_notebook_;

// the port used to send data
Port outportSpeak_;
Port outPortExpression_;
Port outPortTaskPlayer_;





//=============================================
//              PLAY
//=============================================

GtkWidget *playTabFixed_;
GtkWidget *playlistListFixed_;
GtkWidget *playTabFrame_;

std::deque<history_record> list_to_play_;
GtkWidget *playingInfoTab_;
history_record *currentPlayingItem_ = NULL;
pthread_t play_thread_;



//=============================================
//              ARMS RECORDS
//=============================================
string armRecordDir_;

//=============================================
//              UTILS
//=============================================

//infoTab
GtkWidget *infoTab_;
GtkWidget *infoFixed_;
std::list<std::string> infoList_;

//Stop
bool g_stop_ = false;
bool is_playing_= false;

std::deque<facialExpression> listFacialExpression_;

// GTK
//==================================================================
// close window
void destroy (void)
{
    gtk_main_quit ();
}

// new version of configure RF complying with new RF method and application file placement
void configureRF (ResourceFinder finder, int argc, char* argv[])
{
    string tmpfile;
    readString(finder,"modulename",moduleName,"WoZ");
    readString (finder, "robotname", robotName_, "icubsim");
    //robotName_ = tmpfile;

    // Configuration file
    //---------------------------------------------------
    cout << "===> Reading configuration files" << endl;
    
    // Set speech configuration file
    readString(finder,"filespeech",tmpfile,"woz_speech.conf");
    tmpfile = finder.findFile (tmpfile);
    
    RFspeechConf_.setVerbose(true);
    RFspeechConf_.setDefaultContext("taskRecorder");
    RFspeechConf_.setDefaultConfigFile (tmpfile.c_str ());
    RFspeechConf_.configure(1,&argv[0]);
    cout << " speech - " << tmpfile << endl;

    // Set action configurationfile  
    readString (finder, "filearmrecord", tmpfile, string ("../data/armsrecords/"));
    armRecordDir_ = finder.findFile (tmpfile);

    // Set facial expression file
    readString(finder, "fileface", tmpfile, "woz_face.conf");
    tmpfile = finder.findFile (tmpfile);
    RFface_.setDefaultContext("taskRecorder");
    RFface_.setDefaultConfigFile (tmpfile.c_str ());
    RFface_.configure(1,&argv[0]);
    cout << " facial - " << tmpfile << endl;

    // Set action
    readString(finder, "fileaction", tmpfile, "basicfiles/action.conf");
    tmpfile = finder.findFile (tmpfile);
    RFaction_.setContext("../WoZ");
    RFaction_.setDefaultConfigFile (tmpfile.c_str ());
    RFaction_.configure("MACSI_ROOT",1,&argv[0]);
    cout << " - " << tmpfile << endl;
    cout << "*********** END File configuration" << endl;

}

/**
void configureRF (ResourceFinder finder, int argc, char* argv[])
{
    string tmp;
    readString(finder,"modulename",moduleName,"WoZ");
    

    // Configuration file
    //---------------------------------------------------
    cout << "*********** File configuration" << endl;
    // Set speech configuration file
    readString(finder,"filespeech",tmp,"basicfiles/speech.conf");
    tmp = finder.findFile (tmp);
    RFspeechConf_.setContext("../WoZ");
    RFspeechConf_.setDefaultConfigFile (tmp.c_str ());
    RFspeechConf_.configure("MACSI_ROOT",1,&argv[0]);
    cout << " - " << tmp << endl;

    // Set action configurationfile  

    // Set facial expression file
    readString(finder, "fileface", tmp, "basicfiles/face.conf");
    tmp = finder.findFile (tmp);
    RFface_.setContext("../WoZ");
    RFface_.setDefaultConfigFile (tmp.c_str ());
    RFface_.configure("MACSI_ROOT",1,&argv[0]);
    cout << " - " << tmp << endl;

    readString (finder, "robotname", tmp, "icubsim");
    robotName_ = tmp;
    readString (finder, "filearmrecord", tmp, string ("../data/armsrecords/"));
    armRecordDir_ = finder.findFile (tmp);

    // Set action
    readString(finder, "fileaction", tmp, "basicfiles/action.conf");
    tmp = finder.findFile (tmp);
    RFaction_.setContext("../WoZ");
    RFaction_.setDefaultConfigFile (tmp.c_str ());
    RFaction_.configure("MACSI_ROOT",1,&argv[0]);
    cout << " - " << tmp << endl;
    cout << "*********** END File configuration" << endl;

}
**/



//-----------------------------------------------------------
void button_feedback_positive_click( GtkWidget *widget)
{
    Bottle bot;
    bot.addString("feedback");
    bot.addString("action");
    bot.addString("yes");
    outPort.write(bot);
    cout<<"Sent message: "<<bot.toString()<<endl;

    requireValidation();
}

//-----------------------------------------------------------
void button_feedback_negative_click( GtkWidget *widget)
{
    Bottle bot;
    bot.addString("feedback");
    bot.addString("action");
    bot.addString("no");
    outPort.write(bot);
    cout<<"Sent message: "<<bot.toString()<<endl;

    requireValidation();
}

//=================================================================
//                     MAIN
//=================================================================

int main(int argc, char * argv[])
{

    //initialize yarp network
    Network yarp;
    //g_thread_init (NULL);
    gdk_threads_init ();
    gdk_threads_enter ();

    // create device drivers and connect to icub
    if (!yarp.checkNetwork())
    {
        cout<<"YARP network not available. Aborting."<<endl;
        return -1;
    }
	
    ResourceFinder finder;
    //retrieve information for the list of parts
    finder.setVerbose(true);
    finder.setContext("WoZ");
    finder.setDefaultConfigFile("conf/WoZ.ini");
    finder.configure("MACSI_ROOT",argc,argv);

    if (finder.check("help"))
    {
        cout << "Options:" << endl << endl;
        cout << "\t--from   fileName: input configuration file" << endl;
        cout << "\t--context dirName: resource finder context"  << endl;

        return 0;
    }

    Time::turboBoost();

    // load all Ressource finder files
    configureRF (finder, argc, argv);

    //========================================================================
    //                          OPEN PORTS
    //========================================================================
    
    cout << "WoZ: opening the port sending commands to actionsServer" << endl;
    //opening port to send data to actionsServer
    outPort.open(string("/"+moduleName+"/cmd:o").c_str());
    outGui.open(string("/"+moduleName+"/gui:o").c_str());


    cout << "WoZ: opening the port sending commands to iSpeak" << endl;
    //opening port to send data to actionsServer
    outportSpeak_.open(string("/"+moduleName+"/speech:o").c_str());
    Network::connect(string("/"+moduleName+"/speech:o").c_str(),"/iSpeak");

    cout << "WoZ: opening the port sending commands to facialExpression" << endl;
    outPortExpression_.open(string("/"+moduleName+"/expression:o").c_str());    
    Network::connect(string("/"+moduleName+"/expression:o").c_str(),"/emotion/in");

    if(robotName_ == "icubSim")
        Network::connect(string("/"+moduleName+"/expression:o").c_str(),"/icub/face/emotions/in");
    if(robotName_ == "icub")
        Network::connect(string("/"+moduleName+"/expression:o").c_str(),"/icub/face/emotions/in");
    
    cout << "WoZ: opening the port sending commands to taskPlayer" << endl;
    outPortTaskPlayer_.open(string("/"+moduleName+"/taskPlayer:o").c_str());    
    Network::connect(string("/"+moduleName+"/taskPlayer:o").c_str(),"/taskPlayer/rpc:i");


    //outportCmdFace.open(string("/"+name+"/cmdFace:rpc").c_str());

    //=========================================


    //============ GTK ========================
  


    // This is called in all GTK applications. Arguments are parsed
    // from the command line and are returned to the application.
    gtk_init (&argc, &argv);
  

    // creating tab

	GtkWidget *expressionTab;
	GtkWidget *robotParamTab;
    GtkWidget *infoContainer;
	
	GtkWidget *expressionFixed;
    GtkWidget *statusFixed;
    GtkWidget *robotParamFixed;

    GtkWidget *expressionLabel = gtk_label_new("Expression");
    GtkWidget *statusLabel = gtk_label_new("Status");
	GtkWidget *robotParamLabel = gtk_label_new("Robot Params");

    window_ = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window_), "Actions Server GUI");
    gtk_signal_connect (GTK_OBJECT (window_), "destroy",GTK_SIGNAL_FUNC (destroy), NULL);
    gtk_container_border_width (GTK_CONTAINER (window_), 10);


    GtkWidget* generalTable = gtk_table_new (20, 20, true);
    gtk_container_add (GTK_CONTAINER (window_), generalTable);

    // description of the window
    //-------------------------------------------------------------------------------------------
	gtk_window_set_default_size(GTK_WINDOW(window_), 800, 800);

    //add info tab into mainFixed
    infoContainer = gtk_frame_new ("Info");
    gtk_container_set_border_width (GTK_CONTAINER (infoTab_), 10);
    gtk_table_attach_defaults (GTK_TABLE (generalTable), infoContainer, 0, 13, 16, 20);
    infoTab *infoTab = infoTab::instance ();
    infoTab->container_ = infoContainer;

    //add param tab into mainFixed
    paramTab *paramTab = paramTab::instance ();

    GtkWidget *paramContainer = gtk_frame_new ("Params");
    gtk_container_set_border_width (GTK_CONTAINER (paramTab), 10);
    gtk_table_attach_defaults (GTK_TABLE (generalTable), paramContainer, 13, 20, 0, 16);

    paramTab::container_ = paramContainer;

	// add notebook into mainFixed
    p_notebook_ = gtk_notebook_new ();
    gtk_widget_set_size_request (p_notebook_, 850, 750);
    gtk_container_set_border_width (GTK_CONTAINER (p_notebook_), 10);
    gtk_table_attach_defaults (GTK_TABLE (generalTable), p_notebook_, 0, 13, 0, 16);
    g_signal_connect( G_OBJECT(p_notebook_), "switch-page", G_CALLBACK(paramTab::clear), NULL);

	//SET NoteBook Tab
    //-------------------------------------------------------------------------------------------
    
	// add tab into notebook
	
    StatusTab *statusTab = StatusTab::instance ();
    StatusTab::robotName_ = robotName_;
    StatusTab::add_param ("Robot Name", robotName_);
    statusTab->createTab (p_notebook_);

    ActionTab actionTab;
    actionTab.createTab (p_notebook_);

    Complex_actionTab *cActionTab = Complex_actionTab::instance ();
    cActionTab->configure (RFaction_);
    cActionTab->createTab (p_notebook_);




	//speechTab
    SpeechTab *SpeechTab = SpeechTab::instance ();
    SpeechTab->createSpeechTab (p_notebook_);

	//ExpressionTab
    FacialExpressionTab *facialExpression = FacialExpressionTab::instance ();
    facialExpression->createExpressTab (p_notebook_);

    //ArmRecord
    TaskPlayerTab *taskPlayerTab = TaskPlayerTab::instance ();
    taskPlayerTab->taskRecordDir_ = armRecordDir_;
    taskPlayerTab->createTaskPlayerTab (p_notebook_);

	//PLaylistTab
    PlaylistTab *PlaylistTab = PlaylistTab::instance ();
    PlaylistTab->createPlaylistTab (p_notebook_);

    //playTab
    display_play ();

	gtk_widget_show(p_notebook_);

    gtk_widget_show_all (window_);
    gtk_main ();


    //============ GTK ENDS ========================

    //-------------------------------------------------------------------------------------------
    // closing the GUI
    //-------------------------------------------------------------------------------------------


    cout<<"WoZ: closing the port sending commands"<<endl;
    //closing the port sending commands
    outPort.interrupt();
    outPort.close();
    outportSpeak_.interrupt ();
    outportSpeak_.close ();
    outPortExpression_.interrupt ();
    outPortExpression_.close ();

    // eliminate previous objects
    Bottle obj;
    obj.addString("reset");
    outGui.write(obj);
    outGui.interrupt();
    outGui.close();

    gdk_threads_leave ();
    cout<<"WoZ: finished"<<endl;

    return 1;
}


