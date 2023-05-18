// mam4xx: Copyright (c) 2022,
// Battelle Memorial Institute and
// National Technology & Engineering Solutions of Sandia, LLC (NTESS)
// SPDX-License-Identifier: BSD-3-Clause

#include <mam4xx/ndrop_od.hpp>

#include <iostream>
#include <skywalker.hpp>
#include <validation.hpp>

// This driver computes the binary or ternary rename for the given
// input.

void usage() {
  std::cerr << "ndrop_driver: a Skywalker driver for validating the "
               "MAM4 rename parameterizations."
            << std::endl;
  std::cerr << "ndrop_driver: usage:" << std::endl;
  std::cerr << "ndrop_driver <input.yaml>" << std::endl;
  exit(0);
}

using namespace skywalker;
using namespace mam4;

// Parameterizations used by the aero_model process.
void ccncalc(Ensemble *ensemble);
void get_activate_frac(Ensemble *ensemble);
void activate_modal(Ensemble *ensemble);
void loadaer(Ensemble *ensemble);
void ccncalc_single_cell(Ensemble *ensemble);
void explmix(Ensemble *ensemble);
void maxsat(Ensemble *ensemble);

int main(int argc, char **argv) {
  if (argc == 1) {
    usage();
  }
  validation::initialize(argc, argv);
  std::string input_file = argv[1];
  std::string output_file = validation::output_name(input_file);
  std::cout << argv[0] << ": reading " << input_file << std::endl;

  // Load the ensemble. Any error encountered is fatal.
  Ensemble *ensemble = skywalker::load_ensemble(input_file, "mam4xx");

  // the settings.
  Settings settings = ensemble->settings();
  if (!settings.has("function")) {
    std::cerr << "No function specified in mam4xx.settings!" << std::endl;
    exit(1);
  }

  // Dispatch to the requested function.
  auto func_name = settings.get("function");
  try {
    if (func_name == "ccncalc") {
      ccncalc(ensemble);
    } else if (func_name == "get_activate_frac") {
      get_activate_frac(ensemble);
    } else if (func_name == "activate_modal") {
      activate_modal(ensemble);
    } else if (func_name == "loadaer") {
      loadaer(ensemble);
    } else if (func_name == "ccncalc_single_cell") {
      ccncalc_single_cell(ensemble);
    } else if (func_name == "explmix") {
      explmix(ensemble);
    } else if (func_name == "maxsat") {
      // maxsat(ensemble);
    }

  } catch (std::exception &e) {
    std::cerr << argv[0] << ": Error: " << e.what() << std::endl;
  }

  // Write out a Python module.
  std::cout << argv[0] << ": writing " << output_file << std::endl;
  ensemble->write(output_file);

  // Clean up.
  delete ensemble;
  validation::finalize();
}
