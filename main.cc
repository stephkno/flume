#include <flume.hpp>
#include <iostream>
#include <map>

static map<int, string> txtcolors{
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

int main(){

    //Err << (BGCOLOR)40 << (FGCOLOR)fg << (FXTXT)0 << "FG=" <<txtcolors[fg]<< " BG=" << 40 << " FX=" << txtcolors[fx] << endl;

    for(uint i = 0; i < 32; i++){
        // print messages
        Flume::SetPrintMode((PrintMode)i);
        Log << "This is flume print mode #" << i;
        Msg << "This is flume print mode #" << i;
        Warn << "This is flume print mode #" << i;
        Err << "This is flume print mode #" << i;
    }

    // print messages
    Log << "This is a flume Log";
    Msg << "This is a flume Msg";
    Warn << "This is a flume Warn";
    Err << "This is a flume Err";

    for ( uint fx : { 0,1,2,3,4,5,6,7 } ) {
        Msg << BGNONE << FGNONE << (FX)fx << " FX=" << txtcolors[fx] << endl;
    }
    for ( uint fg : { 30,31,32,33,34,35,39 } ) {
        Msg << BGNONE << (FGCOLOR)fg << FXNONE << " FG=" << txtcolors[fg] << endl;
    }
    for( uint bg : { 41, 42, 43, 44, 45, 46, 47 } ){
        Msg << (BGCOLOR)bg << FGNONE << FXNONE << " BG=" << txtcolors[bg] << endl;
    }

    Log << "Test again";
    Msg << "Final test";
    cout << endl << "Testing stdio" << endl;

}
