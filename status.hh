#ifndef STATUS_HH_
# define STATUS_HH_

#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <yarp/dev/all.h>
#include <gtk/gtk.h>

#include "robot_interfaces.h"
#include "utils.hh"
#include <map>
#include "infoTab.hh"


struct radio_data_option
{
    int mode;
    int id;
};


#define POS  0
#define TRQ  1
#define IMPS 2
#define IMPM 3
#define IMPH 4



class StatusTab {
public:
	void createTab (GtkWidget* notebook);
	void displayOptions (GtkWidget* w);
	void displayStiffnessAndImpedance (GtkWidget *w);
	void displayMarker (GtkWidget* w);


	static void add_param (string key, string value);
	
	static std::map<string, string> paramsList_;

private:
	static StatusTab *StatusTab_;

private:
	GtkWidget *radiobutton_mode_pos[5];
	GtkWidget *radiobutton_mode_trq[5];
	GtkWidget *radiobutton_mode_imp_s[5];
	GtkWidget *radiobutton_mode_imp_m[5];
	GtkWidget *radiobutton_mode_imp_h[5];
	GtkWidget *label_info[5];

public:

	static StatusTab *instance ();
	static string robotName_;
};

void displayRobotParam ();
void send_change_tracking_mode_on_click (GtkWidget* w, gpointer command);
void send_torso_option_on_click (GtkWidget* w, gpointer command);
void send_speech_option_on_click (GtkWidget* w, gpointer command);
void send_marker (GtkButton* w);

#endif //STATUS_HH_