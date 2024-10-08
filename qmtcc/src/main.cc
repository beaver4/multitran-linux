//#include "langpair_selector.hh"
//#include "word_selector.hh"
//#include "art_viewer.hh"

#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>

#include <unistd.h> // For realpath
#include <stdlib.h> // For free

#include "translator.hh"
#include "qmtcc.hh"



int main( int argc, char **argv )
{
    char exepath[PATH_MAX];
    char *resolvedPath = realpath("/proc/self/exe", NULL);
    if (resolvedPath == NULL) {
        perror("Error resolving path");
        return  1;
    }

    strncpy(exepath, resolvedPath, sizeof(exepath));
    free(resolvedPath); // Don't forget to free the memory allocated by realpath

    // Use dirname to get the directory of the actual executable
    char *dir = dirname(exepath);
    if (dir == NULL) {
        perror("Error getting directory name");
        return  1;
    }

    // Construct the data directory path using the directory of the actual executable
    char datadir[PATH_MAX];
    snprintf(datadir, sizeof(datadir), "%s/../share/multitran", dir);
    
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
