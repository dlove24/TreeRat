#!/bin/sh

##
## Restyle all C/C++ code into the project format
##

astyle  --style=banner --indent=spaces=2 --indent-preprocessor --indent-col1-comments --break-blocks=all --pad-oper --pad-paren-out --unpad-paren --add-brackets --convert-tabs --align-pointer=type --recursive lib/*.cpp lib/*.h

astyle  --style=banner --indent=spaces=2 --indent-preprocessor --indent-col1-comments --break-blocks=all --pad-oper --pad-paren-out --unpad-paren --add-brackets --convert-tabs --align-pointer=type bin/main.cpp
