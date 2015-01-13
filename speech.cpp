#include <sstream>

#include "speech.hh"


SpeechTab *SpeechTab::SpeechTab_ = 0;
string SpeechTab::speechText = "Not Initialized";
GtkWidget *SpeechTab::speech_edit = 0;
GtkWidget *SpeechTab::button_speech = 0;
string SpeechTab::speech_lang_ = "fr";


//send speech record to iSpeak_all
void send_speech (history_record *h)
{
    //create bottle
    Bottle bot;
    //add command to bottle
    string s = h->command;
    bot.addString(s.c_str());
    //send command to iSpeak_all
    outportSpeak_.write(bot);

    //save record on history
    saveToHistory("Speech", h->name, 0, h->command);
}

//send command to iPeak_all from interactive entry
void button_speech_click(GtkWidget *widget)
{   
    //get entry data
    SpeechTab::speechText = gtk_entry_get_text(GTK_ENTRY(SpeechTab::speech_edit));
    //create history record
    history_record h;
    std::stringstream  stream(SpeechTab::speechText);
    stream >> h.name;
    // add langauge setting to command (fr || en) 
    h.command = SpeechTab::speech_lang_ + " '" + SpeechTab::speechText + "'";
    //send command
    send_speech (&h);
}

//send command to isSPeak_all from pre recorded sentences
void ExampleButton_speech_click(GtkWidget *widget)
{   
    //get entry snetences
    SpeechTab::speechText = gtk_button_get_label(GTK_BUTTON(widget));
    //create history
    history_record h;
    std::stringstream  stream(SpeechTab::speechText);
    stream >> h.name; 
    // add language setting to command (en || fr)
    h.command = SpeechTab::speech_lang_ + " '" + SpeechTab::speechText + "'";
    send_speech (&h);

}

//Switch between language setting
void set_speech_lang (GtkWidget *w, gpointer lang)
{
    //get current language
    gpointer label = g_object_get_data (G_OBJECT(w), "label");
    SpeechTab::speech_lang_ = (const char*) lang;
    //change language
    string s = SpeechTab::speech_lang_; 
    if (s == "fr")
        gtk_label_set_text (GTK_LABEL(label), "French");

    if (s == "en")
        gtk_label_set_text (GTK_LABEL(label), "English");
    //add info about setting
    infoTab::instance()->add_info ("Set language: " + SpeechTab::speech_lang_);
}

void SpeechTab::createSpeechTab (GtkWidget *notebook)
{
    GtkWidget *speechTab;
    GtkWidget *speechFixed; 
    GtkWidget *speechLabel = gtk_label_new("Speech");

    speechTab = gtk_frame_new ("What i say");
    gtk_container_set_border_width (GTK_CONTAINER (speechTab), 10);
    gtk_notebook_append_page (GTK_NOTEBOOK(notebook), speechTab, speechLabel);

    //---- intercative speech (manual entry)
    speechFixed  = gtk_fixed_new ();
    GtkWidget *interactiveSpeechFixed = gtk_fixed_new ();
    gtk_container_add   (GTK_CONTAINER(speechTab), speechFixed);
    gtk_fixed_put(GTK_FIXED(speechFixed), interactiveSpeechFixed,   400, 0);
   
    // the entry for the speech text
    speech_edit = gtk_entry_new ();
    gtk_entry_set_editable(GTK_ENTRY(speech_edit),true);
    gtk_entry_set_text(GTK_ENTRY(speech_edit), "Hello");
    gtk_widget_set_size_request(speech_edit, 282, 35);  

    // the button to validate
    button_speech = gtk_button_new_with_label("Say this!");
    gtk_widget_set_size_request(button_speech, 100, 35);
    g_signal_connect(G_OBJECT(button_speech), "clicked", G_CALLBACK(button_speech_click), NULL);

    //--- LANGUAGE
    GtkWidget *languageFrame = gtk_frame_new ("Language");
    gtk_widget_set_size_request(languageFrame, 400, 200);
    gtk_fixed_put(GTK_FIXED(interactiveSpeechFixed), languageFrame,  0, 150);
    GtkWidget *langTable = gtk_table_new (4, 2, true);
    gtk_container_add(GTK_CONTAINER(languageFrame), langTable);
    GtkWidget *currentLangLabel2 = gtk_label_new("French");
    gtk_table_attach_defaults (GTK_TABLE (langTable), currentLangLabel2, 1, 2, 0, 1);
    GtkWidget *currentLangLabel1 = gtk_label_new("Current language: ");
    gtk_table_attach_defaults (GTK_TABLE (langTable), currentLangLabel1, 0, 1, 0, 1);
    GtkWidget *setCurrentLangLabel = gtk_label_new("Set Language: ");
    gtk_table_attach_defaults (GTK_TABLE (langTable), setCurrentLangLabel, 0, 2, 1, 2);
    // the button change to french
    GtkWidget *button_set_french = gtk_button_new_with_label("French");
    gtk_widget_set_size_request(button_set_french, 100, 35);
    g_signal_connect(G_OBJECT(button_set_french), "clicked", G_CALLBACK(set_speech_lang), (gpointer) "fr");
    gtk_table_attach_defaults (GTK_TABLE (langTable), button_set_french, 0, 1, 2, 3);
    g_object_set_data (G_OBJECT(button_set_french), "label", (gpointer) currentLangLabel2);

    // the button to change to english
    GtkWidget *button_set_english = gtk_button_new_with_label("English");
    gtk_widget_set_size_request(button_set_english, 100, 35);
    g_signal_connect(G_OBJECT(button_set_english), "clicked", G_CALLBACK(set_speech_lang), (gpointer) "en");
    gtk_table_attach_defaults (GTK_TABLE (langTable), button_set_english, 1, 2, 2, 3);
    g_object_set_data (G_OBJECT(button_set_english), "label", (gpointer) currentLangLabel2);


    gtk_fixed_put(GTK_FIXED(interactiveSpeechFixed), speech_edit,   6, 6);
    gtk_fixed_put(GTK_FIXED(interactiveSpeechFixed), button_speech, 100, 44);


    GtkWidget *scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(speechFixed), scrolledwindow);
    GtkWidget *speechExampleFixed;

    //---- example entry from conf file
    GtkWidget *speechExampleButton;
    speechExampleFixed = gtk_fixed_new ();
    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW(scrolledwindow), speechExampleFixed);
    gtk_widget_set_size_request (scrolledwindow, 300, 600);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_show (scrolledwindow);


    std::vector<string> *speechExampleList = new std::vector<string> ();

    if (RFspeechConf_.check("Example"))
    {
        Bottle &grp = RFspeechConf_.findGroup("Example");
        for (int i = 0; i < grp.size() - 1; i++){
            char buff[100];
            sprintf (buff , "%d", i);
            speechExampleList->push_back (grp.find(buff).asString().c_str());
        }

        for (unsigned int i = 0; i < speechExampleList->size (); i++)
        {
            speechExampleButton = gtk_button_new_with_label (speechExampleList->at (i).c_str ());
            gtk_widget_set_size_request(speechExampleButton, 282, 35);
            gtk_fixed_put(GTK_FIXED(speechExampleFixed), speechExampleButton, 0, 35 * i);
            gtk_widget_show (speechExampleButton);
            g_signal_connect(G_OBJECT(speechExampleButton), "clicked", G_CALLBACK(ExampleButton_speech_click), NULL);
        }
    }
    gtk_widget_show (speechLabel);
}



//get the instance of the singleton SpeechTab
SpeechTab *SpeechTab::instance ()
{
    if (SpeechTab_)
        return (SpeechTab_);
    else
    {
        SpeechTab::SpeechTab_ = new SpeechTab ();
        return (SpeechTab::SpeechTab_);
    }
}

