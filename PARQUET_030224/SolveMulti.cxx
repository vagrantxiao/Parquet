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



#include "FPcommon.h"
#include "Annealer.h"
#include "ClusterDB.h"
#include "CommandLine.h"
#include "SolveMulti.h"
using namespace parquetfp;

SolveMulti::SolveMulti(DB * db, Command_Line* params)
{
  _db = db;
  _params = params;
  _newDB = new DB();
}

SolveMulti::~SolveMulti()
{
  if(_newDB) delete _newDB;
}

void SolveMulti::go(void)
{
  ClusterDB multiCluster(_db, _params);

  Timer T;
  T.stop();
  double totalTime=0;
  T.start(0.0);

  if(_params->clusterPhysical)
    multiCluster.clusterMultiPhysical(_newDB);
  else
    multiCluster.clusterMulti(_newDB);

  T.stop();
  totalTime += T.getUserTime();
  cout<<"Clustering took "<<totalTime<<" seconds "<<endl;

  cout<<"Num Nodes: "<<_newDB->getNumNodes()<<"  Num Nets: "
      <<_newDB->getNets()->getNumNets()<<"  Num Pins: "
      <<_newDB->getNets()->getNumPins()<<endl;
  
  /*  _newDB->plot("cluster.gpl", _newDB->evalArea(), 
	100*(_newDB->evalArea()-_newDB->getNodesArea())/_newDB->getNodesArea(),
	       _newDB->getXSize()/_newDB->getYSize(), 0, _newDB->evalHPWL(), 
	       0, 1, 0);
  */
  double reqdWidth = 1e100;
  double reqdHeight = 1e100;
  double maxArea = (1+_params->maxWS/100)*_newDB->getNodesArea();

  if(_params->reqdAR != -9999)
    {
      reqdHeight = sqrt(maxArea/_params->reqdAR);
      reqdWidth = maxArea/reqdHeight;
    }
  double currXSize, currYSize;
  unsigned maxIter = 0;
  Annealer annealer(_params, _newDB);
  if(_params->takePl)
    {
      //convert placement to sequence pair
      annealer.takeSPfromDB();
      annealer.eval();
    }

  if(_params->initCompact)
    {
      double currArea, lastArea;
      annealer.eval();
      currArea = annealer.getXSize()*annealer.getYSize();
      bool whichDir = 0;
      annealer.evalCompact(whichDir);
      do
	{
	  whichDir = !whichDir;
	  lastArea = currArea;
	  annealer.takeSPfromDB();
	  annealer.evalCompact(whichDir);
	  currArea = annealer.getXSize()*annealer.getYSize();
	}
      while(int(currArea) < int(lastArea));

      //_newDB->plot("out.gpl", currArea, 0, 0, 0, 0, 0, 0, 0);
    }

  if(_params->initQP)
    {
      annealer.solveQP();
      annealer.takeSPfromDB();
    }

  do
    {
      annealer.go();
      currXSize = annealer.getXSize();
      currYSize = annealer.getYSize();
      if(currXSize<=reqdWidth && currYSize<=reqdHeight)
	break;
      
      maxIter++;
      if(maxIter == 50)
	break;
    }
  while(1);

  updatePlaceUnCluster(_newDB);

  if(!_params->solveTop)
    placeSubBlocks();

  _newDB->plot("main.gpl", _newDB->evalArea(), 
	100*(_newDB->evalArea()-_newDB->getNodesArea())/_newDB->getNodesArea(),
	       _newDB->getXSize()/_newDB->getYSize(), 0, _newDB->evalHPWL(), 
	       0, 0, 1);

  _db->plot("final.gpl", _db->evalArea(), 
	    100*(_db->evalArea()-_db->getNodesArea())/_db->getNodesArea(),
	       _db->getXSize()/_db->getYSize(), 0, _db->evalHPWL(), 
	       0, 0, 0);
}

void SolveMulti::placeSubBlocks(void)
{
  Nodes* nodes = _newDB->getNodes();
  Nodes* origNodes = _db->getNodes();

  itNode node;

  Command_Line* params = new Command_Line(*_params);
  params->budgetTime = 0;

  for(node=nodes->nodesBegin(); node!=nodes->nodesEnd(); ++node)
    {
      Point dbLoc;
      dbLoc.x = node->getX();
      dbLoc.y = node->getY();
      params->reqdAR = node->getWidth()/node->getHeight();

      DB * tempDB = new DB(_db, node->getSubBlocks(), dbLoc, params->reqdAR);

      cout<<node->getName()<<"\t"<<node->numSubBlocks()<<endl;

      Annealer annealer(params, tempDB);
      
      double currXSize, currYSize;
      double reqdWidth = node->getWidth();
      double reqdHeight = node->getHeight();

      unsigned maxIter = 0;
      do
	{
	  annealer.go();
	  currXSize = annealer.getXSize();
	  currYSize = annealer.getYSize();
	  if(currXSize<=reqdWidth && currYSize<=reqdHeight)
	    break;

	  maxIter++;
	  if(maxIter == 50)
	    break;
	}
      while(1);

      Point offset;
      offset.x = node->getX();
      offset.y = node->getY();
      
      tempDB->shiftDesign(offset);
 
      Nodes * tempNodes = tempDB->getNodes();

      for(itNode tempNode = tempNodes->nodesBegin(); 
	  tempNode != tempNodes->nodesEnd(); ++tempNode)
	{
	  for(vector<int>::iterator tempIdx = tempNode->subBlocksBegin();
	      tempIdx != tempNode->subBlocksEnd(); ++tempIdx)
	    {
	      Node& origNode = origNodes->getNode(*tempIdx);
	      origNode.putX(tempNode->getX());
	      origNode.putY(tempNode->getY());
	      origNode.changeOrient(tempNode->getOrient(), *(_db->getNets()));
	      origNode.putHeight(tempNode->getHeight());
	      origNode.putWidth(tempNode->getWidth());
	    }
	}
      delete tempDB;
    }
}

void SolveMulti::updatePlaceUnCluster(DB * clusterDB)
{
  Nodes* nodes = _db->getNodes();
  Nodes* newNodes = clusterDB->getNodes();

  itNode node;

  for(node = newNodes->nodesBegin(); node != newNodes->nodesEnd(); ++node)
    {
      for(vector<int>::iterator subBlockIdx = node->subBlocksBegin(); 
	  subBlockIdx != node->subBlocksEnd(); ++subBlockIdx)
	{
	  Node& tempNode = nodes->getNode(*subBlockIdx);
	  tempNode.putX(node->getX());
	  tempNode.putY(node->getY());
	  tempNode.changeOrient(node->getOrient(), *(_db->getNets()));
	}
      if(node->numSubBlocks() == 1)  //the only block
	{
	  vector<int>::iterator subBlockIdx = node->subBlocksBegin();
	  Node& tempNode = nodes->getNode(*subBlockIdx);
	  tempNode.putHeight(node->getHeight());
	  tempNode.putWidth(node->getWidth());
	}
    }
}
