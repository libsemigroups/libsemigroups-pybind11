//
// libsemigroups_pybind11
// Copyright (C) 2022 James D. Mitchell
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

// TODO:
// * iwyu
// * tests
// * remove doc to doc/source

// C std headers....
#include <stddef.h>  // for size_t
#include <stdint.h>  // for int32_t, uint32_t

// C++ stl headers....
#include <initializer_list>  // for initializer_list
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/present.hpp>  // for Presentation

// pybind11....
#include <pybind11/pybind11.h>  // for class_, init, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_present

namespace py = pybind11;
namespace libsemigroups {

  namespace {

    template <typename T>
    std::string presentation_repr(Presentation<T> &p) {
      // FIXME presentation::length does not take const refs but refs and so we
      // must do the same.
      std::ostringstream out;

      out << "<" << (p.contains_empty_word() ? "monoid" : "semigroup")
          << " presentation with " << p.alphabet().size() << " generators and "
          << p.rules.size() << " relations of length "
          << presentation::length(p) + p.alphabet().size() << ">";
      return out.str();
    }

    template <typename T>
    void bind_present(py::module &m, std::string const &name) {
      using size_type = typename Presentation<T>::size_type;

      py::class_<Presentation<T>>(m, name.c_str())
          .def(py::init<>())
          .def(py::init<Presentation<T> const &>())
          .def("alphabet",
               py::overload_cast<>(&Presentation<T>::alphabet, py::const_))
          .def("alphabet",
               py::overload_cast<size_type>(&Presentation<T>::alphabet))
          .def("alphabet",
               py::overload_cast<T const &>(&Presentation<T>::alphabet))
          .def("alphabet_from_rules", &Presentation<T>::alphabet_from_rules)
          .def("letter", &Presentation<T>::letter)
          .def("index", &Presentation<T>::index)
          .def("contains_empty_word",
               py::overload_cast<>(&Presentation<T>::contains_empty_word,
                                   py::const_))
          .def("contains_empty_word",
               py::overload_cast<bool>(&Presentation<T>::contains_empty_word))
          .def_readwrite("rules", &Presentation<T>::rules)
          .def("validate_alphabet",
               py::overload_cast<>(&Presentation<T>::validate_alphabet,
                                   py::const_))
          .def("validate_letter", &Presentation<T>::validate_letter)
          .def("validate_rules", &Presentation<T>::validate_rules)
          .def("validate", &Presentation<T>::validate)
          .def("__repr__", &presentation_repr<T>);

      m.def("add_rule",
            py::overload_cast<Presentation<T> &, T const &, T const &>(
                &presentation::add_rule<T>));
      m.def("add_rule_and_check",
            py::overload_cast<Presentation<T> &, T const &, T const &>(
                &presentation::add_rule_and_check<T>));
      m.def("add_rules", &presentation::add_rules<T>);
      m.def("add_identity_rules", &presentation::add_identity_rules<T>);
      m.def("add_inverse_rules", &presentation::add_inverse_rules<T>);
      m.def("remove_duplicate_rules", &presentation::remove_duplicate_rules<T>);
      m.def("reduce_complements", &presentation::reduce_complements<T>);
      m.def("sort_each_rule", &presentation::sort_each_rule<T>);
      m.def("sort_rules", &presentation::sort_rules<T>);
      m.def("longest_common_subword",
            py::overload_cast<Presentation<T> &>(
                &presentation::longest_common_subword<T>));
      m.def("replace_subword",
            py::overload_cast<Presentation<T> &, T const &>(
                &presentation::replace_subword<T>));
      m.def("length", &presentation::length<T>);
      m.def("reverse", &presentation::reverse<T>);
      m.def("normalize_alphabet", &presentation::normalize_alphabet<T>);

      // TODO add make
    }
  }  // namespace

  void init_present(py::module &m) {
    bind_present<word_type>(m, "PresentationWords");
    bind_present<std::string>(m, "PresentationStrings");
  }
}  // namespace libsemigroups
