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


from libsemigroups_pybind11 import (
    Sims1,
    Sims2,
    Presentation,
    presentation,
    SimsRefinerIdeals,
    SimsRefinerFaithful,
)


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
    assert sims.number_of_threads(2).number_of_congruences(2) == 67
    assert sims.number_of_threads(4).number_of_congruences(2) == 67
    assert sims.number_of_threads(8).number_of_congruences(2) == 67


def test_sims1_002():
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


def test_sims_refiner_ideals_001():
    p = Presentation([0, 1, 2])
    p.contains_empty_word(True)
    presentation.add_rule(p, [0, 0], [])
    presentation.add_rule(p, [0, 2], [2])
    presentation.add_rule(p, [1, 1], [1])
    presentation.add_rule(p, [2, 0], [2])
    presentation.add_rule(p, [2, 2], [2])
    presentation.add_rule(p, [1, 2, 1], [1])
    presentation.add_rule(p, [2, 1, 2], [2])
    presentation.add_rule(p, [0, 1, 0, 1], [1, 0, 1])
    presentation.add_rule(p, [1, 0, 1, 0], [1, 0, 1])

    ip = SimsRefinerIdeals(p)

    s = Sims1(p)
    s.add_pruner(ip)
    assert s.number_of_congruences(15) == 15
    assert s.number_of_threads(2).number_of_congruences(15) == 15
    # assert s.number_of_threads(4).number_of_congruences(15) == 15
    # assert s.number_of_threads(8).number_of_congruences(15) == 15


test_sims_refiner_ideals_001()


def test_sims_refiner_ideals_002():
    p = Presentation([0, 1])
    presentation.add_rule(p, [0, 0, 0], [1, 1])
    presentation.add_rule(p, [0, 0, 1], [1, 0])

    sims = Sims2(p)
    pruner = SimsRefinerIdeals(sims.presentation())
    sims.add_pruner(pruner)
    assert sims.number_of_congruences(1) == 1
    assert sims.number_of_congruences(2) == 3
    assert sims.number_of_congruences(3) == 5
    assert sims.number_of_congruences(4) == 7
    assert sims.number_of_congruences(5) == 9
    assert sims.number_of_congruences(6) == 11
    assert sims.number_of_congruences(7) == 12
    for n in range(8, 20):
        assert sims.number_of_congruences(n) == 12
    assert sims.number_of_threads(2).number_of_congruences(7) == 12
    assert sims.number_of_threads(4).number_of_congruences(7) == 12
    assert sims.number_of_threads(8).number_of_congruences(7) == 12


# def test_sims_refiner_faithful_002():
#     p = Presentation([0, 1])
#     presentation.add_rule(p, [0, 0, 0], [1, 1])
#     presentation.add_rule(p, [0, 0, 1], [1, 0])
#
#     sims = Sims2(p)
#     pruner = SimsRefinerFaithful(sims.presentation())
#     sims.add_pruner(pruner)
#     assert sims.number_of_congruences(1) == 1
#     assert sims.number_of_congruences(2) == 3
#     assert sims.number_of_congruences(3) == 5
#     assert sims.number_of_congruences(4) == 7
#     assert sims.number_of_congruences(5) == 9
#     assert sims.number_of_congruences(6) == 11
#     assert sims.number_of_congruences(7) == 12
#     for n in range(8, 20):
#         assert sims.number_of_congruences(n) == 12
#     # FIXME: Fix issue with deadlocks when using pruners
#     # assert sims.number_of_threads(2).number_of_congruences(7) == 12
#     # assert sims.number_of_threads(4).number_of_congruences(7) == 12
#     # assert sims.number_of_threads(8).number_of_congruences(7) == 12
