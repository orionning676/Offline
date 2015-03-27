//
// Class to find cluster of simply connected crystals
// 
// Original author B. Echenard
//

// Mu2e includes
#include "CaloCluster/inc/CaloClusterFinder.hh"
#include "CalorimeterGeom/inc/Calorimeter.hh"
#include "GeometryService/inc/GeomHandle.hh"
#include "RecoDataProducts/inc/CaloCrystalHitCollection.hh"

// C++ includes
#include <iostream>
#include <queue>
#include <list>
#include <unordered_map>
#include <algorithm>


namespace mu2e {


       CaloClusterFinder::CaloClusterFinder(Calorimeter const& cal, CaloCrystalHit const& crystalSeed, double deltaTimePlus, double deltaTimeMinus, double ExpandCut) : 
          _cal(&cal), _crystalSeed(&crystalSeed),_seedTime(_crystalSeed->time()), _clusterList(), _inspected(), _crystalToVisit(), _isVisited(cal.nCrystal()),
	  _deltaTimePlus(deltaTimePlus), _deltaTimeMinus(deltaTimeMinus), _ExpandCut(ExpandCut)
       {}
       
       


       void CaloClusterFinder::formCluster(std::vector<CaloCrystalVec>& idHitVec)  
       { 

	    _inspected.clear();
	    
	    _clusterList.push_front(_crystalSeed);
	    _crystalToVisit.push(_crystalSeed->id());  
            _inspected.push_front(_crystalSeed);


	    
	    CaloCrystalVec& vecSeed = idHitVec[_crystalSeed->id()];
	    for (unsigned int ivec=0; ivec < vecSeed.size(); ++ivec)
	       if (vecSeed[ivec]==_crystalSeed) {vecSeed[ivec]=0; break;}


	    while (!_crystalToVisit.empty())
	    {            
        	 int visitId         = _crystalToVisit.front();
		 _isVisited[visitId] = 1;

		 std::vector<int> const& neighborsId = _cal->neighbors(visitId);
         	 for (auto& iId : neighborsId)
        	 {               
		     if (_isVisited[iId]) continue;
		     _isVisited[iId]=1;

		     CaloCrystalVec& vec = idHitVec[iId];
		     for (unsigned int ivec=0; ivec < vec.size(); ++ivec)
		     {
			 if (vec[ivec]==0) continue;
			 CaloCrystalHit const* hit = vec[ivec];

			 if (hit->time() - _seedTime > _deltaTimePlus)  continue;
        		 if (_seedTime - hit->time() > _deltaTimeMinus) continue;

         		 if (hit->energyDep() > _ExpandCut) _crystalToVisit.push(iId);
                         _clusterList.push_front(hit);

 			 _inspected.push_front(hit);			 
        		 vec[ivec]=0;
		     } 
		     
		 }
		      
		 _crystalToVisit.pop();		 
	    }
	    
	   // keep sorting proto-clustres, even if they are sorted in the cluster module, in case somebody 
	   // uses the proto-clusters instead of clusters he will get the same behaviour
	   _clusterList.sort([] (CaloCrystalHit const* lhs, CaloCrystalHit const* rhs) {return lhs->energyDep() > rhs->energyDep();} );
	    	    
       } 

}