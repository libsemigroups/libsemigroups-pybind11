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

// C std headers....
// C++ stl headers....
// libsemigroups....
#include <libsemigroups/bmat.hpp>       // for ImageRightAction
#include <libsemigroups/bmat8.hpp>      // for BMat8
#include <libsemigroups/konieczny.hpp>  // for Kambites
#include <libsemigroups/matrix.hpp>     // for BMat
#include <libsemigroups/transf.hpp>     // for PPerm, Transf, Perm, LeastPPerm

// pybind11....
#include <pybind11/chrono.h>      // for auto conversion into run_for
#include <pybind11/functional.h>  // for auto conversion for run_until
#include <pybind11/pybind11.h>    // for class_, init, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_konieczny

namespace py = pybind11;

namespace libsemigroups {

  template <typename T, typename S = KoniecznyTraits<T>>
  void bind_konieczny(py::module& m, std::string typestr) {
    using Konieczny_         = Konieczny<T, S>;
    using DClass             = typename Konieczny_::DClass;
    using element_type       = typename Konieczny<T, S>::element_type;
    using const_reference    = typename Konieczny<T, S>::const_reference;
    std::string pyclass_name = std::string("Konieczny") + typestr;

    py::class_<Konieczny_> x(
        m, pyclass_name.c_str(), py::buffer_protocol(), py::dynamic_attr());

    x.attr("element_type") = m.attr(typestr.c_str());
    x.def(py::init<std::vector<element_type> const&>())
        .def("add_generator", &Konieczny_::add_generator)
        .def("contains", &Konieczny_::contains)
        .def("__contains__", &Konieczny_::contains, py::is_operator())
        .def("size", &Konieczny_::size)
        .def("is_regular_element", &Konieczny_::is_regular_element)
        .def("D_class_of_element",
             &Konieczny_::D_class_of_element,
             py::return_value_policy::reference_internal)
        .def("D_classes",
             [](Konieczny_ const& k) {
               return py::make_iterator(k.cbegin_D_classes(),
                                        k.cend_D_classes());
             })
        .def("regular_D_classes",
             [](Konieczny_ const& k) {
               return py::make_iterator(k.cbegin_regular_D_classes(),
                                        k.cend_regular_D_classes());
             })
        .def("number_of_D_classes", &Konieczny_::number_of_D_classes)
        .def("number_of_L_classes", &Konieczny_::number_of_L_classes)
        .def("number_of_R_classes", &Konieczny_::number_of_R_classes)
        .def("number_of_H_classes", &Konieczny_::number_of_H_classes)
        .def("number_of_regular_D_classes",
             &Konieczny_::number_of_regular_D_classes)
        .def("number_of_regular_L_classes",
             &Konieczny_::number_of_regular_L_classes)
        .def("number_of_regular_R_classes",
             &Konieczny_::number_of_regular_R_classes)
        .def("number_of_regular_elements",
             &Konieczny_::number_of_regular_elements)
        .def("current_number_of_D_classes",
             &Konieczny_::current_number_of_D_classes)
        .def("current_number_of_L_classes",
             &Konieczny_::current_number_of_L_classes)
        .def("current_number_of_R_classes",
             &Konieczny_::current_number_of_R_classes)
        .def("current_number_of_H_classes",
             &Konieczny_::current_number_of_H_classes)
        .def("current_number_of_regular_D_classes",
             &Konieczny_::current_number_of_regular_D_classes)
        .def("current_number_of_regular_L_classes",
             &Konieczny_::current_number_of_regular_L_classes)
        .def("current_number_of_regular_R_classes",
             &Konieczny_::current_number_of_regular_R_classes)
        .def("current_number_of_regular_elements",
             &Konieczny_::current_number_of_regular_elements)
        .def("number_of_idempotents", &Konieczny_::number_of_idempotents)
        .def("generator", &Konieczny_::generator)
        .def("number_of_generators", &Konieczny_::number_of_generators)
        .def("degree", &Konieczny_::degree)
        .def("current_size", &Konieczny_::current_size)
        .def("current_number_of_idempotents",
             &Konieczny_::current_number_of_idempotents)
        .def("generators",
             [](Konieczny_ const& k) {
               return py::make_iterator(k.cbegin_generators(),
                                        k.cend_generators());
             })
        .def("dead", &Konieczny_::dead)
        .def("finished", &Konieczny_::finished)
        .def("started", &Konieczny_::started)
        .def("stopped", &Konieczny_::stopped)
        .def("timed_out", &Konieczny_::timed_out)
        .def("running", &Konieczny_::running)
        .def("stopped_by_predicate", &Konieczny_::stopped_by_predicate)
        .def("kill", &Konieczny_::kill)
        .def("run", &Konieczny_::run)
        .def("run_for",
             (void(Konieczny_::*)(std::chrono::nanoseconds)) & Runner::run_for)
        .def("run_until",
             (void(Konieczny_::*)(std::function<bool()>&)) & Runner::run_until)
        .def("report_every",
             (void(Konieczny_::*)(std::chrono::nanoseconds))
                 & Runner::report_every)
        .def("report_every",
             (void(Konieczny_::*)(std::chrono::nanoseconds))
                 & Runner::report_every)
        .def("report", &Konieczny_::report)
        .def("report_why_we_stopped", &Konieczny_::report_why_we_stopped)
        .def("running_for", &Konieczny_::running_for)
        .def("running_until", &Konieczny_::running_until);

    py::class_<DClass> d(m, (pyclass_name + "DClass").c_str());

    d.def("rep",
          &DClass::rep,
          R"pbdoc(
                   Returns a representative of the $\mathscr{D}$-class.

                   :return: A const_reference.
                   )pbdoc")
        .def("is_regular_D_class",
             &DClass::is_regular_D_class,
             R"pbdoc(
             Test regularity of a $\mathscr{D}$-class.

             :return: A value of type size_t.
             )pbdoc")
        .def("number_of_idempotents",
             &DClass::number_of_idempotents,
             R"pbdoc(
             Returns the number of idempotents.

             )pbdoc")
        .def("number_of_L_classes",
             &DClass::number_of_L_classes,
             R"pbdoc(
             Returns the number of $\mathscr{L}$-classes.

             :return: A value of type size_t.
             )pbdoc")
        .def("number_of_R_classes",
             &DClass::number_of_R_classes,
             R"pbdoc(
             Returns the number of $\mathscr{R}$-classes.

             :return: A value of type size_t.
             )pbdoc")
        .def("size",
             &DClass::size,
             R"pbdoc(
             Returns the size of a $\mathscr{D}$-class.

             :return: A value of type size_t.
             )pbdoc")
        .def("size_H_class",
             &DClass::size_H_class,
             R"pbdoc(
             Returns the size of the $\mathscr{H}$-classes.

             :return: A value of type size_t.
             )pbdoc")
        .def("__contains__",
             py::overload_cast<const_reference>(&DClass::contains),
             py::is_operator())
        .def("contains",
             py::overload_cast<const_reference>(&DClass::contains),
             py::arg("x"));
  }

  void init_konieczny(py::module& m) {
    bind_konieczny<LeastTransf<16>>(m, "Transf16");
    bind_konieczny<Transf<0, uint8_t>>(m, "Transf1");
    bind_konieczny<Transf<0, uint16_t>>(m, "Transf2");
    bind_konieczny<Transf<0, uint32_t>>(m, "Transf4");
    bind_konieczny<LeastPPerm<16>>(m, "PPerm16");
    bind_konieczny<PPerm<0, uint8_t>>(m, "PPerm1");
    bind_konieczny<PPerm<0, uint16_t>>(m, "PPerm2");
    bind_konieczny<PPerm<0, uint32_t>>(m, "PPerm4");
    bind_konieczny<BMat8>(m, "BMat8");
    bind_konieczny<BMat<>>(m, "BMat");
  }
}  // namespace libsemigroups
