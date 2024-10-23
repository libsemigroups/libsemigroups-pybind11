//
// libsemigroups_pybind11
// Copyright (C) 2024 Reinis Cirpons
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

// C std headers....
// TODO complete or delete

// C++ stl headers....
// TODO complete or delete
#include <iosfwd>  // for string
#include <vector>  // for vector

// libsemigroups....

#include <libsemigroups/presentation.hpp>  // for Presentation
#include <libsemigroups/rx/ranges.hpp>  // for rx::begin, rx::end, rx::transform
#include <libsemigroups/sims.hpp>       // for Sims1, Sims2, ....
#include <libsemigroups/types.hpp>      // for word_type
#include <libsemigroups/word-graph.hpp>  // for WordGraph

// pybind11....
// #include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
//  TODO uncomment/delete

// libsemigroups_pybind11....
#include "main.hpp"  // for init_sims

namespace py = pybind11;

namespace libsemigroups {
  using node_type       = uint32_t;
  using word_graph_type = WordGraph<node_type>;
  using size_type       = typename word_graph_type::size_type;

  template <typename Subclass>
  void bind_sims(py::module& m, std::string const& name) {
    using SimsSettings_ = SimsSettings<Subclass>;

    py::class_<SimsSettings_> ss(m,
                                 name.c_str(),
                                 R"pbdoc(
For setting the presentation and various runtime parameters of the Sims low index algorithm.

Defined in ``sims.hpp``. On this page we describe the :any:`SimsSettings` class. The purpose of this class is to allow us to use the same interface for settings for :any:`Sims1` , :any:`Sims2` , :any:`RepOrc` , and :any:`MinimalRepOrc`.

.. seealso::  :any:`Sims1`

.. seealso::  :any:`Sims2`

.. seealso::  :any:`RepOrc`

.. seealso::  :any:`MinimalRepOrc`)pbdoc");
    ss.def(py::init<>(), R"pbdoc(
Default constructor.Constructs a :any:`SimsSettings` object. Defaults to a single thread and 64 idle thread restarts, no other settings set. Use

*  :any:`presentation` to set the presentation;
*  :any:`number_of_threads` to set the number of threads;
*  :any:`include` to set the pairs to be included;
*  :any:`exclude` to set the pairs to be excluded;
*  :any:`add_pruner` to add a pruner;
*  :any:`long_rule_length` to set the length of long rules;
*  :any:`idle_thread_restarts` to set the number of idle thread restarts.



:parameters (none): 

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.)pbdoc");
    ss.def(py::init<SimsSettings_ const&>(), R"pbdoc(

:param that: the SimsSettings to copy.
:type that: SimsSettings
Copy constructor.Returns a :any:`SimsSettings` object that is a copy of ``that`` . The state of the new :any:`SimsSettings` object is the same as ``that``.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.)pbdoc");
    ss.def(
        "init",
        [](SimsSettings_& self) -> Subclass& { return self.init(); },
        R"pbdoc(
Reinitialize an existing SimsSettings object.
This function puts a :any:`SimsSettings` object back into the same state as if it had been newly default constructed.

:parameters (none): 

:raises This:  function guarantees not to throw a :any:`LibsemigroupsError`.

:returns: A reference to ``self``.

:rtype: Subclass
)pbdoc");
    ss.def("settings",
           &SimsSettings_::settings,
           R"pbdoc(
Returns the settings object of *this.The settings object contains all the settings that are common to :any:`Sims1` , :any:`Sims2` , :any:`RepOrc` , and :any:`MinimalRepOrc` , which are currently:

*  :any:`presentation`
*  :any:`long_rules`
*  :any:`number_of_threads`

The return value of this function can be used to initialise another :any:`Sims1` , :any:`Sims2` , :any:`RepOrc` , or :any:`MinimalRepOrc` with these settings.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:returns: A const reference to :any:`SimsSettings`.

:rtype: SimsSettings
)pbdoc");
    ss.def(
        "number_of_threads",
        // TODO(0): add -> Subclass& here and everywhere else where we return a
        // subclass
        [](SimsSettings_& self, size_t val) -> Subclass& {
          return self.number_of_threads(val);
        },
        py::arg("val"),
        R"pbdoc(
Set the number of threads.

:param val: the maximum number of threads to use.
:type val: int
This function sets the number of threads to be used by :any:`Sims1` or :any:`Sims2`.The default value is ``1``.

:raises LibsemigroupsError:  if the argument ``val`` is 0.


:returns: A reference to ``self``.

:rtype: Subclass
)pbdoc");
    ss.def(
        "number_of_threads",
        [](SimsSettings_ const& self) { return self.number_of_threads(); },
        R"pbdoc(
Get the number of threads.
Returns the current number of threads.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:returns: A ``int``.

:rtype: int
)pbdoc");
    ss.def(
        "presentation",
        [](SimsSettings_& self, Presentation<word_type> const& p) -> Subclass& {
          return self.presentation(p);
        },
        py::arg("p"),
        R"pbdoc(
Set the presentation over which the congruences produced by an instance are defined.

:param p: the presentation.
:type p: Presentation
Set the presentation over which the congruences produced by an instance are defined. These are the rules used at every node in the depth first search conducted by objects of this type.If the template parameter ``Word`` is not ``word_type`` , then the parameter ``p`` is first converted to a value of type ``Presentation`` and it is this converted value that is used.

:raises LibsemigroupsError:  if ``to_presentation(p)`` throws.

:raises LibsemigroupsError:  if ``p`` is not valid.

:raises LibsemigroupsError:  if the alphabet of ``p`` is non-empty and not equal to that of :any:`long_rules` or :any:`presentation`.

:raises LibsemigroupsError:  if ``p`` has 0-generators and 0-relations.


:returns: A reference to ``self``.

:rtype: typename Word
)pbdoc");
    ss.def(
        "presentation",
        [](SimsSettings_& self, Presentation<std::string> const& p)
            -> Subclass& { return self.presentation(p); },
        py::arg("p"),
        R"pbdoc(
Set the presentation over which the congruences produced by an instance are defined.

:param p: the presentation.
:type p: Presentation
Set the presentation over which the congruences produced by an instance are defined. These are the rules used at every node in the depth first search conducted by objects of this type.If the template parameter ``Word`` is not ``word_type`` , then the parameter ``p`` is first converted to a value of type ``Presentation`` and it is this converted value that is used.

:raises LibsemigroupsError:  if ``to_presentation(p)`` throws.

:raises LibsemigroupsError:  if ``p`` is not valid.

:raises LibsemigroupsError:  if the alphabet of ``p`` is non-empty and not equal to that of :any:`long_rules` or :any:`presentation`.

:raises LibsemigroupsError:  if ``p`` has 0-generators and 0-relations.


:returns: A reference to ``self``.

:rtype: typename Word
)pbdoc");
    ss.def(
        "presentation",
        [](SimsSettings_ const& self) { return self.presentation(); },
        R"pbdoc(
Get the presentation over which the congruences produced by an instance are defined.
Returns a const reference to the current relations of the underlying presentation.This function returns the defining presentation of a :any:`Sims1` or :any:`Sims2` instance. The congruences computed by ``cbegin`` and ``cend`` of the appropriate subclass are defined over the semigroup or monoid defined by this presentation.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:returns: A const reference to ``Presentation``.

:rtype: Presentation
)pbdoc");
    ss.def(
        "first_long_rule_position",
        [](SimsSettings_& self, size_t pos) -> Subclass& {
          return self.cbegin_long_rules(pos);
        },
        py::arg("pos"),
        R"pbdoc(
Set the beginning of the long rules (position).

:param pos: position of the the left hand side of the first long rule.
:type pos: int
Set the beginning of the long rules using a position in ``presentation().rules``.The "long rules" are the rules used after a complete deterministic word graph has been found in the search. If such a word graph is compatible with the long rules specified by this function, then this word graph is accepted, and if not it is rejected.The purpose of this is to improve the backtrack search by reducing the time spent processing "long" rules in each node of the search tree, and to only check them at the leaves.

:raises LibsemigroupsError:  if ``pos`` is not a valid position in ``presentation().rules``

:raises LibsemigroupsError:  if the rule at position ``pos`` is not the left hand side of a rule (i.e. if ``pos`` is odd).


:returns: A reference to ``self``.

:rtype: Subclass
)pbdoc");
    ss.def(
        "long_rules",
        [](SimsSettings_ const& self) {
          return py::make_iterator(self.cbegin_long_rules(),
                                   self.presentation().rules.cend());
        },
        R"pbdoc(
Get the pointer to the first long rule.
Returns the pointer to the first long rule.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:returns: A const reference to ``Presentation``.

:rtype: Iterator
)pbdoc");
    ss.def(
        "clear_long_rules",
        [](SimsSettings_& self) -> Subclass& {
          return self.clear_long_rules();
        },
        R"pbdoc(
Clear the set of long rules.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.

:returns: A reference to ``self``.

:rtype: Subclass
)pbdoc");
    ss.def("number_of_long_rules",
           &SimsSettings_::number_of_long_rules,
           R"pbdoc(
Returns the number of rules marked as long rules.
)pbdoc");
    ss.def(
        "long_rule_length",
        [](SimsSettings_& self, size_t val) -> Subclass& {
          return self.long_rule_length(val);
        },
        py::arg("val"),
        R"pbdoc(
Set the length of a long rule.

:param val: the value of the long rule length.
:type val: int
Define the length of a "long" rule.This function modifies :any:`presentation` so that the rules whose length (sum of the lengths of both sizes) is at least ``val`` (if any) occur at the end of ``presentation().rules`` and so that ``cbegin_long_rules`` points at the such rule.The relative orders of the rules within :any:`presentation` may not be preserved.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.


:returns: A reference to ``self``.

:rtype: Subclass
)pbdoc");
    ss.def("pruners",
           &SimsSettings_::pruners,
           R"pbdoc(
Get all active pruners of the search tree.
Returns a const reference to the set of pruners.A pruner is any function that takes as input a word graph and returns a boolean. We require that if a pruner returns false for a word graph ``wg`` , then it returns false for all word graphs that are descended from ``wg`` in the Sims word graph search tree. The pruners are used to refine the congruence search tree during the execution of the Sims algorithm. As such, the congruences computed by this instance are only taken among those whose word graphs are accepted by all pruners returned by :any:`pruners`.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:returns: A const reference to ``std::vector>`` , the set of all pruners.

:rtype: auto
)pbdoc");
    ss.def(
        "add_pruner",
        [](SimsSettings_& self, SimsRefinerIdeals const& func) -> Subclass& {
          return self.add_pruner(func);
        },
        R"pbdoc(
           TODO(0)
)pbdoc");
    ss.def(
        "add_pruner",
        [](SimsSettings_& self, SimsRefinerFaithful const& func) -> Subclass& {
          return self.add_pruner(func);
        },
        R"pbdoc(
           TODO(0)
)pbdoc");
    ss.def(
        "add_pruner",
        [](SimsSettings_&                              self,
           std::function<bool(word_graph_type const&)> func) -> Subclass& {
          return self.add_pruner(func);
        },
        R"pbdoc(
           TODO(0)
)pbdoc");
    ss.def(
        "clear_pruners",
        [](SimsSettings_& self) -> Subclass& { return self.clear_pruners(); },
        R"pbdoc(
Clear the set of pruners.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.

:returns: A reference to ``self``.

:rtype: Subclass
)pbdoc");
    ss.def(
        "include",
        [](SimsSettings_ const& self) { return self.include(); },
        R"pbdoc(
Get the set of pairs that must be included in every congruence.
Returns a const reference to the (one-sided) defining pairs.The congruences computed by a :any:`Sims1` or :any:`Sims2` instance always contain the relations of this presentation. In other words, the congruences computed by this instance are only taken among those that contains the pairs of elements of the underlying semigroup (defined by the presentation returned by :any:`presentation` and :any:`long_rules` ) represented by the relations of the presentation returned by :any:`include`.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:returns: A const reference to ``Presentation``.

:rtype: list
)pbdoc");
    ss.def(
        "include",
        [](SimsSettings_& self, word_type const& lhs, word_type const& rhs)
            -> Subclass& { return self.include(lhs, rhs); },
        py::arg("lhs"),
        py::arg("rhs"),
        R"pbdoc(
Add a pair that should be included in every congruence.

:param lhs: the left hand side of the rule being added. 
:type lhs: word_type

:param rhs: the right hand side of the rule being added.
:type rhs: word_type
Add a pair that should be included in every congruence.

:raises LibsemigroupsError:  if ``validate_word(lhs)`` or ``validate_word(rhs)`` throws.

.. seealso::  :any:`include`


:returns: A reference to ``self``.

:rtype: Subclass
)pbdoc");
    ss.def(
        "include",
        [](SimsSettings_& self, std::vector<word_type> const& c) -> Subclass& {
          return self.include(c);
        },
        py::arg("c"),
        R"pbdoc(
Define the set of pairs that must be included in every congruence (container).

:param c: A container of rules to be included.
:type c: Container
Define the set of pairs that must be included in every congruence.The congruences computed by an instance of this type will always contain the relations input here. In other words, the congruences computed are only taken among those that contains the pairs of elements of the underlying semigroup (defined by the presentation returned by :any:`presentation` ) represented by the relations returned by :any:`include`.

:raises LibsemigroupsError:  if ``c`` does not define a set of pairs, i.e. there are an odd number of words in ``c``.

:raises LibsemigroupsError:  if ``validate_word(w)`` throws for any word ``w`` in ``c``.


:returns: A reference to ``self``.

:rtype: typename Container
)pbdoc");
    ss.def(
        "clear_include",
        [](SimsSettings_& self) -> Subclass& { return self.clear_include(); },
        R"pbdoc(
Clear the set of included words.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.

:returns: A reference to ``self``.

:rtype: Subclass
)pbdoc");
    ss.def(
        "exclude",
        [](SimsSettings_ const& self) { return self.exclude(); },
        R"pbdoc(
Get the set of pairs that must be excluded from every congruence.
Returns a const reference to the excluded pairs.The congruences computed by a :any:`Sims1` or :any:`Sims2` instance will never contain the relations of this presentation. In other words, the congruences computed by this instance are only taken among those that do not contain any of the pairs of elements of the underlying semigroup (defined by the presentation returned by :any:`presentation` and :any:`long_rules` ) represented by the relations of the presentation returned by :any:`exclude`.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:returns: A const reference to ``Presentation``.

:rtype: list
)pbdoc");
    ss.def(
        "exclude",
        [](SimsSettings_&   self,
           word_type const& lhs,
           word_type const& rhs) -> Subclass { return self.exclude(lhs, rhs); },
        py::arg("lhs"),
        py::arg("rhs"),
        R"pbdoc(
Add a pair that must be excluded from every congruence.

:param lhs: the left hand side of the rule being added. 
:type lhs: word_type

:param rhs: the right hand side of the rule being added.
:type rhs: word_type
Add a pair that must be excluded from every congruence.

:raises LibsemigroupsError:  if ``validate_word(lhs)`` or ``validate_word(rhs)`` throws.

.. seealso::  :any:`exclude`)pbdoc");
    ss.def(
        "exclude",
        [](SimsSettings_& self, std::vector<word_type> const& c) -> Subclass& {
          return self.exclude(c);
        },
        py::arg("c"),
        R"pbdoc(
Define the set of pairs that must be excluded from every congruence (container).

:param c: A container of rules to be excluded.
:type c: Container
Define the set of pairs that must be excluded from every congruence.The congruences computed by an instance of this type will never contain the relations input here. In other words, the congruences computed are only taken among those that do not contain the pairs of elements of the underlying semigroup (defined by the presentation returned by :any:`presentation` ) represented by the relations returned by ``exclude()``.Define a set of pairs that should be included in every congruence.

:raises LibsemigroupsError:  if ``c`` does not define a set of pairs, i.e. there are an odd number of words in ``c``.

:raises LibsemigroupsError:  if ``validate_word(w)`` throws for any word ``w`` in ``c``.


:returns: A reference to ``self``.

:rtype: typename Container
)pbdoc");
    ss.def(
        "clear_exclude",
        [](SimsSettings_& self) -> Subclass& { return self.clear_exclude(); },
        R"pbdoc(
Clear the set of excluded words.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.

:returns: A reference to ``self``.

:rtype: Subclass
)pbdoc");
    ss.def("stats",
           &SimsSettings_::stats,
           R"pbdoc(
Get the current stats object.
Returns a const reference to the current stats object.The value returned by this function is a ``SimsStats`` object which contains some statistics related to the current :any:`Sims1` or :any:`Sims2` instance and any part of the depth first search already conducted.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:returns: A const reference to ``SimsStats``.

:rtype: SimsStats
)pbdoc");
    ss.def(
        "idle_thread_restarts",
        [](SimsSettings_ const& self) { return self.idle_thread_restarts(); },
        R"pbdoc(
Get the idle thread restart attempt count.
Returns the number of times an idle thread will attempt to restart before yielding during the execution of :any:`Sims1` or :any:`Sims2`.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:returns: A const reference to ``std::vector>`` , the set of all pruners.

:rtype: int
)pbdoc");
    ss.def(
        "idle_thread_restarts",
        [](SimsSettings_& self, size_t val) -> Subclass& {
          return self.idle_thread_restarts(val);
        },
        py::arg("val"),
        R"pbdoc(
Set the idle thread restart attempt count.

:param val: the maximum number of times an idle thread will attempt to restart before yielding.
:type val: int
This function sets the idle thread restart attempt count used in :any:`Sims1` or :any:`Sims2`.The default value is ``64``.

:raises LibsemigroupsError:  if the argument ``val`` is 0.


:returns: A reference to ``self``.

:rtype: Subclass
)pbdoc");
    ss.def(py::init<Subclass const&>(), R"pbdoc(
Construct from Subclass object
)pbdoc");
    ss.def(py::init<Sims1 const&>(), R"pbdoc(
Construct from Sims1 object
)pbdoc");
    ss.def(py::init<Sims2 const&>(), R"pbdoc(
Construct from Sims2 object
)pbdoc");
    ss.def(py::init<RepOrc const&>(), R"pbdoc(
Construct from RepOrc object
)pbdoc");
    ss.def(py::init<MinimalRepOrc const&>(), R"pbdoc(
Construct from MinimalRepOrc object
)pbdoc");
    ss.def(
        "init",
        [](SimsSettings_& self, Subclass const& that) {
          return self.init(that);
        },
        py::arg("that"),
        R"pbdoc(
Initialize from Subclass object.
)pbdoc");
    ss.def(
        "init",
        [](SimsSettings_& self, Sims1 const& that) { return self.init(that); },
        py::arg("that"),
        R"pbdoc(
Initialize from Sims1 object.
)pbdoc");
    ss.def(
        "init",
        [](SimsSettings_& self, Sims2 const& that) { return self.init(that); },
        py::arg("that"),
        R"pbdoc(
Initialize from Sims2 object.
)pbdoc");
    ss.def(
        "init",
        [](SimsSettings_& self, RepOrc const& that) { return self.init(that); },
        py::arg("that"),
        R"pbdoc(
Initialize from RepOrc object.
)pbdoc");
    ss.def(
        "init",
        [](SimsSettings_& self, MinimalRepOrc const& that) {
          return self.init(that);
        },
        py::arg("that"),
        R"pbdoc(
Initialize from MinimalRepOrc object.
)pbdoc");
  }

  void init_sims(py::module& m) {
    bind_sims<Sims1>(m, "SimsSettingsSims1");
    bind_sims<Sims2>(m, "SimsSettingsSims2");
    bind_sims<RepOrc>(m, "SimsSettingsRepOrc");
    bind_sims<MinimalRepOrc>(m, "SimsSettingsMinimalRepOrc");

    py::class_<SimsStats> st(m,
                             "SimsStats",
                             R"pbdoc(
For keeping track of various statistics arising during the runtime of the low index algorithm.

Defined in ``sims.hpp``.On this page we describe the :any:`SimsStats` class. The purpose of this class is to collect some statistics related to :any:`Sims1` or :any:`Sims2` class template.

.. seealso::  :any:`Sims1`

.. seealso::  :any:`Sims2`)pbdoc");
    st.def("__repr__",
           [](SimsStats const& st) { return to_human_readable_repr(st); });
    st.def(
        "count_last",
        [](SimsStats const& d) { return d.count_last.load(); },
        R"pbdoc(
Number of congruences found at time of last report.This member holds the of the number of congruences found by the :any:`Sims1` or :any:`Sims2` algorithm at the time of the last call to :any:`stats_check_point`.

.. seealso::  :any:`stats_check_point`

.. seealso::  :any:`count_now`)pbdoc");
    st.def(
        "count_now",
        [](SimsStats const& d) { return d.count_now.load(); },
        R"pbdoc(
Number of congruences found up to this point.This member tracks the total number of congruences found during the running of the :any:`Sims1` or :any:`Sims2` algorithm.

.. seealso::  :any:`count_last`)pbdoc");
    st.def(
        "max_pending",
        [](SimsStats const& d) { return d.max_pending.load(); },
        R"pbdoc(
The maximum number of pending definitions.A *pending definition* is just an edge that will be defined at some point in the future in the :any:`WordGraph` represented by a :any:`Sims1` or :any:`Sims2` instance at any given moment.This member tracks the maximum number of such pending definitions that occur during the running of the algorithms in :any:`Sims1` or :any:`Sims2`.)pbdoc");
    st.def(
        "total_pending_last",
        [](SimsStats const& d) { return d.total_pending_last.load(); },
        R"pbdoc(
The total number of pending definitions at time of last report.A *pending definition* is just an edge that will be defined at some point in the future in the :any:`WordGraph` represented by a :any:`Sims1` or :any:`Sims2` instance at any given moment.This member tracks the total number of pending definitions that occur at the time of the last call to :any:`stats_check_point` . This is the same as the number of nodes in the search tree encounter during the running of :any:`Sims1` or :any:`Sims2`.

.. seealso::  :any:`stats_check_point`

.. seealso::  :any:`total_pending_now`)pbdoc");
    st.def(
        "total_pending_now",
        [](SimsStats const& d) { return d.total_pending_now.load(); },
        R"pbdoc(
The total number of pending definitions.A *pending definition* is just an edge that will be defined at some point in the future in the :any:`WordGraph` represented by a :any:`Sims1` or :any:`Sims2` instance at any given moment.This member tracks the total number of pending definitions that occur during the running of the algorithms in :any:`Sims1` or :any:`Sims2` . This is the same as the number of nodes in the search tree encounter during the running of :any:`Sims1` or :any:`Sims2`.

.. seealso::  :any:`total_pending_last`)pbdoc");
    st.def(py::init<>(), R"pbdoc(
Default constructor.Constructs a :any:`SimsStats` object with all statistics set to zero.

:parameters (none): 

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.)pbdoc");
    st.def(py::init<SimsStats const&>(), R"pbdoc(

:param that: the SimsStats to copy.
:type that: SimsStats
Copy constructor.Returns a :any:`SimsStats` object that is a copy of ``that`` . The state of the new :any:`SimsStats` object is the same as ``that`` . This triggers an atomic load on the member variables of ``that``.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.)pbdoc");
    st.def(
        "init",
        [](SimsStats& self) -> SimsStats& { return self.init(); },
        R"pbdoc(
Reinitialize an existing SimsStats object.
This function puts a :any:`SimsStats` object back into the same state as if it had been newly default constructed.

:parameters (none): 

:raises This:  function guarantees not to throw a :any:`LibsemigroupsError`.

:returns: A reference to ``self``.

:rtype: SimsStats
)pbdoc");
    st.def(
        "init",
        [](SimsStats& self, SimsStats const& that) -> SimsStats& {
          return self.init(that);
        },
        py::arg("that"),
        R"pbdoc(
Initialize from SimsStats.
)pbdoc");
    st.def("stats_check_point",
           &SimsStats::stats_check_point,
           R"pbdoc(
Store the current statistic values.
Overwrites the values of :any:`count_last` and :any:`total_pending_last` with :any:`count_now` and :any:`total_pending_now` respectively. Triggers an atomic load on :any:`count_now` and :any:`total_pending_now`.

:parameters (none): 

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.)pbdoc");
    st.def("stats_zero",
           &SimsStats::stats_zero,
           R"pbdoc(
Set all statistics to zero.
Set all statistics to zero.

:parameters (none): 

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.)pbdoc");

    py::class_<Sims1, SimsSettings<Sims1>> s1(m,
                                              "Sims1",
                                              R"pbdoc(
For computing finite index right congruences of a finitely presented semigroup or monoid.

The algorithm implemented by this class is essentially the low index subgroup algorithm
for finitely presented groups described in Section 5.6 of :cite:`Sims1994aa`.
The low index subgroups algorithm was adapted for semigroups and monoids by
R. Cirpons, J. D. Mitchell, and M. Tsalakou; see :cite:`Anagnostopoulou-Merkouri2023aa`. 

The purpose of this class is to provide the functions :py:meth:`~Sims1.iterator`,
:py:meth:`~Sims1.for_each` and :py:meth:`~Sims1.find_if`,
which permit iterating through the one-sided congruences of a semigroup or monoid defined
by a presentation containing (a possibly empty) set of pairs and with at most a given
number of classes. An iterator returned by :py:meth:`~Sims1.iterator` yields :any:`WordGraph`
instances representing the action of the semigroup or monoid on the classes of a congruence.

.. seealso::  :any:`Sims2` for equivalent functionality for 2-sided congruences.)pbdoc");

    s1.def("__repr__",
           [](Sims1 const& s1) { return to_human_readable_repr(s1); });
    s1.def(py::init<>(), R"pbdoc(
Default constructor.
)pbdoc");
    s1.def(py::init<Presentation<word_type> const&>(),
           py::arg("p"),
           R"pbdoc(
Construct from a presentation.

The rules of the presentation ``p`` are used at every node in the
depth first search conducted by an object of this type. 

:raises LibsemigroupsError:  if :any:`to_presentation` throws

:raises LibsemigroupsError:  if ``p`` has 0-generators and 0-relations.

.. seealso::  :any:`presentation`, :py:meth:`~Sims1.init`)pbdoc");
    s1.def(py::init<Presentation<std::string> const&>(),
           py::arg("p"),
           R"pbdoc(
Construct from a presentation.

The rules of the presentation ``p`` are used at every node in the
depth first search conducted by an object of this type. 

:raises LibsemigroupsError:  if :any:`to_presentation` throws

:raises LibsemigroupsError:  if ``p`` has 0-generators and 0-relations.

.. seealso::  :any:`presentation`, :py:meth:`~Sims1.init`)pbdoc");
    s1.def(py::init<Sims1 const&>(),
           py::arg("other"),
           R"pbdoc(
Default copy constructor.
)pbdoc");
    s1.def(
        "init",
        [](Sims1& self, Presentation<word_type> const& p) -> Sims1& {
          return self.init(p);
        },
        py::arg("p"),
        R"pbdoc(
Reinitialize an existing Sims1 object.

:param p: the presentation.
:type p: Presentation
This function puts an object back into the same state as if it had been newly constructed from the presentation ``p``.

:raises LibsemigroupsError:  if ``to_presentation(p)`` throws

:raises LibsemigroupsError:  if ``p`` is not valid

:raises LibsemigroupsError:  if ``p`` has 0-generators and 0-relations.

.. seealso::  presentation(Presentation<Word> const&)


:returns: A reference to ``self``.

:rtype: typename Word
)pbdoc");
    s1.def(
        "init",
        [](Sims1& self, Presentation<std::string> const& p) -> Sims1& {
          return self.init(p);
        },
        py::arg("p"),
        R"pbdoc(
Reinitialize an existing Sims1 object.

:param p: the presentation.
:type p: Presentation
This function puts an object back into the same state as if it had been newly constructed from the presentation ``p``.

:raises LibsemigroupsError:  if ``to_presentation(p)`` throws

:raises LibsemigroupsError:  if ``p`` is not valid

:raises LibsemigroupsError:  if ``p`` has 0-generators and 0-relations.

.. seealso::  presentation(Presentation<Word> const&)


:returns: A reference to ``self``.

:rtype: typename Word
)pbdoc");
    s1.def("number_of_congruences",
           &Sims1::number_of_congruences,
           py::arg("n"),
           R"pbdoc(
Returns the number of one-sided congruences with up to a given number of classes.

:param n: the maximum number of congruence classes.
:type n: int
This function is similar to ``std::distance(begin(n), end(n))`` and exists to:

*  provide some feedback on the progress of the computation if it runs for more than 1 second.
*  allow for the computation of ``std::distance(begin(n), end(n))`` to be performed using :any:`number_of_threads` in parallel.



:raises LibsemigroupsError:  if ``n`` is ``0``.

:raises LibsemigroupsError:  if ``presentation()`` has 0-generators and 0-relations (i.e. it has not been initialised).


:returns: A value of type ``int``.

:rtype: int
)pbdoc");
    s1.def("for_each",
           &Sims1::for_each,
           py::arg("n"),
           py::arg("pred"),
           R"pbdoc(
Apply a unary predicate to every one-sided congruence with at most a given number of classes.

:param n: the maximum number of congruence classes. 
:type n: int

:param pred: the predicate applied to every congruence found.
:type pred: std::function
Apply the function ``pred`` to every one-sided congruence with at most ``n`` classesThis function is similar to ``std::for_each(begin(n), end(n), pred)`` and exists to:

*  provide some feedback on the progress of the computation if it runs for more than 1 second.
*  allow for the computation of ``std::for_each(begin(n), end(n), pred)`` to be performed using :any:`number_of_threads` in parallel.



:raises LibsemigroupsError:  if ``n`` is ``0``.

:raises LibsemigroupsError:  if ``presentation()`` has 0-generators and 0-relations (i.e. it has not been initialised).

.. seealso::  :any:`cbegin`


:returns:  (None)

:rtype: None
)pbdoc");
    s1.def("find_if",
           &Sims1::find_if,
           py::arg("n"),
           py::arg("pred"),
           R"pbdoc(
Apply a unary predicate to one-sided congruences with at most a given number of classes, until it returns true.

:param n: the maximum number of congruence classes. 
:type n: int

:param pred: the predicate applied to every congruence found.
:type pred: std::function
Apply the function ``pred`` to every one-sided congruence with at most ``n`` classes, until it returns ``True``.This function is similar to ``std::find_if(begin(n), end(n), pred)`` and exists to:

*  provide some feedback on the progress of the computation if it runs for more than 1 second.
*  allow for the computation of ``std::find_if(begin(n), end(n), pred)`` to be performed using :any:`number_of_threads` in parallel.



:raises LibsemigroupsError:  if ``n`` is ``0``.

:raises LibsemigroupsError:  if ``presentation()`` has 0-generators and 0-relations (i.e. it has not been initialised).

.. seealso::  :any:`cbegin`


:returns: The first :any:`WordGraph` for which ``pred`` returns ``True``.

:rtype: word_graph_type
)pbdoc");
    s1.def(
        "iterator",
        [](Sims1 const& self, size_type n) {
          return py::make_iterator(self.cbegin(n), self.cend(n));
        },
        py::arg("n"),
        R"pbdoc(
Returns a forward iterator pointing at the first congruence.

:param n: the maximum number of classes in a congruence.
:type n: int
Returns a forward iterator pointing to the :any:`WordGraph` representing the first congruence described by an object of this type with at most ``n`` classes.If incremented, the iterator will point to the next such congruence. The order in which the congruences are returned is implementation specific. Iterators of the type returned by this function are equal whenever they point to equal objects. The iterator is exhausted if and only if it points to an :any:`WordGraph` with zero nodes.The meaning of the :any:`WordGraph` pointed at by the returned iterator depends on whether the input is a monoid presentation (i.e. :any:`Presentation::contains_empty_word()` returns ``True`` ) or a semigroup presentation. If the input is a monoid presentation for a monoid :math:`M` , then the :any:`WordGraph` pointed to by an iterator of this type has precisely ``n`` nodes, and the right action of :math:`M` on the nodes of the word graph is isomorphic to the action of :math:`M` on the classes of a right congruence.If the input is a semigroup presentation for a semigroup :math:`S` , then the :any:`WordGraph` has ``n`` + 1 nodes, and the right action of :math:`S` on the nodes :math:`\{1, \ldots, n\}` of the :any:`WordGraph` is isomorphic to the action of :math:`S` on the classes of a right congruence. It'd probably be better in this case if node :math:`0` was not included in the output :any:`WordGraph` , but it is required in the implementation of the low-index congruence algorithm, and to avoid unnecessary copies, we've left it in for the time being.

:raises LibsemigroupsError:  if ``n`` is ``0``.

:raises LibsemigroupsError:  if ``presentation()`` has 0-generators and 0-relations (i.e. it has not been initialised).

.. seealso::  :any:`cend`


:returns: An iterator ``it`` of type ``Iterator`` pointing to an :any:`WordGraph` with at most ``n`` nodes.

:rtype: Iterator
)pbdoc");

    py::class_<Sims2, SimsSettings<Sims2>> s2(m,
                                              "Sims2",
                                              R"pbdoc(
For computing finite index two-sided congruences of a finitely presented semigroup or monoid.

Defined in ``sims.hpp``.On this page we describe the functionality relating to the small index congruence algorithm for 2-sided congruences. The algorithm implemented by this class is described in :any:`[1]`.The purpose of this class is to provide the functions :any:`cbegin` , :any:`cend` , :any:`for_each` , and :any:`find_if` which permit iterating through the two-sided congruences of a semigroup or monoid defined by a presentation containing, or not containing, (possibly empty) sets of pairs and with at most a given number of classes. An iterator returned by :any:`cbegin` points at an :any:`WordGraph` instance containing the action of the semigroup or monoid on the classes of a congruence.

.. seealso::  :any:`Sims1` for equivalent functionality for 1-sided congruences.

.. seealso::  :any:`SimsSettings` for the various things that can be set in a :any:`Sims2` object.)pbdoc");
    s2.def("__repr__",
           [](Sims2 const& s2) { return to_human_readable_repr(s2); });
    s2.def(py::init<>(), R"pbdoc(
Default constructor.
)pbdoc");
    s2.def(py::init<Sims2 const&>(), R"pbdoc(
Default copy constructor.
)pbdoc");
    s2.def(py::init<Presentation<word_type> const&>(), R"pbdoc(
Construct from a presentation.

:param p: the presentation
:type p: Presentation
Constructs an instance from a presentation of any kind.The rules of the presentation ``p`` are used at every node in the depth first search conducted by an object of this type.If the template parameter ``Word`` is not :any:`word_type` , then the parameter ``p`` is first converted to a value of type ``Presentation`` and it is this converted value that is used.

:raises LibsemigroupsError:  if ``to_presentation(p)`` throws

:raises LibsemigroupsError:  if ``p`` is not valid

:raises LibsemigroupsError:  if ``p`` has 0-generators and 0-relations.

.. seealso::  presentation

.. seealso::  :any:`init`)pbdoc");
    s2.def(py::init<Presentation<std::string> const&>(), R"pbdoc(
Construct from a presentation.

:param p: the presentation
:type p: Presentation
Constructs an instance from a presentation of any kind.The rules of the presentation ``p`` are used at every node in the depth first search conducted by an object of this type.If the template parameter ``Word`` is not :any:`word_type` , then the parameter ``p`` is first converted to a value of type ``Presentation`` and it is this converted value that is used.

:raises LibsemigroupsError:  if ``to_presentation(p)`` throws

:raises LibsemigroupsError:  if ``p`` is not valid

:raises LibsemigroupsError:  if ``p`` has 0-generators and 0-relations.

.. seealso::  presentation

.. seealso::  :any:`init`)pbdoc");
    // TODO: same as above
    //     s2.def("init",
    //            &Sims2::init,
    //            R"pbdoc(
    // Reinitialize an existing Sims2 object.
    // This function puts a :any:`Sims2` object back into the same state as if
    // it had been newly default constructed.
    //
    // :parameters (none):
    //
    // :raises This:  function guarantees not to throw a ``LibsemigroupsError``.
    //
    // :returns: A reference to ``self``.
    //
    // :rtype: Sims2
    // )pbdoc");
    s2.def("number_of_congruences",
           &Sims2::number_of_congruences,
           py::arg("n"),
           R"pbdoc(
Returns the number of one-sided congruences with up to a given number of classes.

:param n: the maximum number of congruence classes.
:type n: int
This function is similar to ``std::distance(begin(n), end(n))`` and exists to:

*  provide some feedback on the progress of the computation if it runs for more than 1 second.
*  allow for the computation of ``std::distance(begin(n), end(n))`` to be performed using :any:`number_of_threads` in parallel.



:raises LibsemigroupsError:  if ``n`` is ``0``.

:raises LibsemigroupsError:  if ``presentation()`` has 0-generators and 0-relations (i.e. it has not been initialised).


:returns: A value of type ``int``.

:rtype: int
)pbdoc");
    s2.def("for_each",
           &Sims2::for_each,
           py::arg("n"),
           py::arg("pred"),
           R"pbdoc(
Apply a unary predicate to every one-sided congruence with at most a given number of classes.

:param n: the maximum number of congruence classes. 
:type n: int

:param pred: the predicate applied to every congruence found.
:type pred: std::function
Apply the function ``pred`` to every one-sided congruence with at most ``n`` classesThis function is similar to ``std::for_each(begin(n), end(n), pred)`` and exists to:

*  provide some feedback on the progress of the computation if it runs for more than 1 second.
*  allow for the computation of ``std::for_each(begin(n), end(n), pred)`` to be performed using :any:`number_of_threads` in parallel.



:raises LibsemigroupsError:  if ``n`` is ``0``.

:raises LibsemigroupsError:  if ``presentation()`` has 0-generators and 0-relations (i.e. it has not been initialised).

.. seealso::  :any:`cbegin`


:returns:  (None)

:rtype: None
)pbdoc");
    s2.def("find_if",
           &Sims2::find_if,
           py::arg("n"),
           py::arg("pred"),
           R"pbdoc(
Apply a unary predicate to one-sided congruences with at most a given number of classes, until it returns true.

:param n: the maximum number of congruence classes. 
:type n: int

:param pred: the predicate applied to every congruence found.
:type pred: std::function
Apply the function ``pred`` to every one-sided congruence with at most ``n`` classes, until it returns ``True``.This function is similar to ``std::find_if(begin(n), end(n), pred)`` and exists to:

*  provide some feedback on the progress of the computation if it runs for more than 1 second.
*  allow for the computation of ``std::find_if(begin(n), end(n), pred)`` to be performed using :any:`number_of_threads` in parallel.



:raises LibsemigroupsError:  if ``n`` is ``0``.

:raises LibsemigroupsError:  if ``presentation()`` has 0-generators and 0-relations (i.e. it has not been initialised).

.. seealso::  :any:`cbegin`


:returns: The first :any:`WordGraph` for which ``pred`` returns ``True``.

:rtype: word_graph_type
)pbdoc");
    s2.def(
        "iterator",
        [](Sims2 const& self, size_type n) {
          return py::make_iterator(self.cbegin(n), self.cend(n));
        },
        py::arg("n"),
        R"pbdoc(
Returns a forward iterator pointing at the first congruence.

:param n: the maximum number of classes in a congruence.
:type n: int
Returns a forward iterator pointing to the :any:`WordGraph` representing the first congruence described by an object of this type with at most ``n`` classes.If incremented, the iterator will point to the next such congruence. The order in which the congruences are returned is implementation specific. Iterators of the type returned by this function are equal whenever they point to equal objects. The iterator is exhausted if and only if it points to an :any:`WordGraph` with zero nodes.The meaning of the :any:`WordGraph` pointed at by the returned iterator depends on whether the input is a monoid presentation (i.e. :any:`Presentation::contains_empty_word()` returns ``True`` ) or a semigroup presentation. If the input is a monoid presentation for a monoid :math:`M` , then the :any:`WordGraph` pointed to by an iterator of this type has precisely ``n`` nodes, and the right action of :math:`M` on the nodes of the word graph is isomorphic to the action of :math:`M` on the classes of a right congruence.If the input is a semigroup presentation for a semigroup :math:`S` , then the :any:`WordGraph` has ``n`` + 1 nodes, and the right action of :math:`S` on the nodes :math:`\{1, \ldots, n\}` of the :any:`WordGraph` is isomorphic to the action of :math:`S` on the classes of a right congruence. It'd probably be better in this case if node :math:`0` was not included in the output :any:`WordGraph` , but it is required in the implementation of the low-index congruence algorithm, and to avoid unnecessary copies, we've left it in for the time being.

:raises LibsemigroupsError:  if ``n`` is ``0``.

:raises LibsemigroupsError:  if ``presentation()`` has 0-generators and 0-relations (i.e. it has not been initialised).

.. seealso::  :any:`cend`


:returns: An iterator ``it`` of type ``Iterator`` pointing to an :any:`WordGraph` with at most ``n`` nodes.

:rtype: Iterator
)pbdoc");

    py::class_<RepOrc, SimsSettings<RepOrc>> ro(m,
                                                "RepOrc",
                                                R"pbdoc(
For computing small degree transformation representations of a finite semigroup or monoid.

Defined in ``sims.hpp``.This class is a helper for :any:`Sims1` calling the :any:`word_graph` member function attempts to find a right congruence, represented as an :any:`WordGraph` , of the semigroup or monoid defined by the presentation consisting of its :any:`presentation` and long_rules with the following properties:

*  the transformation semigroup defined by the :any:`WordGraph` has size :any:`target_size` ;
*  the number of nodes in the :any:`WordGraph` is at least :any:`min_nodes` and at most :any:`max_nodes`.

If no such :any:`WordGraph` can be found, then an empty :any:`WordGraph` is returned (with ``0`` nodes and ``0`` edges).)pbdoc");
    ro.def("__repr__",
           [](RepOrc const& ro) { return to_human_readable_repr(ro); });
    ro.def(py::init<>(), R"pbdoc(
Default constructor.
)pbdoc");
    ro.def(
        "init",
        [](RepOrc& self) -> RepOrc& { return self.init(); },
        R"pbdoc(
)pbdoc");
    ro.def(
        "max_nodes",
        [](RepOrc const& self) { return self.max_nodes(); },
        R"pbdoc(
Get the current maximum number of nodes.
This function returns the current value for the maximum number of nodes in the :any:`WordGraph` that we are seeking.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:returns: A value of type ``int``.

:rtype: int
)pbdoc");
    ro.def(
        "max_nodes",
        [](RepOrc& self, size_t val) -> RepOrc& { return self.max_nodes(val); },
        py::arg("val"),
        R"pbdoc(
Set the maximum number of nodes.

:param val: the maximum number of nodes
:type val: int
This function sets the maximum number of nodes in the :any:`WordGraph` that we are seeking.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.


:returns: A reference to ``self``.

:rtype: RepOrc
)pbdoc");
    ro.def(
        "min_nodes",
        [](RepOrc const& self) { return self.min_nodes(); },
        R"pbdoc(
Get the current minimum number of nodes.
This function returns the current value for the minimum number of nodes in the :any:`WordGraph` that we are seeking.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:returns: A value of type ``int``.

:rtype: int
)pbdoc");
    ro.def(
        "min_nodes",
        [](RepOrc& self, size_t val) -> RepOrc& { return self.min_nodes(val); },
        py::arg("val"),
        R"pbdoc(
Set the minimum number of nodes.

:param val: the minimum number of nodes
:type val: int
This function sets the minimal number of nodes in the :any:`WordGraph` that we are seeking.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.


:returns: A reference to ``self``.

:rtype: RepOrc
)pbdoc");
    ro.def(
        "target_size",
        [](RepOrc const& self) { return self.target_size(); },
        R"pbdoc(
Get the current target size.
This function returns the current value for the target size, i.e. the desired size of the transformation semigroup corresponding to the :any:`WordGraph` returned by the function :any:`word_graph`.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:returns: A value of type ``int``.

:rtype: int
)pbdoc");
    ro.def(
        "target_size",
        [](RepOrc& self, size_t val) -> RepOrc& {
          return self.target_size(val);
        },
        py::arg("val"),
        R"pbdoc(
Set the target size.

:param val: the target size.
:type val: int
This function sets the target size, i.e. the desired size of the transformation semigroup corresponding to the :any:`WordGraph` returned by the function :any:`word_graph`.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.


:returns: A reference to ``self``.

:rtype: RepOrc
)pbdoc");
    ro.def("word_graph",
           &RepOrc::word_graph,
           R"pbdoc(
Get the word_graph.

:param (None): this function has no parameters.This function attempts to find a right congruence, represented as an :any:`WordGraph` , of the semigroup or monoid defined by the presentation consisting of its :any:`presentation` and long_rules with the following properties:

*  the transformation semigroup defined by the :any:`WordGraph` has size :any:`target_size` ;
*  the number of nodes in the :any:`WordGraph` is at least :any:`min_nodes` and at most :any:`max_nodes`.

If no such :any:`WordGraph` can be found, then an empty :any:`WordGraph` is returned (with ``0`` nodes and ``0`` edges).

:exceptions this function guarantees not to throw a libsemigroupsexception. : 


:returns: A value of type ``WordGraph``.

:rtype: Sims1::word_graph_type
)pbdoc");

    py::class_<MinimalRepOrc, SimsSettings<MinimalRepOrc>> mro(m,
                                                               "MinimalRepOrc",
                                                               R"pbdoc(
For computing the minimal degree of a transformation representation arising from a right congruences of a finite semigroup or monoid.

Defined in ``sims.hpp``.This class is a helper for :any:`Sims1` , calling the :any:`word_graph` member function attempts to find a right congruence, represented as an :any:`WordGraph` , with the minimum possible number of nodes such that the action of the semigroup or monoid defined by the presentation consisting of its :any:`presentation` on the nodes of the :any:`WordGraph` corresponds to a semigroup of size :any:`target_size`.If no such :any:`WordGraph` can be found, then an empty :any:`WordGraph` is returned (with ``0`` nodes and ``0`` edges).)pbdoc");
    mro.def("__repr__", [](MinimalRepOrc const& mro) {
      return to_human_readable_repr(mro);
    });
    mro.def(py::init<>(), R"pbdoc(
Default constructor.
)pbdoc");
    mro.def(
        "init",
        [](MinimalRepOrc& self) -> MinimalRepOrc& { return self.init(); },
        R"pbdoc(
)pbdoc");
    mro.def(
        "target_size",
        [](MinimalRepOrc const& self) { return self.target_size(); },
        R"pbdoc(
Get the current target size.
This function returns the current value for the target size, i.e. the desired size of the transformation semigroup corresponding to the :any:`WordGraph` returned by the function :any:`word_graph`.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:returns: A value of type ``int``.

:rtype: int
)pbdoc");
    mro.def(
        "target_size",
        [](MinimalRepOrc& self, size_t val) -> MinimalRepOrc& {
          return self.target_size(val);
        },
        py::arg("val"),
        R"pbdoc(
Set the target size.

:param val: the target size.
:type val: int
This function sets the target size, i.e. the desired size of the transformation semigroup corresponding to the :any:`WordGraph` returned by the function :any:`word_graph`.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.


:returns: A reference to ``self``.

:rtype: MinimalRepOrc
)pbdoc");
    mro.def("word_graph",
            &MinimalRepOrc::word_graph,
            R"pbdoc(
Get the word graph.

:param (None): this function has no parameters.This function attempts to find a right congruence, represented as an :any:`WordGraph` , with the minimum possible number of nodes such that the action of the semigroup or monoid defined by the presentation consisting of its :any:`presentation` and long_rules on the nodes of the :any:`WordGraph` corresponds to a semigroup of size :any:`target_size`.If no such :any:`WordGraph` can be found, then an empty :any:`WordGraph` is returned (with ``0`` nodes and ``0`` edges).The algorithm implemented by this function repeatedly runs:

.. code-block::

RepOrc(* this )
    .min_nodes(1)
    .max_nodes(best)
    .target_size( :any:`target_size` ())
    .word_graph(); where ``best`` is initially :any:`target_size` , until the returned :any:`WordGraph` is empty, and then the penultimate :any:`WordGraph` is returned (if any).

:exceptions this function guarantees not to throw a libsemigroupsexception. : 


:returns: A value of type ``WordGraph``.

:rtype: Sims1::word_graph_type
)pbdoc");

    py::class_<SimsRefinerFaithful> srf(m,
                                        "SimsRefinerFaithful",
                                        R"pbdoc(
For pruning the search tree when looking for congruences arising from right or two-sided congruences representing faithful actions.

Defined in ``sims.hpp``.This class provides a pruner for pruning the search tree when looking for right congruences representing faithful actions. A right congruence represents a faithful action if and only if it does not contain any non-trivial two-sided congruence. Equivalently, a word graph of a right congruence represents a faithful action if and only if there is no nontrivial pair of words :math:`(u, v)` such that every vertex of the word graph is compatible with :math:`(u, v)`.

.. seealso::  :any:`SimsSettings::pruners`

.. seealso::  :any:`SimsSettings::add_pruner`)pbdoc");
    srf.def("__repr__", [](SimsRefinerFaithful const& srf) {
      return to_human_readable_repr(srf);
    });
    srf.def(py::init<>(), R"pbdoc(
Default constructor.
)pbdoc");
    srf.def(py::init<std::vector<word_type> const&>(), R"pbdoc(
Construct from set of forbidden pairs.
Constructs a :any:`SimsRefinerFaithful` pruner with respect to the set of forbidden relations in ``forbid``.If ``forbid`` contains no trivial pairs (i.e. pairs of words that are equal in the underlying semigroup or monoid), then all word graphs rejected by :any:`SimsRefinerFaithful` are guaranteed to not be extendable to a word graph representing a faithful congruence. Otherwise, the pruner will incorrectly reject all word graphs.If in addition ``forbid`` is a set of relations containing all minimal congruence generating pairs of a given semigroup or monoid, then :any:`SimsRefinerFaithful` will also correctly determine if a complete word graph represents a faithful congruence. Otherwise, the complete word graphs accepted by :any:`SimsRefinerFaithful` are not guaranteed to be faithful and must be checked by some other means.)pbdoc");
    srf.def("forbid",
            &SimsRefinerFaithful::forbid,
            R"pbdoc(
Get the forbidden pairs defining the refiner.
Returns a const reference to the current forbidden pairs.This function returns the defining forbidden pairs of a :any:`SimsRefinerFaithful` instance.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:returns: A const reference to ``list``.

:rtype: list
)pbdoc");
    srf.def(
        "init",
        [](SimsRefinerFaithful& self) -> SimsRefinerFaithful& {
          return self.init();
        },
        R"pbdoc(
Reinitialize an existing SimsRefinerFaithful object.
This function puts an object back into the same state as if it had been newly default constructed.

:returns: A reference to ``self``.

:rtype: SimsRefinerFaithful
)pbdoc");
    srf.def(
        "init",
        [](SimsRefinerFaithful& self, std::vector<word_type> const& forbid)
            -> SimsRefinerFaithful& { return self.init(forbid); },
        py::arg("forbid"),
        R"pbdoc(
Reinitialize an existing SimsRefinerFaithful object from a set of forbidden pairs.
This function puts an object back into the same state as if it had been newly constructed from set of forbidden pairs ``forbid``.

:returns: A reference to ``self``.

:rtype: SimsRefinerFaithful
)pbdoc");
    srf.def(
        "__call__",
        [](SimsRefinerFaithful& d, word_graph_type const& wg) { return d(wg); },
        py::arg("wg"),
        R"pbdoc(
Check if a word graph can be extended to one defining a faithful congruence.
Returns ``False`` if there is no way of adding edges and nodes to

:wg:  which will result in a word graph defining a faithful congruence. Otherwise returns ``True``.)pbdoc");

    py::class_<SimsRefinerIdeals> sri(m,
                                      "SimsRefinerIdeals",
                                      R"pbdoc(
For pruning the search tree when looking for congruences arising from right or two-sided ideals.

Defined in ``sims.hpp``.This class provides a pruner for pruning the search tree when looking for right or two-sided congruences arising from ideals (Rees congruences).

.. seealso::  :any:`SimsSettings::pruners`

.. seealso::  :any:`SimsSettings::add_pruner`)pbdoc");
    sri.def("__repr__", [](SimsRefinerIdeals const& sri) {
      return to_human_readable_repr(sri);
    });
    sri.def(py::init<>(), R"pbdoc(
Default constructor.
)pbdoc");
    sri.def(py::init<Presentation<std::string> const&>(), R"pbdoc(
Construct from presentation.
Constructs a :any:`SimsRefinerIdeals` pruner for the semigroup or monoid defined by ``p``.)pbdoc");
    sri.def(py::init<Presentation<word_type> const&>(), R"pbdoc(
Construct from presentation.
Constructs a :any:`SimsRefinerIdeals` pruner for the semigroup or monoid defined by ``p``.)pbdoc");
    sri.def(
        "init",
        [](SimsRefinerIdeals& self) -> SimsRefinerIdeals {
          return self.init();
        },
        R"pbdoc(
Reinitialize an existing SimsRefinerIdeals object.
This function puts an object back into the same state as if it had been newly default constructed.

:returns: A reference to ``self``.

:rtype: typename Word
)pbdoc");
    sri.def(
        "init",
        [](SimsRefinerIdeals& self, Presentation<std::string> const& p)
            -> SimsRefinerIdeals& { return self.init(p); },
        py::arg("p"),
        R"pbdoc(
Reinitialize an existing SimsRefinerIdeals object from a presentation.
This function puts an object back into the same state as if it had been newly constructed from the presentation ``p``.

:raises LibsemigroupsError:  if ``p`` is not valid

:raises LibsemigroupsError:  if ``p`` has 0-generators and 0-relations.

.. seealso::  presentation(Presentation<std::string> const&)

:returns: A reference to ``self``.

:rtype: typename Word
)pbdoc");
    sri.def(
        "init",
        [](SimsRefinerIdeals& self, Presentation<word_type> const& p)
            -> SimsRefinerIdeals& { return self.init(p); },
        py::arg("p"),
        R"pbdoc(
Reinitialize an existing SimsRefinerIdeals object from a presentation.
This function puts an object back into the same state as if it had been newly constructed from the presentation ``p``.

:raises LibsemigroupsError:  if ``p`` is not valid

:raises LibsemigroupsError:  if ``p`` has 0-generators and 0-relations.

.. seealso::  presentation(Presentation<std::string> const&)

:returns: A reference to ``self``.

:rtype: typename Word
)pbdoc");
    sri.def("presentation",
            &SimsRefinerIdeals::presentation,
            R"pbdoc(
Get the presentation over which the refiner is defined.
Returns a const reference to the current relations of the underlying presentation.This function returns the defining presentation of a :any:`SimsRefinerIdeals` instance.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:returns: A const reference to ``Presentation``.

:rtype: Presentation
)pbdoc");
    sri.def(
        "__call__",
        [](SimsRefinerIdeals& d, word_graph_type const& wg) { return d(wg); },
        py::arg("wg"),
        R"pbdoc(
Check if a word graph can be extended to one defining a Rees congruence.
Returns ``False`` if there is no way of adding edges and nodes to

:wg:  which will result in a word graph defining a Rees congruence. Otherwise returns ``True``.)pbdoc");

    m.def(
        "right_generating_pairs",
        [](Presentation<word_type> const& p, word_graph_type const& wg) {
          auto gen_pairs
              = sims::right_generating_pairs(p, wg)
                | rx::transform([](auto x) -> relation_type { return x; });
          return py::make_iterator(rx::begin(gen_pairs), rx::end(gen_pairs));
        },
        py::arg("p"),
        py::arg("wg"),
        R"pbdoc(
TODO(0): document
)pbdoc");
    m.def(
        "right_generating_pairs",
        [](word_graph_type const& wg) {
          auto gen_pairs
              = sims::right_generating_pairs(wg)
                | rx::transform([](auto x) -> relation_type { return x; });
          return py::make_iterator(rx::begin(gen_pairs), rx::end(gen_pairs));
        },
        py::arg("wg"),
        R"pbdoc(
TODO(0): document
)pbdoc");
    m.def(
        "two_sided_generating_pairs",
        [](Presentation<word_type> const& p, word_graph_type const& wg) {
          auto gen_pairs
              = sims::two_sided_generating_pairs(p, wg)
                | rx::transform([](auto x) -> relation_type { return x; });
          return py::make_iterator(rx::begin(gen_pairs), rx::end(gen_pairs));
        },
        py::arg("p"),
        py::arg("wg"),
        R"pbdoc(
TODO(0): document
)pbdoc");
    m.def(
        "two_sided_generating_pairs",
        [](word_graph_type const& wg) {
          auto gen_pairs
              = sims::two_sided_generating_pairs(wg)
                | rx::transform([](auto x) -> relation_type { return x; });
          return py::make_iterator(rx::begin(gen_pairs), rx::end(gen_pairs));
        },
        py::arg("wg"),
        R"pbdoc(
TODO(0): document
)pbdoc");
    m.def(
        "is_right_congruence",
        [](Presentation<word_type> const& p, word_graph_type const& wg) {
          return sims::is_right_congruence(p, wg);
        },
        py::arg("p"),
        py::arg("wg"),
        R"pbdoc(
TODO(0): document
)pbdoc");
    m.def(
        "is_right_congruence_of_dual",
        [](Presentation<word_type> const& p, word_graph_type const& wg) {
          return sims::is_right_congruence_of_dual(p, wg);
        },
        py::arg("p"),
        py::arg("wg"),
        R"pbdoc(
TODO(0): document
)pbdoc");
    m.def(
        "is_two_sided_congruence",
        [](Presentation<word_type> const& p, word_graph_type const& wg) {
          return sims::is_two_sided_congruence(p, wg);
        },
        py::arg("p"),
        py::arg("wg"),
        R"pbdoc(
TODO(0): document
)pbdoc");
    m.def(
        "is_maximal_right_congruence",
        [](Presentation<word_type> const& p, word_graph_type const& wg) {
          return sims::is_maximal_right_congruence(p, wg);
        },
        py::arg("p"),
        py::arg("wg"),
        R"pbdoc(
TODO(0): document
)pbdoc");
    m.def(
        "poset",
        [](Sims1& sims, size_t n) {
          return sims::poset(sims.cbegin(n), sims.cend(n));
        },
        py::arg("sims"),
        py::arg("n"),
        R"pbdoc(
TODO(0): document
)pbdoc");
    m.def(
        "poset",
        [](Sims2& sims, size_t n) {
          return sims::poset(sims.cbegin(n), sims.cend(n));
        },
        py::arg("sims"),
        py::arg("n"),
        R"pbdoc(
TODO(0): document
)pbdoc");
  }  // init_sims

}  // namespace libsemigroups
