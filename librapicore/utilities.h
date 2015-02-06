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

#ifndef RAPIUTILITIES_H
#define RAPIUTILITIES_H

#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <limits>
#include <stdlib.h>

namespace Rapi
{

#ifdef _RAPI_GUI
  #define RAPI_GUI 1
#endif

/** Espilon */
double const ESPILON = 1e-6;

#ifndef INFINITY
  #define INFINITY infinity()
#endif

/** Seconds in one hour */
double const SEC_PER_HOUR  = 3600.0;

/** Well... ?! */
double const PI = 3.14159265358979323846;

/** pi/2 */
double const HALF_PI = 1.570796326794896619;

/** pi/4 */
double const QUARTER_PI = 0.78539816339;

/** 2 times pi = 6.28... */
double const TWO_PI = 6.28318530717958;

/**
 * Figure out the sign of a number, 0 is assumed to be positive
 * @param x value to get sign for
 * @return -1 or +1
 */
template<typename T>
inline int sign( T x)
{
  if ( x < 0 )
    return -1;
  return 1;
}

/**
 * Calculates euclidian distance
 * @return distance
 */
inline double euclidian(double x, double y, double a, double b) {
  return sqrt( (x-a)*(x-a) + (y-b)*(y-b));
}

/**
 * Conversts x from radians to degree
 * @param x value [rad]
 * @return [deg]
 */
inline double R2D(double x)
{
  return x*57.2957795;
}

/**
 * Conversts x from degree to radians
 * @param x value [deg]
 * @return [rad]
 */
inline double D2R(double x)
{
 return x*0.01745329;
}

/**
 * Maximum value of a and b
 * @param a
 * @param b
 * @return max of a and b
 */
inline double max(double a, double b)
{
 return ((a > b) ? (a) : (b));
}

/**
 * Minimum value of a and b
 * @param a
 * @param b
 * @return min of a and b
 */

inline double min(double a, double b)
{
  return ((a < b) ? (a) : (b));
}

/**
 * Check if the modulo is about zero
 * @param x
 * @param y
 * @return true is about zero, false otherwise
 */
inline bool isModAboutZero(double x, double y)
{
  if ( fmod(x, y) < ESPILON)
    return true;
  return false;
}

/**
 * Checks if x is about zero, within 1e-6
 * @param x value to check
 * @return true if about zero, false otherwise
 */
inline bool isAboutZero(double x)
{
  if ( fabs( x ) < ESPILON )
    return true;
  return false;
}

/**
 * Normalizes the value z to be in the interval [-pi, pi]
 * @param z to be normalized
 * @return normalized value
 */
template<typename T>
inline T normalizeAngle(T z)
{
  while( z < -PI ) z += TWO_PI;
  while( z > PI ) z -= TWO_PI;
  return z;
}

/**
 * Limits the value x to be in the interval [a, b]
 * @param a lower limit
 * @param b upper limit
 * @return limited value
 */
inline double limit(double x, double a, double b)
{
 if ( x < a )
   return a;
 if ( x > b)
   return b;
 return x;
}

/**
 * Rounds a number
 * @return rounded number
 */
template<typename T>
inline T round(T x)
{
  return floor(x+0.5);
}

/**
 * Checks if a x is infinity or not
 * @return true is x is infinity, false otherwise
 */
template<typename T>
inline bool isNan ( T value )
{
  return value != value;
}

/**
 * Checks if a x is nan or not
 * @return true is x is nan, false otherwise
 */
template<typename T>
inline bool isInf ( T value )
{
  return std::numeric_limits<T>::has_infinity &&
         value == std::numeric_limits<T>::infinity();
}

/**
 * Checks if x and y are about equal, that is within epsilon
 * @param x
 * @param y
 * @param epslion
 * @return true if about equal, false otherwise
 */
inline bool epsilonEqual(double x, double y, double epsilon = ESPILON)
{
  if ( fabs ( x - y ) < epsilon )
    return true;

  return false;
}

/**
 * Calculates the square of x
 * @param x
 * @return x^2
 */
template<typename T>
inline T pow2(T x) { return x*x; };

/**
 * Generates a time stamp
 * @return [s]
 */
inline double timeStamp()
{
  struct timeval tv;
  gettimeofday ( &tv, 0 );
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

/**
 * Generates a uniform random number in the interval
 * [minimum, maximum]
 * @param minimum
 * @param maximum
 * @return random number
 */
inline double randNo ( double minimum, double maximum )
{
  double r;

  r = drand48();  // r = [0,1]
  return minimum + r * ( maximum - minimum );
}

/**
 * \fn void initRandomNumberGenerator()
 * Initializes the random number generator
 */
void initRandomNumberGenerator();


//-----------------------------------------------------------------------------

} // namespace
#endif

