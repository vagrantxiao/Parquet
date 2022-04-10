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



#include "SPeval.h"
#include "PlToSP.h"
using namespace parquetfp;

SPeval::SPeval(const vector<double> heights, const vector<double> widths)
{
  _heights=heights;
  _widths=widths;
  unsigned size = heights.size();
  _match.resize(size);
  _L.resize(size);
  _reverseSeq.resize(size);
  _X.resize(size);
  _Y.resize(size);
  xloc.resize(size);
  yloc.resize(size);
  xSlacks.resize(size);
  ySlacks.resize(size);
  xlocRev.resize(size);
  ylocRev.resize(size);
  _TCGMatrixInitialized = false;
}

void SPeval::_initializeTCGMatrix(unsigned size)
{
  _TCGMatrixVert.resize(size);
  _TCGMatrixHoriz.resize(size);
  for(unsigned i=0; i<size; ++i)
    {
      _TCGMatrixVert[i].resize(size);
      _TCGMatrixHoriz[i].resize(size);
    }
  _TCGMatrixInitialized = true;
}

double SPeval::_lcsCompute(const vector<unsigned>& X,
			   const vector<unsigned>& Y,
			   const vector<double>& weights,
			   vector<unsigned>& match,
			   vector<double>& P,
			   vector<double>& L
			   )
{
  unsigned size = X.size();
  for(unsigned i=0;i<size;++i)
    {
      match[Y[i]]=i;
      L[i]=0;
    }
  
  double t;
  unsigned j;
  for(unsigned i=0;i<size;++i)
    {
      unsigned p = match[X[i]];
      P[X[i]]=L[p];
      t = P[X[i]]+weights[X[i]];
      
      for(j=p;j<size;++j)
	{
	  if(t>L[j])
	    L[j]=t;
	  else
	    break;
	}
    }
  return L[size-1];
}


double SPeval::xEval()
{
  fill(_match.begin(),_match.end(),0);
  return _lcsCompute( _X, _Y, _widths, _match, xloc, _L);
}

double SPeval::yEval()
{
  _reverseSeq = _X;
  reverse(_reverseSeq.begin(),_reverseSeq.end());
  fill(_match.begin(),_match.end(),0);
  return _lcsCompute( _reverseSeq, _Y, _heights, _match, yloc, _L);
}

void SPeval::evaluate(vector<unsigned>& X, vector<unsigned>& Y)
{
  _X = X;
  _Y = Y;
  xSize = xEval();
  ySize = yEval();
}

void SPeval::evaluateCompact(vector<unsigned>& X, vector<unsigned>& Y,
			     bool whichDir)
{
  _X = X;
  _Y = Y;
  if(whichDir == 0)  //evaluate yloc first and then compact
    {
      ySize = yEval();
      xSize = xEvalCompact();
    }
  else               //evaluate xloc first and then compact
    {
      xSize = xEval();
      ySize = yEvalCompact();
    }
}

double SPeval::xEvalCompact()
{
  fill(_match.begin(),_match.end(),0);
  return _lcsComputeCompact( _X, _Y, _widths, _match, xloc, _L, yloc, 
			     _heights);
}

double SPeval::yEvalCompact()
{
  _reverseSeq = _X;
  reverse(_reverseSeq.begin(),_reverseSeq.end());
  fill(_match.begin(),_match.end(),0);
  return _lcsComputeCompact( _reverseSeq, _Y, _heights, _match, yloc, _L, 
			     xloc, _widths);
}

double SPeval::_lcsComputeCompact(const vector<unsigned>& X,
				  const vector<unsigned>& Y,
				  const vector<double>& weights,
				  vector<unsigned>& match,
				  vector<double>& P,
				  vector<double>& L,
				  vector<double>& oppLocs,
				  vector<double>& oppWeights
				  )
{
  double finalSize = -1e100;
  unsigned size = X.size();
  for(unsigned i=0;i<size;++i)
    {
      match[Y[i]]=i;
      L[i]=0;
    }
  
  double t;
  unsigned j;
  for(unsigned i=0;i<size;++i)
    {
      unsigned p = match[X[i]];
      P[X[i]]=L[p];
      t = P[X[i]]+weights[X[i]];
      
      double iStart = oppLocs[X[i]];
      double iEnd = oppLocs[X[i]] + oppWeights[X[i]];

      for(j=p;j<size;++j)
	{
	  double jStart = oppLocs[Y[j]];
	  double jEnd = oppLocs[Y[j]] + oppWeights[Y[j]];
	  
	  if(iStart >= jEnd || iEnd <= jStart) //no constraint
	    continue;
	  
	  if(t>L[j])
	    {
	      L[j]=t;
	      if(t > finalSize)
		finalSize = t;
	    }
	}
    }
  return finalSize;
}

void SPeval::computeConstraintGraphs()
{
  unsigned size = _X.size();
  if(!_TCGMatrixInitialized)
    _initializeTCGMatrix(size);

  vector<unsigned> matchX;
  vector<unsigned> matchY;
  matchX.resize(size);
  matchY.resize(size);

  for(unsigned i=0;i<size;++i)
    {
      matchX[_X[i]]=i;
      matchY[_Y[i]]=i;
    }

  for(unsigned i=0;i<size;++i)
    {
      for(unsigned j=0; j<size; ++j)
	{
	  if(i==j)
	    {
	      _TCGMatrixHoriz[i][j] = 1;
	      _TCGMatrixVert[i][j] = 1;
	      continue;
	    }

	  _TCGMatrixHoriz[i][j] = 0;
	  _TCGMatrixVert[i][j] = 0;
	  _TCGMatrixHoriz[j][i] = 0;
	  _TCGMatrixVert[j][i] = 0;

	  
	  if(matchX[i] < matchX[j] && matchY[i] < matchY[j])
	    _TCGMatrixHoriz[i][j] = 1;
	  else if(matchX[i] > matchX[j] && matchY[i] > matchY[j])
	    _TCGMatrixHoriz[j][i] = 1;
	  else if(matchX[i] < matchX[j] && matchY[i] > matchY[j])
	    _TCGMatrixVert[j][i] = 1;
	  else if(matchX[i] > matchX[j] && matchY[i] < matchY[j])
	    _TCGMatrixVert[i][j] = 1;
	  else
	    cout<<"ERROR: in computeConstraintGraph \n";
	}
    }
}

void SPeval::removeRedundantConstraints(bool knownDir)
{
  unsigned size = _X.size();
  double iStart, iEnd, jStart, jEnd;
  for(unsigned i=0; i<size; ++i)
    {
      if(knownDir == 0) //horizontal
	{
	  iStart = xloc[i];
	  iEnd = iStart+_widths[i];
	}
      else  //vertical
	{
	  iStart = yloc[i];
	  iEnd = iStart+_heights[i];
	}
      for(unsigned j=0; j<size; ++j)
	{
	  if(i == j)
	    continue;

	  if(knownDir == 0)
	    {
	      jStart = xloc[j];
	      jEnd = jStart+_widths[j];
	    }
	  else
	    {
	      jStart = yloc[j];
	      jEnd = jStart+_heights[j];
	    }
	  
	  if(knownDir == 0)
	    {
	      if(_TCGMatrixVert[i][j] == 1)
		{
		  if(iStart >= jEnd || iEnd <= jStart) //no constraint
		    {
		      _TCGMatrixVert[i][j] = 0;
		      if(iStart < jStart)
			_TCGMatrixHoriz[i][j] = 1;
		      else
			_TCGMatrixHoriz[j][i] = 1;
		    }
		}
	    }
	  else
	    {
	      if(_TCGMatrixHoriz[i][j] == 1)
		{
		  if(iStart >= jEnd || iEnd <= jStart) //no constraint
		    {
		      cout<<i<<"\t"<<j<<"\t"<<iStart<<"\t"<<iEnd<<"\t"<<
			jStart<<"\t"<<jEnd<<endl;
		      _TCGMatrixHoriz[i][j] = 0;
		      if(iStart < jStart)
			_TCGMatrixVert[i][j] = 1;
		      else
			_TCGMatrixVert[j][i] = 1;
		    }
		}
	    }
	}
    }
}

void SPeval::computeSPFromCG()
{
  unsigned size = _X.size();
  for(unsigned i=0; i<size; ++i)
    {
      _X[i] = i;
      _Y[i] = i;
    }
  
  SPXRelation SPX(_TCGMatrixHoriz, _TCGMatrixVert);
  SPYRelation SPY(_TCGMatrixHoriz, _TCGMatrixVert);

  std::sort(_X.begin(), _X.end(), SPX);
  std::sort(_Y.begin(), _Y.end(), SPY);
}

void SPeval::changeWidths(vector<double>& widths)
{
  _widths = widths;
}

void SPeval::changeHeights(vector<double>& heights)
{
  _heights = heights;
}

void SPeval::changeNodeWidth(unsigned index, double width)
{
  _widths[index] = width;
}

void SPeval::changeNodeHeight(unsigned index, double height)
{
  _heights[index] = height;
}

void SPeval::changeOrient(unsigned index)
{
  double tempWidth = _heights[index];
  _heights[index] = _widths[index];
  _widths[index] = tempWidth;
}





double SPeval::_lcsReverseCompute(const vector<unsigned>& X,
				  const vector<unsigned>& Y,
				  const vector<double>& weights,
				  vector<unsigned>& match,
				  vector<double>& P,
				  vector<double>& L
				  )
{
  unsigned size = X.size();
  for(unsigned i=0;i<size;++i)
    {
      match[Y[i]]=i;
      L[i]=0;
    }
  
  double t;
  int j;
  for(int i=size-1;i>=0;--i)
    {
      unsigned p = match[X[i]];
      P[X[i]]=L[p];
      t = P[X[i]]+weights[X[i]];
      
      for(j=p;j>=0;--j)
	{
	  if(t>L[j])
	    L[j]=t;
	  else
	    break;
	}
    }
  return L[0];
}


double SPeval::xEvalRev()
{
  fill(_match.begin(),_match.end(),0);
  return _lcsReverseCompute( _X, _Y, _widths, _match, xlocRev, _L);
}

double SPeval::yEvalRev()
{
  _reverseSeq = _X;
  reverse(_reverseSeq.begin(),_reverseSeq.end());
  fill(_match.begin(),_match.end(),0);
  return _lcsReverseCompute( _reverseSeq, _Y, _heights, _match, ylocRev, _L);
}

void SPeval::evalSlacks(vector<unsigned>& X, vector<unsigned>& Y)
{
  _X = X;
  _Y = Y;
  xSize = xEval();
  ySize = yEval();
  xEvalRev();
  yEvalRev();

  for(unsigned i=0; i<_X.size(); ++i)
    {
      xlocRev[i] = xSize - xlocRev[i] - _widths[i];
      ylocRev[i] = ySize - ylocRev[i] - _heights[i];
      xSlacks[i] = (xlocRev[i] - xloc[i])*100/xSize;
      ySlacks[i] = (ylocRev[i] - yloc[i])*100/ySize;
    }

}
