cmake_minimum_required(VERSION 2.8)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

SET( LIB SiTNetwork )
SET( PROJECT_ROOT "${CMAKE_SOURCE_DIR}/" )

SET( MAJOR_VERSION 1.0.0 )
SET( MINOR_VERSION 1.0 )

include(ExternalProject)
include(shared)
IS_LINUX()


IF(LINUX)
SET( LIB_INSTALL_PATH /usr/local/lib )
SET( HEADERS_INSTALL_PATH /usr/local/include/SiTNetwork )
ENDIF(LINUX)

IF(WIN32)
ADDDEF(WINDOWS)
SET( LIB_INSTALL_PATH c:/windows/system32)
SET( HEADERS_INSTALL_PATH /include)
ENDIF(WIN32)

#--------------------------------------------------------------------
# Set source files.
#--------------------------------------------------------------------
include(FindSourceProject)
FIND_SOURCES(${PROJECT_ROOT}source)

#--------------------------------------------------------------------
# Set include.
#--------------------------------------------------------------------
FIND_HEADERS(${PROJECT_ROOT}include)
SET( HEADERS ${PROJECT_ROOT}include/ )

#--------------------------------------------------------------------
# Set Threads.
#--------------------------------------------------------------------
find_package (Threads REQUIRED)
SET( REQUIRED_LIBRARIES ${REQUIRED_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

#--------------------------------------------------------------------
# Set OpenSSL.
#--------------------------------------------------------------------
IF(NOT EXISTS ${PROJECT_ROOT}lib/OpenSSL)
ExternalProject_Add(
        OpenSSL
        GIT_REPOSITORY "https://github.com/openssl/openssl.git"
	PREFIX ${PROJECT_ROOT}lib/OpenSSL
	CONFIGURE_COMMAND ./config -fPIC --prefix=${PROJECT_ROOT}lib/OpenSSL
	BUILD_COMMAND make depend && make
	INSTALL_COMMAND make install
	BUILD_IN_SOURCE 1
)
ENDIF()

IF(LINUX)
add_library(OpenSSL STATIC IMPORTED)
set_property(TARGET OpenSSL PROPERTY
    IMPORTED_LOCATION ${PROJECT_ROOT}lib/OpenSSL/lib/libssl.a)

add_library(crypto STATIC IMPORTED)
set_property(TARGET crypto PROPERTY
    IMPORTED_LOCATION ${PROJECT_ROOT}lib/OpenSSL/lib/libcrypto.a)
ENDIF(LINUX)

SET( REQUIRED_LIBRARIES ${REQUIRED_LIBRARIES} OpenSSL)
SET( REQUIRED_LIBRARIES ${REQUIRED_LIBRARIES} crypto)

SET( HEADERS ${HEADERS} ${PROJECT_ROOT}lib/OpenSSL/include)

#--------------------------------------------------------------------
# Set DL.
#--------------------------------------------------------------------
IF(LINUX)
SET( REQUIRED_LIBRARIES ${REQUIRED_LIBRARIES} -ldl)
ENDIF(LINUX)

#--------------------------------------------------------------------
# Compile
#--------------------------------------------------------------------
IF(NOT MSVC)
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -o0 -g3 -std=c++11")
ENDIF()

INCLUDE_DIRECTORIES(${HEADERS})
LINK_DIRECTORIES( ${LIB_INSTALL_PATH} )

ADD_LIBRARY( ${LIB} STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})

TARGET_LINK_LIBRARIES( ${LIB} ${REQUIRED_LIBRARIES})

SET_TARGET_PROPERTIES(${LIB}
	PROPERTIES 
	VERSION ${MAJOR_VERSION}
	SOVERSION ${MINOR_VERSION}
)

INSTALL( TARGETS ${LIB} DESTINATION lib )
INSTALL( DIRECTORY ${PROJECT_ROOT}/include/
         DESTINATION include/SiTNetwork
         FILES_MATCHING PATTERN "*.h" )
