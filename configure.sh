#!/bin/bash

TPUTRESET=$(tput sgr0 2> /dev/null)
GREEN=$(tput setaf 2 2> /dev/null)
GREY=$(tput setaf 7 2> /dev/null)
RED=$(tput setaf 9 2> /dev/null)
YELLOW=$(tput setaf 11 2> /dev/null)

function green_echo
{
  echo -e $GREEN$@$TPUTRESET
}

function grey_echo
{
  echo -e $GREY$@$TPUTRESET
}

function red_echo
{
  echo -e $RED$@$TPUTRESET
}

CONFIGURE_LOCATION=$(readlink -f $0)
BASEDIR=$(dirname $CONFIGURE_LOCATION)

COMPILER=gcc

# $1 path
# $2 Debug/Release
# $N additional flags
function cmake_config
{
  OUTPUT_PATH=$1
  shift
  mkdir -p $OUTPUT_PATH
  if [ 0 -eq $? ]; then
    green_echo "[Will build artifacts in $OUTPUT_PATH]"
  else
    red_echo "[Could not create $OUTPUT_PATH]"
  fi
  pushd > /dev/null $OUTPUT_PATH 2>&1
  grey_echo "  ==> compiler=$YELLOW$COMPILER"
  cmake $@ $BASEDIR
  #cmake --trace $@ $BASEDIR
  RES=$?
  popd > /dev/null 2>&1
  if [ $RES -ne 0 ]; then
    exit 1
  fi
}

function invoke_cmake
{
  DEBUGDIR=$BASEDIR/build/Debug
  RELEASEDIR=$BASEDIR/build/Release
  case $COMPILER in
    clang)
      export CXX=$(which clang++)
      export TOOLCHAIN_FILE=ClangToolchain.cmake ;;
    gcc)
      export CXX=$(which g++)
      export TOOLCHAIN_FILE=GNUToolchain.cmake ;;
  esac
  shift
  cmake_config $DEBUGDIR \
               -DCMAKE_BUILD_TYPE=Debug \
               -DCMAKE_TOOLCHAIN_FILE:string=$TOOLCHAIN_FILE \
               -DCMAKE_INSTALL_PREFIX=$CMAKE_INSTALL_PREFIX \
               $@
  cmake_config $RELEASEDIR \
               -DCMAKE_BUILD_TYPE=Release \
               -DCMAKE_TOOLCHAIN_FILE:string=$TOOLCHAIN_FILE \
               -DCMAKE_INSTALL_PREFIX=$CMAKE_INSTALL_PREFIX \
               $@
}

USAGE_STR=\
"usage: $0 \
[-c|--compiler <gcc|clang>] \
[-p|--prefix <install_path>] \
[-h|--help]"

function usage_die()
{
  red_echo $USAGE_STR
  exit 1
}

function usage_help()
{
  green_echo $USAGE_STR
  exit 0
}

GETOPT_CMD=\
$(getopt \
    -o c:p:h \
    -l compiler:,prefix:,help \
    -n $0 -- $@)

if [ $? -ne 0 ]; then
  usage_die
fi

while true; do
  case $1 in
    -c|--compiler)
      case $2 in
        gcc|clang) COMPILER=$2 ;;
        *) usage_die ;;
      esac; shift 2 ;;
    -p|--prefix)
      export CMAKE_INSTALL_PREFIX=$2; shift 2 ;;
    -h|--help) usage_help ;;
    *) break ;;
  esac
done

invoke_cmake
