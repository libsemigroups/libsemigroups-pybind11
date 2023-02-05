//
// libsemigroups - C++ library for semigroups and monoids
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

// libsemigroups....
#include <libsemigroups/constants.hpp>  // for POSITIVE_INFINITY
#include <libsemigroups/stephen.hpp>    // for Stephen

// pybind11....
#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>  // for class_, init, make_iterator, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "doc-strings.hpp"  // for doc_strings
#include "main.hpp"         // for init_stephen

namespace py = pybind11;

namespace libsemigroups {

  void init_stephen(py::module& m) {
    py::class_<Stephen>(m, "Stephen")
        .def(py::init<Presentation<word_type> const&>(),
             py::arg("p"),
             R"pbdoc(
               Construct from a presentation with relation words given by lists
               of integers.

               :Parameters: **p** (Presentation) - the presentation.

               :Raises:
                 ``RunTimeError`` if ``p.validate()`` raises.
               :Raises:
                 ``RunTimeError`` if ``p.alphabet().size()`` is ``0``.
             )pbdoc")
        .def(py::init<Presentation<std::string> const&>(),
             py::arg("p"),
             R"pbdoc(
               Construct from a presentation with relation words given by
               strings.

               :Parameters: **p** (Presentation) - the presentation.

               :Raises:
                 ``RunTimeError`` if ``p.validate()`` raises.
               :Raises:
                 ``RunTimeError`` if ``p.alphabet().size()`` is ``0``.

               .. note:
                 :py:class:`Stephen` uses a presentation with relation words
                 given by lists of integers, and it is necessary to use
                 integers to represent letters in the alphabet in the relevant
                 methods for this class.
             )pbdoc")
        .def(py::init<Stephen const&>(),
             py::arg("s"),
             R"pbdoc(
               Copy a :py:class:`Stephen` instance.
             )pbdoc")
        .def("init",
             &Stephen::init<Presentation<word_type> const&>,
             py::arg("p"),
             R"pbdoc(
               Initialize from a presentation with relation words given by
               lists of integers.

               Replaces the current value (if any) returned by
               :py:meth:`Stephen.presentation` by the argument, and the state
               of the object is the same as if it had been newly constructed
               from the presentation ``p``.

               :Parameters: **p** (Presentation) - the presentation.

               :Returns: ``self``.

               :Raises:
                 ``RunTimeError`` if ``p.validate()`` raises.
               :Raises:
                 ``RunTimeError`` if ``p.alphabet().size()`` is ``0``.
             )pbdoc")
        .def("init",
             &Stephen::init<Presentation<std::string> const&>,
             py::arg("p"),
             R"pbdoc(
               Initialize from a presentation with relation words given by
               strings.

               Replaces the current value (if any) returned by
               :py:meth:`Stephen.presentation` by the argument, and the state
               of the object is the same as if it had been newly constructed
               from the presentation ``p``.

               :Parameters: **p** (Presentation) - the presentation.

               :Returns: ``self``.

               :Raises:
                 ``RunTimeError`` if ``p.validate()`` raises.
               :Raises:
                 ``RunTimeError`` if ``p.alphabet().size()`` is ``0``.
             )pbdoc")
        .def("set_word",
             py::overload_cast<word_type const&>(&Stephen::set_word),
             py::arg("w"),
             R"pbdoc(
               Set the word.

               :Parameters: **w** (List[int]) - the input word

               :Returns: ``self``.
               )pbdoc")
        .def("word",
             &Stephen::word,
             R"pbdoc(
               The word.

               :return:
                 The word used to initialise the current instance of
                 :py:meth:`Stephen`.
               )pbdoc")
        .def("presentation",
             &Stephen::presentation,
             R"pbdoc(
               The input presentation.

               :return: the input presentation
               :rtype: Presentation
               )pbdoc")
        .def("word_graph",
             &Stephen::word_graph,
             R"pbdoc(
               The word graph.

               Returns the word graph in its present state. The algorithm
               implemented in this class is not triggered by calls to this
               function.

               :return: The underlying word graph.
               :rtype: ActionDigraph
             )pbdoc")
        .def("accept_state",
             &Stephen::accept_state,
             R"pbdoc(
               The accept state of the word graph.

               This function triggers the algorithm implemented in this class (if it
               hasn't been triggered already), and then returns the accept state of
               the produced word graph.

               :return: the accept state
               :rtype: int

               :raises RuntimeError:
                 if no presentation was set at the construction of ``s`` or with
                 :py:meth:`Stephen.init`.

               :warning:
                 The problem of determining whether two words are equal in a
                 finitely presented semigroup is undecidable in general, and this
                 function may never terminate.
             )pbdoc")
        .def("dead", &Stephen::dead, runner_doc_strings::dead)
        .def("finished", &Stephen::finished, runner_doc_strings::finished)
        .def("started", &Stephen::started, runner_doc_strings::started)
        .def("stopped", &Stephen::stopped, runner_doc_strings::stopped)
        .def("timed_out", &Stephen::timed_out, runner_doc_strings::timed_out)
        .def("running", &Stephen::running, runner_doc_strings::running)
        .def("stopped_by_predicate",
             &Stephen::stopped_by_predicate,
             runner_doc_strings::stopped_by_predicate)
        .def("kill", &Stephen::kill, runner_doc_strings::kill)
        .def("run", &Stephen::run, runner_doc_strings::run)
        .def("run_for",
             (void(Stephen::*)(std::chrono::nanoseconds)) & Runner::run_for,
             py::arg("t"),
             runner_doc_strings::run_for)
        .def("run_until",
             (void(Stephen::*)(std::function<bool()>&)) & Runner::run_until,
             py::arg("func"),
             runner_doc_strings::run_until)
        .def("report_every",
             (void(Stephen::*)(std::chrono::nanoseconds))
                 & Runner::report_every,
             py::arg("t"),
             runner_doc_strings::report_every)
        .def("report", &Stephen::report, runner_doc_strings::report)
        .def("report_why_we_stopped",
             &Stephen::report_why_we_stopped,
             runner_doc_strings::report_why_we_stopped);
    m.def("accepts",
          py::overload_cast<Stephen&, word_type const&>(&stephen::accepts),
          py::arg("s"),
          py::arg("w"),
          R"pbdoc(
            Check if a word is equivalent to :py:meth:`Stephen.word`.

            This function triggers the algorithm implemented in this class (if
            it hasn't been triggered already), and then returns ``true`` if the
            input word ``w`` is equivalent to :py:meth:`Stephen.word` in the
            semigroup defined by :py:meth:`Stephen.presentation`. A word is
            equivalent to :py:meth:`Stephen.word` if it labels a path in
            :py:meth:`Stephen.word_graph` with source \c 0 and target
            :py:meth:`Stephen.accept_state`.

            :param s: the :py:class:`Stephen` instance.
            :type s: Stephen
            :param w: the input word.
            :type w: List[int]

            :returns:
              ``True`` if the words is equivalent and ``False`` otherwise.

            :raises RuntimeError:
              if no presentation was set at the construction of ``s`` or with
              :py:meth:`Stephen.init`.

            :warning:
              The problem of determining whether two words are equal in a
              finitely presented semigroup is undecidable in general, and this
              function may never terminate.
          )pbdoc");
    m.def("is_left_factor",
          &stephen::is_left_factor,
          py::arg("s"),
          py::arg("w"),
          R"pbdoc(
            Check if a word is a left factor of :py:meth:`Stephen.word`.

            This function triggers the algorithm implemented in this class (if
            it hasn't been triggered already), and then returns ``true`` if the
            input word ``w`` is a left factor of :py:meth:`Stephen.word` in the
            semigroup defined by :py:meth:`Stephen.presentation`. A word is a
            left factor of :py:meth:`Stephen.word` if it labels a path in
            :py:meth:`Stephen.word_graph` with source ``0``.

            :param s: the :py:class:`Stephen` instance.
            :type s: Stephen
            :param w: the input word.
            :type w: List[int]

            :returns:
              ``True`` if ``w`` is a left factor of :py:meth:`Stephen.word` and
              ``False`` if not.
            :rtype: bool

            :raises RuntimeError:
              if no presentation was set at the construction of ``s`` or with
              :py:meth:`Stephen.init`.

            :warning:
              The problem of determining whether a word is a left factor of
              another word in a finitely presented semigroup is undecidable in
              general, and this function may never terminate.
          )pbdoc");
    m.def(
        "words_accepted",
        [](Stephen& s, size_t min, size_t max) {
          return py::make_iterator(stephen::cbegin_words_accepted(s, min, max),
                                   stephen::cend_words_accepted(s));
        },
        py::arg("s"),
        py::arg("min") = 0,
        py::arg("max") = static_cast<size_t>(POSITIVE_INFINITY),
        R"pbdoc(
          Returns an iterator pointing at the first word equivalent to
          :py:meth:`Stephen.word` in short-lex order.

          This function triggers the algorithm implemented in this class (if it
          hasn't been triggered already).

          :param s:
            the :py:class:`Stephen` instance
          :type s:
            Stephen
          :param min:
            the minimum length of an equivalent word (default: ``0``)
          :type min:
            int
          :param max:
            the maximum length of an equivalent word (default:
            :py:class:`POSITIVE_INFINITY`)
          :type max:
            int

          :returns: An iterator.

          :raises RuntimeError:
            if no presentation was set at the construction of ``s`` or with
            :py:meth:`Stephen.init`.

          :warning:
            The problem of determining whether two words are equal in a
            finitely presented semigroup is undecidable in general, and this
            function may never terminate.

          .. seealso::
            :py:meth:`ActionDigraph.pstislo` for more information about the
            iterators returned by this function.
          )pbdoc");
    m.def(
        "left_factors",
        [](Stephen& s, size_t min, size_t max) {
          return py::make_iterator(stephen::cbegin_left_factors(s, min, max),
                                   stephen::cend_left_factors(s));
        },
        py::arg("s"),
        py::arg("min") = 0,
        py::arg("max") = static_cast<size_t>(POSITIVE_INFINITY),
        R"pbdoc(
          Returns an iterator pointing at the first word (in short-lex order)
          that is a left factor of :py:meth:`Stephen.word`.

          This function triggers the algorithm implemented in this class (if it
          hasn't been triggered already).

          :param s:
            the :py:class:`Stephen` instance
          :type s:
            Stephen
          :param min:
            the minimum length of an equivalent word (default: ``0``)
          :type min:
            int
          :param max:
            the maximum length of an equivalent word (default:
            :py:class:`POSITIVE_INFINITY`)
          :type max:
            int

          :returns: An iterator.

          :raises RuntimeError:
            if no presentation was set at the construction of ``s`` or with
            :py:meth:`Stephen.init`.

          :warning:
            The problem of determining whether a word is a left factor of
            another word in a finitely presented semigroup is undecidable in
            general, and this function may never terminate.
        )pbdoc");
    m.def("number_of_words_accepted",
          &stephen::number_of_words_accepted,
          py::arg("s"),
          py::arg("min") = 0,
          py::arg("max") = static_cast<size_t>(POSITIVE_INFINITY),
          R"pbdoc(
            Returns the number of words accepted with length in a given range.

            This function returns the number of words that are equivalent to
            :py:meth:`Stephen.word` in the instance ``s`` with length between
            ``min`` and ``max``. This is the same as the number of paths in
            :py:meth:`Stephen.word_graph` (if :py:meth:`Stephen.run` has been
            called) with source ``0``, target :py:meth:`Stephen.accept_state`,
            and length in the range ``min`` to ``max``.

            :param s:
              the :py:class:`Stephen` instance.
            :type s:
              Stephen
            :param min:
              the minimum length of a word (default: ``0``).
            :type min:
              int
            :param max:
              one more than the maximum length of a word (default:
              :py:class:`POSITIVE_INFINITY`).
            :type max:
              int

            :returns: An ``int``.

            :raises RuntimeError:
              if no presentation was set at the construction of ``s`` or with
              :py:meth:`Stephen.init`.

            .. seealso:: :py:meth:`ActionDigraph.number_of_paths`
          )pbdoc");
    m.def("number_of_left_factors",
          &stephen::number_of_left_factors,
          py::arg("s"),
          py::arg("min") = 0,
          py::arg("max") = static_cast<size_t>(POSITIVE_INFINITY),
          R"pbdoc(
            Returns the number of left factors with length in a given range.

            This function returns the number of left factors of the
            :py:meth:`Stephen.word` in the instance ``s`` with length between
            ``min`` and ``max``. This is the same as the number of paths in
            :py:meth:`Stephen.word_graph` (if :py:meth:`Stephen.run` has been
            called) with source ``0`` and length in the range ``min`` to
            ``max``.

            :param s:
              the :py:class:`Stephen` instance.
            :type s:
              Stephen
            :param min:
              the minimum length of a word (default: ``0``).
            :type min:
              int
            :param max:
              one more than the maximum length of a word (default:
              :py:class:`POSITIVE_INFINITY`).
            :type max:
              int

            :returns: An ``int``.

            :raises RuntimeError:
              if no presentation was set at the construction of ``s`` or with
              :py:meth:`Stephen.init`.
          )pbdoc");
  }
}  // namespace libsemigroups
