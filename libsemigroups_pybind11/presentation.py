# -*- coding: utf-8 -*-

# Copyright (c) 2022, J. D. Mitchell
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
    PresentationStrings,
    PresentationWords,
    add_rule,
    add_rules,
    add_identity_rules,
    add_zero_rules,
    add_inverse_rules,
    remove_duplicate_rules,
    remove_trivial_rules,
    remove_redundant_generators,
    reduce_complements,
    sort_each_rule,
    sort_rules,
    replace_subword,
    replace_word,
    replace_word_with_new_generator,
    length,
    reverse,
    normalize_alphabet,
    # add_zero_rules,
    # are_rules_sorted,
    # change_alphabet,
    # letter,
    # character,
    # first_unused_letter,
    # greedy_reduce_length,
    # longest_rule,
    # shortest_rule,
    # longest_rule_length,
    # shortest_rule_length,
    # make_semigroup,
    # is_strongly_compressible,
    # strongly_compress,
    # reduce_to_2_generators,
)


def Presentation(arg):
    """
    Construct a Presentation instance of the type specified by its argument.
    """
    if isinstance(arg, PresentationStrings):
        result = PresentationStrings(arg)
    elif isinstance(arg, PresentationWords):
        result = PresentationWords(arg)
    elif isinstance(arg, str):
        result = PresentationStrings()
        result.alphabet(arg)
    elif isinstance(arg, list) and all(isinstance(x, int) for x in arg):
        result = PresentationWords()
        result.alphabet(arg)
    else:
        raise TypeError(
            "expected the argument to be a Presentation, string, or list of ints"
        )
    return result
