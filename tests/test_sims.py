# -*- coding: utf-8 -*-

# Copyright (c) 2024, R. Cirpons
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the libsemigroups_pybind11 functionality
arising from sims.*pp in libsemigroups.
"""

# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name


from libsemigroups_pybind11 import Sims1, Sims2, Presentation, presentation


def test_sims1_001():
    p = Presentation(list(range(4)))
    presentation.add_rule(p, [1, 2, 1], [1, 1])
    presentation.add_rule(p, [3, 3], [1, 1])
    presentation.add_rule(p, [1, 1, 1], [1, 1])
    presentation.add_rule(p, [3, 2], [2, 3])
    presentation.add_rule(p, [3, 1], [1, 1])
    presentation.add_rule(p, [2, 2], [2])
    presentation.add_rule(p, [1, 3], [1, 1])
    sims = Sims1(p)
    assert sims.number_of_congruences(2) == 67
    # FIXME: make number of threads work
    assert sims.number_of_threads(2).number_of_congruences(2) == 67
    assert sims.number_of_threads(4).number_of_congruences(2) == 67
    assert sims.number_of_threads(8).number_of_congruences(2) == 67


def test_sims1_002():
    # FIXME: support string presentations
    p = Presentation("abcd")
    presentation.add_rule(p, "bcb", "bb")
    presentation.add_rule(p, "dd", "bb")
    presentation.add_rule(p, "bbb", "bb")
    presentation.add_rule(p, "dc", "cd")
    presentation.add_rule(p, "db", "bb")
    presentation.add_rule(p, "cc", "c")
    presentation.add_rule(p, "bd", "bb")
    sims = Sims1(p)
    assert sims.number_of_congruences(2) == 67
    assert sims.number_of_threads(2).number_of_congruences(2) == 67
    assert sims.number_of_threads(4).number_of_congruences(2) == 67
    assert sims.number_of_threads(8).number_of_congruences(2) == 67


def test_sims2_001():
    p = Presentation([0, 1])
    presentation.add_rule(p, [0, 1], [1, 0])

    sims = Sims2(p)
    assert sims.number_of_congruences(1) == 1
    assert sims.number_of_congruences(2) == 9
    assert sims.number_of_congruences(3) == 37
    assert sims.number_of_congruences(4) == 117
    assert sims.number_of_congruences(5) == 301
    assert sims.number_of_congruences(6) == 699
    assert sims.number_of_congruences(7) == 1484
    assert sims.number_of_congruences(8) == 2947
    assert sims.number_of_congruences(9) == 5607
    assert sims.number_of_congruences(10) == 10168
    assert sims.number_of_congruences(11) == 17942
    assert sims.number_of_congruences(12) == 30650
    assert sims.number_of_congruences(13) == 51292
    assert sims.number_of_threads(2).number_of_congruences(4) == 117
    assert sims.number_of_threads(4).number_of_congruences(4) == 117
    assert sims.number_of_threads(8).number_of_congruences(4) == 117


def test_sims2_002():
    p = Presentation("ab")
    presentation.add_rule(p, "ab", "ba")

    sims = Sims2(p)
    assert sims.number_of_congruences(1) == 1
    assert sims.number_of_congruences(2) == 9
    assert sims.number_of_congruences(3) == 37
    assert sims.number_of_congruences(4) == 117
    assert sims.number_of_congruences(5) == 301
    assert sims.number_of_congruences(6) == 699
    assert sims.number_of_congruences(7) == 1484
    assert sims.number_of_congruences(8) == 2947
    assert sims.number_of_congruences(9) == 5607
    assert sims.number_of_congruences(10) == 10168
    assert sims.number_of_congruences(11) == 17942
    assert sims.number_of_congruences(12) == 30650
    assert sims.number_of_congruences(13) == 51292
    assert sims.number_of_threads(2).number_of_congruences(4) == 117
    assert sims.number_of_threads(4).number_of_congruences(4) == 117
    assert sims.number_of_threads(8).number_of_congruences(4) == 117
