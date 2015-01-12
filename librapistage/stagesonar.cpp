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

#include "stagesonar.h"
#include "utilities.h"

namespace Rapi
{
//---------------------------------------------------------------------------
// Callback for stage, stage calls this function if the corresponding model
// is updated
int sonarUpdate ( Stg::ModelRanger* ranger, CStageSonar* sonar )
{
  sonar->updateData( ranger->GetUpdateInterval() / 1e6 );
  return 0; // ok
}
//---------------------------------------------------------------------------
CStageSonar::CStageSonar( Stg::ModelRanger* stgModel, std::string devName )
    : ARangeFinder( devName )
{
  assert( stgModel );
  mStgRanger = stgModel;
  mFgEnabled = false;
  mStgRanger->GetWorld()->AddUpdateCallback ( ( Stg::world_callback_t )
                                 sonarUpdate,
                                 this );

  setEnabled( true );
}
//---------------------------------------------------------------------------
CStageSonar::~CStageSonar()
{
  if ( mRelativeBeamPose )
    delete[] mRelativeBeamPose;

  if ( mRangeData )
    delete[] mRangeData;
}
//---------------------------------------------------------------------------
int CStageSonar::init()
{
  mMaxRange = -INFINITY;
  mMinRange = INFINITY;

  const std::vector<Stg::ModelRanger::Sensor> sensors = mStgRanger->GetSensors();
  mNumSamples = sensors.size();
  mRangeResolution = 0;
  mFov = TWO_PI;

  // initialize data structures
  mRelativeBeamPose = new CPose2d[mNumSamples];
  mRangeData = new tRangeData[mNumSamples];
  for ( unsigned int i = 0; i < mNumSamples; i++ ) {
    mRelativeBeamPose[i].mX = sensors[i].pose.x;
    mRelativeBeamPose[i].mY = sensors[i].pose.y;
    mRelativeBeamPose[i].mYaw =  sensors[i].pose.a;
    mMaxRange = max( mMaxRange, sensors[i].range.max );
    mMinRange = max( mMinRange, sensors[i].range.min );
    mBeamConeAngle =  sensors[i].fov;
  }

  return 1; // success
}
//---------------------------------------------------------------------------
void CStageSonar::setEnabled( bool enable )
{
  if ( mFgEnabled == enable )
    return;  // we are really have the correct status

  if ( enable )
    mStgRanger->Subscribe();
  else
    mStgRanger->Unsubscribe();

  mFgEnabled = enable;
}
//---------------------------------------------------------------------------
void CStageSonar::updateData( const double dt)
{
  if ( mFgEnabled ) {
    const std::vector<Stg::ModelRanger::Sensor> sensors = mStgRanger->GetSensors();
    if ( !sensors[0].ranges.empty() ) {
      for ( unsigned int i = 0; i < mNumSamples; i++ ) {
        mRangeData[i].range = sensors[i].ranges[0];
      }
    }
    mTimeStamp = mStgRanger->GetWorld()->SimTimeNow() / 1e6;
    notifyDataUpdateObservers();
  }
}
//---------------------------------------------------------------------------
void CStageSonar::print()
{
  printf( "SONAR: " );
  for ( unsigned int i = 0; i < mNumSamples; i++ ) {
    printf( " %01.3f ", mRangeData[i].range );
  }
  printf( "\n" );
}
//---------------------------------------------------------------------------

} // namespace
