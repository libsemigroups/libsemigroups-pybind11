# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This package provides a the user-facing python part of libsemigroups_pybind11
relating to matrices.
"""

from _libsemigroups_pybind11 import (
    BMat,
    IntMat,
    MaxPlusMat,
    MinPlusMat,
    ProjMaxPlusMat,
    MaxPlusTruncMat,
    MinPlusTruncMat,
    NTPMat,
)

from enum import Enum


class MatrixKind(Enum):
    """
    This class is used as the argument to :py:class:`Matrix` to distinguish
    which semiring the matrix should be over.
    """

    Boolean = 0
    Integer = 1
    MaxPlus = 2
    MinPlus = 3
    ProjMaxPlus = 4
    MaxPlusTrunc = 5
    MinPlusTrunc = 6
    NTP = 7


_Matrix = {
    MatrixKind.Boolean: BMat,
    MatrixKind.Integer: IntMat,
    MatrixKind.MaxPlus: MaxPlusMat,
    MatrixKind.MinPlus: MinPlusMat,
    MatrixKind.ProjMaxPlus: ProjMaxPlusMat,
    MatrixKind.MaxPlusTrunc: MaxPlusTruncMat,
    MatrixKind.MinPlusTrunc: MinPlusTruncMat,
    MatrixKind.NTP: NTPMat,
}


def Matrix(kind: MatrixKind, *args):
    if not isinstance(kind, MatrixKind):
        raise TypeError("the 1st argument must be a MatrixKind")
    return _Matrix[kind](*args)


def make_identity(kind: MatrixKind, *args) -> Matrix:
    return _Matrix[kind].make_identity(*args)


def make(kind: MatrixKind, *args) -> Matrix:
    return _Matrix[kind].make(*args)