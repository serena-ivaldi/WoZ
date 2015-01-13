#ifndef INFOTAB_HH_
# define INFOTAB_HH_

#include "utils.hh"

class infoTab 
{

public:
	static infoTab *instance ();

	void display_infoTab ();
	void add_info (string s);

private:
	static infoTab *infoTab_;
	static std::list<std::string> infoList_;


public:
	static GtkWidget *container_;
};

#endif // INFOTAB_HH_