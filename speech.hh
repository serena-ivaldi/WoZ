#ifndef SPEECH_HH_
# define SPEECH_HH_

#include "utils.hh"

class SpeechTab
{
public:
	void createSpeechTab (GtkWidget *p_notebook_);

	static SpeechTab *instance ();
	static string speechText;
	static GtkWidget *speech_edit;
	static GtkWidget *button_speech;
	static string speech_lang_;

private:
	static SpeechTab *SpeechTab_;

};

//=============================================
//              SPEECH
//=============================================


//=========================================
//			     SPEECH
//=========================================

void createSpeechTab (GtkWidget *p_notebook_);
void ExampleButton_speech_click(GtkWidget *widget);
void button_speech_click(GtkWidget *widget);
void send_speech (history_record *h);
void set_speech_lang (GtkWidget *w, gpointer lang);

#endif //SPEECH_HH_