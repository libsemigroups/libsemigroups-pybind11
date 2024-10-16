# -*- coding: utf-8 -*-

# Copyright (c) 2024 R. Cirpons
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, unused-import, fixme,
# pylint: disable=unidiomatic-typecheck

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the Sims class from libsemigroups.
"""

from _libsemigroups_pybind11 import (
    right_generating_pairs,
    two_sided_generating_pairs,
    is_right_congruence,
    is_right_congruence_of_dual,
    is_two_sided_congruence,
    is_maximal_right_congruence,
    poset,
)
