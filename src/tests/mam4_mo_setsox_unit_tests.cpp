#include "atmosphere_utils.hpp"
#include "testing.hpp"

// #include "mam4xx/aero_modes.hpp"
// #include "mam4xx/conversions.hpp"
// #include <mam4xx/mode_dry_particle_size.hpp>
// #include <mam4xx/aero_config.hpp>

// #include <haero/constants.hpp>
#include <haero/floating_point.hpp>
#include <haero/haero.hpp>

// #include "mam4xx/conversions.hpp"

#include <catch2/catch.hpp>
#include <ekat/logging/ekat_logger.hpp>
#include <ekat/mpi/ekat_comm.hpp>
#include <mam4xx/mam4.hpp>

// using namespace haero;
using namespace mam4;
// using namespace mam4::conversions;
const int nspec_gas = AeroConfig::num_gas_phase_species();

TEST_CASE("test_mo_setsox_init", "mam4_mo_setsox_unit_tests") {
  ekat::Comm comm;
  ekat::logger::Logger<> logger("mo_setsox unit tests",
                                ekat::logger::LogLevel::debug, comm);

  Real a = 1.0;
  mam4::mo_setsox::sox_init(a);
  mam4::mo_setsox::setsox();
  REQUIRE(FloatingPoint<Real>::equiv(a, 1.0));
}

TEST_CASE("test_sox_cldaero_create_obj", "mam4_mo_setsox_unit_tests") {
  ekat::Comm comm;
  ekat::logger::Logger<> logger("mo_setsox unit tests",
                                ekat::logger::LogLevel::debug, comm);

  const int nspec = mam4::mo_setsox::nspec_gas;
  const Real cldfrc1 = 1.0;
  const Real cldfrc0 = 0.0;
  Real qcw[nspec];
  for (int i = 0; i < nspec; ++i) {
    qcw[i] = 1;
  }
  // const int lptr_so4_cw_amode[3] = {0, 1, 2};
  const Real lwc = 1;
  const Real cfact = 1;
  int loffset = 0;

  // cldfrc > 0 => calculate xlwc, so4c comes from adding 3 entries of qwc,
  // and so4_fact = 1
  mam4::mo_setsox::Cloudconc cldconc = mam4::mo_setsox::sox_cldaero_create_obj(
      cldfrc1, qcw, lwc, cfact, loffset);
  logger.debug("so4c = {}, xlwc = {}, so4_fact = {}", cldconc.so4c,
               cldconc.xlwc, cldconc.so4_fact);
  REQUIRE(FloatingPoint<Real>::equiv(cldconc.so4c, 3.0));
  REQUIRE(FloatingPoint<Real>::equiv(cldconc.xlwc, 1.0));
  REQUIRE(FloatingPoint<Real>::equiv(cldconc.so4_fact, 1.0));

  // cldfrc = 0 => xlwc = 0
  cldconc = mam4::mo_setsox::sox_cldaero_create_obj(cldfrc0, qcw, lwc, cfact,
                                                    loffset);
  logger.debug("so4c = {}, xlwc = {}, so4_fact = {}", cldconc.so4c,
               cldconc.xlwc, cldconc.so4_fact);
  REQUIRE(FloatingPoint<Real>::equiv(cldconc.so4c, 3.0));
  REQUIRE(FloatingPoint<Real>::equiv(cldconc.xlwc, 0.0));
  REQUIRE(FloatingPoint<Real>::equiv(cldconc.so4_fact, 1.0));
}

TEST_CASE("test_cldaero_uptakerate", "mam4_mo_setsox_unit_tests") {
  ekat::Comm comm;
  ekat::logger::Logger<> logger("mo_setsox unit tests",
                                ekat::logger::LogLevel::debug, comm);

  Real xl = 1.0;
  Real cldnum = 1.0;
  Real cfact = 1.0;
  Real cldfrc = 1.0;
  Real tfld = 1.0;
  Real press = 1.0;
  Real uptkrate = 1.0;

  uptkrate = mam4::mo_setsox::cldaero_uptakerate(xl, cldnum, cfact, cldfrc,
                                                 tfld, press);
  logger.debug("uptkrate = {}", uptkrate);
  REQUIRE(FloatingPoint<Real>::equiv(uptkrate, uptkrate));
}

TEST_CASE("test_sox_cldaero_update", "mam4_mo_setsox_unit_tests") {
  ekat::Comm comm;
  ekat::logger::Logger<> logger("mo_setsox unit tests",
                                ekat::logger::LogLevel::debug, comm);

  int loffset = 0;
  Real dt = 0;
  Real mbar = 0;
  Real pdel = 0;
  Real press = 0;
  Real tfld = 0;
  Real cldnum = 0;
  Real cldfrc = 0;
  Real cfact = 0;
  Real xlwc = 0;
  Real delso4_hprxn = 0;
  Real xh2so4 = 0;
  Real xso4 = 0;
  Real xso4_init = 0;
  Real qcw[nspec_gas], qin[nspec_gas];
  for (int i = 0; i < nspec_gas; ++i) {
    qcw[i] = 0;
    qin[i] = 0;
  }

  mam4::mo_setsox::sox_cldaero_update(loffset, dt, mbar, pdel, press, tfld,
                                      cldnum, cldfrc, cfact, xlwc, delso4_hprxn,
                                      xh2so4, xso4, xso4_init, qcw, qin);
}

// update_tmr(Real &tmr, Real dqdt, Real dtime)
// void update_tmr_nonzero(tmr, idx)

// calc_ph_values
// henry_factor_co2
// calc_ynetpos
