# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, wrong-import-position, unused-import,
# pylint: disable=import-error

"""
This package provides the user-facing python part of libsemigroups_pybind11
"""

import pkgconfig

DISCLAIMER = (
    "(You should not see this message unless you are installing libsemigroups_pybind11 from its "
    "sources. If you are not installing from the sources, please raise an issue at "
    "https://github.com/libsemigroups/libsemigroups_pybind11)"
)

assert pkgconfig.exists("libsemigroups")

try:
    from _libsemigroups_pybind11 import (
        NEGATIVE_INFINITY,
        POSITIVE_INFINITY,
        LIMIT_MAX,
        UNDEFINED,
        error_message_with_prefix,
        Forest,
        Gabow,
        ReportGuard,
        StringRange,
        ToString,
        ToWord,
        WordGraph,
        WordRange,
        random_word,
        random_string,
        random_strings,
        congruence_kind,
        number_of_words,
        Order,
        lexicographical_compare,
        recursive_path_compare,
        shortlex_compare,
        overlap,
        to_presentation,
        to_inverse_presentation,
        LibsemigroupsError,
        is_obviously_infinite,
        BMat8,
        side,
        AhoCorasick,
        Bipartition,
        Blocks,
        freeband_equal_to,
        to_forest,
        Meeter,
        to_word_graph,
        Joiner,
        Dot,
        PBR,
        Ukkonen,
        tril,
        Sims1,
        Sims2,
    )
except ModuleNotFoundError as e:
    raise ModuleNotFoundError(
        (
            f'{e.msg}, did you forget to run "pip install ." in the libsemigroups_pybind11 '
            f"directory? {DISCLAIMER}"
        )
    ) from e

from .detail.dot import _Dot as Dot

from .action import Action, RightAction, LeftAction
from .adapters import ImageRightAction, ImageLeftAction


# from .knuth_bendix import KnuthBendix
from .matrix import _Matrix as Matrix, _MatrixKind as MatrixKind
from .presentation import Presentation, InversePresentation
from .transf import (
    PPerm,
    Transf,
    Perm,
    one,
    inverse,
    image,
    domain,
    right_one,
    left_one,
)

# The following are imported from path since we modify the methods count and max
# to return the POSITIVE_INFINITY object where applicable.
from .paths import Paths, ReversiblePaths

from .todd_coxeter import ToddCoxeter

# The following fools sphinx into thinking that MatrixKind is not an alias.
MatrixKind.__module__ = __name__
MatrixKind.__name__ = "MatrixKind"

from .froidure_pin import FroidurePin
from .schreier_sims import SchreierSims
