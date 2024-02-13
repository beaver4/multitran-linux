//#include "langpair_selector.hh"
//#include "word_selector.hh"
//#include "art_viewer.hh"

#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>

#include "translator.hh"
#include "qmtcc.hh"

int main( int argc, char **argv )
{
    char exepath[PATH_MAX]; char datadir[PATH_MAX];
    
    strncpy(exepath, argv[0], sizeof(exepath));
    dirname(exepath);
    snprintf(datadir, sizeof(datadir), "%s/../share/multitran", exepath);
    mt::datapath = datadir;
    
    qmtcc a(argc,argv);
//    langpair_selector sel;

/*
    mt::phrase ph;
    ph.push_back("good");
    ph.push_back("morning");
    ph.push_back("moscow");
    ph.push_back("listed");
    ph.push_back("qwerqwerqwer");
    
    mt::translation tr(ph,mt::english,mt::russian);
    

//    art_viewer sel;
//    sel.set(tr.asets()[2]);
    word_selector sel;
    sel.set(tr.asets());
    sel.show();
*/
    translator sel;
    a.setMainWidget(&sel);
    sel.show();
    return a.exec();
}
