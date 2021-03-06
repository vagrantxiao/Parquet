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




#include <stdlib.h>

using namespace std;

namespace parquetfp
{
class Command_Line;

class AnalSolve
{
 private:
  Command_Line* _params;
  DB* _db;

  vector<double> _xloc;
  vector<double> _yloc;

 public:
  AnalSolve(){}
  AnalSolve(Command_Line* params, DB* db);
  ~AnalSolve(){}

  Point getOptLoc(int index, vector<double>& xloc, vector<double>& yloc);
                                                  //location of cells passed
  Point getDesignOptLoc();  //get the optimum location of the entire placement

  vector<double>& getXLocs()
   { return _xloc; }
  
  vector<double>& getYLocs()
   { return _yloc; }

  void solveSOR();  //uses the DB placements as initial solution
};
}
//using namespace parquetfp;
