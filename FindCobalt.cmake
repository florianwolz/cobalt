#   Copyright 2017 The Cobalt Authors.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.


# - Try to find Cobalt include dirs and libraries
#
# Usage of this module as follows:
#
#    FIND_PACKAGE(Cobalt)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#   COBALT_ROOT_DIR          Set this variable to the root installation of
#                            Cobalt if the module has problems finding the
#                            proper installation path.
#
# Variables defined by this module:
#
#   COBALT_FOUND            System has Cobalt, include and lib dirs found
#   COBALT_INCLUDE_DIRS     The Cobalt include directories.
#   COBALT_LIBRARY          The Cobalt library.

SET(COBALT_ROOT_DIR "${PROJECT_SOURCE_DIR}/thirdparty/COBALT_FOUND")
SET(COBALT_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/thirdparty/cobalt")
SET(COBALT_LIBRARY "")

IF(COBALT_INCLUDE_DIRS AND COBALT_LIBRARY)
    SET(COBALT_FOUND TRUE)
ELSE(COBALT_INCLUDE_DIRS AND COBALT_LIBRARY)
    SET(COBALT_FOUND FALSE)
ENDIF(COBALT_INCLUDE_DIRS AND COBALT_LIBRARY)

MARK_AS_ADVANCED(
    COBALT_INCLUDE_DIRS
    COBALT_LIBRARY
)
