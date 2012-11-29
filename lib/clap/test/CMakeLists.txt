add_executable ( cli CLITest.cpp )
include_directories ( ${libCLAP} )
link_directories ( ${libCLAP} )
target_link_Libraries ( cli CLAP )
