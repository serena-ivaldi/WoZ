#ifndef PLAYLISTTAB_HH_
# define PLAYLISTTAB_HH_

#include "utils.hh"
#include <yarp/os/Time.h>

class PlaylistTab
{

public:
	static PlaylistTab *instance ();

	void createPlaylistTab (GtkWidget* notebook);
	void display_infoTab ();
	static void display_playlist_tab ();
	static void display_history_tab ();


	
private:
	static PlaylistTab *playlistTab_;

public:
	static std::deque<history_record> history_;
	static std::deque<history_record> playlist_;

	//PlaylistTab
	static GtkWidget *playlistFixed_;
	//HistoryTab
	static GtkWidget *historyFixed_;

};

//=========================================
//			 PLAYLIST / HISTORY
//=========================================
void saveHistoryToFile(char* file);

// Save/Load playlist
void open_load_playlist_dialog_box (GtkWidget *w, deque<history_record> *list);
void open_save_history_dialog_box ();

// Save/Load history
void open_save_playlist_dialog_box (GtkWidget *w, deque<history_record> *list);
void savePlaylistToFile(char* file, deque<history_record> *list);
void load_history_record ();
void load_playlist_from_file (char* filename, deque<history_record> *list);
//clear history
void clear_history (GtkWidget w, gpointer p);


//=============================================
//               PLAYLIST
//=============================================





#endif //PLAYLISTTAB_HH_