
//
// libsemigroups_pybind11
// Copyright (C) 2024 Joseph Edwards
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

// TODO(0) Check types

// C std headers....
// TODO complete or delete

// C++ stl headers....
#include <memory>  // for allocator, make_unique, unique_ptr

// libsemigroups headers
#include <libsemigroups/libsemigroups.hpp>
#include <libsemigroups/schreier-sims.hpp>

// pybind11....
#include <pybind11/pybind11.h>
// #include <pybind11/chrono.h>
// #include <pybind11/functional.h>
// #include <pybind11/stl.h>
// TODO uncomment/delete

// libsemigroups_pybind11....
#include "main.hpp"  // for init_schreier_sims

namespace py = pybind11;

namespace libsemigroups {

  namespace {
    template <size_t N, typename Point, typename Element>
    void bind_schreier_sims(py::module& m, std::string const& name) {
      using SchreierSims_ = SchreierSims<N, Point, Element>;

      std::string pyclass_name = std::string("SchreierSims") + name;

      py::class_<SchreierSims_> thing(m,
                                      pyclass_name.c_str(),
                                      R"pbdoc(
This class implements a deterministic version of the Schreier-Sims algorithm
acting on a relatively small number of points (< 1000).

:example:

.. doctest:: python

    >>> from libsemigroups_pybind11 import SchreierSims, Perm
    >>> p1 = Perm([1, 0, 2, 3, 4] + list(range(5, 255)))
    >>> p2 = Perm([1, 2, 3, 4, 0] + list(range(5, 255)))
    >>> S = SchreierSims(p1, p2)
    >>> S.size()
    120
)pbdoc");
      thing.def("__repr__", [](SchreierSims_ const& S) {
        return to_human_readable_repr(S);
      });
      thing.def(py::init<>(), R"pbdoc(
:sig=(self: SchreierSimsPerm1, gens: List[Element]) -> None:
Construct from a list of generators.

This function constructs a :any:`SchreierSimsPerm1` instance with generators in
the list *gens*.

:param gens: the list of generators.
:type gens: List[Element]

:raises LibsemigroupsError: if the generators do not have degree equal to
      :math:`255` or :math:`511`, or the number of generators exceeds the
      maximum capacity.
)pbdoc");

      thing.def(py::init<SchreierSims_ const&>(), R"pbdoc(
      Default copy constructor.
)pbdoc");
      thing.def("__copy__",
                [](SchreierSims_ const& S) { return SchreierSims_(S); });
      thing.def("add_base_point",
                &SchreierSims_::add_base_point,
                py::arg("pt"),
                R"pbdoc(
Add a base point to the stabiliser chain.

:param pt: the base point to add.
:type pt: int

:raises LibsemigroupsError:  if *pt* is out of range.

:raises LibsemigroupsError:  if *pt* is already a base point.

:raises LibsemigroupsError:  if :any:`SchreierSimsPerm1.finished()` returns ``True``.

:complexity: Linear in the current number of base points.)pbdoc");
      thing.def("add_generator",
                &SchreierSims_::add_generator,
                py::arg("x"),
                R"pbdoc(
Add a generator.

This functions adds the argument *x* as a new generator if and only if *x* is
not already an element of the group represented by the Schreier-Sims object.

:param x: the generator to add.
:type x: Element

:returns:  ``True`` if *x* is added as a generator and ``False`` if it is not.
:rtype: bool

:raises LibsemigroupsError:  if the degree of *x* is not equal to :math:`255`
      or :math:`511`, or if ``self`` already contains the maximum number of
      elements.

:complexity: Constant
)pbdoc");
      thing.def("base",
                &SchreierSims_::base,
                py::arg("index"),
                R"pbdoc(
Get a base point.

This function gets the base point with a given index.

:param index: the index of the base point.
:type index: int

:returns: The base point with index *index*.
:rtype: int

:raises LibsemigroupsError:  if *index* is out of range.

:complexity: Constant.

)pbdoc");
      thing.def("base_size",
                &SchreierSims_::base_size,
                R"pbdoc(
Get the size of the current base.

:returns: The base size.
:rtype: int

:complexity: Constant.
)pbdoc");
      thing.def("contains",
                &SchreierSims_::contains,
                py::arg("x"),
                R"pbdoc(
Test membership of an element.

:param x: the possible element.
:type x: Element

:returns: ``True`` if *element* is a contained in the :any:`SchreierSimsPerm1`
      instance, and ``False`` otherwise.
:rtype: bool
)pbdoc");
      thing.def("currently_contains",
                &SchreierSims_::currently_contains,
                py::arg("x"),
                R"pbdoc(
Test membership of an element without running.

This function tests the membership of an element without running the algorithm.

:param x: the possible element.
:type x: Element

:returns: ``True`` if *element* is a contained in the :any:`SchreierSimsPerm1`
      instance, and ``False`` otherwise.
:rtype: bool
)pbdoc");
      thing.def("empty",
                &SchreierSims_::empty,
                R"pbdoc(
Check if any generators have been added so far.

:returns:  ``True`` if ``number_of_generators() == 0`` and ``False`` otherwise.
:rtype: bool

:complexity: Constant.
)pbdoc");
      thing.def("finished",
                &SchreierSims_::finished,
                R"pbdoc(
Check if the stabiliser chain is fully enumerated.

:returns:  ``True`` if the stabiliser chain is fully enumerated and ``False`` otherwise.
:rtype: bool

:complexity: Constant.
)pbdoc");
      thing.def("generator",
                &SchreierSims_::generator,
                py::arg("index"),
                R"pbdoc(
Get a generator.

This function returns the generator with a given index.

:param index: the index of the generator to return.
:type index: int

:returns: The generator with index *index*.
:rtype: Element

:raises LibsemigroupsError:  if the *index* is out of bounds.

:complexity: Constant.
)pbdoc");
      thing.def("init",
                &SchreierSims_::init,
                R"pbdoc(
Reset to the trivial group.

This function removes all generators, and orbits, and resets ``self`` so that it
represents the trivial group, as if ``self`` had been newly constructed.

:complexity: Constant.
)pbdoc");
      thing.def("inverse_transversal_element",
                &SchreierSims_::inverse_transversal_element,
                py::arg("depth"),
                py::arg("pt"),
                R"pbdoc(
Get an inverse of a transversal element.

This function returns the transversal element at depth *depth* which sends *pt*
to the basepoint.

:param depth: the depth.
:type depth: int

:param pt: the point to map to the base point under the inverse transversal
      element.
:type pt: int

:returns: the inverse transversal element.
:rtype: Element

:raises LibsemigroupsError:  if the *depth* is out of bounds.

:raises LibsemigroupsError:  if *pt* is not in the orbit of the basepoint.

:complexity: Constant.
)pbdoc");
      thing.def("number_of_generators",
                &SchreierSims_::number_of_generators,
                R"pbdoc(
The number of generators.

This function returns the number of generators.

:returns: The number of generators.
:rtype: int

:complexity: Constant.
)pbdoc");
      thing.def("number_of_strong_generators",
                &SchreierSims_::number_of_strong_generators,
                py::arg("depth"),
                R"pbdoc(
The number of strong generators at a given depth.

This function returns the number of strong generators of the stabiliser chain at
a given depth.

:param depth: the depth.
:type depth: int

:returns: The number of strong generators.
:rtype: int

:raises LibsemigroupsError:  if the *depth* is out of bounds.

:complexity: Constant.
)pbdoc");
      thing.def("one",
                &SchreierSims_::one,
                R"pbdoc(
Returns a const reference to the identity.

:returns: The identity element.
:rtype: Element
)pbdoc");
      thing.def("orbit_lookup",
                &SchreierSims_::orbit_lookup,
                py::arg("depth"),
                py::arg("pt"),
                R"pbdoc(
Check if a point is in the orbit of a basepoint.

:param depth: the depth.
:type depth: int

:param pt: the point.
:type pt: int

:returns: ``True`` if the point *pt* is in the orbit of the basepoint of
      ``self`` at depth *depth*, and ``False`` otherwise.
:rtype: bool

:raises LibsemigroupsError:  if the *depth*` is out of bounds or if *pt* is out
      of bounds.

:complexity: Constant.
)pbdoc");
      thing.def("run",
                &SchreierSims_::run,
                R"pbdoc(
Run the Schreier-Sims algorithm.


:complexity:  :math:`O(N^2\log^3|G|+|T|N^2\log|G|)` time and
      :math:`O(N^2\log|G|+|T|N)` space, where ``N`` is the degree of the
      generators, :math:`|G|` is the size of the group and :math:`|T|` is the
      number of generators of the group.
)pbdoc");
      thing.def("sift",
                &SchreierSims_::sift,
                py::arg("x"),
                R"pbdoc(
Sift an element through the stabiliser chain.

:param x: A group element.
:type x: Element

:returns: A sifted element.
:rtype: Element

:raises LibsemigroupsError:  if the degree of *x* is not equal to the degree of
      the generators.
)pbdoc");
      thing.def("sift_inplace",
                &SchreierSims_::sift_inplace,
                py::arg("x"),
                R"pbdoc(
Sift an element through the stabiliser chain in-place.

:param x: a group element.
:type x: Element

:raises LibsemigroupsError:  if the degree of *x* is not equal to the degree of
      the generators.
)pbdoc");
      thing.def("size",
                &SchreierSims_::size,
                R"pbdoc(
Returns the size of the group represented by ``self``.

:returns:  the size of the group.
:rtype: int
)pbdoc");
      thing.def("current_size",
                &SchreierSims_::current_size,
                R"pbdoc(
Returns the size of the group represented by this, without running the algorithm.

:returns:  the size of the group.
:rtype: int
)pbdoc");
      thing.def("strong_generator",
                &SchreierSims_::strong_generator,
                py::arg("depth"),
                py::arg("index"),
                R"pbdoc(
Get a strong generator.

This function returns the generator with a given depth and index.

:param depth: the depth.
:type depth: int

:param index: the index of the generator to return.
:type index: int

:returns: The strong generator of at depth *depth* and with index *index*.
:rtype: Element

:raises LibsemigroupsError:  if the *depth* is out of bounds.

:raises LibsemigroupsError:  if the *index* is out of bounds.

:complexity: Constant.
)pbdoc");
      thing.def("transversal_element",
                &SchreierSims_::transversal_element,
                py::arg("depth"),
                py::arg("pt"),
                R"pbdoc(
Get an transversal element.

This function returns the transversal element at depth *depth* which sends the
corresponding basepoint to the point *pt*.

:param depth: the depth.
:type depth: int

:param pt: the image of the base point under the traversal.
:type pt: int

:returns: The transversal element.
:rtype: Element

:raises LibsemigroupsError:  if *depth* is out of bounds.

:raises LibsemigroupsError:  if *pt* is not in the orbit of the basepoint.

:complexity: Constant.
)pbdoc");

      m.def(
          "intersection",
          [](SchreierSims_& T, SchreierSims_& S1, SchreierSims_& S2) {
            return schreier_sims::intersection(T, S1, S2);
          },
          py::arg("T"),
          py::arg("S1"),
          py::arg("S2"),
          R"pbdoc(
Find the intersection of two permutation groups.

This function finds the intersection of two permutation groups.
It modifies the first parameter *T* to be the :any:`SchreierSimsPerm1` object
corresponding to the intersection of *S1* and *S2*.

:param T: an empty SchreierSims object that will hold the result.
:type T: SchreierSimsPerm1

:param S1: the first group of the intersection.
:type S1: SchreierSimsPerm1

:param S2: the second group of the intersection.
:type S2: SchreierSimsPerm1

:raises LibsemigroupsError:  if *T* is not empty.
)pbdoc");
    }  // bind_schreier_sims
  }    // namespace

  void init_schreier_sims(py::module& m) {
    // One call to bind is required per list of types
    bind_schreier_sims<255, uint8_t, Perm<0, uint8_t>>(m, "Perm1");
    bind_schreier_sims<511, uint16_t, Perm<0, uint16_t>>(m, "Perm2");
  }

}  // namespace libsemigroups