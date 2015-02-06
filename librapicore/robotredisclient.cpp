#include "robotredisclient.h"
#include <sstream>
#include <cstdlib>
#include <vector>

const char ROBOT_REDIS_DELIM = ':';
//-----------------------------------------------------------------------------
//maybe put these functions into utilities.h
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}
std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}

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
int CRobotRedisClient::postPose(CPose2d pose, float time_ms) {
  int ret;
  std::ostringstream strs;
  strs << time_ms << ROBOT_REDIS_DELIM<< pose.mX << ROBOT_REDIS_DELIM << pose.mY << ROBOT_REDIS_DELIM << pose.mYaw;
  ret = post(strs.str());
  return ret;
}
//-----------------------------------------------------------------------------
CPose2d* CRobotRedisClient::getPose(const std::string name, float* time_ms) {
  int ret;
  std::string encodedPose;
  ret = mRedisClient->get(name, encodedPose);
  if (ret) {
    std::vector<std::string> vs = split(encodedPose, ROBOT_REDIS_DELIM);
    if (vs.size() == 4) {
      if (time_ms) *time_ms = atof(vs[0].c_str());
        return new CPose2d(atof(vs[1].c_str()),atof(vs[2].c_str()),atof(vs[3].c_str()));
    }
  }
  return NULL;
}
//-----------------------------------------------------------------------------
std::vector<std::string> CRobotRedisClient::getMsg(const std::string key) {
  int ret;
  std::string encodedPose;
  std::vector<std::string> msg;
  ret = mRedisClient->get(key, encodedPose);
  if (ret) {
    msg = split(encodedPose, ROBOT_REDIS_DELIM);
  }
  return msg;
}
//-----------------------------------------------------------------------------
int CRobotRedisClient::postMsg(std::vector<std::string> msg) {
  int ret;
  std::ostringstream strs;
  if (msg.size() >= 1) {
    strs << msg[0];
  }
  for (unsigned int i = 1 ; i < msg.size(); i++) {
    strs << ROBOT_REDIS_DELIM << msg[i];
  }
  ret = post(strs.str());
  return ret;
}
} // namespace
