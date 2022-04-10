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



#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
using namespace std;

namespace parquetfp
{
class SPeval
{
 private:
  //buffers go in here
  vector<unsigned> _match;
  vector<double> _L;
  vector<unsigned> _reverseSeq;
  vector<unsigned> _X;
  vector<unsigned> _Y;
  vector<double> _heights;
  vector<double> _widths;

  vector< vector<bool> > _TCGMatrixHoriz;
  vector< vector<bool> > _TCGMatrixVert;

  double _lcsCompute(const vector<unsigned>& X,
		     const vector<unsigned>& Y,
		     const vector<double>& weights,
		     vector<unsigned>& match,
		     vector<double>& P,
		     vector<double>& L
		     );

  double _lcsReverseCompute(const vector<unsigned>& X,
			    const vector<unsigned>& Y,
			    const vector<double>& weights,
			    vector<unsigned>& match,
			    vector<double>& P,
			    vector<double>& L
			    );
  
  double _lcsComputeCompact(const vector<unsigned>& X,
			    const vector<unsigned>& Y,
			    const vector<double>& weights,
			    vector<unsigned>& match,
			    vector<double>& P,
			    vector<double>& L,
			    vector<double>& oppLocs,
			    vector<double>& oppWeights
			    );
  
  bool _TCGMatrixInitialized;
  void _initializeTCGMatrix(unsigned size);

 public:
  vector<double> xloc;
  vector<double> yloc;
  double xSize;
  double ySize;
  vector<double> xSlacks;
  vector<double> ySlacks;
  vector<double> xlocRev;
  vector<double> ylocRev;


  SPeval(const vector<double> heights, const vector<double> widths);

  
  void evaluate(vector<unsigned>& X, vector<unsigned>& Y);
  double xEval();
  double yEval();
  void evalSlacks(vector<unsigned>& X, vector<unsigned>& Y);
  double xEvalRev();
  double yEvalRev();

  void evaluateCompact(vector<unsigned>& X, vector<unsigned>& Y, 
		       bool whichDir);
  double xEvalCompact();
  double yEvalCompact();
  void computeConstraintGraphs();
  void removeRedundantConstraints(bool knownDir);
  void computeSPFromCG();

  void changeWidths(vector<double>& widths);
  void changeHeights(vector<double>& heights);
  void changeNodeWidth(unsigned index, double width);
  void changeNodeHeight(unsigned index, double height);
  void changeOrient(unsigned index);
};
}
//using namespace parquetfp;
