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
#include "stagelights.h"

namespace Rapi
{
// Callback for stage, stage calls this function if the corresponding model
// is updated
int lightUpdate ( Stg::ModelBlinkenlight* mod, CStageLights* lights )
{
  lights->updateData( mod->GetUpdateInterval() / 1e6 );
  return 0; // ok
}

//-----------------------------------------------------------------------------
CStageLights::CStageLights ( Stg::ModelBlinkenlight* stgModel,
                             std::string devName )
    : ALights ( devName )
{
  assert(stgModel );
  mStgLights = stgModel;
  setEnabled( true );
}
//-----------------------------------------------------------------------------
CStageLights::~CStageLights()
{
}
//-----------------------------------------------------------------------------
 int CStageLights::init()
{
  return 1; // success
}
//-----------------------------------------------------------------------------
void CStageLights::setEnabled ( bool enable )
{
}
//-----------------------------------------------------------------------------
void CStageLights::updateData( const double dt)
{
  mTimeStamp = mStgLights->GetWorld()->SimTimeNow() / 1e6;
}
//-----------------------------------------------------------------------------

} // namespace
