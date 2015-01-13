#ifndef WOZ
#define WOZ




#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <yarp/dev/all.h>
#include <macsi/modHelp/modHelp.h>
#include <macsi/objects/objects.h>

#include "record.hh"
#include "action.hh"
#include "utils.hh"
#include "complex_action.hh"
#include "status.hh"
#include "taskPlayerTab.hh"
#include "speech.hh"
#include "playlistTab.hh"
#include "infoTab.hh"
#include "paramTab.hh"
#include "facialExpression.hh"



using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;
using namespace macsi::modHelp;
using namespace macsi::objects;
using namespace std;




//========================================================================================
//									FONCTIONS
//========================================================================================



void configureRF (ResourceFinder finder, int argc, char* argv[]);



void load_playlist_from_file (char* filename);



//=========================================
//			 		UTILS
//=========================================


void add_record (deque<history_record> *list, int id, string type, string name, double tim, string command);
void add_record (deque<history_record> *list, const history_record h);
void add_from_history_to_playlist (GtkWidget *w, gpointer p);
void delete_playlist_item (GtkWidget *w, gpointer p);
void saveToHistory(string type, string name, double tim, string command);
void modify_item_from_edit_tab(GtkWidget *w, gpointer p);








//=========================================
//			     EDIT
//=========================================
void display_edit_panel (GtkWidget *w, gpointer p);
void undisplay_edit_tab ();

//=========================================
//			     PLAY
//=========================================

void display_play ();
void display_play_list ();
void display_play_menu (GtkWidget *w);
void play_item_from_playlist (GtkWidget *w, gpointer i);
void play_playlist (GtkWidget *w, gpointer i);
void send_stop ();
void play_item (history_record *h);
void play_item_t (history_record *h);
void *play_playlist_in_thread (void *);
void set_stop (GtkWidget* w, gpointer p);
void display_playing_tab ();

//=============================================
//              ACTION
//=============================================




//========================================================================================
//									VARIABLES
//========================================================================================

//=============================================
// 					GLOBAL
//=============================================
// configuration files
extern ResourceFinder RFspeechConf_;
extern ResourceFinder RFhistory_;
extern ResourceFinder RFface_;

//global interface
//GTK window
extern GtkWidget *window_;
extern GtkWidget *p_notebook_;

// the port used to send data
extern Port outportSpeak_;
extern Port outPortExpression_;
extern Port outPortTaskPlayer_;

extern bool g_stop_;




//=============================================
//              PLAY
//=============================================

extern GtkWidget *playTabFixed_;
extern GtkWidget *playlistListFixed_;
extern GtkWidget *playTabFrame_;
extern std::deque<history_record> list_to_play_;
extern bool is_playing_;
extern GtkWidget *playingInfoTab_;
extern pthread_t play_thread_;
extern history_record *currentPlayingItem_;

extern Port outPort;

// the port used to send previews to the icubgui
extern Port outGui;











#endif //WOZ