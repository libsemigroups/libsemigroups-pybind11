//
// libsemigroups_pybind11
// Copyright (C) 2024 James Mitchell
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

// libsemigroups headers
#include <libsemigroups/bipart.hpp>
#include <libsemigroups/bmat8.hpp>
#include <libsemigroups/froidure-pin.hpp>
#include <libsemigroups/matrix.hpp>
#include <libsemigroups/pbr.hpp>
#include <libsemigroups/transf.hpp>

// pybind11....
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_froidure_pin

namespace py = pybind11;

namespace libsemigroups {

  namespace {
    template <typename Element>
    void bind_froidure_pin(py::module& m, std::string const& name) {
      using FroidurePin_ = FroidurePin<Element>;

      std::string pyclass_name = std::string("FroidurePin") + name;
      py::class_<FroidurePin_, FroidurePinBase> thing(m,
                                                      pyclass_name.c_str(),
                                                      R"pbdoc(
Class implementing the Froidure-Pin algorithm.

A :any:`FroidurePinPBR` instance represents a semigroup or monoid defined by a
collection of generators such as transformations, partial permutations, or
matrices.

In the following documentation the type of the elements of the semigroup
represented by a :any:`FroidurePinBase` instance is denoted by ``Element``.

The class :any:`FroidurePinPBR` implements the Froidure-Pin algorithm as
described in the article :cite:`Froidure1997aa`. A :any:`FroidurePinPBR`
instance is defined by a generating set, and the main function is :any:`run`,
which implements the Froidure-Pin Algorithm. If :any:`run` is invoked and
:any:`finished` returns ``True``, then the size :any:`FroidurePinBase.size`,
the left and right Cayley graphs :any:`FroidurePinBase.left_cayley_graph` and
:any:`FroidurePinBase.right_cayley_graph` are determined, and a confluent
terminating presentation :any:`froidure_pin.rules` for the semigroup is
known.

.. seealso::  :any:`FroidurePinBase` and :any:`Runner`.
)pbdoc");
      // thing.attr("Element") = py::class_<Element>(m);
      thing.def("__repr__", [](FroidurePin_ const& x) {
        return to_human_readable_repr(x);
      });
      thing.def("__getitem__", &FroidurePin_::at, py::is_operator());
      thing.def("__iter__", [](FroidurePin_& self) {
        self.run();
        return py::make_iterator(self.begin(), self.end());
      });
      thing.def(
          "current_elements",
          [](FroidurePin_ const& self) {
            return py::make_iterator(self.begin(), self.end());
          },
          R"pbdoc(
Returns an iterator yielding the so-far enumerated elements.

This function returns an iterator yielding the so-far enumerated elements.
Calling this function does not trigger any enumeration.

:param self: the :any:`FroidurePinPBR` instance.
:type self: FroidurePinPBR

:returns: An iterator yielding the so-far enumerated elements.
:rtype: Iterator
)pbdoc");
      thing.def(py::init([](std::vector<Element> const& gens) {
                  return to_froidure_pin(gens);
                }),
                py::arg("gens"),
                R"pbdoc(
Construct from a list of generators.

This function constructs a :any:`FroidurePinPBR` instance with generators
in the list *gens*.

:param gens: the list of generators.
:type gens: List[Element]

:raises LibsemigroupsError: if the generators do not all have the same degree.
)pbdoc");
      thing.def("__copy__",
                [](FroidurePin_ const& self) { return FroidurePin_(self); });
      thing.def(
          "copy",
          [](FroidurePin_ const& self) { return FroidurePin_(self); },
          R"pbdoc(
Copy a :any:`FroidurePinPBR` object.

:returns: A copy.
:rtype: FroidurePinPBR
)pbdoc");
      thing.def("add_generator",
                &FroidurePin_::add_generator,
                py::arg("x"),
                R"pbdoc(
Add a copy of an element to the generators.

This function can be used to add new generators to an existing
:any:`FroidurePinPBR` instance in such a way that any previously enumerated data
is preserved and not recomputed, or copied. This can be faster than recomputing
the semigroup generated by the old generators and the new generators.This
function changes the semigroup in-place, thereby invalidating possibly
previously known data about the semigroup, such as the left or right Cayley
graphs, number of idempotents, and so on.

The generator in *x* is added regardless of whether or not it is already an
element of the semigroup. After calling this function the generator *x* will be
the generator with the largest index. There can be duplicate generators and
although they do not count as distinct elements, they do count as distinct
generators.

The :any:`FroidurePinPBR` instance is returned in a state where all of the
previously enumerated elements which had been multiplied by all of the old
generators, have now been multiplied by all of the old and new generators. This
means that after this function is called the semigroup might contain many more
elements than before (whether it is fully enumerating or not).

:param x: the generator to add.
:type x: Element

:returns: ``self``.
:rtype: FroidurePinPBR

:raises ValueError:
   if the degree of *x* is incompatible with the existing degree (if any).

:raises TypeError:
   if *x* is not of the same type as the existing generators (if any).
   )pbdoc");
      thing.def(
          "add_generators",
          [](FroidurePin_&               self,
             std::vector<Element> const& gens) -> FroidurePin_& {
            return froidure_pin::add_generators(self, gens);
          },
          py::arg("gens"),
          R"pbdoc(
Add a list of generators.

See :any:`add_generator` for a detailed description.

:param gens: the list of generators.
:type gens: List[Element]

:returns: ``self``.
:rtype: FroidurePinPBR

:raises TypeError:
   if *gens* is not a list.

:raises TypeError:
   if any item in *gens* is not of the same type as the existing generators (if any).

:raises ValueError:
   if the degree of any item in *gens* is incompatible with the existing degree (if any).
)pbdoc");

      thing.def(
          "current_position",
          [](FroidurePin_ const& self, Element const& x) {
            return self.current_position(x);
          },
          py::arg("x").noconvert(),
          R"pbdoc(
Find the position of an element with no enumeration.

This function returns the position of the element *x* in the semigroup if it
is already known to belong to the semigroup or :any:`UNDEFINED` . This
function finds the position of the element *x* if it is already known to belong
to a :any:`FroidurePinPBR` instance, and :any:`UNDEFINED` if not. If a
:any:`FroidurePinPBR` instance is not yet fully enumerated, then this function
may return :any:`UNDEFINED` when *x* does belong to the fully enumerated instance.

:param x: a possible element.
:type x: Element

:returns:
    The position of *x* if it belongs to a :any:`FroidurePinPBR` instance and
    :any:`UNDEFINED` if not.
:rtype: int | Undefined

.. seealso::  :any:`position` and :any:`sorted_position`.
)pbdoc");

      thing.def(
          "idempotents",
          [](FroidurePin_& self) {
            return py::make_iterator(self.cbegin_idempotents(),
                                     self.cend_idempotents());
          },
          R"pbdoc(
Returns an iterator yielding the idempotents.

If the returned iterator is incremented, then it yields the second
idempotent in the semigroup (if it exists), and every subsequent increment
points to the next idempotent.

:returns:
   A value of type :any:`Iterator`.
:rtype:
   Iterator
)pbdoc");
      thing.def(
          "position_of_generator",
          [](FroidurePinBase const& self, size_t i) {
            return self.position_of_generator(i);
          },
          py::arg("i"),
          R"pbdoc(
Returns the position in of the generator with specified index.

In many cases ``current_position(i)`` will equal *i*, examples of when this
will not be the case are:

* there are duplicate generators;
* :any:`FroidurePinPBR.add_generators` was called after the semigroup was
  already partially enumerated.

:param i: the index of the generator.
:type i: int

:returns: The position of the generator with index *i*.
:rtype: int

:raises LibsemigroupsError:
  if *i* is greater than or equal to :any:`FroidurePinPBR.number_of_generators`.

:complexity: Constant.
)pbdoc");
      thing.def(
          "sorted_elements",
          [](FroidurePin_& self) {
            return py::make_iterator(self.cbegin_sorted(), self.cend_sorted());
          },
          R"pbdoc(
Returns an iterator yielding the sorted elements of a :any:`FroidurePinPBR`
instance.

:returns:
  An iterator yielding the sorted elements.
:rtype:
  Iterator
)pbdoc");

      thing.def(
          "closure",
          [](FroidurePin_& self, std::vector<Element> const& gens)
              -> FroidurePin_& { return froidure_pin::closure(self, gens); },
          py::arg("gens"),
          R"pbdoc(
Add non-redundant generators in list.

This function differs from :any:`FroidurePinPBR.add_generators` in that it
tries to add the new generators one by one, and only adds those generators that
are not products of existing generators (including any new generators that were
added before). The generators are added in the order they occur in *gens*.

This function changes a :any:`FroidurePinPBR` instance in-place, thereby invalidating
some previously computed information, such as the left or right Cayley graphs,
or number of idempotents, for example.

:param gens: the list of generators.
:type gens: List[Element]

:returns: ``self``.
:rtype: FroidurePinPBR

:raises LibsemigroupsError:
    if any of the elements in *gens* do not have degree compatible with any
    existing elements of the :any:`FroidurePinPBR` instance.

:raises LibsemigroupsError:
    if the elements in *gens* do not all have the same degree.
)pbdoc");
      thing.def("contains",
                &FroidurePin_::contains,
                py::arg("x"),
                R"pbdoc(
Test membership of an element.

This function returns ``True`` if *x* belongs to a :any:`FroidurePinPBR`
instance and ``False`` if it does not.

:param x: a possible element.
:type x: Element

:returns:
  Whether or not the element *x* is contained in a :any:`FroidurePinPBR`
  instance.
:rtype: bool
      )pbdoc");
      thing.def(
          "copy_add_generators",
          [](FroidurePin_&               self,
             std::vector<Element> const& gens) -> FroidurePin_ {
            return froidure_pin::copy_add_generators(self, gens);
          },
          py::arg("gens"),
          R"pbdoc(
Copy and add a list of generators.

This function is equivalent to copy constructing a new :any:`FroidurePinPBR`
instance and  then calling :any:`FroidurePinPBR.add_generators` on the copy.
But this function avoids copying the parts of the initial instance that are
immediately invalidated by :any:`FroidurePinPBR.add_generators`.

:param gens: the list of generators.
:type gens: List[Element]

:returns:
   A new :any:`FroidurePinPBR` instance by value generated by the generators of
   *self* and *gens*.
:rtype:
   FroidurePinPBR

:raises LibsemigroupsError:
    if any of the elements in *gens* do not have degree compatible with any
    existing elements of the :any:`FroidurePinPBR` instance.

:raises LibsemigroupsError:
    if the elements in *gens* do not all have the same degree.
)pbdoc");

      thing.def(
          "copy_closure",
          [](FroidurePin_&               self,
             std::vector<Element> const& gens) -> FroidurePin_ {
            return froidure_pin::copy_closure(self, gens);
          },
          py::arg("gens"),
          R"pbdoc(
Copy and add non-redundant generators.

This function is equivalent to copy constructing a new :any:`FroidurePinPBR`
instance and then calling :any:`closure` on the copy. But this function
avoids copying the parts of the initial :any:`FroidurePinPBR` instance that are
immediately discarded by :any:`closure`.

:param gens: the list of generators.
:type gens: List[Element]

:returns:
   A new :any:`FroidurePinPBR` instance by value generated by the generators of
   *self* and the non-redundant generators in *gens*.
:rtype:
   FroidurePinPBR

:raises LibsemigroupsError:
    if any of the elements in *gens* do not have degree compatible with any
    existing elements of the :any:`FroidurePinPBR` instance.

:raises LibsemigroupsError:
    if the elements in *gens* do not all have the same degree.
)pbdoc");

      thing.def("fast_product",
                &FroidurePin_::fast_product,
                py::arg("i"),
                py::arg("j"),
                R"pbdoc(
Multiply elements via their indices.

This function returns the position of the product of the element with
index *i* and the element with index *j*.

This function either:

* follows the path in the right or left Cayley graph from *i* to *j*,
  whichever is shorter using :any:`froidure_pin.product_by_reduction`; or

* multiplies the elements in positions *i* and *j* together;

whichever is better.

For example, if the complexity of the multiplication is linear and ``self`` is
a semigroup of transformations of degree 20, and the shortest paths in the left
and right Cayley graphs from *i* to *j* are of length 100 and 1131, then it is
better to just multiply the transformations together.

:param i: the index of the first element to multiply.
:type i: int

:param j: the index of the second element to multiply.
:type j: int

:returns: The index of the product.
:rtype: int

:raises LibsemigroupsError:
    if the values *i* and *j* are greater than or equal to
    :any:`FroidurePinBase.current_size`.
)pbdoc");

      thing.def("generator",
                &FroidurePin_::generator,
                py::arg("i"),
                R"pbdoc(
Returns the generator with specified index.

This function returns the generator with index *i*, where the order
is that in which the generators were added at construction, or via
:any:`init`, :any:`add_generator`, :any:`add_generators`,
:any:`closure`, :any:`copy_closure`, or :any:`copy_add_generators`.

:param i: the index of a generator.
:type i: int

:returns: The generator with given index.
:rtype: Element

:raises LibsemigroupsError:
  if *i* is greater than or equal to :any:`number_of_generators()`.
)pbdoc");

      thing.def(
          "init",
          [](FroidurePin_& self) -> FroidurePin_& { return self.init(); },
          R"pbdoc(
Reinitialize a :any:`FroidurePinPBR` object.

This function re-initializes a :any:`FroidurePinPBR` object so that it is in
the same state as if it had just been default constructed.

:returns: ``self``
:rtype: FroidurePinPBR
)pbdoc");

      thing.def(
          "init",
          [](FroidurePin_& self, std::vector<Element> const& gens) {
            FroidurePin_::throw_if_inconsistent_degree(gens.cbegin(),
                                                       gens.cend());
            return froidure_pin::init(self, gens);
          },
          py::arg("gens"),
          R"pbdoc(
Reinitialize a :any:`FroidurePinPBR` object from a list of generators.

This function re-initializes a :any:`FroidurePinPBR` object so that it is
in the same state as if it had just been constructed from *gens*.

:param gens: the generators.
:type gens: List[Element]

:returns: ``self``.
:rtype: FroidurePinPBR

:raises LibsemigroupsError:
    if the elements in *gens* do not all have the same degree.
)pbdoc");

      thing.def("is_finite",
                &FroidurePin_::is_finite,
                R"pbdoc(
Check finiteness.

This function returns :any:`tril.TRUE` if the semigroup represented by ``self``
is finite, :any:`tril.FALSE` if it is infinite, and :any:`tril.unknown` if it
is not known. For some types of elements, such as matrices over the integers,
for example, it is undecidable, in general, if the semigroup generated by these
elements is finite or infinite. On the other hand, for other types, such as
transformation, the semigroup is always finite.

:returns:
   If the :any:`FroidurePinPBR` object is finite, or not finite, or it isn't possible to
   answer this question without performing a full enumeration.
:rtype:
   tril
)pbdoc");

      thing.def("is_idempotent",
                &FroidurePin_::is_idempotent,
                py::arg("i"),
                R"pbdoc(
Check if an element is an idempotent via its index.

This function returns ``True`` if the element in position *i* is an
idempotent and ``False`` if it is not.

:param i: the index of the element.
:type i: int

:returns: A value of type ``bool``.
:rtype: bool

:raises LibsemigroupsError:
  if *i* is greater than or equal to the size of the :any:`FroidurePinPBR`
  instance.
)pbdoc");

      thing.def("number_of_generators",
                &FroidurePin_::number_of_generators,
                R"pbdoc(
Returns the number of generators.

This function returns the number of generators of a :any:`FroidurePinPBR` instance.

:returns:
   The number of generators.
:rtype:
   int
)pbdoc");

      thing.def("number_of_idempotents",
                &FroidurePin_::number_of_idempotents,
                R"pbdoc(
Returns the number of idempotents.

This function returns the number of idempotents in the semigroup represented by
a :any:`FroidurePinPBR` instance. Calling this function triggers a full enumeration.

:returns:
   The number of idempotents.
:rtype:
   int
)pbdoc");

      thing.def(
          "position",
          [](FroidurePin_& self, Element const& x) { return self.position(x); },
          py::arg("x"),
          R"pbdoc(
Find the position of an element with enumeration if necessary.

This function the position of *x* in a :any:`FroidurePinPBR` instance, or
:any:`UNDEFINED` if *x* is not an element.

:param x: a possible element.
:type x: Element

:returns: The position of *x*.
:rtype: int | Undefined

.. seealso::  :any:`current_position` and :any:`sorted_position`.
)pbdoc");

      thing.def("reserve",
                &FroidurePin_::reserve,
                py::arg("val"),
                R"pbdoc(
Requests the given capacity for elements.

The parameter ``val`` is also used to initialise certain data members of a
:any:`FroidurePinPBR` instance. If you know a good upper bound for the size of
your semigroup, then it might be a good idea to call this function with that
upper bound as an argument; this can significantly improve the performance of
the :any:`run` function, and consequently every other function too.

:param val: the number of elements to reserve space for.
:type val: int

:returns: ``self``.
:rtype: FroidurePinPBR
)pbdoc");

      thing.def("sorted_at",
                &FroidurePin_::sorted_at,
                py::arg("i"),
                R"pbdoc(
Access element specified by sorted index with bound checks.

This function triggers a full enumeration, and the parameter *i* is
the index when the elements are sorted.

:param i: the sorted index of the element to access.
:type i: int

:returns: The element with index *i* (if any).
:rtype: Element

:raises LibsemigroupsError:
  if *i* is greater than or equal to the return value of
  :any:`FroidurePinBase.size`.
)pbdoc");

      thing.def("sorted_position",
                &FroidurePin_::sorted_position,
                py::arg("x"),
                R"pbdoc(
Returns the sorted index of an element.

This function returns the position of *x* in the elements of a
:any:`FroidurePinPBR` when they are sorted, or :any:`UNDEFINED`
if *x* is not an element.

:param x: a possible element.
:type x: Element

:returns: The position of *x* in the sorted list of elements.
:rtype: int | Undefined

.. seealso::  :any:`current_position` and :any:`position`.
)pbdoc");

      thing.def("to_sorted_position",
                &FroidurePin_::to_sorted_position,
                py::arg("i"),
                R"pbdoc(
Returns the sorted index of an element via its index.

This function returns the position of the element with index *i* when the
elements are sorted, or :any:`UNDEFINED` if *i* is greater than
:any:`FroidurePinBase.size`.

:param i: the index of the element.
:type i: int

:returns: The sorted position of the element with position *i*.
:rtype: int | Undefined
 )pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Helper functions
      ////////////////////////////////////////////////////////////////////////

      // The following function is documented in
      // libsemigroups_pybind11/froidure_pin.py
      m.def(
          "froidure_pin_current_position",
          [](FroidurePinBase const& fpb, word_type const& w) {
            return froidure_pin::current_position(fpb, w);
          },
          py::arg("fpb"),
          py::arg("w"));

      // The following function is documented in
      // libsemigroups_pybind11/froidure_pin.py
      m.def(
          "froidure_pin_equal_to",
          [](FroidurePin_& fp, word_type const& x, word_type const& y) {
            return froidure_pin::equal_to(fp, x, y);
          },
          py::arg("fp"),
          py::arg("x"),
          py::arg("y"));

      // The following function is documented in
      // libsemigroups_pybind11/froidure_pin.py
      m.def(
          "froidure_pin_factorisation",
          [](FroidurePin_& fp, Element const& x) {
            return froidure_pin::factorisation(fp, x);
          },
          py::arg("fp"),
          py::arg("x"));

      // The following function is documented in
      // libsemigroups_pybind11/froidure_pin.py
      m.def(
          "froidure_pin_factorisation",
          [](FroidurePinBase& fpb, FroidurePinBase::element_index_type pos) {
            return froidure_pin::factorisation(fpb, pos);
          },
          py::arg("fpb"),
          py::arg("pos"));

      // The following function is documented in
      // libsemigroups_pybind11/froidure_pin.py
      m.def(
          "froidure_pin_minimal_factorisation",
          [](FroidurePin_& fp, Element const& x) {
            return froidure_pin::minimal_factorisation(fp, x);
          },
          py::arg("fp"),
          py::arg("x"));

      // The following function is documented in
      // libsemigroups_pybind11/froidure_pin.py
      m.def(
          "froidure_pin_minimal_factorisation",
          [](FroidurePin_& fp, size_t i) {
            return froidure_pin::minimal_factorisation(fp, i);
          },
          py::arg("fp"),
          py::arg("i"));

      // The following function is documented in
      // libsemigroups_pybind11/froidure_pin.py
      m.def(
          "froidure_pin_position",
          [](FroidurePin_& fp, word_type const& w) {
            return froidure_pin::position(fp, w);
          },
          py::arg("fp"),
          py::arg("w"));

      // The following function is documented in
      // libsemigroups_pybind11/froidure_pin.py
      m.def(
          "froidure_pin_to_element",
          [](FroidurePin_& fp, word_type const& w) {
            return froidure_pin::to_element(fp, w);
          },
          py::arg("fp"),
          py::arg("w").noconvert());

      // TODO(1) are there some functions missing here?
    }  // bind_froidure_pin
  }  // namespace

  void init_froidure_pin(py::module& m) {
    // TODO(0) uncomment bind_froidure_pin<HPCombiTransf<16>>(m, "Transf16");
    bind_froidure_pin<Transf<0, uint8_t>>(m, "Transf1");
    bind_froidure_pin<Transf<0, uint16_t>>(m, "Transf2");
    bind_froidure_pin<Transf<0, uint32_t>>(m, "Transf4");
    // TODO(0) uncomment bind_froidure_pin<LeastPPerm<16>>(m, "PPerm16");
    bind_froidure_pin<PPerm<0, uint8_t>>(m, "PPerm1");
    bind_froidure_pin<PPerm<0, uint16_t>>(m, "PPerm2");
    bind_froidure_pin<PPerm<0, uint32_t>>(m, "PPerm4");
    // TODO(0) uncomment bind_froidure_pin<LeastPerm<16>>(m, "Perm16");
    bind_froidure_pin<Perm<0, uint8_t>>(m, "Perm1");
    bind_froidure_pin<Perm<0, uint16_t>>(m, "Perm2");
    bind_froidure_pin<Perm<0, uint32_t>>(m, "Perm4");
    // TODO(0) uncomment
    // bind_froidure_pin<detail::KBE,
    //                   FroidurePinTraits<detail::KBE,
    //                   fpsemigroup::KnuthBendix>>(
    //     m, "KBE");
    // bind_froidure_pin<detail::TCE,
    //                   FroidurePinTraits<detail::TCE, detail::TCE::Table>>(
    //     m, "TCE");
    bind_froidure_pin<Bipartition>(m, "Bipartition");
    bind_froidure_pin<PBR>(m, "PBR");

    bind_froidure_pin<BMat8>(m, "BMat8");
    bind_froidure_pin<BMat<>>(m, "BMat");
    bind_froidure_pin<IntMat<0, 0, int64_t>>(m, "IntMat");
    bind_froidure_pin<MaxPlusMat<0, 0, int64_t>>(m, "MaxPlusMat");
    bind_froidure_pin<MinPlusMat<0, 0, int64_t>>(m, "MinPlusMat");
    bind_froidure_pin<ProjMaxPlusMat<0, 0, int64_t>>(m, "ProjMaxPlusMat");
    bind_froidure_pin<MaxPlusTruncMat<0, 0, 0, int64_t>>(m, "MaxPlusTruncMat");
    bind_froidure_pin<MinPlusTruncMat<0, 0, 0, int64_t>>(m, "MinPlusTruncMat");
    bind_froidure_pin<NTPMat<0, 0, 0, 0, int64_t>>(m, "NTPMat");
  }
}  // namespace libsemigroups
