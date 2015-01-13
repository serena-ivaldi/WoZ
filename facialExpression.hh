#ifndef FACIAL_EXPR_HH_
# define FACIAL_EXPR_HH_

#include "utils.hh"


struct facialExpression
{
	string mou;
    string eli; 
    string leb; 
    string reb;
    string name; 
    int id;
};

class FacialExpressionTab
{
public:
	static FacialExpressionTab *instance ();
	void createExpressTab (GtkWidget *p_notebook_);
	void get_facial_expression_from_file ();

private:
	static FacialExpressionTab *FacialExpressionTab_;

public:

	static std::deque<facialExpression> listFacialExpression_;
};

void ExampleButton_express_click (GtkWidget *w, gpointer p);
void send_expression (history_record *h);

#endif // FACIAL_EXPR_HH_