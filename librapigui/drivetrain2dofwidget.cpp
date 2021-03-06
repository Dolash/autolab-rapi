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
#include "drivetrain2dofwidget.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
CDrivetrain2dofWidget::CDrivetrain2dofWidget ( ADrivetrain2dof* drivetrain,
    QWidget* parent ) : ADeviceWidget ( parent )
{
  mDrivetrain = drivetrain;
  setTitle ( "Drivetrain" );

  mStalledLed = new CDataLed ( mGeneralInfoBox, "Stalled" );
  mStalledLed->setData ( CDataLed::RED_OFF );
  mGeneralInfoBoxLayout->addWidget ( mStalledLed );

  mStalledTime = new CDataLine ( mGeneralInfoBox, "Stall time [s]" );
  mGeneralInfoBoxLayout->addWidget ( mStalledTime );

  mVelocityCmdWidget = new CVelocity2dWidget ( this );
  mVelocityCmdWidget->setTitle ( "Velocity Command" );
  mMainLayout->addWidget ( mVelocityCmdWidget );

  mVelocityMeasWidget = new CVelocity2dWidget ( this );
  mVelocityMeasWidget->setTitle ( "Velocity" );
  mMainLayout->addWidget ( mVelocityMeasWidget );


  mOdometryWidget = new CPose2dWidget ( this );
  mOdometryWidget->setTitle ( "Odometry" );
  mMainLayout->addWidget ( mOdometryWidget );
}
//-----------------------------------------------------------------------------
CDrivetrain2dofWidget::~CDrivetrain2dofWidget()
{
}
//-----------------------------------------------------------------------------
void CDrivetrain2dofWidget::updateData ()
{
  if ( isChecked() ) {
    mOdometryWidget->setHidden ( false );
    mVelocityCmdWidget->setHidden ( false );
    mVelocityMeasWidget->setHidden ( false );
    mVelocityCmdWidget->updateData ( mDrivetrain->getVelocityCmd() );
    mVelocityMeasWidget->updateData ( mDrivetrain->getVelocity() );
    mOdometryWidget->updateData ( mDrivetrain->getOdometry()->getPose() );
    mStalledTime->setData ( mDrivetrain->stalledSince() );
    if ( mDrivetrain->isStalled() )
      mStalledLed->setData ( CDataLed::RED_ON );
    else
      mStalledLed->setData ( CDataLed::RED_OFF );
  }
  else {
    mOdometryWidget->setHidden ( true );
    mVelocityCmdWidget->setHidden ( true );
    mVelocityMeasWidget->setHidden ( true );
  }
  ADeviceWidget::updateData ( mDrivetrain );
  mStalledLed->setHidden ( !mGeneralInfoBox->isChecked() );
  mStalledTime->setHidden ( !mGeneralInfoBox->isChecked() );
}
//-----------------------------------------------------------------------------

} // namespace
