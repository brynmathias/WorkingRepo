import FWCore.ParameterSet.Config as cms

# make L1 ntuples from RAW+RECO

process = cms.Process("L1NTUPLE")

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/Geometry_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/SimL1Emulator_cff')
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load('Configuration/StandardSequences/EndOfProcess_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')


process.mergedSuperClusters = cms.EDFilter("EgammaSuperClusterMerger",
#src = cms.VInputTag(cms.InputTag("correctedHybridSuperClusters"),cms.InputTag("correctedMulti5x5SuperClustersWithPreshower"))
src = cms.VInputTag(cms.InputTag("hybridSuperClusters"),cms.InputTag("multi5x5SuperClustersWithPreshower"))
)



# global tag
process.GlobalTag.globaltag = 'GR_E_V14::All'

# output file
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('L1Tree_2.root')
)

# analysis
process.load("L1Trigger.Configuration.L1Extra_cff")
process.load("UserCode.L1TriggerDPG.l1NtupleProducer_cfi")
process.load("UserCode.L1TriggerDPG.l1RecoTreeProducer_cfi")
process.load("UserCode.L1TriggerDPG.l1ExtraTreeProducer_cfi")
process.load("UserCode.L1TriggerDPG.l1MuonRecoTreeProducer_cfi")
process.HLTfilter =cms.EDFilter("HLTHighLevel",
     TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
     HLTPaths = cms.vstring('HLT_L1Tech_BSC_minBias_threshold1_v1'),
     eventSetupPathsKey = cms.string(''), 
     andOr = cms.bool(True), # for multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
     throw = cms.bool(False)    # throw exception on unknown path names
 )
# to select "good" collision events using tracking
process.primaryVertexFilter = cms.EDFilter("VertexSelector",
   src = cms.InputTag("offlinePrimaryVertices"),
   cut = cms.string("!isFake && ndof > 4 && abs(z) <= 15 && position.Rho <= 2"), # tracksSize() > 3 for the older cut
   filter = cms.bool(True),   # otherwise it won't filter the events, just produce an empty vertex collection.
)
process.noscraping = cms.EDFilter("FilterOutScraping",
applyfilter = cms.untracked.bool(True),
debugOn = cms.untracked.bool(False),
numtrack = cms.untracked.uint32(10),
thresh = cms.untracked.double(0.25)
)

process.p = cms.Path(
    process.HLTfilter+
    process.primaryVertexFilter+
    process.noscraping+
    process.gtDigis
    +process.gtEvmDigis
    +process.gctDigis
    +process.dttfDigis
    +process.csctfDigis
    +process.l1NtupleProducer
    +process.l1extraParticles
    +process.l1ExtraTreeProducer
    +process.l1RecoTreeProducer
    +process.l1MuonRecoTreeProducer
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(250) )

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",
                             fileNames = readFiles,
                             secondaryFileNames = secFiles
                             )

readFiles.extend( [
    '/store/express/Run2011A/ExpressPhysics/FEVT/Express-v1/000/160/413/028F7F9B-D34D-E011-BFC0-0030487CD76A.root'
#'/store/data/Commissioning10/ZeroBias/RECO/v3/000/130/293/FC72D3D0-F029-DF11-B249-000423D9870C.root'
] )

secFiles.extend( [
   ] )
