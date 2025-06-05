#
# Copyright (c) since 2025 by PopolonY2k and Leidson Campos A. Ferreira
# 
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
# claim that you wrote the original software. If you use this software
# in a product, an acknowledgment in the product documentation would be
# appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
# misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
#

############################################################
# cmake helper script with default parameters to help 
# clean compilation (everithing will be on build diretory, 
# instead sparsed through src and test directories). 
############################################################

build_type=$1

if [ -z "$build_type" ]; then
    build_type="debug"
fi

cmake -DCMAKE_BUILD_TYPE:STRING=$build_type -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/clang -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++ -B./build -G "Unix Makefiles"
