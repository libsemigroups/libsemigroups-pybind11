# -*- coding: utf-8 -*-

# Copyright (c) 2022, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, unused-import

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the Presentation class from libsemigroups.
"""

from _libsemigroups_pybind11 import (
    PresentationStrings,
    PresentationWords,
    add_rule,
    add_rule_and_check,
    add_rules,
    add_identity_rules,
    add_inverse_rules,
    remove_duplicate_rules,
    reduce_complements,
    sort_each_rule,
    sort_rules,
    longest_common_subword,
    replace_subword,
    length,
    reverse,
    normalize_alphabet,
    redundant_rule_strings,
    redundant_rule_words,
)


def Presentation(alphabet):
    """
    Construct a Presentation instance of the type specified by its argument
    (which is the alphabet to be used).
    """
    if isinstance(alphabet, PresentationStrings):
        result = PresentationStrings(alphabet)
    elif isinstance(alphabet, PresentationWords):
        result = PresentationWords(alphabet)
    elif isinstance(alphabet, str):
        result = PresentationStrings()
        result.alphabet(alphabet)
    elif isinstance(alphabet, list) and all(
        isinstance(x, int) for x in alphabet
    ):
        result = PresentationWords()
        result.alphabet(alphabet)
    else:
        raise TypeError("expected string or list of ints argument")
    return result


def redundant_rule(p, t):
    """
    Returns the index of the first from the end redundant rule in the
    presentation p.
    """
    if isinstance(p.alphabet(), str):
        return redundant_rule_strings(p, t)
    if isinstance(p.alphabet(), list):
        return redundant_rule_words(p, t)
    raise TypeError("expected a presentation over strings or lists of ints")
