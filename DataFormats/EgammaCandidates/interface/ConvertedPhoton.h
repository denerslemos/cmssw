#ifndef EgammaCandidates_ConvertedPhoton_h
#define EgammaCandidates_ConvertedPhoton_h
/** \class reco::ConvertedPhoton ConvertedPhoton.h DataFormats/EgammaCandidates/interface/ConvertedPhoton.h
 *
 * Reco Candidates with an ConvertedPhoton component
 *
 * \author N.Marinelli  University of Notre Dame, US
 *
 * \version $Id: ConvertedPhoton.h,v 1.14 2007/09/17 18:28:22 nancy Exp $
 *
 */
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h" 
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h" 
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
namespace reco {
    class ConvertedPhoton : public RecoCandidate {
  public:
    /// default constructor
    ConvertedPhoton() : RecoCandidate() { }
  

    ConvertedPhoton( const reco::SuperClusterRef sc, 
                     const std::vector<reco::TrackRef> tr, 
                     Charge q, const LorentzVector & p4, double r9,
		     const std::vector<math::XYZPoint> trackPositionAtEcal , 
		     const reco::Vertex  &  convVtx,
		     const std::vector<reco::BasicCluster> & matchingBC, 
		     const Point & vtx);



    /// destructor
    virtual ~ConvertedPhoton();
    /// returns a clone of the candidate
    ConvertedPhoton * clone() const;
    /// reference to a SuperCluster
    reco::SuperClusterRef superCluster() const ;

    /// vector of references to  tracks
    std::vector<reco::TrackRef> tracks() const ; 
    /// reference to one of multiple Tracks: implements the method inherited from RecoCandidate
    reco::TrackRef track( size_t ) const;
    /// Bool flagging objects having track size >0
    bool isConverted() const;
    /// Number of tracks= 0,1,2
    unsigned int nTracks() const {return  tracks().size(); }
    /// if nTracks=2 returns the pair invariant mass
    double pairInvariantMass() const {return invMass_;}
    /// Delta cot(Theta) where Theta is the angle in the (y,z) plane between the two tracks 
    double pairCotThetaSeparation() const {return dCotTheta_;}
    /// Conversion tracks momentum 
    GlobalVector  pairMomentum() const {return momTracks_;}
    /// Phi  
    double pairMomentumPhi() const {return  phiTracks_;}
    /// Eta 
    double pairMomentumEta() const {return etaTracks_;}
    /// Pt from tracks divided by the super cluster transverse energy
    double pairPtOverEtSC() const {return ptOverEtSC_;}
    /// Super Cluster energy divided by tracks momentum
    double EoverP() const {return ep_;}
    /// ratio of E(3x3)/ESC
    double r9() const { return r9_; }
    /// returns  the reco conversion vertex
    reco::Vertex const & conversionVertex()  { return theConversionVertex_ ; }
     /// positions of the track extrapolation at the ECAL front face
    std::vector<math::XYZPoint> const & ecalImpactPosition()  {return thePositionAtEcal_;}
    //  pair of BC matching the tracks
    const std::vector<reco::BasicCluster>&  bcMatchingWithTracks() const { return theMatchingBCs_;}
    /// set primary event vertex used to define photon direction
    double zOfPrimaryVertexFromTracks() const {return theZOfPrimaryVertexFromTracks_;}
    void setVertex(const Point & vertex);


  private:
    /// check overlap with another candidate
    virtual bool overlap( const Candidate & ) const;
   

    /// reference to a SuperCluster
    reco::SuperClusterRef superCluster_;
    /// reference to a vector Track references
    std::vector<reco::TrackRef>  tracks_;
    double r9_;
    std::vector<math::XYZPoint>  thePositionAtEcal_;
    reco::Vertex theConversionVertex_;
    std::vector<reco::BasicCluster> theMatchingBCs_;



    void makePairInvariantMass() ;
    void makePairCotThetaSeparation();
    void makePairMomentum() ;
    void makePairMomentumEta() ;
    void makePairMomentumPhi() ;
    void makePairPtOverEtSC() ;
    void makeEoverP() ;
    void makePrimaryVertexZ();

    double invMass_;
    double dCotTheta_;
    double etaTracks_;
    double phiTracks_;
    GlobalVector  momTracks_;
    double ptOverEtSC_;
    double ep_;
    double theZOfPrimaryVertexFromTracks_;


  };
  
}

#endif
