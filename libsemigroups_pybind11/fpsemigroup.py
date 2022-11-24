# -*- coding: utf-8 -*-

# Copyright (c) 2022, M. T. Whyte
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, unused-import, fixme

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the Presentation class from libsemigroups.
"""

from _libsemigroups_pybind11 import (
    author,
    _symmetric_group, # I'll want to change this so the user can't access it.
    # The same strange error happens with brauer_monoid(author.Sutov)... But this
    # function doesn't even take an author value at all.
    alternating_group,
    full_transformation_monoid,
    partial_transformation_monoid,
    symmetric_inverse_monoid,
    dual_symmetric_inverse_monoid,
    uniform_block_bijection_monoid,
    partition_monoid,
    brauer_monoid,
    rectangular_band,
    stellar_monoid,
    chinese_monoid,
    monogenic_semigroup,
    plactic_monoid,
    stylic_monoid,
    fibonacci_semigroup,
    temperley_lieb_monoid,
    singular_brauer_monoid,
    orientation_preserving_monoid,
    orientation_reversing_monoid
)

from _libsemigroups_pybind11 import make_presentation as make

def symmetric_group(n, val = author.Carmichael):
    if not isinstance(n, int):
        raise TypeError("the 1st argument must be an int, found ", type(n))
    return _symmetric_group(n, val)
