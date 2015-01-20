#include "robotredisclient.h"
#include <sstream>
#include <cstdlib>

std::string ROBOT_REDIS_DELIM = ":";

namespace Rapi
{

//-----------------------------------------------------------------------------
CRobotRedisClient::CRobotRedisClient( std::string clientname, std::string hostname, int port )
{
  mClientName = clientname;
  mHostName = hostname;
  mPort = port;
  mRedisClient = CRedisClient::getInstance(mHostName, mPort);
}
//-----------------------------------------------------------------------------
CRobotRedisClient::~CRobotRedisClient() { }

CRedisClient* CRobotRedisClient::getInstance()
{
  return CRedisClient::getInstance(mHostName, mPort);
}
//-----------------------------------------------------------------------------
int CRobotRedisClient::ping()
{
  return mRedisClient->ping();
}
//-----------------------------------------------------------------------------
int CRobotRedisClient::post(const std::string value)
{
  int r;
  r = mRedisClient->set(mClientName, value);
  return r;
}
//-----------------------------------------------------------------------------
int CRobotRedisClient::get(const std::string key, std::string& value)
{
  int ret;
  std::string val;

  ret = mRedisClient->get(key, val);
  if (ret) {
    value = val;
    return ret; // succes
  }
  return ret; // failure
}
//-----------------------------------------------------------------------------
int CRobotRedisClient::postPose(CPose2d pose) {
  int ret;
  std::ostringstream strs;
  strs << pose.mX << ROBOT_REDIS_DELIM << pose.mY << ROBOT_REDIS_DELIM << pose.mYaw;
  ret = post(strs.str());
  return ret;
}
//-----------------------------------------------------------------------------
CPose2d* CRobotRedisClient::getPose(const std::string name) {
  int ret;
  std::string encodedPose;
  ret = mRedisClient->get(name, encodedPose);
  if (ret) {
    std::size_t delimPos = encodedPose.find(ROBOT_REDIS_DELIM);
    double x, y, yaw;
    if (delimPos == std::string::npos) {
      return NULL;
    }
    x = atof(encodedPose.substr(0, delimPos).c_str());
    encodedPose.erase(0, delimPos+1);
    delimPos = encodedPose.find(ROBOT_REDIS_DELIM);
    if (delimPos == std::string::npos) {
      return NULL;
    }
    y = atof(encodedPose.substr(0, delimPos).c_str());
    encodedPose.erase(0, delimPos+1);
    yaw = atof(encodedPose.c_str());

    CPose2d* pose = new CPose2d(x, y, yaw);
    return pose;
  }
  return NULL;
}
} // namespace
