#!/bin/bash

# Returns filenames and directories, appending a slash to directory names.
# Based on https://stackoverflow.com/a/40227233
_civ_complete_filenames() {
    local cur="$1"
    local suffix="$2"

    if [ -z "$suffix" ]; then
        # Files, excluding directories:
        grep -v -F -f <(compgen -d -P ^ -S '$' -- "$cur") \
            <(compgen -f -P ^ -S '$' -- "$cur") |
            sed -e 's/^\^//' -e 's/\$$/ /'
    else
        # Files with correct suffix, excluding directories:
        grep -v -F -f <(compgen -d -P ^ -S '$' -- "$cur") \
            <(compgen -f -P ^ -S '$' -- "$cur") |
            sed -e 's/^\^//' -e 's/\$$/ /' | grep "\.${suffix}\s*\$"
    fi

    # Directories:
    compgen -d -S / -- "$cur"
}

_civas() {
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    opts="--help -o"

    if [[ ${prev} == "--help" ]]; then
        return 0
    fi

    if [[ ${prev} == "-o" ]]; then
        compopt -o nospace
        COMPREPLY=( $(_civ_complete_filenames "$cur" "o") )
        return 0
    fi

    if [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi

    compopt -o nospace
    COMPREPLY=( $(_civ_complete_filenames "$cur" "s") )
    return 0
}

_civvm() {
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    opts="--size --instrs --help"

    if [[ ${prev} == "--help" ]]; then
        return 0
    fi

    if [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi

    compopt -o nospace
    COMPREPLY=( $(_civ_complete_filenames "$cur" "o") )
    return 0
}

_civcc_breakpoints="SPdoScanParse ContextAnalysisPhase ContextAnalysisPhase. RemoveVariableInitializationPhase RemoveVariableInitializationPhase. TypeCheckingPhase TypeCheckingPhase. PreCompilationPhase PreCompilationPhase. OptimisationPhase OptimisationPhase. AssemblePhase AssemblePhase."
_civcc_contextbreakpoints="ContextAnalysisPhase ContextAnalysisPhase.HoistGlobals ContextAnalysisPhase.RemoveArrayExpressions ContextAnalysisPhase.VarSymTable ContextAnalysisPhase.FunSymTable ContextAnalysisPhase.HoistFor"
_civcc_remvarbreakpoints="RemoveVariableInitializationPhase RemoveVariableInitializationPhase.RemoveLocalDeclarations RemoveVariableInitializationPhase.RemoveGlobalDeclarations"
_civcc_typecheckbreakpoints="TypeCheckingPhase TypeCheckingPhase.BasicTypeChecking TypeCheckingPhase.ReturnTypeChecking"
_civcc_precompbreakpoints="PreCompilationPhase PreCompilationPhase.ParameterPassingArrays PreCompilationPhase.ArrayDimensionReduction PreCompilationPhase.CompileDisjunctionConjunction PreCompilationPhase.CompileGeneralCast PreCompilationPhase.ForLoopToWhile PreCompilationPhase.RenameExternArrayDims PreCompilationPhase.RemoveVarstid"
_civcc_optbreakpoints="OptimisationPhase OptimisationPhase.OPdoOptimisation"
_civcc_asmbreakpoints="AssemblePhase AssemblePhase.ConstructConstantTable AssemblePhase.FunctionLabelGeneration AssemblePhase.CodeGeneration"

_civcc() {
    local cur prev opts noopts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    opts="-v -b -l -o -O -r -s -V -? -h --verbose --breakpoint --max-optimisation-loop --output --extra-opt --no- --structure --verbose-optimisation --help --usage"
    noopts="--no-opt --no-algebraic-simplification --no-cast-simplification --no-code-after-return-removal --no-propagation --no-redundant-branches-removal --no-unused-function-removal --no-unused-varlet-removal --no-unused-varste-removal"

    if [[ ${prev} == "-h" || ${prev} == "-?" || ${prev} == "-s" || ${prev} == "--usage" || ${prev} == "--help" || ${prev} == "--structure" ]]; then
        return 0
    fi

    if [[ $prev == "-v" || $prev == "--verbose" || $prev == "-V" || $prev == "--verbose-optimisation" ]]; then
        COMPREPLY=( $(compgen -W "0 1 2 3" -- ${cur}) )
        return 0
    fi

    if [[ $prev == "-l" || $prev == "--max-optimisation-loop" ]]; then
        return 0
    fi

    if [[ $prev == "-b" || $prev == "--breakpoint" ]]; then
        if [[ ${cur} == ContextAnalysisPhase* ]]; then
            COMPREPLY=( $(compgen -W "${_civcc_contextbreakpoints}" -- ${cur}) )
        elif [[ ${cur} == RemoveVariableInitializationPhase* ]]; then
            COMPREPLY=( $(compgen -W "${_civcc_remvarbreakpoints}" -- ${cur}) )
        elif [[ ${cur} == TypeCheckingPhase* ]]; then
            COMPREPLY=( $(compgen -W "${_civcc_typecheckbreakpoints}" -- ${cur}) )
        elif [[ ${cur} == PreCompilationPhase* ]]; then
            COMPREPLY=( $(compgen -W "${_civcc_precompbreakpoints}" -- ${cur}) )
        elif [[ ${cur} == OptimisationPhase* ]]; then
            COMPREPLY=( $(compgen -W "${_civcc_optbreakpoints}" -- ${cur}) )
        elif [[ ${cur} == AssemblePhase* ]]; then
            COMPREPLY=( $(compgen -W "${_civcc_asmbreakpoints}" -- ${cur}) )
        else
            COMPREPLY=( $(compgen -W "${_civcc_breakpoints}" -- ${cur}) )
        fi
        return 0
    fi

    if [[ ${prev} == "-o" || ${prev} == "--output" ]]; then
        compopt -o nospace
        COMPREPLY=( $(_civ_complete_filenames "$cur" "s") )
        return 0
    fi

    if [[ ${cur} == --n* ]] ; then
        COMPREPLY=( $(compgen -W "${noopts}" -- ${cur}) )
        return 0
    fi

    if [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi

    compopt -o nospace
    COMPREPLY=( $(_civ_complete_filenames "$cur" "cvc") )
    return 0
}

_civrun() {
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    opts="-v -k -b -l -o -O -r -s -V -h --verbose --breakpoint --keep-assembly --max-optimisation-loop --output --extra-opt --no-opt --structure --verbose-optimisation --size --instrs --vm-verbose --help --usage"

    if [[ ${prev} == "-h" || ${prev} == "-s" || ${prev} == "--usage" || ${prev} == "--help" || ${prev} == "--structure" ]]; then
        return 0
    fi

    if [[ $prev == "-v" || $prev == "--verbose" || $prev == "-V" || $prev == "--verbose-optimisation" ]]; then
        COMPREPLY=( $(compgen -W "0 1 2 3" -- ${cur}) )
        return 0
    fi

    if [[ $prev == "-l" || $prev == "--max-optimisation-loop" ]]; then
        return 0
    fi

    if [[ $prev == "-b" || $prev == "--breakpoint" ]]; then
        if [[ ${cur} == ContextAnalysisPhase* ]]; then
            COMPREPLY=( $(compgen -W "${_civcc_contextbreakpoints}" -- ${cur}) )
        elif [[ ${cur} == RemoveVariableInitializationPhase* ]]; then
            COMPREPLY=( $(compgen -W "${_civcc_remvarbreakpoints}" -- ${cur}) )
        elif [[ ${cur} == TypeCheckingPhase* ]]; then
            COMPREPLY=( $(compgen -W "${_civcc_typecheckbreakpoints}" -- ${cur}) )
        elif [[ ${cur} == PreCompilationPhase* ]]; then
            COMPREPLY=( $(compgen -W "${_civcc_precompbreakpoints}" -- ${cur}) )
        elif [[ ${cur} == OptimisationPhase* ]]; then
            COMPREPLY=( $(compgen -W "${_civcc_optbreakpoints}" -- ${cur}) )
        elif [[ ${cur} == AssemblePhase* ]]; then
            COMPREPLY=( $(compgen -W "${_civcc_asmbreakpoints}" -- ${cur}) )
        else
            COMPREPLY=( $(compgen -W "${_civcc_breakpoints}" -- ${cur}) )
        fi
        return 0
    fi

    if [[ ${prev} == "-o" || ${prev} == "--output" ]]; then
        compopt -o nospace
        COMPREPLY=( $(_civ_complete_filenames "$cur" "s") )
        return 0
    fi

    if [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi

    compopt -o nospace
    COMPREPLY=( $(_civ_complete_filenames "$cur" "cvc") )
    return 0
}

complete -F _civas civas
complete -F _civvm civvm
complete -F _civcc civcc
complete -F _civrun civrun
