#ifndef RECORD_HH_
#define RECORD_HH_

#include <string>


// struct of an history record 
struct history_record
{
    std::string type;
    std::string command;
    std::string name;
    double time;
    int success;
    double err;
    unsigned int id;
    double lastTimePlayed;
};

#endif //RECORD_HH_