/* Shared Use License: This file is owned by Derivative Inc. (Derivative) and
 * can only be used, and/or modified for use, in conjunction with 
 * Derivative's TouchDesigner software, and only if you are a licensee who has
 * accepted Derivative's TouchDesigner license or assignment agreement (which
 * also govern the use of this file).  You may share a modified version of this
 * file with another authorized licensee of Derivative's TouchDesigner software.
 * Otherwise, no redistribution or sharing of this file, with or without
 * modification, is permitted.
 */

/*
 * COMMENTS:
 *      This class is for inter-processes locking based on named mutex
 *
 */

#ifndef __UT_Mutex__
#define __UT_Mutex__


#ifdef WIN32
#include <windows.h>
#else
#include <libkern/OSAtomic.h>
#endif

#ifdef WIN32
    typedef HANDLE mutexId;
#else
    typedef int mutexId;
#endif


// Needed so people can compile this outside the TouchDesigner build environement
#ifndef UT_DLLEXP
#define UT_DLLEXP
#endif 
    
class UT_DLLEXP UT_Mutex
{
public:
     UT_Mutex(const char *name);
     ~UT_Mutex();

     bool       lock(int timeout);
     bool       unlock();
     
     // This class is distributed to the users, so make sure it doesn't
     // rely on any internal TouchDesigner classes

private:
     mutexId       myMutex;
};


#endif /* __UT_Mutex__ */
