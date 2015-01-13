#ifndef ACTION_HH_
# define ACTION_HH_


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

// recording status
#define STATUS_IDLE         0
#define STATUS_RECORDING    1
#define VALIDATION_REQUIRED 2
#define STATUS_VALIDATED    3
#define STATUS_SENT         4




struct radio_data
{
    bool activeParams[30];
    int id;
};





class ActionTab
{
public:
	void createTab (GtkWidget* notebook);

private:
	GtkWidget* tab_;

};


void button_send_command_click( GtkWidget *widget);
void button_reset_click( GtkWidget *widget);
void button_validate_click( GtkWidget *widget);
void button_feedback_positive_click( GtkWidget *widget);
void button_feedback_negative_click( GtkWidget *widget);
void radio_click_actions( GtkWidget *widget, void* r);
void radio_click_arm( GtkWidget *widget, void* r);
void combo_selected_grasp(GtkWidget *widget, void* r);
void requireValidation();
void buildCommand();

void send_command_action (history_record *h);

#endif //ACTION_HH_