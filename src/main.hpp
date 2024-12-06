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

#ifndef SRC_MAIN_HPP_
#define SRC_MAIN_HPP_

#include <pybind11/pybind11.h>

namespace libsemigroups {

  namespace py = pybind11;

  void init_reporter(py::module&);
  void init_runner(py::module&);
  void init_bmat8(py::module&);
  void init_imagerightaction(py::module&);

  void init_aho_corasick(py::module&);
  void init_blocks(py::module&);
  void init_bipart(py::module&);
  void init_PBR(py::module&);
  void init_forest(py::module&);
  void init_gabow(py::module&);
  void init_knuth_bendix(py::module&);
  void init_order(py::module&);
  void init_paths(py::module&);
  void init_present(py::module&);
  void init_inverse_present(py::module&);
  void init_to_present(py::module&);
  void init_sims(py::module&);
  void init_transf(py::module&);
  void init_ranges(py::module&);
  void init_words(py::module&);
  void init_word_graph(py::module&);
  void init_action(py::module&);
  void init_freeband(py::module&);
  void init_dot(py::module&);
  void init_matrix(py::module&);
  void init_obvinf(py::module&);
  void init_froidure_pin_base(py::module&);
  void init_ukkonen(py::module&);
  void init_froidure_pin(py::module&);
  void init_schreier_sims(py::module&);
  void init_cong_intf(py::module&);
  void init_todd_coxeter(py::module&);

}  // namespace libsemigroups

#endif  // SRC_MAIN_HPP_
