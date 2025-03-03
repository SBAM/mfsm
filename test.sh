#! env bash

TPUTRESET=$(tput sgr0 2> /dev/null)
GREEN=$(tput setaf 2 2> /dev/null)
GREY=$(tput setaf 7 2> /dev/null)
RED=$(tput setaf 9 2> /dev/null)

function green_echo
{
  echo -e $GREEN$@$TPUTRESET
}

function grey_echo
{
  echo -e "$GREY$@$TPUTRESET"
}

function red_echo
{
  echo -e $RED$@$TPUTRESET
}

SCRIPT_LOCATION=$(readlink -f $0)
BASEDIR=$(dirname $SCRIPT_LOCATION)

COMPILER=clang

function invoke_conan_build
{
  conan_cmd=$(cat <<-END
    conan test $BASEDIR
      --build=missing
      --settings:build build_type=$1
      --settings:host build_type=$1
      $2
      $3
      mfsm/*
END
)
  green_echo "Invoking conan"
  grey_echo "$conan_cmd"
  $conan_cmd
}

function invoke_conan
{
  case $COMPILER in
    clang)
      profs=(linux clang) ;;
    gcc)
      profs=(linux gnu) ;;
  esac
  profs_h=$(printf -- "-pr:h $BASEDIR/conan_profiles/%s " "${profs[@]}")
  profs_b=$(printf -- "-pr:b $BASEDIR/conan_profiles/%s " "${profs[@]}")
  invoke_conan_build "Debug" "$profs_h" "$profs_b"
  invoke_conan_build "Release" "$profs_h" "$profs_b"
}

USAGE_STR="usage: $0 [-c|--compiler <clang|gcc>] [-h|--help]"

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
    -o c:h \
    -l compiler:,help \
    -n $0 -- $@)

if [ $? -ne 0 ]; then
  usage_die
fi

while true; do
  case $1 in
    -c|--compiler)
      case $2 in
        clang|gcc) COMPILER=$2 ;;
        *) usage_die ;;
      esac; shift 2 ;;
    -h|--help) usage_help ;;
    *) break ;;
  esac
done

invoke_conan
