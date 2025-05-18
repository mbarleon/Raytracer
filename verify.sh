#!/usr/bin/env bash

##################################################################
# INFO: Logger functions with color support
##################################################################

export GREEN="\033[1;32m"
export RED="\033[1;31m"
export ILC="\033[3m"
export ORG="\033[1;33m"
export YELLOW="\033[1;33m"
export BOLD="\033[1m"
export RST="\033[0m"

function _error() {
    echo -e "${RED}${BOLD}[❌] ERROR:\n${RST}\t$1\n\t${ILC}\"$2\"${RST}"
    exit 84
}

function _warning() {
    echo -e "${YELLOW}${BOLD}[⚠️] WARNING:\n${RST}\t$1\n\t${ILC}\"$2\"${RST}"
}

function _success() {
    echo -e "${GREEN}[✅] SUCCESS:\t${RST} ${ILC}$1${RST}"
}

function _info() {
    echo -e "${ORG}[🚧] RUNNING:\t${RST} ${ILC}$1${RST}"
}

##################################################################
# INFO: C++ code verification using clang-tidy
##################################################################

JOBS=$(nproc 2>/dev/null || echo 1)
DIRS=(src include)
CHECKS="*,-llvm-header-guard,modernize-*,performance-*,readability-*,bugprone-*,clang-analyzer-*"
COMPILER_FLAGS="-- -std=c++20 -Wall -Wextra -pedantic -Werror -Wconversion"

command -v clang-tidy >/dev/null 2>&1 \
    || _error "clang-tidy not found in PATH" "Please install clang-tidy and try again"

_info "Locating source files in ${DIRS[*]}"
FILES=()
while IFS= read -r f; do
    FILES+=("$f")
done < <(find "${DIRS[@]}" -type f \( -iname '*.cpp' -o -iname '*.hpp' \))

if [[ ${#FILES[@]} -eq 0 ]]; then
    _warning "No C/C++ source files found under: ${DIRS[*]}" ""
    exit 0
fi

_info "Found ${#FILES[@]} files; running on up to $JOBS parallel jobs"

##################################################################
# INFO: Run clang-tidy on a single file
##################################################################
function run_tidy()
{
    local file="$1"
    _info "Analyzing: $file"
    local output
    output=$(clang-tidy -checks="$CHECKS" "$file" $COMPILER_FLAGS 2>&1)
    local rc=$?

    if [[ $rc -ne 0 ]]; then
        _error "clang-tidy failed on $file" "$output"
    fi

    if grep -q "warning:" <<<"$output"; then
        local count
        count=$(grep -c "warning:" <<<"$output")
        _warning "$count warning(s) in $file" "$output"
    else
        _success "$file is clean"
    fi
}

export -f _error _warning _success _info run_tidy

printf "%s\n" "${FILES[@]}" \
    | xargs -P "$JOBS" -I{} bash -c 'run_tidy "{}"'

_success "All files processed successfully"
