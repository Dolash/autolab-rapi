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

#ifndef RAPIROBOT_H
#define RAPIROBOT_H

#include "robotupdateinterface.h"
#include "rangefinder.h"
#include "drivetrain2dof.h"
#include "powerpack.h"
#include "fiducialfinder.h"
#include "lights.h"
#include "textdisplay.h"
#include "binarysensorarray.h"
#include "variablemonitor.h"
#include <list>
#include <vector>
#include <string>

namespace Rapi
{

/**
 * Base class for all robot. Robot are used as a central point to obtain all
 * devices the robot is configured with.
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */

class ARobot
{
  public:
    /** Default destructor */
    virtual ~ARobot();
    /**
     * Ininitializes the robot
     * @return 1 if successfull, 0 otherwise
     */
    virtual int init() = 0;
    /**
     * Registers a robot controller with the frame work. The frame work
     * calls the update method of a registered controller in every time
     * step.
     * @param ctrl to be registered
     */
    void registerRobotController ( IRobotUpdate* ctrl );
    /**
     * Registers a state variable with the robot
     * @param var to register
     */
    void registerStateVariable( IRobotUpdate* var);
    /**
     * Gets the name of the robot
     * @return robot name
     */
    std::string getName();
    /**
     * Gets the number of available devices
     * @return number of devices
     */
    unsigned int getNumOfDevices() { return mDeviceList.size(); };
    /**
     * Gets a device by the index of the device list
     * @param index of device
     * @return device or NULL
     */
    ADevice* getDeviceByIndex(unsigned int index);
    /**
     * Gets the current time of the robot, this maybe simulated time, real time,
     * elapsed time since start of robot etc.
     * @return [s]
     */
    virtual double getCurrentTime() = 0;
    /**
     * Gets a device with a given device index
     * @param devName name of device
     * @return device
     * @return 1 if successfull, 0 otherwise
     */
    virtual int findDevice ( ARangeFinder* &device, std::string devName ) = 0;
    virtual int findDevice ( ADrivetrain2dof* &device,  std::string devName ) = 0;
    virtual int findDevice ( APowerPack* &device, std::string devName ) = 0;
    virtual int findDevice ( AFiducialFinder* &device, std::string devName ) = 0;
    virtual int findDevice ( ALights* &device, std::string devName ) = 0;
    virtual int findDevice ( ATextDisplay* &device, std::string devName ) = 0;
    virtual int findDevice ( ABinarySensorArray* &device, std::string devName ) = 0;

    CVariableMonitor mVariableMonitor;
  protected:
    /** Default constructor */
    ARobot();
    /**
     * Calls the update method in all registered robot controllers
     */
    void updateControllers();
    /**
     * Calls the update method in all registered state variables controllers
     */
    void updateStateVariable();
    /** Robot controller list */
    std::list<IRobotUpdate*> mRobotCtrlList;
    /** State variable list */
    std::list<IRobotUpdate*> mStateVariableList;
    /** Update interval [s] */
    double mUpdateInterval;
    /** Name of robot */
    std::string mName;
    /** List of all devices generated */
    std::vector<ADevice*> mDeviceList;
};

} // namespace
#endif
