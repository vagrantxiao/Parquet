/**************************************************************************
***    
*** Copyright (c) 1995-2000 Regents of the University of Michigan,
***               Saurabh Adya and Igor L. Markov
***
***  Contact author(s): sadya@eecs.umich.edu, imarkov@eecs.umich.edu
***  Original Affiliation:   UoM, Electrical Engineering and Computer Science 
			     Department, Ann Arbor, MI 48109 USA
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



#include <vector>
#include <algorithm>
#include <math.h>
#include <stdlib.h>

using namespace std;

namespace parquetfp
{
class DB;

class SolveMulti
{
 private:
  
  DB * _db;
  Command_Line* _params;
  DB * _newDB;

 public:
  SolveMulti(DB * db, Command_Line* params);
  ~SolveMulti();
  void go(void);
  void placeSubBlocks(void);
  void updatePlaceUnCluster(DB * clusterDB);

};
}
//using namespace parquetfp;
