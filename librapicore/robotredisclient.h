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
#ifndef CROBOTREDISCLIENT_H
#define CROBOTREDISCLIENT_H

#include "redisclient.h"
#include "pose2d.h"
#include "utilities.h"
#include <vector>

namespace Rapi
{

/**
 * Convience class for message passing between a swarm of robots using
 * an underlying REDIS client/server model. Clients may post messages
 * to the server using their name as the key. Messages may be read by
 * specifying a name of another client.
 * @author Jacob Perron
 */
class CRobotRedisClient
{
  public:
    /** Default destructor */
    virtual ~CRobotRedisClient();
    /**
     * Attempts to connect to a specified redis server.
     * @param clientname name of this client
     * @param hostname name of server
     * @param port of server
     */
    CRobotRedisClient( std::string clientname, std::string hostname, int port=6379);
    /**
     * @return instance of the underlying Redis client object.
     */
    CRedisClient* getInstance();
    /**
     * pings the redis server
     * @return 1 if success, 0 otherwise
     */
    int ping();
    /**
     * Posts a message to the server
     * @param value
     * @return 1 if success, 0 otherwise
     */
    int post(const std::string value);
    /**
     * Gets a message from the server
     * @param key name of client who posted message
     * @param value
     * @return 1 if success, 0 otherwise
     */
    int get(const std::string key, std::string& value);
    /**
     * Posts the provided pose to the server
     * @param pose
     * @param time_ms : The timestamp associated with the pose.
     */
    int postPose(CPose2d pose, float time_ms = timeStamp());

    /**
     * Gets the pose of the specified client
     * @param name of client
     * @param time_ms : Assigned the time associated with the returned pose.
     * @return the 2d pose of the client, or NULL if unable to parse pose.
     */
    CPose2d* getPose(const std::string name, float* time_ms);

    /**
     * Gets a message containing any number of terms.
     * @param key : The key associated with the message.
     * @return The message composed as a vector of strings.
     */
    std::vector<std::string> getMsg(const std::string key);

    /**
     * Posts a message under the name of this robot name.
     * @param msg : The message composed as a vector of strings.
     * @return 1 if succesfully posted message, 0 otherwise.
     */
    int postMsg(std::vector<std::string> msg);

  protected:

  private:
    /** Client name */
    std::string mClientName;
    /** Hostname of server */
    std::string mHostName;
    /** Port number of server */
    int mPort;
    /** Redis client */
    CRedisClient* mRedisClient;
};

} // namespace
#endif // CROBOTREDISCLIENT_H
