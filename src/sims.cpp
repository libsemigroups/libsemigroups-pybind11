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

// libsemigroups....

#include <libsemigroups/presentation.hpp>  // for Presentation
#include <libsemigroups/sims.hpp>          // for Sims1, Sims2, ....
#include <libsemigroups/types.hpp>         // for word_type
#include <libsemigroups/word-graph.hpp>    // for WordGraph

// pybind11....
// #include <pybind11/chrono.h>
// #include <pybind11/functional.h>
#include <pybind11/pybind11.h>
// #include <pybind11/stl.h>
//  TODO uncomment/delete

// libsemigroups_pybind11....
#include "main.hpp"  // for init_sims

namespace py = pybind11;

namespace libsemigroups {
  using node_type = uint32_t;
  using size_type = typename WordGraph<node_type>::size_type;

  void init_sims(py::module& m) {
    py::class_<Sims1> s1(m,
                         "Sims1",
                         R"pbdoc(
For computing finite index right congruences of a finitely presented semigroup or monoid.

Defined in ``sims.hpp``.On this page we describe the functionality relating to the small index congruence algorithm for 1-sided congruences. The algorithm implemented by this class is essentially the low index subgroup algorithm for finitely presented groups described in Section 5.6 of`Computation with Finitely Presented Groups <https://doi.org/10.1017/CBO9780511574702>`_ by C. Sims. The low index subgroups algorithm was adapted for semigroups and monoids by R. Cirpons, J. D. Mitchell, and M. Tsalakou; see :any:`[1]`The purpose of this class is to provide the functions :any:`cbegin` , :any:`cend` , :any:`for_each` , and :any:`find_if` which permit iterating through the one-sided congruences of a semigroup or monoid defined by a presentation containing (a possibly empty) set of pairs and with at most a given number of classes. An iterator returned by :any:`cbegin` points at an :any:`WordGraph` instance containing the action of the semigroup or monoid on the classes of a congruence.

.. seealso::  :any:`Sims2` for equivalent functionality for 2-sided congruences.

.. seealso::  :any:`SimsSettings` for the various things that can be set in a :any:`Sims1` object.)pbdoc");

    s1.def("__repr__", [](Sims1 const& d) {
      std::string result = "<Sims1 with repr tbd>";
      return result;
    });
    // TODO: uncomment once const_rcgp_iterator and const_cgp_iterator are added
    // s.def("sims::const_rcgp_iterator",
    //           &Sims1::sims::const_rcgp_iterator,
    //           R"pbdoc(
    // )pbdoc");
    //     s.def("sims::const_cgp_iterator",
    //           &Sims1::sims::const_cgp_iterator,
    //           R"pbdoc(
    // )pbdoc");
    s1.def(py::init<>(), R"pbdoc(
Default constructor.
)pbdoc");
    s1.def(py::init<Presentation<word_type> const&>(), R"pbdoc(
Construct from a presentation.

:param p: the presentation
:type p: Presentation
Constructs an instance from a presentation of any kind.The rules of the presentation ``p`` are used at every node in the depth first search conducted by an object of this type.If the template parameter ``Word`` is not :any:`word_type` , then the parameter ``p`` is first converted to a value of type ``Presentation`` and it is this converted value that is used.

:raises LibsemigroupsError:  if ``to_presentation(p)`` throws

:raises LibsemigroupsError:  if ``p`` is not valid

:raises LibsemigroupsError:  if ``p`` has 0-generators and 0-relations.

.. seealso::  presentation

.. seealso::  :any:`init`)pbdoc");
    s1.def(py::init<Sims1 const&>(), R"pbdoc(
Default copy constructor.
)pbdoc");
    // TODO: Figure out why this stupid function won't work
    //     s1.def("init",
    //            &Sims1::init<Presentation<word_type> const&>,
    //            py::arg("p"),
    //            R"pbdoc(
    // Reinitialize an existing Sims1 object.
    //
    // :param p: the presentation
    // :type p: Presentation
    // This function puts an object back into the same state as if it had been
    // newly constructed from the presentation ``p``.
    //
    // :raises LibsemigroupsError:  if ``to_presentation(p)`` throws
    //
    // :raises LibsemigroupsError:  if ``p`` is not valid
    //
    // :raises LibsemigroupsError:  if ``p`` has 0-generators and 0-relations.
    //
    // .. seealso::  presentation(Presentation<word_type> const&)
    //
    //
    // :returns: A reference to ``self``.
    //
    // :rtype: typename word_type
    // )pbdoc");
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

    py::class_<Sims2> s2(m,
                         "Sims2",
                         R"pbdoc(
For computing finite index two-sided congruences of a finitely presented semigroup or monoid.

Defined in ``sims.hpp``.On this page we describe the functionality relating to the small index congruence algorithm for 2-sided congruences. The algorithm implemented by this class is described in :any:`[1]`.The purpose of this class is to provide the functions :any:`cbegin` , :any:`cend` , :any:`for_each` , and :any:`find_if` which permit iterating through the two-sided congruences of a semigroup or monoid defined by a presentation containing, or not containing, (possibly empty) sets of pairs and with at most a given number of classes. An iterator returned by :any:`cbegin` points at an :any:`WordGraph` instance containing the action of the semigroup or monoid on the classes of a congruence.

.. seealso::  :any:`Sims1` for equivalent functionality for 1-sided congruences.

.. seealso::  :any:`SimsSettings` for the various things that can be set in a :any:`Sims2` object.)pbdoc");
    s2.def("__repr__", [](Sims2 const& d) {
      std::string result = "<Sims2 with repr tbd>";
      return result;
    });
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
  }  // init_sims

}  // namespace libsemigroups
