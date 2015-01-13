#ifndef TRAJPLAYERTAB_H
#define TRAJPLAYERTAB_H

#include "edit.hh"


class TaskPlayerTab
{

public:

	void createTaskPlayerTab (GtkWidget *w);

 	string taskRecordDir_;

	void displayAndLoadTaskRecord (GtkWidget *w);
	void displayTaskPlayerOption(GtkWidget *w);

	GtkWidget *init_mode_combo;
	GtkWidget *control_mode_combo;

	static string current_filename;

	static TaskPlayerTab *instance ();

private:
	static TaskPlayerTab *taskPlayerTab_;


};


void reloadOnClick (GtkWidget* w, gpointer p);

void armRecordButton_click (GtkWidget *w, gpointer p);


void send_ArmRecord (history_record *h);

history_record create_task_history_record (string filename);

void start_on_click (GtkWidget* w, gpointer p);

void displayMouvementOnClick (GtkWidget* w, gpointer p);

void clearMouvementOnClick (GtkWidget* w, gpointer p);





#endif //TRAJPLAYERTAB_H