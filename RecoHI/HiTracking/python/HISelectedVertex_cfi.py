import FWCore.ParameterSet.Config as cms

# sort by number of tracks and keep the best
hiBestAdaptiveVertex = cms.EDFilter("HIBestVertexSelection",
    src = cms.InputTag("hiPixelAdaptiveVertex"),
	maxNumber = cms.uint32(1)
)

# select best of precise vertex, fast vertex, and beamspot
hiSelectedVertex = cms.EDProducer("HIBestVertexProducer",
    beamSpotLabel = cms.InputTag("offlineBeamSpot"),
    adaptiveVertexCollection = cms.InputTag("hiBestAdaptiveVertex"),
    medianVertexCollection = cms.InputTag("hiPixelMedianVertex")
)

# best vertex sequence
bestHiVertex = cms.Sequence( hiBestAdaptiveVertex * hiSelectedVertex ) # vertexing run BEFORE tracking


from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *  
from RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi import *
hiOfflinePrimaryVertices=offlinePrimaryVertices.clone( # vertexing run AFTER tracking
    TrackLabel = cms.InputTag("hiGeneralTracks"),
    beamSpotLabel = cms.InputTag("offlineBeamSpot"),
                                        
    TkFilterParameters = cms.PSet(
        algorithm=cms.string('filter'),
        maxNormalizedChi2 = cms.double(20.0),
        minPixelLayersWithHits=cms.int32(4),
        minSiliconLayersWithHits = cms.int32(8),
        maxD0Significance = cms.double(5.0), 
        minPt = cms.double(0.9),
        trackQuality = cms.string("any")
    )
)
