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

#include "cbsound.h"
#include "rapierror.h"
#include "printerror.h"
#include "utilities.h"

namespace Rapi
{

//---------------------------------------------------------------------------
CCBSound::CCBSound ( CCBDriver* cbDriver, std::string devName )
  : ASound ( devName )
{
  mCBDriver = cbDriver;

  setEnabled ( true );

}
//---------------------------------------------------------------------------
CCBSound::~CCBSound()
{
  // if a chatterbox beeps in a forest with no one around, does anyone hear it?
}
//---------------------------------------------------------------------------
void CCBSound::setEnabled ( bool enable )
{
  if ( enable == true ) {
    mFgEnabled = true;
  }
  else {
    mFgEnabled = false;
  }
}
//----------------------------------------------------------------------------
int CCBSound::init()
{
  return 1;
}
//----------------------------------------------------------------------------
void CCBSound::updateData( const double dt)
{
  mUpdateInterval = dt;

  if ( mFgEnabled ) {
    // update time stamp of this measurement
    mTimeStamp = timeStamp();
  } // enabled
}
//----------------------------------------------------------------------------
int CCBSound::playSong (unsigned int id)
{
  if (id >= 0 && id <= 15) {
	return mCBDriver->playSong((unsigned char)id);
  }
  ERROR2 ("Song id out of bounds %d [0..%d]", id, 15);
  return 0;
}
//----------------------------------------------------------------------------
int CCBSound::playTone(unsigned int note, unsigned int duration)
{
  unsigned char id = (unsigned char) 15;
  unsigned char seq[1] = { (unsigned char) note };
  if (mCBDriver->defineSoundSequence(id, seq, 1) == 1) {
	return mCBDriver->playSong(id);
  }
  return 0;
}
//----------------------------------------------------------------------------
void CCBSound::print() const
{
  printf ( "CCBSound: " );
  printf ( "\n" );
}
//----------------------------------------------------------------------------


} // namespace
