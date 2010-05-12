#ifndef RSLICE_HH
#define RSLICE_HH
//
// Hold information about one RSlice in a calorimeter.
//

//
// $Id: RSlice.hh,v 1.2 2010/05/12 14:57:36 rhbob Exp $
// $Author: rhbob $ 
// $Date: 2010/05/12 14:57:36 $
//
// Original author R. Bernstein and Rob Kutschke
//

#include <vector>
#include <deque>

#include "CalorimeterGeom/inc/RSliceId.hh"
#include "CalorimeterGeom/inc/Crystal.hh"

#include "CLHEP/Vector/ThreeVector.h"

namespace mu2e {
  namespace calorimeter{
    class Calorimeter;
    class RSlice{

      friend class ZSlice;
      friend class Vane;
      friend class Calorimeter;
      friend class CalorimeterMaker;

    public:

      // A free function, returning void, that takes a const RSlice& as an argument.
      typedef void (*RSliceFunction)( const RSlice& s);

      RSlice();

      RSlice(const RSliceId& id);

      RSlice(const RSliceId&   id,
	     int        nCrystals,
	     const CLHEP::Hep3Vector& origin,
	     const CLHEP::Hep3Vector& delta
	     );

  
      ~RSlice (){};
 
      const RSliceId& Id() const { return _id;}
  
      int nCrystals() const { return _crystals.size(); }

      const CLHEP::Hep3Vector getOrigin() const {return _orig;}
      const CLHEP::Hep3Vector getDelta()  const {return _delta;}

      const Crystal& getCrystal( int n ) const {
	return *_crystals.at(n);
      }
  
      const Crystal& getCrystal( const CrystalId& id ) const {
	return getCrystal(id._n);
      }
  
      const std::vector<const Crystal*>& getCrystals() const { 
	return _crystals;
      }

      // Formatted string embedding the id of the layer.
      std::string name( std::string const& base ) const;

      // Return Id of the last crystal in the layer.
      // Return an illegal id if there are no crystals.
      //  - which should never happen.
      CrystalId getIdLastCrystal() const{

	return ( _crystals.size() != 0 )?
	  _crystals.back()->Id():
	  CrystalId();
      }
  
      // Compiler generated copy and assignment constructors
      // should be OK.

      // Options:
      // 1) return F or void; std-like says return F.
      //    Can be inefficient if F has a lot of state.
      //    THis is a copy in copy out model.
      // 2) Order is not defined.  In practice when run on
      //    vector it is defined.
      template <class F>
      void for_each( F f ) const {
	std::for_each( _crystals.begin(),
		       _crystals.end(),
		       f);
      }
  
      // Loop over all crystals and call F.
      // F can be a class with an operator() or a free function.
      template <class F>
      inline void RSlice::forAllCrystals ( F& f) const{
	for ( std::vector<const Crystal*>::const_iterator i=_crystals.begin(), e=_crystals.end();
	      i !=e; ++i){
	  f(**i);
	}
      }

    protected:

      RSliceId _id;

      // Number of crystals.  Needed because of 2 phase construction.
      // The member _crystals is not filled until the second phase
      // but this is neede beforehand. Keep it strictly private.
      int _nCrystals;


      // Nominal position of wire 0 and offset from wire 0 to wire 1.
      // This is exactly only all wires are in their nominal positions.
      CLHEP::Hep3Vector _orig;
      CLHEP::Hep3Vector _delta;

      // Pointers to the crystals in this layer.
      // These pointers do not own the crystals to which they point.
      std::vector<const Crystal*> _crystals;

      std::vector<CrystalIndex> _indices;

    };
  }// namespace calorimeter
} //namespace mu2e
#endif
