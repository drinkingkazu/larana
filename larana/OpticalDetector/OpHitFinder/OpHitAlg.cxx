// -*- mode: c++; c-basic-offset: 2; -*-
/*!
 * Title:   OpHit Algorithims
 * Authors, editors:  Ben Jones, MIT
 *                    Wes Ketchum wketchum@lanl.gov
 *                    Gleb Sinev  gleb.sinev@duke.edu
 *                    Alex Himmel ahimmel@fnal.gov
 *                    Kevin Wood  kevin.wood@stonybrook.edu
 *
 * Description:
 * These are the algorithms used by OpHitFinder to produce optical hits.
 */

#include "OpHitAlg.h"

#include "larana/OpticalDetector/OpHitFinder/PulseRecoManager.h"
#include "lardataalg/DetectorInfo/DetectorClocks.h"
#include "lardataalg/DetectorInfo/ElecClock.h"
#include "lardataobj/RawData/OpDetWaveform.h"
#include "lardataobj/RecoBase/OpHit.h"
#include "larreco/Calibrator/IPhotonCalibrator.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include <vector>

namespace opdet{
  //----------------------------------------------------------------------------
  void RunHitFinder(std::vector< raw::OpDetWaveform > const&
                                                    opDetWaveformVector,
                    std::vector< recob::OpHit >&    hitVector,
                    pmtana::PulseRecoManager const& pulseRecoMgr,
                    pmtana::PMTPulseRecoBase const& threshAlg,
                    geo::GeometryCore const&        geometry,
                    float                           hitThreshold,
                    detinfo::DetectorClocks const&  detectorClocks,
                    calib::IPhotonCalibrator const& calibrator,
		    bool fUseStartTime) {

    for (auto const& waveform : opDetWaveformVector) {

      const int channel = static_cast< int >(waveform.ChannelNumber());

      if (!geometry.IsValidOpChannel(channel)) {
        mf::LogError("OpHitFinder") << "Error! unrecognized channel number "
                         << channel << ". Ignoring pulse";
        continue;
      }

      pulseRecoMgr.Reconstruct(waveform);

      // Get the result
      auto const& pulses = threshAlg.GetPulses();

      const double timeStamp = waveform.TimeStamp();


      for (auto const& pulse : pulses)
        ConstructHit(hitThreshold,
                     channel,
                     timeStamp,
                     pulse,
                     hitVector,
                     detectorClocks,
                     calibrator,
		     fUseStartTime);

    }
  }


  //----------------------------------------------------------------------------
  void ConstructHit(float                           hitThreshold,
                    int                             channel,
                    double                          timeStamp,
                    pmtana::pulse_param const&      pulse,
                    std::vector< recob::OpHit >&    hitVector,
                    detinfo::DetectorClocks const&  detectorClocks,
                    calib::IPhotonCalibrator const& calibrator,
		    bool fUseStartTime ) {

    if (pulse.peak < hitThreshold) return;

    //double absTime = pulse.t_max*detectorClocks.OpticalClock().TickPeriod();//timeStamp + pulse.t_max*detectorClocks.OpticalClock().TickPeriod();
    double absTime = timeStamp + pulse.t_max*detectorClocks.OpticalClock().TickPeriod();
    if(fUseStartTime)
      absTime = timeStamp + pulse.t_start*detectorClocks.OpticalClock().TickPeriod();

    double relTime = absTime - detectorClocks.TriggerTime();
    //double relTime = pulse.t_max*detectorClocks.OpticalClock().TickPeriod() -  detectorClocks.TriggerTime() ;
    
    int frame = detectorClocks.OpticalClock().Frame(timeStamp);

    double PE = 0.0;
    if (calibrator.UseArea()) PE = calibrator.PE(pulse.area, channel);
    else                      PE = calibrator.PE(pulse.peak, channel);

    double width = (pulse.t_end - pulse.t_start) * detectorClocks.OpticalClock().TickPeriod();
    
    hitVector.emplace_back(channel,
                           relTime,
                           absTime,
                           frame,
                           width,
                           pulse.area,
                           pulse.peak,
                           PE,
                           0.0);

  }

} // End namespace opdet
