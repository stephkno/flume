
#ifndef FLUME_HPP
#define FLUME_HPP

#include <iostream>
#include <chrono>
#include <vector>
#include <ostream>
#include <fstream>
#include <filesystem>
#include <string>

enum FGCOLOR{
    FGNONE = 39,
    FGBLACK = 30,
    FGRED,
    FGGREEN,
    FGYELLOW,
    FGBLUE,
    FGMAGENTA,
    FGCYAN,
};

enum BGCOLOR{
    BGNONE = 49,
    BGBLACK = 40,
    BGRED,
    BGGREEN,
    BGYELLOW,
    BGBLUE,
    BGMAGENTA,
    BGCYAN,
    BGWHITE,
};

enum FX{
    FXNONE = 0,
    FXBOLD,
    FXFAINT, 
    FXITALIC,
    FXUNDERLINE,
    FXBLINK, 
    FXREVERSE,
    FXSTRIKETHROUGH
};

enum MsgType{
    MSG,
    ERR,
    LOG,
    WARN,
    DEFAULT
};

using namespace std;

map<int, string> txtcolors{
    {39, "FGNONE"},
    {30, "FGBLACK"},
    {31, "FGRED"},
    {32, "FGGREEN"},
    {33, "FGYELLOW"},
    {34, "FGBLUE"},
    {35, "FGMAGENTA"},
    {35, "FGCYAN"},
    {49, "BGNONE"},
    {40, "BGBLACK"},
    {41, "BGRED"},
    {42, "BGGREEN"},
    {43, "BGYELLOW"},
    {44, "BGBLUE"},
    {45, "BGMAGENTA"},
    {46, "BGCYAN"},
    {47, "BGWHITE"},
    {0, "FXNONE"},
    {1, "FXBOLD"},
    {2, "FXFAINT"},
    {3, "FXITALIC"},
    {4, "FXUNDERLINE"},
    {5, "FXBLINK"},
    {6, "FXREVERSE"},
    {7, "FXSTRIKETHROUGH"}
};

#define Msg Flume(MSG, __FILE__, __LINE__, __FUNCTION__)
#define Err Flume(ERR, __FILE__, __LINE__, __FUNCTION__)
#define Log Flume(LOG, __FILE__, __LINE__, __FUNCTION__)
#define Warn Flume(WARN, __FILE__, __LINE__, __FUNCTION__)

// status flag for print mode
// 1 - timestamp
// 2 - filename
// 3 - function name
// 4 - line number
// 5 - message type
#define PrintMode unsigned int


enum Verbosity{
    NONE,
    ALL, // render all text and tags
    MIN, // render minimum tags (time)
    PLAIN, // render just text and no tags
    DELTA // render only changes in tags
};
/*
struct Record{
    chrono::system_clock::time_point time;
    string msg;

    Record(string msg, chrono::system_clock::time_point time) : msg(msg), time(time)
    {
    }

    string GetTimestamp(){
        // Convert the time point to a std::time_t object
        std::time_t now_c = std::chrono::system_clock::to_time_t(time);
        std::tm* now_tm = std::localtime(&now_c);
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%H:%M:%S", now_tm);
        return buffer;
    }
};

class FileWriter{
    
    ofstream logfile;
    
    string GetProcessname(){
        std::string name;
        std::ifstream cmdline("/proc/self/cmdline");
        if (cmdline.is_open()) {
            std::getline(cmdline, name, '\0');
        }
        return name;
    }

public:

    FileWriter(){
        string filepath = filesystem::current_path().string() + "../logs/" + GetProcessname() + ".log";
        logfile.open(filepath);
    }
    ~FileWriter(){
        logfile.close();
    }
    static void Write(string message){
        Get().logfile << message;
    }
    static FileWriter & Get(){
        static FileWriter instance;
        return instance;
    }

};
*/

class Flume{

    static PrintMode printmode;
    static uint lognum;

    // enable stdio logging 
    FGCOLOR fg = FGNONE;
    BGCOLOR bg = BGNONE;
    FX fx = FXNONE;
    MsgType msgtype;

    //vector<Record> records;
    //ANSI_Color color = FGNORMAL;
    string file, line, func;
    
    bool show_line = true;
    bool show_func = true;
    bool show_file = true;
    bool show_time = true;
    bool print_title_ = true;

    
public:
   Flume(MsgType m, const char * f, int l, const char* ff){
        file = f;
        line = to_string(l);
        func = ff;
        print_title_=true;
        msgtype = m;
        fg = GetMsgColor(m);
    }

    ~Flume()
    {
        cout << "\033[" << FGNONE << ";" << BGNONE << ";" << FXNONE << "m";
    }

    static void SetPrintMode(PrintMode p){
        printmode = p;
    }
    
    FGCOLOR GetMsgColor(MsgType m)
    {
        if(m==MSG){
            return FGGREEN;
        }else if(m == LOG){
            return FGCYAN;
        }else if(m == WARN){
            return FGYELLOW;
        }else if(m == ERR){
            return FGRED;
        }
        return FGNONE;
    }

    string GetType()
    {
        switch(msgtype){
            case MSG:{
                return "Msg";
            }
            case ERR:{
                return "ERR";
            }
            case LOG:{
                return "LOG";
            }
            case WARN:{
                return "WARN";
            }
        }
        return "????";
    }

    /*
        0: default
        30: black
        31: red
        32: green
        33: yellow
        34: blue
        35: magenta
        36: cyan
        37: white

    void SetColor(ostringstream & ss){
        ss << "\033[" << c << "m";
    }
    void SetFX(ostringstream & ss){
        ss << "\033[" << c << "m";
    }
    void Reset(ostringstream & ss){
        ss << "\033[0m";
    }
    */

    void SetFGColor(FGCOLOR c){
        fg = c;
    }
    void SetBGColor(BGCOLOR c){
        bg = c;
    }
    void SetFX(FX f){
        fx = f;
    }

    bool GetBit(uint byte, uint bit){
        return ( byte & (2^bit) ) > 0x0;
    }

    void PrintStatus(ostringstream & os, ostringstream & ds){
        
        if (!print_title_) {
            return;
        }

        // add newline
        os << endl;

        // Print the title string
        print_title_ = false;

        string msg_type = GetType();
        if(GetBit(printmode, 0)){
            os <<  "[";
            os <<   lognum;
            os <<  "]";
        }

        lognum++;

        if(GetBit(printmode, 1)){
            // print timestamp

            //Record r(ds.str(), std::chrono::system_clock::now());
            //records.push_back(r);

            string timestamp = r.GetTimestamp();
            /*
            auto now = std::chrono::system_clock::now();
            auto duration = now.time_since_epoch();
            auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            */

            os <<  "(";

            if(show_time){
                os << timestamp;
            }

            os <<  "):";
            // end timestamp
        }

        // print filename
        if(GetBit(printmode, 2)){
            os << trim(file);
            os << ":";

        }

        // print function name
        if(GetBit(printmode, 3)){
            os << func;
            os << ":";
        }

        // print file line number
        if(GetBit(printmode, 4)){
            os << line;
            os << ":";
        }

        // print log message type
        if(GetBit(printmode, 5)){
            os <<  "[";
            os << msg_type;
            os <<  "] ";
        }

    }

    Flume& operator<<(const FGCOLOR& mod) {
        SetFGColor(mod);
        return *this;
    }
    Flume& operator<<(const BGCOLOR& mod) {
        SetBGColor(mod);
        return *this;
    }
    Flume& operator<<(const FX& mod) {
        SetFX(mod);
        return *this;
    }
    
    // actual print log method
    template<typename T>
    Flume& operator<<(const T& data) {
       
        ostringstream ds;
        ostringstream os;
        ostringstream osn;
        
        ds << data;

        // \033[fg;bg;fxm
        FGCOLOR c = GetMsgColor(msgtype);
        os << "\033[" << (uint)c << "m";// << BGNONE << ";" << FXNONE << "m";

        PrintStatus(os, ds);

        os << "\033[" << FGNONE << "m";
        os << "\033[" << (uint)fg << ";" << (uint)bg << ";" << (uint)fx << "m";

        os << ds.str();
        osn << ds.str();

        //FileWriter::Write(osn.str()); // broken?
        cout << os.str();
        os << "\033[" <<  (uint)FGNONE << ";" <<  (uint)BGNONE << ";" <<  (uint)FXNONE << "m";

        // write file
        return *this;

    }

    // overload operator<< for any stdio manipulator(?)
    Flume& operator<<(std::ostream& (*manipulator)(std::ostream&)) {
        if(msgtype){
            //manipulator(std::cout);
        }
        //write file

        return *this;
    }

    std::string trim(const string& fileString)
    {
        size_t lastSlashIndex = fileString.rfind("/");
        if (lastSlashIndex == std::string::npos) {
            return fileString;
        } else {
            return fileString.substr(lastSlashIndex+1);
        }
    }

};

PrintMode Flume::printmode = 31;
uint Flume::lognum = 0;
#endif
