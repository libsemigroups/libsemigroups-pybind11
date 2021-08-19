.. Copyright (c) 2020, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Semigroups
==========

TODO: Update!

``libsemigroups`` implements several algorithms for computing the structure of
a finite semigroup or monoid. The main algorithms implemented are the
Froidure-Pin, and Schreier-Sims (for groups); see the links below for further
details. 

The implementations of these algorithms are generic, and can be easily adapted
for user-defined types.  In other words, it is possible to directly apply these
algorithms to any type that satisfy some minimal prerequisites, via traits
classes and/or the adapters described here.

Many of the classes for semigroups in ``libsemigroups`` can be used
"interactively", in the sense that they can be run for a particular amount of
time, or until some condition is met; for further details see, for example, 
:cpp:any:`void libsemigroups::Runner::run_for(std::chrono::nanoseconds)` and
:cpp:func:`libsemigroups::Runner::run_until`.

The classes for semigroups and monoids in ``libsemigroups`` are:
