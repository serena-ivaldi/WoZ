
#include "paramTab.hh"


paramTab *paramTab::paramTab_ = 0;
GtkWidget *paramTab::container_ = 0;

paramTab *paramTab::instance ()
{
    if (paramTab_)
        return (paramTab_);
    else
    {
        paramTab::paramTab_ = new paramTab ();
        return (paramTab::paramTab_);
    }
}

void paramTab::clear ()
{
	if (container_ != 0)
		clear_widget (container_);
}
