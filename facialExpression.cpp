#include <sstream>

#include "facialExpression.hh"


FacialExpressionTab *FacialExpressionTab::FacialExpressionTab_ = 0;
std::deque<facialExpression> FacialExpressionTab::listFacialExpression_ = std::deque<facialExpression> ();

void FacialExpressionTab::createExpressTab (GtkWidget *p_notebook_)
{
    GtkWidget *expressTab;
    GtkWidget *expressFixed; 
    GtkWidget *expressLabel = gtk_label_new("Expression");

    expressTab = gtk_frame_new ("What i look like");
    gtk_container_set_border_width (GTK_CONTAINER (expressTab), 10);
    gtk_notebook_append_page (GTK_NOTEBOOK(p_notebook_), expressTab, expressLabel);

    //---- intercative speech (manual entry)
    expressFixed  = gtk_fixed_new ();
    GtkWidget *interactiveexpressFixed = gtk_fixed_new ();
    gtk_container_add   (GTK_CONTAINER(expressTab), expressFixed);
    gtk_fixed_put(GTK_FIXED(expressFixed), interactiveexpressFixed,   400, 0);


    GtkWidget *scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(expressFixed), scrolledwindow);

    GtkWidget *expressExampleFixed;
    //---- example entry from conf file
    GtkWidget *expressExampleButton;
    expressExampleFixed = gtk_fixed_new ();
    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW(scrolledwindow), expressExampleFixed);
    gtk_widget_set_size_request (scrolledwindow, 300, 600);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_show (scrolledwindow);


    get_facial_expression_from_file ();

    for (unsigned int i = 0; i < FacialExpressionTab::listFacialExpression_.size (); i++)
    {
        expressExampleButton = gtk_button_new_with_label (FacialExpressionTab::listFacialExpression_[i].name.c_str ());
        gtk_widget_set_size_request(expressExampleButton, 282, 35);
        gtk_fixed_put(GTK_FIXED(expressExampleFixed), expressExampleButton, 0, 35 * i);
        gtk_widget_show (expressExampleButton);
        g_signal_connect(G_OBJECT(expressExampleButton), "clicked", G_CALLBACK(ExampleButton_express_click), &listFacialExpression_[i].id);
    }
    
    gtk_widget_show (expressTab);
}

void FacialExpressionTab::get_facial_expression_from_file ()
{
    bool err = false;
    facialExpression face;
    int i = 0;
    //get the facial expression define in the file set in RFface_
    while (true)
    {
        char buff[100];
        sprintf (buff , "%d", i);
        Bottle &grp = RFface_.findGroup(buff);
        if (grp.isNull ())
            break;
        else
        {
            face.mou = grp.find ("mou").asString ();
            face.eli = grp.find ("eli").asString ();
            face.leb = grp.find ("leb").asString ();
            face.reb = grp.find ("reb").asString ();
            face.name = grp.find ("name").asString ();
            face.id = i;
            if (face.name == "" || face.mou == "" || face.leb == "" || face.eli == "" || face.reb == "")
                cout << "Error in loading facial expression" << endl;
            FacialExpressionTab::listFacialExpression_.push_back (face);
        }
        i++;
    }
}

void ExampleButton_express_click (GtkWidget *w, gpointer p)
{
    // get data
    int id =  *(int *) p;
    std::deque<facialExpression>::iterator it = FacialExpressionTab::listFacialExpression_.begin ();
    // find the expression selected
    while (it->id != id && FacialExpressionTab::listFacialExpression_.end() != it)
        it++;
    if (it == FacialExpressionTab::listFacialExpression_.end ())
        std::cout << "Expression: BUG !!! Error from selection" << std::endl; 
    

    //create a record with the command
    history_record h;
    h.name = it->name;
    string command = "";
    command += " set mou " + it->mou;
    command += " set eli " + it->eli;
    command += " set leb " + it->leb;
    command += " set reb " + it->reb;
    h.command = command;

    //send the record
    send_expression (&h);
    return;

}

void send_expression (history_record *h)
{
    //get the command from record to add it to bootle
    string command = h->command;
    std::stringstream  stream(command);
    for (int i = 0; i < 4; i++)
    {
        Bottle bot;
        for (int j = 0; j < 3; j++)
        {
            string word;
            stream >> word;
            bot.addString (word.c_str());
        }
        cout << "Expression: " << bot.toString ().c_str () << endl;
        //send bottle to outPortExpression_
        outPortExpression_.write (bot);
    }

    //save the record to history
    infoTab::instance ()->add_info ("Send Expression: " + h->name);
    saveToHistory("Expression", h->name, 0, command);
}

FacialExpressionTab *FacialExpressionTab::instance ()
{
    if (FacialExpressionTab_)
        return (FacialExpressionTab_);
    else
    {
        FacialExpressionTab_ = new FacialExpressionTab ();
        return (FacialExpressionTab_);
    }
}
