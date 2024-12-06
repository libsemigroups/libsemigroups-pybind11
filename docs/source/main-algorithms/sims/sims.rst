.. Copyright (c) 2024 R. Cirpons

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Sims
====

On this page we describe the functionality relating to the Sims's
low-index congruence algorithm for semigroups and monoids that is available in
``libsemigroups_pybind11``. This page contains a details of the methods
of the classes:

* :py:class:`Sims1` for computing finite index right congruences,
* :py:class:`Sims2` for computing finite index two-sided congruences,
* :py:class:`RepOrc` for computing small degree transformation representations of a finite semigroup or monoid,
* :py:class:`MinimalRepOrc` for computing minimal degree transformation representations of a finite semigroup or monoid.

.. doctest::

   >>> from libsemigroups_pybind11 import Sims1, Presentation, presentation
   >>> p = Presentation([0, 1])
   >>> presentation.add_rule(p, [0, 1], [1, 0])
   >>> sims = Sims1(p)
   >>> sims.number_of_congruences(4)
   117

Contents
--------

.. autosummary::
   :nosignatures:

   ~Sims1
   ~Sims2
   ~RepOrc
   ~MinimalRepOrc
   ~SimsStats
   ~SimsRefinerFaithful
   ~SimsRefinerIdeals

Full API
--------

.. autoclass:: Sims1
   :members:
   :inherited-members:

.. autoclass:: Sims2
   :members:
   :inherited-members:

.. autoclass:: RepOrc
   :members:
   :inherited-members:

.. autoclass:: MinimalRepOrc
   :members:
   :inherited-members:

.. autoclass:: SimsStats
   :members:
   :inherited-members:

.. autoclass:: SimsRefinerFaithful
   :members:
   :inherited-members:
   :special-members: __call__

.. autoclass:: SimsRefinerIdeals
   :members:
   :inherited-members:
   :special-members: __call__
