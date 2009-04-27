cmake_minimum_required(VERSION 2.6)

PROJECT(autolab-rapi)
SET( V_MAJOR 0 )
SET( V_MINOR 1 )
SET( V_BUGFIX 0 )

SET( VERSION ${V_MAJOR}.${V_MINOR}.${V_BUGFIX} )
SET( APIVERSION ${V_MAJOR}.${V_MINOR} )

ADD_DEFINITIONS(-g -Wall -O2)
ADD_SUBDIRECTORY(librapicore)
ADD_SUBDIRECTORY(librapistage)
ADD_SUBDIRECTORY(librapichatterbox)
#ADD_SUBDIRECTORY(test_stage)

# Create and install the pkgconfig file from the .in template
SET( PKGFILE ${CMAKE_CURRENT_SOURCE_DIR}/autolab-rapi.pc )
CONFIGURE_FILE( ${PKGFILE}.in ${PKGFILE} @ONLY)
INSTALL( FILES ${PKGFILE} DESTINATION lib/pkgconfig/)

SET( PKGFILE ${CMAKE_CURRENT_SOURCE_DIR}/autolab-rapi-stage.pc )
CONFIGURE_FILE( ${PKGFILE}.in ${PKGFILE} @ONLY)
INSTALL( FILES ${PKGFILE} DESTINATION lib/pkgconfig/)

SET( PKGFILE ${CMAKE_CURRENT_SOURCE_DIR}/autolab-rapi-chatterbox.pc )
CONFIGURE_FILE( ${PKGFILE}.in ${PKGFILE} @ONLY)
INSTALL( FILES ${PKGFILE} DESTINATION lib/pkgconfig/)