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
#include "loosestagedrivetrain2dof.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
CLooseStageDrivetrain2dof::CLooseStageDrivetrain2dof ( Stg::ModelPosition* stgModel,
    std::string devName )
    : CStageDrivetrain2dof (stgModel, devName )
{
}
//-----------------------------------------------------------------------------
CLooseStageDrivetrain2dof::~CLooseStageDrivetrain2dof()
{
}
//-----------------------------------------------------------------------------
Stg::ModelPosition* CLooseStageDrivetrain2dof::getStageModel ()
{
  return mStgPosition;
}
//-----------------------------------------------------------------------------
bool CLooseStageDrivetrain2dof::isSelectedInStage()
{
  if (mStgPosition->GetWorld()->RecentlySelectedModel() == mStgPosition)
    return true; // selected

  return false; // not selected
}
//-----------------------------------------------------------------------------
} // namespace
