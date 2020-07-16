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
#include "larana/OpticalDetector/OpHitFinder/PMTPulseRecoBase.h"

#include <vector>

namespace calib { class IPhotonCalibrator; }
namespace detinfo { class DetectorClocks; }
namespace pmtana { class PulseRecoManager; }

namespace opdet{

  void RunHitFinder(std::vector< raw::OpDetWaveform > const&,
                    std::vector< recob::OpHit >&,
                    pmtana::PulseRecoManager const&,
                    pmtana::PMTPulseRecoBase const&,
                    geo::GeometryCore const&,
                    float,
                    detinfo::DetectorClocks const&,
                    calib::IPhotonCalibrator const&,
		    bool fUseStartTime=false);

  void ConstructHit(float,
                    int,
                    double,
                    pmtana::pulse_param const&,
                    std::vector< recob::OpHit >&,
                    detinfo::DetectorClocks const&,
                    calib::IPhotonCalibrator const&,
		    bool fUseStartTime=false);

} // End opdet namespace

#endif
