//
// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2021-2024 James D. Mitchell
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

#include "main.hpp"

// C std headers....
#include <stddef.h>  // for size_t

// C++ stl headers....
#include <initializer_list>  // for initializer_list

// libsemigroups....
#include <libsemigroups/config.hpp>     // for LIBSEMIGROUPS_EIGEN_ENABLED
#include <libsemigroups/cong-intf.hpp>  // for congruence_kind, congruence_kind:...
#include <libsemigroups/constants.hpp>  // for PositiveInfinity, Undefined, POSI...
#include <libsemigroups/detail/kbe.hpp>     // for KBE, operator<<
#include <libsemigroups/detail/report.hpp>  // for ReportGuard
#include <libsemigroups/detail/string.hpp>  // for to_string
#include <libsemigroups/detail/tce.hpp>     // for TCE
#include <libsemigroups/exception.hpp>      // for LibsemigroupsException
#include <libsemigroups/types.hpp>          // for tril, tril::FALSE, tril::TRUE

// pybind11....
#include <pybind11/operators.h>  // for self, operator<, operator==, self_t
#include <pybind11/pybind11.h>   // for module_, class_, enum_, init
#include <stdexcept>

namespace py = pybind11;

namespace libsemigroups {

  bool ERROR_MESSAGE_WITH_PREFIX = false;

  void error_message_with_prefix(bool value) {
    ERROR_MESSAGE_WITH_PREFIX = value;
  }

  bool error_message_with_prefix() {
    return ERROR_MESSAGE_WITH_PREFIX;
  }

  std::string formatted_error_message(std::runtime_error const& e) {
    if (error_message_with_prefix()) {
      return std::string(e.what());
    } else {
      // TODO this doesn't work well if backward is enabled.
      std::string out(e.what());
      size_t      pos = out.find(": ");
      if (pos != std::string::npos) {
        out.erase(0, pos + 2);
      }
      return out;
    }
  }

  // This has its own function so the py::options can be set for just this enum
  void init_tril(py::module& m) {
    py::options options;
    options.disable_enum_members_docstring();
    py::enum_<tril>(m, "tril", R"pbdoc(
The values in this enum can be used to indicate a result is true, false, or not
currently known.

The valid values are:

.. py:attribute:: tril.false
  :value: <tril.false: 0>

  Value representing false.

.. py:attribute:: tril.true
  :value: <tril.true: 1>

  Value representing true.

.. py:attribute:: tril.unknown
  :value: <tril.unknown: 2>

  Value representing unknown (either true or false).
)pbdoc")
        .value("true", tril::TRUE)
        .value("false", tril::FALSE)
        .value("unknown", tril::unknown);
  }

  PYBIND11_MODULE(_libsemigroups_pybind11, m) {
    py::class_<Undefined>(m, "Undefined")
        .def("__repr__",
             [](Undefined const& val) -> std::string { return "UNDEFINED"; })
        .def(py::self < Undefined())
        .def(
            "__eq__",
            [](Undefined const& lhop, size_t rhop) -> bool {
              return lhop == rhop;
            },
            py::is_operator())
        .def(
            "__eq__",
            [](size_t lhop, Undefined const& rhop) -> bool {
              return lhop == rhop;
            },
            py::is_operator())
        .def(
            "__eq__",
            [](Undefined const& lhop, int rhop) -> bool {
              return lhop == rhop;
            },
            py::is_operator())
        .def(
            "__eq__",
            [](int lhop, Undefined const& rhop) -> bool {
              return lhop == rhop;
            },
            py::is_operator())
        .def(
            "__eq__",
            [](Undefined const& lhop, uint64_t rhop) -> bool {
              return lhop == rhop;
            },
            py::is_operator())
        .def(
            "__eq__",
            [](uint64_t lhop, Undefined const& rhop) -> bool {
              return lhop == rhop;
            },
            py::is_operator())
        .def(
            "__int__",
            [](Undefined const& x) -> size_t { return static_cast<size_t>(x); })
        .def("__chr__",
             [](Undefined const& x) -> char { return static_cast<char>(x); });

    m.attr("UNDEFINED") = UNDEFINED;

#ifdef LIBSEMIGROUPS_EIGEN_ENABLED
    m.attr("LIBSEMIGROUPS_EIGEN_ENABLED")
        = static_cast<bool>(LIBSEMIGROUPS_EIGEN_ENABLED);
#else
    m.attr("LIBSEMIGROUPS_EIGEN_ENABLED") = false;
#endif

#ifdef LIBSEMIGROUPS_HPCOMBI_ENABLED
    m.attr("LIBSEMIGROUPS_HPCOMBI_ENABLED")
        = static_cast<bool>(LIBSEMIGROUPS_HPCOMBI_ENABLED);
#else
    m.attr("LIBSEMIGROUPS_HPCOMBI_ENABLED") = false;
#endif

    ////////////////////////////////////////////////////////////////////////
    // Exceptions
    ////////////////////////////////////////////////////////////////////////

    // TODO this doesn't seem to properly catch all LibsemigroupsExceptions,
    // particularly on macOS. This may have been resolved in pybind11 2.12.0
    static py::exception<LibsemigroupsException> exc(
        m, "LibsemigroupsError", PyExc_RuntimeError);
    py::register_exception_translator([](std::exception_ptr p) {
      try {
        if (p) {
          std::rethrow_exception(p);
        }
      } catch (LibsemigroupsException const& e) {
        exc(formatted_error_message(e).c_str());
      } catch (py::stop_iteration const& e) {
        throw e;
      } catch (std::runtime_error const& e) {
        exc(formatted_error_message(e).c_str());
      }
    });

    ////////////////////////////////////////////////////////////////////////
    // Classes
    ////////////////////////////////////////////////////////////////////////

    init_reporter(m);
    init_runner(m);

    ////////////////////////////////////////////////////////////////////////
    // Misc that needs to be initialised early
    ////////////////////////////////////////////////////////////////////////

    init_tril(m);

    ////////////////////////////////////////////////////////////////////////
    // Abstract classes that are required by other classes
    ////////////////////////////////////////////////////////////////////////

    init_imagerightaction(m);
    init_action(m);
    init_bmat8(m);
    init_aho_corasick(m);
    init_blocks(m);
    init_bipart(m);
    init_PBR(m);
    init_forest(m);
    init_freeband(m);
    init_gabow(m);
    //    init_knuth_bendix(m);
    init_order(m);
    init_obvinf(m);
    init_paths(m);
    init_present(m);
    init_inverse_present(m);
    init_to_present(m);
    init_sims(m);
    init_transf(m);
    init_ranges(m);
    init_words(m);
    init_word_graph(m);
    init_dot(m);
    init_matrix(m);
    init_froidure_pin_base(m);
    init_ukkonen(m);
    init_froidure_pin(m);
    init_schreier_sims(m);
    init_cong_intf(m);
    init_todd_coxeter(m);

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif

    ////////////////////////////////////////////////////////////////////////
    // Enums
    ////////////////////////////////////////////////////////////////////////

    py::enum_<congruence_kind>(m, "congruence_kind", R"pbdoc(
    The values in this class can be used to indicate that a congruence should
    be 1-sided or 2-sided.
  )pbdoc")
        .value("onesided", congruence_kind::onesided)
        .value("twosided", congruence_kind::twosided);

    // TODO better repr?
    py::enum_<KnuthBendix<>::options::overlap>(m,
                                               "overlap",
                                               R"pbdoc(
           Values for specifying how to measure the length of an overlap.

           The values in this enum determine how a :py:class:`_libsemigroups_pybind11.KnuthBendixRewriteTrie`
           instance measures the length :math:`d(AB, BC)` of the overlap of
           two words :math:`AB` and :math:`BC`.

           .. seealso:: :py:meth:`KnuthBendix.overlap_policy()<_libsemigroups_pybind11.KnuthBendixRewriteTrie.overlap_policy>`
         )pbdoc")
        .value("ABC",
               KnuthBendix<>::options::overlap::ABC,
               R"pbdoc(
               :math:`d(AB, BC) = |A| + |B| + |C|`
             )pbdoc")
        .value("AB_BC",
               KnuthBendix<>::options::overlap::AB_BC,
               R"pbdoc(
               :math:`d(AB, BC) = |AB| + |BC|`
             )pbdoc")
        .value("MAX_AB_BC",
               KnuthBendix<>::options::overlap::MAX_AB_BC,
               R"pbdoc(
               :math:`d(AB, BC) = max(|AB|, |BC|)`
             )pbdoc");

    py::class_<ReportGuard>(m,
                            "ReportGuard",
                            R"pbdoc(
      Objects of this type can be used to enable printing of some information
      during various of the computation in ``libsemigroups_pybind11``. Reporting
      is enable (or not) at construction time, and disable when the
      :py:class:`ReportGuard` goes out of scope.
      )pbdoc")
        .def(py::init<bool>(),
             py::arg("val") = true,
             R"pbdoc(
Constructs a :py:class:`ReportGuard` with reporting enabled by
default.

:param val:
   whether to report or not (default: ``True``).

:type val:
   bool
  )pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Constants
    ////////////////////////////////////////////////////////////////////////

    py::class_<PositiveInfinity>(m, "PositiveInfinity")
        .def("__repr__",
             [](PositiveInfinity const& val) -> std::string {
               return u8"+\u221E";
             })
        .def(pybind11::self < PositiveInfinity())
        .def(pybind11::self < NegativeInfinity())
        .def(pybind11::self < int())
        .def(int() < pybind11::self)
        .def(
            "__eq__",
            [](PositiveInfinity const& lhop,
               NegativeInfinity const& rhop) -> bool { return lhop == rhop; },
            py::is_operator())
        .def(
            "__eq__",
            [](int lhop, PositiveInfinity const& rhop) -> bool {
              return lhop == rhop;
            },
            py::is_operator())
        .def(
            "__eq__",
            [](PositiveInfinity const& lhop, int rhop) -> bool {
              return lhop == rhop;
            },
            py::is_operator())
        .def(
            "__eq__",
            [](int64_t lhop, PositiveInfinity const& rhop) -> bool {
              return lhop == rhop;
            },
            py::is_operator())
        .def(
            "__eq__",
            [](PositiveInfinity const& lhop, int64_t rhop) -> bool {
              return lhop == rhop;
            },
            py::is_operator())
        .def(
            "__eq__",
            [](uint64_t lhop, PositiveInfinity const& rhop) -> bool {
              return lhop == rhop;
            },
            py::is_operator())
        .def(
            "__eq__",
            [](PositiveInfinity const& lhop, uint64_t rhop) -> bool {
              return lhop == rhop;
            },
            py::is_operator())
        .def("to_int", [](PositiveInfinity const& x) -> int64_t {
          return static_cast<int64_t>(x);
        });

    m.attr("POSITIVE_INFINITY") = POSITIVE_INFINITY;

    py::class_<NegativeInfinity>(m, "NegativeInfinity")
        .def("__repr__",
             [](NegativeInfinity const& val) -> std::string {
               return u8"-\u221E";
             })
        .def(pybind11::self < PositiveInfinity())
        .def(pybind11::self < NegativeInfinity())
        .def(pybind11::self < int())
        .def(int() < pybind11::self)
        .def(
            "__eq__",
            [](NegativeInfinity const& lhop,
               PositiveInfinity const& rhop) -> bool { return lhop == rhop; },
            py::is_operator())
        .def(
            "__eq__",
            [](int lhop, NegativeInfinity const& rhop) -> bool {
              return lhop == rhop;
            },
            py::is_operator())
        .def(
            "__eq__",
            [](NegativeInfinity const& lhop, int rhop) -> bool {
              return lhop == rhop;
            },
            py::is_operator())
        .def(
            "__eq__",
            [](int64_t lhop, NegativeInfinity const& rhop) -> bool {
              return lhop == rhop;
            },
            py::is_operator())
        .def(
            "__eq__",
            [](NegativeInfinity const& lhop, int64_t rhop) -> bool {
              return lhop == rhop;
            },
            py::is_operator())
        .def("to_int", [](NegativeInfinity const& x) -> int64_t {
          return static_cast<int64_t>(x);
        });

    m.attr("NEGATIVE_INFINITY") = NEGATIVE_INFINITY;

    py::class_<LimitMax>(m, "LimitMax")
        .def("__repr__",
             [](LimitMax const& val) -> std::string { return "<LIMIT_MAX>"; })
        .def(pybind11::self < LimitMax())
        .def(pybind11::self < int())
        .def(int() < pybind11::self)
        .def(
            "__eq__",
            [](int lhop, LimitMax const& rhop) -> bool { return lhop == rhop; },
            py::is_operator())
        .def(
            "__eq__",
            [](LimitMax const& lhop, int rhop) -> bool { return lhop == rhop; },
            py::is_operator())
        .def(
            "__eq__",
            [](int64_t lhop, LimitMax const& rhop) -> bool {
              return lhop == rhop;
            },
            py::is_operator())
        .def(
            "__eq__",
            [](LimitMax const& lhop, int64_t rhop) -> bool {
              return lhop == rhop;
            },
            py::is_operator())
        .def(
            "__sub__",
            [](LimitMax const& lhs, int rhs) { return lhs - rhs; },
            py::is_operator())
        .def(
            "__rsub__",
            [](LimitMax const& rhs, int lhs) { return lhs - rhs; },
            py::is_operator())
        .def(
            "__sub__",
            [](LimitMax const& lhs, int64_t rhs) { return lhs - rhs; },
            py::is_operator())
        .def(
            "__rsub__",
            [](LimitMax const& rhs, int64_t lhs) { return lhs - rhs; },
            py::is_operator())
        .def("to_int",
             [](LimitMax const& x) -> int { return static_cast<int>(x); });

    m.attr("LIMIT_MAX") = LIMIT_MAX;

    ////////////////////////////////////////////////////////////////////////
    // Global variables
    ////////////////////////////////////////////////////////////////////////

    // TODO: Doc
    m.def("error_message_with_prefix",
          py::overload_cast<>(&error_message_with_prefix));
    m.def("error_message_with_prefix",
          py::overload_cast<bool>(&error_message_with_prefix));

    ////////////////////////////////////////////////////////////////////////
    // Things so short they don't merit their own file
    ////////////////////////////////////////////////////////////////////////

    using TCE = detail::TCE;
    py::class_<TCE>(m, "TCE")
        .def("__repr__", &detail::to_string<TCE>)
        .def(pybind11::self == pybind11::self)
        .def(pybind11::self < pybind11::self);

    //     using KBE = detail::KBE;
    //     py::class_<KBE>(m, "KBE")
    //         .def("__repr__", &detail::to_string<KBE>)
    //         .def(pybind11::self == pybind11::self)
    //         .def(pybind11::self < pybind11::self);
    //
  }

  ////////////////////////////////////////////////////////////////////////
  // Init
  ////////////////////////////////////////////////////////////////////////
}  // namespace libsemigroups
