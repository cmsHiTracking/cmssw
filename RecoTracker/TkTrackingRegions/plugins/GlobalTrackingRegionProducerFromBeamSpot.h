#ifndef RecoTracker_TkTrackingRegions_GlobalTrackingRegionProducerFromBeamSpot_H
#define RecoTracker_TkTrackingRegions_GlobalTrackingRegionProducerFromBeamSpot_H

#include "RecoTracker/TkTrackingRegions/interface/TrackingRegionProducer.h"
#include "RecoTracker/TkTrackingRegions/interface/GlobalTrackingRegion.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include <iostream>

class GlobalTrackingRegionProducerFromBeamSpot : public TrackingRegionProducer {

public:

  GlobalTrackingRegionProducerFromBeamSpot(const edm::ParameterSet& cfg,
	   edm::ConsumesCollector && iC):
    GlobalTrackingRegionProducerFromBeamSpot(cfg, iC)
  {}
  GlobalTrackingRegionProducerFromBeamSpot(const edm::ParameterSet& cfg,
	   edm::ConsumesCollector & iC) {

    edm::ParameterSet regionPSet = cfg.getParameter<edm::ParameterSet>("RegionPSet");
    thePtMin            = regionPSet.getParameter<double>("ptMin");
    theOriginRadius     = regionPSet.getParameter<double>("originRadius");
    if (!regionPSet.existsAs<double>("nSigmaZ") && !regionPSet.existsAs<double>("originHalfLength")) {
        throw cms::Exception("Configuration") << "GlobalTrackingRegionProducerFromBeamSpot: at least one of nSigmaZ, originHalfLength must be present in the cfg.\n";
    }
    theNSigmaZ          = (regionPSet.existsAs<double>("nSigmaZ")          ? regionPSet.getParameter<double>("nSigmaZ")          : 0.0);
    theOriginHalfLength = (regionPSet.existsAs<double>("originHalfLength") ? regionPSet.getParameter<double>("originHalfLength") : 0.0);
    token_beamSpot      = iC.consumes<reco::BeamSpot>(regionPSet.getParameter<edm::InputTag>("beamSpot"));
    thePrecise          = regionPSet.getParameter<bool>("precise");
    theUseMS            = (regionPSet.existsAs<bool>("useMultipleScattering") ? regionPSet.getParameter<bool>("useMultipleScattering") : false);
    theOriginRScaling   = (regionPSet.existsAs<bool>("originRScaling4BigEvts") ? regionPSet.getParameter<bool>("originRScaling4BigEvts") : false);
    thePtMinScaling   = (regionPSet.existsAs<bool>("ptMinScaling4BigEvts") ? regionPSet.getParameter<bool>("ptMinScaling4BigEvts") : false);
    theHalfLengthScaling   = (regionPSet.existsAs<bool>("halfLengthScaling4BigEvts") ? regionPSet.getParameter<bool>("halfLengthScaling4BigEvts") : false);
    theMinOriginR       = (regionPSet.existsAs<double>("minOriginR") ? regionPSet.getParameter<double>("minOriginR") : 0.0);
    theMaxPtMin         = (regionPSet.existsAs<double>("maxPtMin") ? regionPSet.getParameter<double>("maxPtMin") : 1000.0);
    theMinHalfLength    = (regionPSet.existsAs<double>("minHalfLength") ? regionPSet.getParameter<double>("minHalfLength") : 0.0);
    theScalingStart     = (regionPSet.existsAs<double>("scalingStartNPix") ? regionPSet.getParameter<double>("scalingStartNPix") : 0.0);
    theScalingEnd       = (regionPSet.existsAs<double>("scalingEndNPix") ? regionPSet.getParameter<double>("scalingEndNPix") : 1.0);
    //pixelClusterCollectionInputTag_ = (regionPSet.existsAs<edm::InputTag>("pixelClustersForScaling") ? regionPSet.getParameter<edm::InputTag>("PixelClusterCollectionLabel") : "siPixelClusters"; 
    if(theOriginRScaling || thePtMinScaling || theHalfLengthScaling) token_pc = iC.consumes<edmNew::DetSetVector<SiPixelCluster> >(edm::InputTag("siPixelClusters"));
  }

  virtual ~GlobalTrackingRegionProducerFromBeamSpot(){}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    {
      edm::ParameterSetDescription desc;

      desc.add<bool>("precise", true);
      desc.add<bool>("useMultipleScattering", false);
      desc.add<bool>("originRScaling4BigEvts",false);
      desc.add<bool>("ptMinScaling4BigEvts",false);
      desc.add<bool>("halfLengthScaling4BigEvts",false);
      desc.add<double>("nSigmaZ", 4.0);
      desc.add<double>("originHalfLength", 0.0); // this is the default in constructor
      desc.add<double>("originRadius", 0.2);
      desc.add<double>("ptMin", 0.9);
      desc.add<double>("minOriginR",0);
      desc.add<double>("maxPtMin",1000);
      desc.add<double>("minHalfLength",0);
      desc.add<double>("scalingStartNPix",0.0);
      desc.add<double>("scalingEndNPix",1.0);
      desc.add<edm::InputTag>("beamSpot", edm::InputTag("offlineBeamSpot"));

      // Only for backwards-compatibility
      edm::ParameterSetDescription descRegion;
      descRegion.add<edm::ParameterSetDescription>("RegionPSet", desc);

      descriptions.add("globalTrackingRegionFromBeamSpot", descRegion);
    }

    {
      edm::ParameterSetDescription desc;

      desc.add<bool>("precise", true);
      desc.add<bool>("useMultipleScattering", false);
      desc.add<bool>("originRScaling4BigEvts",false);
      desc.add<bool>("ptMinScaling4BigEvts",false);
      desc.add<bool>("halfLengthScaling4BigEvts",false);
      desc.add<double>("nSigmaZ", 0.0); // this is the default in constructor
      desc.add<double>("originHalfLength", 21.2);
      desc.add<double>("originRadius", 0.2);
      desc.add<double>("ptMin", 0.9);
      desc.add<double>("minOriginR",0);
      desc.add<double>("maxPtMin",1000);
      desc.add<double>("minHalfLength",0);
      desc.add<double>("scalingStartNPix",0.0);
      desc.add<double>("scalingEndNPix",1.0);
      desc.add<edm::InputTag>("beamSpot", edm::InputTag("offlineBeamSpot"));

      // Only for backwards-compatibility
      edm::ParameterSetDescription descRegion;
      descRegion.add<edm::ParameterSetDescription>("RegionPSet", desc);

      descriptions.add("globalTrackingRegionFromBeamSpotFixedZ", descRegion);
    }
  }

  virtual std::vector<std::unique_ptr<TrackingRegion> > regions(const edm::Event&ev, const edm::EventSetup&) const override {
    std::vector<std::unique_ptr<TrackingRegion> > result;
    edm::Handle<reco::BeamSpot> bsHandle;
    ev.getByToken( token_beamSpot, bsHandle);
    if(bsHandle.isValid()) {

      const reco::BeamSpot & bs = *bsHandle; 

      GlobalPoint origin(bs.x0(), bs.y0(), bs.z0()); 

      //Uses a linear scaling of the Origin Radius/halflength/min pt based on number of pixels hit in order to reduce Tracking timing for high-occupancy Heavy Ion events
      if(theOriginRScaling || thePtMinScaling || theHalfLengthScaling){
        double scaledOriginRadius = theOriginRadius;
        double scaledHalfLength   = theOriginHalfLength;
        double scaledPtMin        = thePtMin;
        //calculate nPixels (adapted from TkSeedGenerator/src/ClusterChecker.cc (is there a cleaner way to doing this?)
        //does not ignore detectors above some nHit cut in order to be conservative on the nPix calculation
        double nPix = 0;
        edm::Handle<edmNew::DetSetVector<SiPixelCluster> > pixelClusterDSV;
        ev.getByToken(token_pc, pixelClusterDSV);
        if (!pixelClusterDSV.failedToGet()) {
          const edmNew::DetSetVector<SiPixelCluster> & input = *pixelClusterDSV;
          nPix = input.dataSize();
        }
        else{
          edm::LogError("GlobalTrackingRegionProducerFromBeamSpot")<<"could not get any SiPixel cluster collections of type edm::DetSetVector<SiPixelCluster>";
          nPix = theScalingEnd+1;//ensures the minimum radius is used below 
        } 

        if((nPix > theScalingEnd) || ((theScalingEnd-theScalingStart) <= 0)){//first condition is for high occupancy, second makes sure we won't divide by zero or a negative number
          if(theOriginRScaling)    scaledOriginRadius = theMinOriginR;   // sets radius to minimum value from PSet
          if(theHalfLengthScaling) scaledHalfLength = theMinHalfLength;
          if(thePtMinScaling)      scaledPtMin = theMaxPtMin; 
        }
        else if((nPix <= theScalingEnd) && (nPix > theScalingStart)){//scale radius linearly by Npix in the region from ScalingStart to ScalingEnd
          if(theOriginRScaling) scaledOriginRadius = theOriginRadius - (theOriginRadius-theMinOriginR)*(nPix-theScalingStart)/(theScalingEnd-theScalingStart);
          if(theHalfLengthScaling) scaledHalfLength = theOriginHalfLength - (theOriginHalfLength-theMinHalfLength)*(nPix-theScalingStart)/(theScalingEnd-theScalingStart);
          if(thePtMinScaling) scaledPtMin = thePtMin - (thePtMin-theMaxPtMin)*(nPix-theScalingStart)/(theScalingEnd-theScalingStart);
        }
        std::cout << "NumberOfPixels: " <<  nPix << std::endl;
        std::cout << "Scaled Origin R: " << scaledOriginRadius << " Default Origin R: " << theOriginRadius <<" Scaled HalfLength: " << scaledHalfLength << " Default H.L.: " << theOriginHalfLength <<" Scaled pT Min: " << scaledPtMin << " Default pT Min: " << thePtMin << std::endl;
        //otherwise use the unscaled radius
        if(scaledOriginRadius!=0 && scaledHalfLength !=0){
          //if region should have 0 size, return 'result' empty, otherwise make a tracking region 
          //(prevents making some tracks even with R=0 due to stat uncert on seed propagation)
          result.push_back( std::make_unique<GlobalTrackingRegion>(
            scaledPtMin, origin, scaledOriginRadius, std::max(theNSigmaZ*bs.sigmaZ(), scaledHalfLength), thePrecise,theUseMS));
        }
      }//end of linear scaling code
      else{
        result.push_back( std::make_unique<GlobalTrackingRegion>(
            thePtMin, origin, theOriginRadius, std::max(theNSigmaZ*bs.sigmaZ(), theOriginHalfLength), thePrecise,theUseMS));
      }
    }
    return result;
  }

private:
  double thePtMin;
  double theOriginRadius;
  double theOriginHalfLength; 
  double theNSigmaZ;
  edm::EDGetTokenT<reco::BeamSpot> 	 token_beamSpot; 
  bool thePrecise;
  bool theUseMS;
  bool theOriginRScaling; 
  bool thePtMinScaling; 
  bool theHalfLengthScaling; 
  double theMinOriginR;
  double theMaxPtMin;
  double theMinHalfLength;
  double theScalingStart;   
  double theScalingEnd;
  edm::InputTag pixelClusterCollectionInputTag_;
  edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster> > token_pc;
};

#endif
