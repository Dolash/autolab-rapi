/***************************************************************************
 * Project: RAPI                                                           *
 * Author:  Jacob Perron (jperron@sfu.ca)                                  *
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
#ifndef CHATTERBOXCTRL_H
#define CHATTERBOXCTRL_H

#include <RapiChatterbox>
#include <RapiAutolabTracker>

using namespace Rapi;

typedef enum {RUN, CHARGING, DOCKING, UNDOCKING} tState;

/**
 * A template controller for chatterbox.
 * Includes obstacle avoidence and docking/undocking behaviours.
 * @author Jacob Perron
 */
class CChatterboxCtrl : public ARobotCtrl
{
  public:
    /**
     * Default constructor
     * @param robot this controller controls
     */
    CChatterboxCtrl ( ARobot* robot );
    /** Default destructor */
    ~CChatterboxCtrl();


  protected:
    /**
     * Update controller for the current time step
     * @param dt time since last upate [s]
     */
    void updateData(float dt);
    /**
     * The core behaviour of the robot.
     * This is called every time step
     */
    void run();
    /**
     * Checks if home base is detected or not
     * @return true if detected, false otherwise
     */
    bool homeBaseDetected();
    /** Obstacle avoidance routine */
    void obstacleAvoid();
    /** Emergency stop routine */
    void emergencyStop();

    /** Drivetrain */
    CCBDrivetrain2dof* mDrivetrain;
    /** Infrared sensors */
    ARangeFinder* mIr;
    /** Power pack */
    APowerPack* mPowerPack;
    /** Text display */
    ATextDisplay* mTextDisplay;
    /** Lights */
    ALights* mLights;
    /** Bumper */
    ABinarySensorArray* mBumper;
    /** Wheel drop */
    ABinarySensorArray* mWheelDrop;
    /** Low side driver */
    ASwitchArray* mLowSideDriver;
    /** Laser range finder */
    ARangeFinder* mLaser;
    /** Top fiducial */
    AFiducialFinder* mTopFiducial;
    /** Front fiducial */
    AFiducialFinder* mFrontFiducial;
    /** Photo sensor */
    AAnalogSensorArray* mPhoto;
    /** Create button */
    ABinarySensorArray* mButton;
    /** Cliff sensor */
    ABinarySensorArray* mCliff;
    /** Odometry */
    COdometry* mOdometry;
    /** Some limit */
    CLimit mLimit;
    /** Data logger */
    CDataLogger* mDataLogger;

    /** Redis client */
    CRedisClient* mRedis;
    /** Overhead camera tracker */
    CAutolabTracker* mTracker;

  private:
    /** State machine */
    tState mState;
    /** Time since start of controller [s] */
    float mTime;
    /** Flag if we should run the main behaviour or not */
    bool mFgRun;
    /** Red Led index */
    unsigned int mRedLedId;
    /** Text for 7seg display */
    int mText;
    /** Flags if motors are enabled */
    bool mFgMotor;
    /** General purpose timer */
    float mTimer;

    /** Min photo value */
    float mMinPhoto;

};

#endif
