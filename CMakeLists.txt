cmake_minimum_required(VERSION 2.8)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

SET( LIB SiTNetwork )
SET( PROJECT_ROOT "${CMAKE_SOURCE_DIR}/" )

SET( LIB_INSTALL_PATH /usr/local/lib )
SET( HEADERS_INSTALL_PATH /usr/local/include/SiTNetwork )

SET( MAJOR_VERSION 1.0.0 )
SET( MINOR_VERSION 1.0 )

IF(UNIX AND NOT APPLE)
    SET(LINUX ON)
ENDIF(UNIX AND NOT APPLE)

IF(WIN32)
	MESSAGE(SEND_ERROR "WINDOWS is unsupported yet ;)")
ENDIF(WIN32)

#--------------------------------------------------------------------
# Set source files.
#--------------------------------------------------------------------
AUX_SOURCE_DIRECTORY( ${PROJECT_ROOT}/source SOURCES )
AUX_SOURCE_DIRECTORY( ${PROJECT_ROOT}/source/Network SOURCES_NETWORK )
AUX_SOURCE_DIRECTORY( ${PROJECT_ROOT}/source/Network/Client SOURCES_NETWORK_CLIENT )
AUX_SOURCE_DIRECTORY( ${PROJECT_ROOT}/source/Network/Server SOURCES_NETWORK_SERVER )
AUX_SOURCE_DIRECTORY( ${PROJECT_ROOT}/source/Http SOURCES_HTTP )
AUX_SOURCE_DIRECTORY( ${PROJECT_ROOT}/source/Http/Client SOURCES_HTTP_CLIENT )
AUX_SOURCE_DIRECTORY( ${PROJECT_ROOT}/source/Http/Server SOURCES_HTTP_SERVER )

SET(PROJECT_SOURCES
    ${SOURCES}
    ${SOURCES_NETWORK}
    ${SOURCES_NETWORK_CLIENT}
    ${SOURCES_NETWORK_SERVER}
    ${SOURCES_HTTP}
    ${SOURCES_HTTP_CLIENT}
    ${SOURCES_HTTP_SERVER}
)
#--------------------------------------------------------------------
# Set include.
#--------------------------------------------------------------------
SET( HEADERS ${PROJECT_ROOT}/include/ )

#--------------------------------------------------------------------
# Set Threads.
#--------------------------------------------------------------------
find_package (Threads)
SET( REQUIRED_LIBRARIES ${REQUIRED_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

#--------------------------------------------------------------------
# Set OpenSSL.
#--------------------------------------------------------------------
find_package (OpenSSL)
SET( HEADERS ${HEADERS} ${OPENSSL_INCLUDE_DIR})
SET( REQUIRED_LIBRARIES ${REQUIRED_LIBRARIES} ${OPENSSL_LIBRARIES})

#--------------------------------------------------------------------
# Compile
#--------------------------------------------------------------------
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -o0 -g3 -std=c++11")

INCLUDE_DIRECTORIES(${HEADERS})
LINK_DIRECTORIES( ${LIB_INSTALL_PATH} )

#ADD_LIBRARY( ${LIB} SHARED ${PROJECT_SOURCES} )
ADD_EXECUTABLE( ${LIB} ${PROJECT_SOURCES} )

TARGET_LINK_LIBRARIES( ${LIB} ${REQUIRED_LIBRARIES})

SET_TARGET_PROPERTIES(${LIB}
	PROPERTIES 
	VERSION ${MAJOR_VERSION}
	SOVERSION ${MINOR_VERSION}
)

INSTALL( TARGETS ${LIB} DESTINATION ${LIB_INSTALL_PATH} )
INSTALL( DIRECTORY ${HEADERS}/
         DESTINATION ${HEADERS_INSTALL_PATH}
         FILES_MATCHING PATTERN "*.h" )
