/***************************************************************************
 * Project: RAPI                                                           *
 * Author:  Jens Wawerla (jwawerla@sfu.ca)                                 *
 * $Id: $
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 **************************************************************************/

#include "stagelaser.h"
#include "utilities.h"
#include "rapierror.h"
#include "pose2d.h"

namespace Rapi
{

// Callback for stage, stage calls this function if the corresponding model
// is updated
int laserUpdate( Stg::ModelRanger* mod, CStageLaser* laser )
{
  laser->updateData( mod->GetUpdateInterval() / 1e6 );
  return 0; // ok
}

//-----------------------------------------------------------------------------
CStageLaser::CStageLaser( Stg::ModelRanger* stgLaser, std::string devName )
    : ARangeFinder( devName )
{
  assert( stgLaser );
  mStgLaser = stgLaser;
  mFgEnabled = false;
  mStgLaser->GetWorld()->AddUpdateCallback(( Stg::world_callback_t )
                               laserUpdate,
                               this );
  setEnabled( true );
}
//-----------------------------------------------------------------------------
CStageLaser::~CStageLaser()
{
  if ( mRelativeBeamPose )
    delete[] mRelativeBeamPose;

  if ( mRangeData )
    delete[] mRangeData;
}
//-----------------------------------------------------------------------------
void CStageLaser::setEnabled( bool enable )
{
  if ( mFgEnabled == enable )
    return;  // we are really have the correct status

  if ( enable )
    mStgLaser->Subscribe();
  else
    mStgLaser->Unsubscribe();

  mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
int CStageLaser::init()
{
  float beamBearing;
  float beamIncrement;

  Stg::ModelRanger::Sensor sensor = mStgLaser->GetSensors()[0];
  mNumSamples = sensor.sample_count;
  mRangeResolution = 0;
  mFov = sensor.fov; //TWO_PI

  // initialize data structures
  mRelativeBeamPose = new CPose2d[mNumSamples];
  mRangeData = new tRangeData[mNumSamples];

  mBeamConeAngle = 0;
  beamBearing = -mFov / 2.0f;
  beamIncrement = mFov / float( mNumSamples - 1 );
  mMaxRange = sensor.range.max;
  mMinRange = sensor.range.min;

  for ( unsigned int i = 0; i < mNumSamples; i++ ) {
    mRelativeBeamPose[i].mX = 0.0;
    mRelativeBeamPose[i].mY = 0.0;
    mRelativeBeamPose[i].mYaw =  beamBearing;
    beamBearing += beamIncrement;
    mRangeData[i].range = 0.0;
    mRangeData[i].reflectance = 0.0;
  }

  return 1; // success
}
//-----------------------------------------------------------------------------
void CStageLaser::updateData( const double dt )
{
  if ( mFgEnabled ) {
    // get range data from stage, if the simulation is paused at start up
    // stage returns NULL, so we store the result in a temporal variable
    // and store is only if it is any good
    Stg::ModelRanger::Sensor sensor = mStgLaser->GetSensors()[0];
    uint32_t sampleCount = sensor.sample_count;
    if ( sampleCount != mNumSamples ) {
      ERROR2( "Got wrong number of laser readings from Stage, "\
              "expected %d but got %d", mNumSamples, sampleCount );
    }
    else {
      if ( !sensor.ranges.empty() ) { //!sample.empty() ) {
        for ( unsigned int i = 0; i < mNumSamples; i ++ ) {
          mRangeData[i].range = sensor.ranges[i];
          mRangeData[i].reflectance = sensor.intensities[i];
        }
      }
    }
    mTimeStamp = mStgLaser->GetWorld()->SimTimeNow() / 1e6;
    notifyDataUpdateObservers();
  }
}
//-----------------------------------------------------------------------------

} // namespace
