#ifndef C_ACTION_HH_
# define C_ACTION_HH_


//
#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <yarp/dev/all.h>
//
#include <gtk/gtk.h>
//
#include <macsi/modHelp/modHelp.h>
#include <macsi/objects/objects.h>
//
#include <string>
#include <vector>
#include <iostream>
#include <deque>
#include <list>
#include <fstream>
#include <mutex>

#include "record.hh"
#include "utils.hh"


using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;
using namespace macsi::modHelp;
using namespace macsi::objects;
using namespace std;


struct ComplexActionParams {

	Vector R_HAND_SKYWARD;
    Vector R_HAND_MIDDLE;
    Vector R_HAND_GROUND;

    Vector L_HAND_SKYWARD;
    Vector L_HAND_MIDDLE;
    Vector L_HAND_GROUND;

    Vector L_POINTING;
    Vector R_POINTING;
};


class Complex_actionTab
{
public:
	void createTab (GtkWidget* notebook);
	static Complex_actionTab *instance ();
	static ComplexActionParams params;
	void configure (ResourceFinder rf);

private:
	void create_look_tab (GtkWidget* notebook);
	void create_home_tab (GtkWidget *frame);

	void create_move_hand_tab (GtkWidget *MoveHandFrame, string part);
	void create_grasp_tab (GtkWidget *headFrame);


private:
	static Complex_actionTab *Complex_actionTab_;

};

void send_look_at_on_click (GtkWidget *w, gpointer p);
void send_move_hand_on_click (GtkWidget *w, gpointer p);
void send_grasp_on_click (GtkWidget *w, gpointer p);
void send_home_position (GtkWidget* w, gpointer p);

void send_to_actionsServer (history_record *h);


#endif //C_ACTION_HH_