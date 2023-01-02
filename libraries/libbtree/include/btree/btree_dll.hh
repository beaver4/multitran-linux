#ifndef BTREE_DLL_HH__
#define BTREE_DLL_HH__

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBMTSUPPORT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBMTSUPPORT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef WIN32
#ifdef LIBBTREE_EXPORTS
#define LIBBTREE_API __declspec(dllexport)
#else
#define LIBBTREE_API __declspec(dllimport)
#endif
#else
#define LIBBTREE_API
#endif

#endif
