/**************************************************************************
***    
*** Copyright (c) 1995-2000 Regents of the University of California,
***               Andrew E. Caldwell, Andrew B. Kahng and Igor L. Markov
***
***  Contact author(s): abk@cs.ucla.edu, imarkov@cs.ucla.edu
***  Affiliation:   UCLA, Computer Science Department,
***                 Los Angeles, CA 90095-1596 USA
***
***  Permission is hereby granted, free of charge, to any person obtaining 
***  a copy of this software and associated documentation files (the
***  "Software"), to deal in the Software without restriction, including
***  without limitation 
***  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
***  and/or sell copies of the Software, and to permit persons to whom the 
***  Software is furnished to do so, subject to the following conditions:
***
***  The above copyright notice and this permission notice shall be included
***  in all copies or substantial portions of the Software.
***
*** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
*** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
*** OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
*** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
*** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
*** OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
*** THE USE OR OTHER DEALINGS IN THE SOFTWARE.
***
***
***************************************************************************/




// February 11, 1999   Mike Oliver  VLSI CAD UCLA ABKGROUP

// This file implements for MSVC++ certain string functions
// that are part of the system in UNIX.

#include "abkstring.h"

#ifdef WIN32
char *ulltostr(unsigned value,char *ptr)
    {
    if (value==0)
        {
        *ptr = '0';
        return ptr;
        }
    else
        {
        while (value>0)
            {
            *(ptr--) = '0'+value%10;
            value /= 10;
            }
        return ptr+1;
        }
    }

char *lltostr(int value,char *ptr)
    {
    if (value>=0)
        return ulltostr(unsigned(value),ptr);
    else
        {
        ptr = ulltostr(unsigned(-value),ptr)-1;
        *ptr = '-';
        return ptr;
        }
    }

#endif