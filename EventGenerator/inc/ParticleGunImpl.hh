#ifndef EventGenerator_ParticleGunImpl_hh
#define EventGenerator_ParticleGunImpl_hh
//
// Shoots a single particle gun and puts its output into a generated event.
// This class implements the common code for "particle gun-like" generators.
//
// $Id: ParticleGunImpl.hh,v 1.2 2012/02/17 18:31:47 mu2ecvs Exp $
// $Author: mu2ecvs $
// $Date: 2012/02/17 18:31:47 $
//
// Original author Rob Kutschke, re-factored for use in multiple generators by Andrei Gaponenko.
// Introduced new features by mjlee. See docdb-2049
//
// The position is given in the Mu2e coordinate system.
//

// Mu2e includes
#include "EventGenerator/inc/GeneratorBase.hh"
#include "MCDataProducts/inc/PDGCode.hh"
#include "Mu2eUtilities/inc/RandomUnitSphere.hh"

#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandPoissonQ.h"
#include "CLHEP/Random/RandGaussQ.h"
#include "CLHEP/Random/RandExponential.h"
#include "CLHEP/Vector/ThreeVector.h"

#include <string>

class TH1F;
class TNtuple;

namespace mu2e {

  // Forward reference.
  class RandomUnitSphereParams;

  class ParticleGunImpl: public GeneratorBase{

  public:
    ParticleGunImpl(double meanMultiplicity,
        PDGCode::type pdgId,
        double pmin,
        double pmax,
        const std::string&    momentumModeString,
        const RandomUnitSphereParams& angles,
        double tmin,
        double tmax,

        const CLHEP::Hep3Vector& point,
        const CLHEP::Hep3Vector& halfLength,
        const std::string& sourceShapeString,

        int   iterationLimit,
        bool  throwOnIterationLimit,

        // empty string "" means don't histogram
        const std::string& histoDir,
        bool  doNtuples,

        bool verbose);

    // Old style constructor. Kept for backward compatibility.
    ParticleGunImpl(double meanMultiplicity,
        PDGCode::type pdgId,
        double pmin,
        double pmax,
        const RandomUnitSphereParams& angles,
        double tmin,
        double tmax,

        const CLHEP::Hep3Vector& point,
        const CLHEP::Hep3Vector& halfLength,

        // empty string "" means don't histogram
        const std::string& histoDir,

        bool verbose);



    virtual void generate( GenParticleCollection&  );
    void setParameters( std::vector<double>& momentumParameters) ;

  private:
    // Disallow copying because of the owned pointer members
    ParticleGunImpl(const ParticleGunImpl&);
    ParticleGunImpl& operator=(const ParticleGunImpl&);

    void initialize (
        const std::string& momentumModeString,
        const std::string& sourceShapeString);

    double getFlatMomentum (void) ;
    double getGaussianMomentum (void) ;
    double getGenericMomentum (void) ;
    double getExponentialMomentum (void) ;


    // Start: Information from the run time configuration.

    // Number of particles per event.
    // If positive, mean of a Poisson distribution.
    // If negative, then exactly that number of particles per event.
    double _mean;

    // PDG particle id code of the particle to be generated.
    PDGCode::type _pdgId;

    // Momentum range of the particle.  Units are MeV.
    double _pmin;
    double _pmax;

    // Momentum distribution mode.
    enum momentumMode {
      momentum_undefined = -1,
      flat = 0,
      gaussian = 1,
      generic = 2,
      exponential = 3
    }  _momentumMode;

    // Parameters for momentum distribution.
    // ex. if pmomentumMode = gaussian, [0] = mean and [1] = stdDev.
    // not used for flat dist
    std::vector<double> _momentumParameters;

    // Time range over which the particle will be produced; units are ns.
    double _tmin;
    double _tmax;

    // Particles will be produced in a box, specified by
    // a point in the Tracker coordinate system and
    // the half lengths of the box.  Units are mm.
    // The point (0,0,0) is at the origin of the Mu2e coordinate system.
    CLHEP::Hep3Vector _point;
    CLHEP::Hep3Vector _halfLength;

    // source distribution shape
    enum sourceShape {
      shape_undefined  = -1,
      box = 0,
      cylinder_x = 1,
      cylinder_y = 2,
      cylinder_z = 3,
      sphere = 4
    } _sourceShape;

    // Limit of generation iteration. To check the limit of momentum or the shaped vertex distribution,
    // pass-and-fail algorithm is applied. This config limits the maximum iteration for pass-and fail.
    // Generation will crash at any point of excess of iterationLimit when throwOnIterationLimit = true. 
    // Else, it will just print warning. (Default is false)
    // Default : 100
    int _iterationLimit;
    bool _throwOnIterationLimit;

    // Enable ntuple of generated particles. Effective only when doHistogram is true.
    bool _doNtuples;

    // enable output
    bool _verbose;
    // Enable histograms
    bool _doHistograms;
    std::string _histoDir;

    // End: Information from the run time configuration.

    // Random number distributions.
    CLHEP::RandFlat     _randFlat;
    CLHEP::RandPoissonQ _randPoissonQ;
    CLHEP::RandGaussQ   _randGaussQ;
    CLHEP::RandExponential _randExponential;
    RandomUnitSphere    _randomUnitSphere;

    // Derived information.

    // Mass of the particle to be generated.
    double _mass;

    // Ranges of momentum and time.
    double _dp, _dt;

    // halfLength square
    double _dx2, _dy2, _dz2;

    // Histogram information.
    TH1F* _hMultiplicity;
    TH1F* _hMomentum;
    TH1F* _hCz;
    TH1F* _hX0;
    TH1F* _hY0;
    TH1F* _hZ0;
    TH1F* _hT0;
    TNtuple* _hGenerationNtuple;


  };

} // end namespace mu2e,

#endif /* EventGenerator_ParticleGunImpl_hh */
