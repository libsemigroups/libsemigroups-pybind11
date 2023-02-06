# -*- coding: utf-8 -*-
# Copyright (c) 2022, M. T. Whyte
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for fpsemi-examples.
"""

# pylint: disable=fixme, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name


import pytest

from libsemigroups_pybind11 import (
    presentation,
    ToddCoxeter,
    congruence_kind,
    Sims1,
)

from libsemigroups_pybind11.fpsemigroup import (
    make,
    author,
    symmetric_group,
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
    orientation_reversing_monoid,
)


def test_symmetric_group_Carmichael():
    n = 5
    p = make(symmetric_group(n, author.Carmichael))
    presentation.replace_word(p, [], [n - 1])
    p.alphabet(n)
    presentation.add_identity_rules(p, n - 1)
    p.validate()

    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(n)
    for i in range(int(len(p.rules) / 2)):
        tc.add_pair(p.rules[2 * i], p.rules[2 * i + 1])
    assert tc.number_of_classes() == 120

    assert symmetric_group(5, author.Carmichael) == symmetric_group(5)


def test_symmetric_group_Coxeter_Moser():
    n = 6
    p = make(symmetric_group(n, author.Coxeter + author.Moser))
    presentation.replace_word(p, [], [n - 1])
    p.alphabet(n)
    presentation.add_identity_rules(p, n - 1)
    p.validate()

    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(n)
    for i in range(int(len(p.rules) / 2)):
        tc.add_pair(p.rules[2 * i], p.rules[2 * i + 1])
    assert tc.number_of_classes() == 720


def test_symmetric_group_Moore():
    n = 6
    p = make(symmetric_group(n, author.Moore))
    presentation.replace_word(p, [], [2])
    p.alphabet(3)
    presentation.add_identity_rules(p, 2)
    p.validate()

    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(3)
    for i in range(int(len(p.rules) / 2)):
        tc.add_pair(p.rules[2 * i], p.rules[2 * i + 1])
    assert tc.number_of_classes() == 720


def test_symmetric_group_exceptions():
    with pytest.raises(TypeError):
        symmetric_group(author.Sutov)
    with pytest.raises(RuntimeError):
        symmetric_group(2)
        symmetric_group(5, author.Sutov)


def test_alternating_group_Moore():
    n = 6
    p = make(alternating_group(n, author.Moore))
    presentation.replace_word(p, [], [n - 2])
    p.alphabet(n - 1)
    presentation.add_identity_rules(p, n - 2)
    p.validate()

    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(n - 1)
    for i in range(int(len(p.rules) / 2)):
        tc.add_pair(p.rules[2 * i], p.rules[2 * i + 1])
    assert tc.number_of_classes() == 360

    assert alternating_group(5, author.Moore) == alternating_group(5)


def test_alternating_group_exceptions():
    with pytest.raises(RuntimeError):
        alternating_group(3, author.Moore)
        alternating_group(4, author.Iwahori)
    with pytest.raises(TypeError):
        alternating_group(author.Sutov)


def test_full_transformation_monoid_Iwahori():
    n = 5
    p = make(full_transformation_monoid(n, author.Iwahori))
    presentation.replace_word(p, [], [n])
    p.alphabet(n + 1)
    presentation.add_identity_rules(p, n)
    p.validate()

    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(n + 1)
    for i in range(int(len(p.rules) / 2)):
        tc.add_pair(p.rules[2 * i], p.rules[2 * i + 1])
    assert tc.number_of_classes() == 3125

    assert full_transformation_monoid(
        5, author.Iwahori
    ) == full_transformation_monoid(5)


def test_full_transformation_monoid_Aizenstat():
    n = 5
    p = make(full_transformation_monoid(n, author.Aizenstat))
    presentation.replace_word(p, [], [3])
    p.alphabet(4)
    presentation.add_identity_rules(p, 3)
    p.validate()

    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(4)
    for i in range(int(len(p.rules) / 2)):
        tc.add_pair(p.rules[2 * i], p.rules[2 * i + 1])
    assert tc.number_of_classes() == 3125


def test_full_transformation_monoid_exceptions():
    with pytest.raises(RuntimeError):
        full_transformation_monoid(3, author.Iwahori)
        full_transformation_monoid(3, author.Aizenstat)
        full_transformation_monoid(5, author.Coxeter)
    with pytest.raises(TypeError):
        full_transformation_monoid(author.Sutov)


def test_partial_transformation_monoid_Sutov():
    n = 5
    p = make(partial_transformation_monoid(n, author.Sutov))
    presentation.replace_word(p, [], [n + 1])
    p.alphabet(n + 2)
    presentation.add_identity_rules(p, n + 1)
    p.validate()

    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(n + 2)
    for i in range(int(len(p.rules) / 2)):
        tc.add_pair(p.rules[2 * i], p.rules[2 * i + 1])
    assert tc.number_of_classes() == 7776

    assert partial_transformation_monoid(
        5, author.Sutov
    ) == partial_transformation_monoid(5)


def test_partial_transformation_monoid_Machine():
    p = make(partial_transformation_monoid(3, author.Machine))
    presentation.replace_word(p, [], [4])
    p.alphabet(5)
    presentation.add_identity_rules(p, 4)
    p.validate()

    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(5)
    for i in range(int(len(p.rules) / 2)):
        tc.add_pair(p.rules[2 * i], p.rules[2 * i + 1])
    assert tc.number_of_classes() == 64


def test_partial_transformation_monoid_exceptions():
    with pytest.raises(RuntimeError):
        partial_transformation_monoid(3, author.Sutov)
        partial_transformation_monoid(4, author.Burnside)
        partial_transformation_monoid(4, author.Machine)
    with pytest.raises(TypeError):
        partial_transformation_monoid(author.Sutov)


def test_symmetric_inverse_monoid_Sutov():
    n = 6
    p = make(symmetric_inverse_monoid(n, author.Sutov))
    presentation.replace_word(p, [], [n])
    p.alphabet(n + 1)
    presentation.add_identity_rules(p, n)
    p.validate()

    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(n + 1)
    for i in range(int(len(p.rules) / 2)):
        tc.add_pair(p.rules[2 * i], p.rules[2 * i + 1])
    assert tc.number_of_classes() == 13327

    assert symmetric_inverse_monoid(
        5, author.Sutov
    ) == symmetric_inverse_monoid(5)


def test_symmetric_inverse_monoid_exceptions():
    with pytest.raises(RuntimeError):
        symmetric_inverse_monoid(3, author.Sutov)
        symmetric_inverse_monoid(4, author.Burnside)
    with pytest.raises(TypeError):
        symmetric_inverse_monoid(author.Sutov)


def test_dual_symmetric_inverse_monoid_Easdown_East_Fitzgerald():
    s = dual_symmetric_inverse_monoid(
        5, author.Easdown + author.East + author.FitzGerald
    )
    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(6)
    for rule in s:
        tc.add_pair(rule[0], rule[1])
    assert tc.number_of_classes() == 6721

    assert dual_symmetric_inverse_monoid(
        5, author.Easdown + author.East + author.FitzGerald
    ) == dual_symmetric_inverse_monoid(5)


def test_dual_symmetric_inverse_monoid_exceptions():
    with pytest.raises(RuntimeError):
        dual_symmetric_inverse_monoid(
            3, author.Easdown + author.East + author.FitzGerald
        )
        dual_symmetric_inverse_monoid(5, author.Burnside)
    with pytest.raises(TypeError):
        dual_symmetric_inverse_monoid(author.Sutov)


def test_uniform_block_bijection_monoid_Fitzgerald():
    s = uniform_block_bijection_monoid(6, author.FitzGerald)
    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(7)
    for rule in s:
        tc.add_pair(rule[0], rule[1])
    assert tc.number_of_classes() == 22482

    assert uniform_block_bijection_monoid(
        5, author.FitzGerald
    ) == uniform_block_bijection_monoid(5)


def test_uniform_block_bijection_monoid_exceptions():
    with pytest.raises(RuntimeError):
        uniform_block_bijection_monoid(2, author.FitzGerald)
        uniform_block_bijection_monoid(5, author.Burnside)
    with pytest.raises(TypeError):
        uniform_block_bijection_monoid(author.Sutov)


def test_partition_monoid_East():
    s = partition_monoid(4, author.East)
    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(5)
    for rule in s:
        tc.add_pair(rule[0], rule[1])
    assert tc.number_of_classes() == 4140

    assert partition_monoid(5, author.East) == partition_monoid(5)


def test_partition_monoid_Machine():
    s = partition_monoid(3, author.Machine)
    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(5)
    for rule in s:
        tc.add_pair(rule[0], rule[1])
    assert tc.number_of_classes() == 203


def test_partition_monoid_exceptions():
    with pytest.raises(RuntimeError):
        partition_monoid(3, author.East)
        partition_monoid(4, author.Machine)
        partition_monoid(5, author.Sutov)
    with pytest.raises(TypeError):
        partition_monoid(author.Sutov)


def test_brauer_monoid():
    s = brauer_monoid(6)
    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(11)
    for rule in s:
        tc.add_pair(rule[0], rule[1])
    assert tc.number_of_classes() == 10395


def test_brauer_monoid_exceptions():
    with pytest.raises(TypeError):
        brauer_monoid(author.Sutov)


def test_rectangular_band():
    s = rectangular_band(4, 5)
    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(9)
    for rule in s:
        tc.add_pair(rule[0], rule[1])
    assert tc.number_of_classes() == 20

    s = rectangular_band(15, 1)
    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(16)
    for rule in s:
        tc.add_pair(rule[0], rule[1])
    assert tc.number_of_classes() == 15


def test_rectangular_band_exceptions():
    with pytest.raises(RuntimeError):
        rectangular_band(1, 0)
        rectangular_band(0, 1)
    with pytest.raises(TypeError):
        rectangular_band(author.Sutov, author.Sutov)


def test_stellar_monoid():
    p = make(stellar_monoid(4))
    C = Sims1(congruence_kind.right)
    C.short_rules(p)

    assert C.number_of_congruences(3) == 79237


def test_stellar_monoid_exceptions():
    with pytest.raises(RuntimeError):
        stellar_monoid(0)
        stellar_monoid(1)
    with pytest.raises(TypeError):
        stellar_monoid(author.Sutov)


def test_chinese_monoid():
    p = make(chinese_monoid(5))
    C = Sims1(congruence_kind.right)
    C.short_rules(p)

    assert C.number_of_congruences(3) == 23504


def test_chinese_monoid_exceptions():
    with pytest.raises(RuntimeError):
        chinese_monoid(0)
        chinese_monoid(1)
    with pytest.raises(TypeError):
        chinese_monoid(author.Sutov)


def test_monogenic_semigroup():
    s = monogenic_semigroup(5, 9)
    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(1)
    for rule in s:
        tc.add_pair(rule[0], rule[1])
    assert tc.number_of_classes() == 13


def test_monogenic_semigroup_exceptions():
    with pytest.raises(RuntimeError):
        monogenic_semigroup(4, 0)
        monogenic_semigroup(0, 0)
    with pytest.raises(TypeError):
        monogenic_semigroup(author.Sutov, author.Sutov)


def test_plactic_monoid():
    p = make(plactic_monoid(4))
    C = Sims1(congruence_kind.right)
    C.short_rules(p)

    assert C.number_of_congruences(4) == 106264


def test_plactic_monoid_exceptions():
    with pytest.raises(RuntimeError):
        plactic_monoid(0)
        plactic_monoid(1)
    with pytest.raises(TypeError):
        plactic_monoid(author.Sutov)


def test_stylic_monoid():
    p = make(stylic_monoid(5))
    C = Sims1(congruence_kind.right)
    C.short_rules(p)

    assert C.number_of_congruences(5) == 19201


def test_stylic_monoid_exceptions():
    with pytest.raises(RuntimeError):
        stylic_monoid(0)
        stylic_monoid(1)
    with pytest.raises(TypeError):
        stylic_monoid(author.Sutov)


def test_fibonacci_semigroup():
    s = fibonacci_semigroup(2, 5)
    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(5)
    for rule in s:
        tc.add_pair(rule[0], rule[1])
    assert tc.number_of_classes() == 11


def test_fibonacci_semigroup_exceptions():
    with pytest.raises(RuntimeError):
        fibonacci_semigroup(0, 1)
        fibonacci_semigroup(1, 0)
        fibonacci_semigroup(0, 0)
    with pytest.raises(TypeError):
        fibonacci_semigroup(author.Sutov, author.Sutov)


def test_temperley_lieb_monoid():
    p = make(temperley_lieb_monoid(8))
    p.alphabet(8)
    presentation.add_identity_rules(p, 7)
    p.validate()
    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(8)
    for i in range(int(len(p.rules) / 2)):
        tc.add_pair(p.rules[2 * i], p.rules[2 * i + 1])
    assert tc.number_of_classes() == 1430


def test_temperley_lieb_monoid_exceptions():
    with pytest.raises(RuntimeError):
        temperley_lieb_monoid(0)
        temperley_lieb_monoid(1)
        temperley_lieb_monoid(2)
    with pytest.raises(TypeError):
        temperley_lieb_monoid(author.Sutov)


def test_singular_brauer_monoid():
    p = make(singular_brauer_monoid(6))
    m = len(p.alphabet())
    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(m)
    for i in range(int(len(p.rules) / 2)):
        tc.add_pair(p.rules[2 * i], p.rules[2 * i + 1])
    assert tc.number_of_classes() == 9675


def test_singular_brauer_monoid_exceptions():
    with pytest.raises(RuntimeError):
        singular_brauer_monoid(0)
        singular_brauer_monoid(1)
        singular_brauer_monoid(2)
    with pytest.raises(TypeError):
        singular_brauer_monoid(author.Sutov)


def test_orientation_preserving_monoid():
    p = make(orientation_preserving_monoid(6))
    p.alphabet(3)
    presentation.replace_word(p, [], [2])
    presentation.add_identity_rules(p, 2)
    p.validate()
    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(3)
    for i in range(0, len(p.rules), 2):
        tc.add_pair(p.rules[i], p.rules[i + 1])
    assert tc.number_of_classes() == 2742


def test_orientation_preserving_monoid_exceptions():
    with pytest.raises(RuntimeError):
        orientation_preserving_monoid(0)
        orientation_preserving_monoid(1)
        orientation_preserving_monoid(2)
    with pytest.raises(TypeError):
        orientation_preserving_monoid(author.Sutov)


def test_orientation_reversing_monoid():
    p = make(orientation_reversing_monoid(5))
    p.alphabet(4)
    presentation.replace_word(p, [], [3])
    presentation.add_identity_rules(p, 3)
    p.validate()
    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(4)
    for i in range(0, len(p.rules), 2):
        tc.add_pair(p.rules[i], p.rules[i + 1])
    assert tc.number_of_classes() == 1015


def test_orientation_reversing_monoid_exceptions():
    with pytest.raises(RuntimeError):
        orientation_reversing_monoid(0)
        orientation_reversing_monoid(1)
        orientation_reversing_monoid(2)
    with pytest.raises(TypeError):
        orientation_reversing_monoid(author.Sutov)
