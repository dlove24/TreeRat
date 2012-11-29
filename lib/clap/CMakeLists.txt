
project ( libCLAP )

add_subdirectory ( CLAP )
add_subdirectory ( Common )
add_subdirectory ( test )

add_library ( CLAP SHARED
              CLAP/CLI.cpp
              CLAP/Exception.cpp
              CLAP/ExecutionPlan.cpp
              CLAP/Option.cpp
              CLAP/Stage.cpp
              CLAP/SubCommand.cpp
              CLAP/Switch.cpp
              Common/Exception.cpp )
set ( CMAKE_INSTALL_PREFIX /usr/local/ )

INSTALL_FILES ( /include/CLAP/Common FILES ./Common/Exception.h
                                           ./Common/Types.h )
INSTALL_FILES ( /include/CLAP/CLAP FILES ./CLAP/CLI.h
                                         ./CLAP/Exception.h
                                         ./CLAP/ExecutionPlan.h
                                         ./CLAP/Option.h
                                         ./CLAP/Stage.h
                                         ./CLAP/SubCommand.h
                                         ./CLAP/Switch.h
                                         ./CLAP/Types.h )
INSTALL_FILES ( /share/ FILES ./libCLAP.so )
add_test ( test1 test/cli -1 c1 -2stuff c2 )
set ( CMAKE_CXX_FLAGS_DEBUG -g -DDEBUG )
set ( CMAKE_CXX_FLAGS_DEBUG -g -DDEBUG )
set ( CMAKE_C_FLAGS_DEBUG -g -DDEBUG )
set ( CMAKE_CXX_FLAGS_RELEASE -O2 -fPIC )
set ( CMAKE_C_FLAGS_RELEASE -O2 -fPIC )
include_directories ( ${libCLAP} )
link_directories ( ${libCLAP} )
enable_testing ()
