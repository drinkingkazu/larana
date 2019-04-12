// -*- mode: c++; c-basic-offset: 2; -*-
#ifndef OPHITALG_H
#define OPHITALG_H
/*!
 * Title:   OpHit Algorithims
 * Author:  Ben Jones, MIT ( Edited by wketchum@lanl.gov, gleb.sinev@duke.edu
 *                           and kevin.wood@stonybrook.edu )
 *
 * Description:
 * These are the algorithms used by OpHit to produce optical hits.
 */

#include "larcorealg/Geometry/GeometryCore.h"
#include "lardataobj/RawData/OpDetWaveform.h"
#include "lardataobj/RecoBase/OpHit.h"
#include "lardataalg/DetectorInfo/DetectorClocks.h"
#include "larreco/Calibrator/IPhotonCalibrator.h"
#include "larana/OpticalDetector/OpHitFinder/PulseRecoManager.h"
#include "larana/OpticalDetector/OpHitFinder/PMTPulseRecoBase.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

#include <vector>

namespace opdet{

  void RunHitFinder(std::vector< raw::OpDetWaveform > const&,
                    std::vector< recob::OpHit >&,
                    pmtana::PulseRecoManager const&,
                    pmtana::PMTPulseRecoBase const&,
                    geo::GeometryCore const&,
                    float,
                    detinfo::DetectorClocks const&,
                    calib::IPhotonCalibrator const&);

  void ConstructHit(float,
                    int,
                    double,
                    pmtana::pulse_param const&,
                    std::vector< recob::OpHit >&,
                    detinfo::DetectorClocks const&,
                    calib::IPhotonCalibrator const&);

} // End opdet namespace

#endif
