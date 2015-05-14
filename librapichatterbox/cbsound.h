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

#ifndef RAPICBSOUND_H
#define RAPICBSOUND_H

#include "sound.h"
#include "cbdriver.h"

namespace Rapi
{

/**
 * This class implements control for the built-in speakers inside the base of
 * the chatterbox.
 * @author Jacob Perron <jperron@sfu.ca>
 */
class CCBSound : public ASound
{
    /** We are friends with our robot, so we get updated */
    friend class CCBRobot;

  public:
    /**
     * Default constructor
     * @param cbDriver HAL of the chatterbox
     * @param devName name of this device
     */
    CCBSound(CCBDriver* cbDriver, std::string devName);
    /** Default destructor */
    ~CCBSound();

    /**
     * Enables or disables the device 
     * @param enable = true to enable, false to disable 
     */
    virtual void setEnabled(bool enable);
    /**
     * Initializes the device
     * @param return 1 if success -1 otherwise
     */
    virtual int init();

    /**
	 * Play demo song.
     * @param id of song. Must be in the range [0-15]
     * @return 1 if successfull, -1 otherwise
     */
    int playSong(unsigned int id);
    /**
     * Prints the devices main information
     */
    virtual void print() const;
    /**
     * Plays a single tone.
     * @param note of the tone. Eight octaves with semi-tones from
	 *     G to G are are available using notes [31-127]. Anything
	 *     outside this range is considered a rest.
     * @param duration of the note played.
     */
    int playTone(unsigned int note, unsigned int duration);

  protected:
    /**
     * This method gets called by the framework every step to update
     * the sensor data
     * @param dt size of time step [s]
     */
    virtual void updateData(const double dt);

  private:
    /** Chatterbox driver to enable or disable the laser power */
    CCBDriver* mCBDriver;
    /** Update interval [s] */
    double mUpdateInterval;

};

} // namespace

#endif

