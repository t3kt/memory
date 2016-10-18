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
 * Filename: UT_Mutex.C
 */

#include <assert.h>

#include "UT_Mutex.h"

// We don't include the leakwatch here
// since this file is used by users

UT_Mutex::UT_Mutex(const char *name)
{
#ifdef WIN32
    myMutex = CreateMutex(NULL, FALSE, name);
#else
    assert(false);
#endif

}


UT_Mutex::~UT_Mutex()
{
#ifdef WIN32
    CloseHandle(myMutex);
#else
#endif 
}

bool
UT_Mutex::lock(int timeout)
{
#ifdef WIN32
    DWORD result = WaitForSingleObject(myMutex, timeout);
    if (result != WAIT_OBJECT_0)
        return false;
    else
        return true;
#else
    return false;
#endif 
}

bool
UT_Mutex::unlock()
{
#ifdef WIN32
    return ReleaseMutex(myMutex);
#else
    return false;
#endif 
}
