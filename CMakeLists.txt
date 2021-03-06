cmake_minimum_required(VERSION 2.6)

PROJECT(autolab-rapi)
SET( V_MAJOR 0 )
SET( V_MINOR 1 )
SET( V_BUGFIX 0 )

SET( VERSION ${V_MAJOR}.${V_MINOR}.${V_BUGFIX} )
SET( APIVERSION ${V_MAJOR}.${V_MINOR} )

CMAKE_POLICY( SET CMP0015 OLD )

# Generate GIT version number
SET(GIT_VERSION_FILE GIT-VERSION-FILE)
SET(GIT_COMMAND ${PROJECT_BINARY_DIR}/GIT-VERSION-GEN)
SET(BUILDNUM_COMMAND ${PROJECT_BINARY_DIR}/BUILDNUM-GEN)
EXECUTE_PROCESS(COMMAND ${GIT_COMMAND})
EXECUTE_PROCESS(COMMAND ${BUILDNUM_COMMAND})
INCLUDE(${GIT_VERSION_FILE})

MESSAGE(STATUS "Build " ${BUILD_NUM})
MESSAGE(STATUS "Git version " ${GIT_VERSION})

# Enable graphics: use with -DWITH_GUI:BOOL=off|on
OPTION (WITH_GUI "Enable building of Rapi graphics stuff" OFF)

# Enable chatterbox: use with -DWITH_CB:BOOL=off|on
OPTION (WITH_CB "Enable building of chatterbox library" ON)

# Enable stage: use with -DWITH_STAGE:BOOL=off|on
OPTION (WITH_STAGE "Enable building of stage library" OFF)

# Enable rpc: use with -DWITH_RPC:BOOL=off|on
OPTION (WITH_RPC "Enable building of RPC client" ON)
 
# Enable examples: use with -DWITH_EXAMPLES:BOOL=off|on
OPTION (WITH_EXAMPLES "Enable building of examples" ON)

# Enable tracker: use with -DWITH_TRACKER:BOOL=off|on
OPTION (WITH_TRACKER "Enable building of autolab tracker" ON)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE( STAGE stage )
FIND_PACKAGE(Qt4)

IF(QT_FOUND)
ELSEIF(QT_FOUND)
  SET(WITH_GUI off)
ENDIF(QT_FOUND)

IF( APPLE )
  # skipping chatterbox, as it needs Linux
  SET(WITH_CB off)
ENDIF( APPLE )

# generate config.h file
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/config.h @ONLY)

# get >1.36 Boost libraries if we are building with RPC support (server or client)
IF( WITH_RPC )
  find_package( Boost 1.36.0 )
  IF( Boost_FOUND )
    MESSAGE(STATUS "Boost Include:"  ${Boost_INCLUDE_DIRS})
    include_directories( ${Boost_INCLUDE_DIRS} )
  ELSE( Boost_FOUND )
    MESSAGE( "Can't find Boost! Building without RPC support" )
    SET( WITH_RPC OFF )
  ENDIF( Boost_FOUND )
ENDIF( WITH_RPC )


# we'll also need pthread
IF( WITH_RPC )
  find_package(Threads)
  IF ( Threads_FOUND )
     MESSAGE(STATUS "Threads Include:"  ${Threads_INCLUDE_DIRS})
  ENDIF (Threads_FOUND)
  IF( NOT CMAKE_USE_PTHREADS_INIT )
    MESSAGE( "You are lacking the pthreads library!  Building without RPC support" )
    SET( WITH_RPC OFF )
  ENDIF( NOT CMAKE_USE_PTHREADS_INIT )
ENDIF( WITH_RPC)

# Create and install the pkgconfig file from the .in template
SET( PKGFILE ${CMAKE_CURRENT_SOURCE_DIR}/autolab-rapi.pc )
CONFIGURE_FILE( ${PKGFILE}.in ${PKGFILE} @ONLY)
INSTALL( FILES ${PKGFILE} DESTINATION lib/pkgconfig/)

#IF (STAGE_FOUND)
  IF (WITH_STAGE)
    MESSAGE(STATUS "lib-rapi-stage will be built!")
    SET( PKGFILE ${CMAKE_CURRENT_SOURCE_DIR}/autolab-rapi-stage.pc )
    CONFIGURE_FILE( ${PKGFILE}.in ${PKGFILE} @ONLY)
    INSTALL( FILES ${PKGFILE} DESTINATION lib/pkgconfig/)
  ELSE(WITH_STAGE)
    MESSAGE(STATUS "lib-rapi-stage will NOT be build, enable with WITH_STAGE")
  ENDIF(WITH_STAGE)
#ENDIF (STAGE_FOUND)

IF (WITH_CB)
  SET( PKGFILE ${CMAKE_CURRENT_SOURCE_DIR}/autolab-rapi-chatterbox.pc )
  CONFIGURE_FILE( ${PKGFILE}.in ${PKGFILE} @ONLY)
  INSTALL( FILES ${PKGFILE} DESTINATION lib/pkgconfig/)
ELSE(WITH_CB)
  MESSAGE(STATUS "lib-rapi-chatterbox will NOT be build, enable with WITH_CB")
ENDIF(WITH_CB)

IF (WITH_TRACKER)
  SET( PKGFILE ${CMAKE_CURRENT_SOURCE_DIR}/autolab-rapi-autolabtracker.pc )
  CONFIGURE_FILE( ${PKGFILE}.in ${PKGFILE} @ONLY)
  INSTALL( FILES ${PKGFILE} DESTINATION lib/pkgconfig/)
ELSE(WITH_TRACKER)
  MESSAGE(STATUS "lib-rapi-autolabtracker will NOT be build, enable with WITH_TRACKER")
ENDIF(WITH_TRACKER)

IF (QT_FOUND)
  IF (WITH_GUI)
    SET( PKGFILE ${CMAKE_CURRENT_SOURCE_DIR}/autolab-rapi-gui.pc )
    CONFIGURE_FILE( ${PKGFILE}.in ${PKGFILE} @ONLY)
    INSTALL( FILES ${PKGFILE} DESTINATION lib/pkgconfig/)
  ELSE(WITH_GUI)
    MESSAGE(STATUS "lib-rapi-gui will NOT be build, enable with WITH_GUI")
  ENDIF(WITH_GUI)
ENDIF (QT_FOUND)

IF (WITH_RPC)
  SET( PKGFILE ${CMAKE_CURRENT_SOURCE_DIR}/autolab-rapi-rpc.pc )
  CONFIGURE_FILE( ${PKGFILE}.in ${PKGFILE} @ONLY)
  INSTALL( FILES ${PKGFILE} DESTINATION lib/pkgconfig/)
ELSE(WITH_RPC)
  MESSAGE(STATUS "lib-rapi-rpc will NOT be build, enable with WITH_RPC")
ENDIF(WITH_RPC)

# Add all the directories
ADD_DEFINITIONS(-g -Wall -O2)
ADD_SUBDIRECTORY(librapicore)

IF (WITH_STAGE)
 ADD_SUBDIRECTORY(librapistage)
ENDIF(WITH_STAGE)

if (WITH_CB)
  ADD_SUBDIRECTORY(librapichatterbox)
ENDIF(WITH_CB)

IF (WITH_GUI)
  ADD_SUBDIRECTORY(librapigui)
  ADD_DEFINITIONS(-D_RAPI_GUI)
  MESSAGE(STATUS "Building Rapi graphics support")
ELSE(WITH_GUI)
  MESSAGE(STATUS "Graphics support disabled")
ENDIF(WITH_GUI)

IF (WITH_RPC)
  ADD_SUBDIRECTORY(librapirpc)
ENDIF (WITH_RPC)

IF (WITH_EXAMPLES)
  ADD_SUBDIRECTORY(examples)
ENDIF(WITH_EXAMPLES)

IF (WITH_TRACKER)
  ADD_SUBDIRECTORY(librapiautolabtracker)
ENDIF(WITH_TRACKER)
