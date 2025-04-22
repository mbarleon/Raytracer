#!/usr/bin/env bash

GREEN="\033[1;32m"
RED="\033[1;31m"
ILC="\033[3m"
ORANGE="\033[38;5;214m"
RST="\033[0m"

function _error()
{
    echo -e "${RED}${BOLD}[❌] ERROR:\n${RST}\t$1\n\t${ILC}\"$2\"${RST}"
    exit 84
}

function _success()
{
    echo -e "${GREEN}[✅] SUCCESS:\t${RST} ${ILC}$1${RST}"
}

function _all()
{
    if ! command -v cmake 2>&1 >/dev/null; then
        _error "command 'cmake' not found" "please install 'cmake' or 'nix develop' 🤓"
    fi
    _success "command 'cmake' found, building..."
    mkdir -p build
    cd build
    cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
    # INFO: Epitech's moulinette does: `cmake --build .` but this is slow as fuck
    make -j$(nproc)
    if [[ $? -eq 0 ]]; then
        _success "compiled raytracer"
        exit 0
    fi
    _error "compilation error" "failed to compile raytracer"
    exit 84
}

function _clean()
{
    rm -rf build
}

function _fclean()
{
    _clean
    rm -rf lib raytracer
}

for args in "$@"
do
    case $args in
        -h|--help)
            cat << EOF
USAGE:
      $0    builds raytracer project

ARGUMENTS:
      $0 [-h|--help]    displays this message
      $0 [-c|--clean]   clean the project
      $0 [-f|--fclean]  fclean the project
EOF
        exit 0
        ;;
    -c|--clean)
        _clean
        exit 0
        ;;
    -f|--fclean)
        _fclean
        exit 0
        ;;
    -r|--re)
        _fclean
        _all
        exit 0
        ;;
    *)
        _error "Invalid arguments: " $args
    esac
done

_all

