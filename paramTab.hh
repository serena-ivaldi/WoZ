#ifndef PARAMTAB_HH_
# define PARAMTAB_HH_

#include "utils.hh"

class paramTab 
{

public:
	static paramTab *instance ();
	static void clear ();

private:
	static paramTab *paramTab_;

public:
	static GtkWidget *container_;
};

#endif // PARAMTAB_HH_
