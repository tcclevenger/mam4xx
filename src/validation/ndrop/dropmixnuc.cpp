// mam4xx: Copyright (c) 2022,
// Battelle Memorial Institute and
// National Technology & Engineering Solutions of Sandia, LLC (NTESS)
// SPDX-License-Identifier: BSD-3-Clause

#include <mam4xx/mam4.hpp>

#include <mam4xx/aero_config.hpp>
#include <skywalker.hpp>
#include <validation.hpp>

using namespace skywalker;
using namespace mam4;
using namespace haero;

void dropmixnuc(Ensemble *ensemble) {
  ensemble->process([=](const Input &input, Output &output) {
    // number of vertical points.
    // validation test from standalone ndrop.
    const Real zero = 0;
    const int maxd_aspectype = 14;
    const int ntot_amode = 4;
    const int nvars = 40;
    const int psat = ndrop::psat;
    const int ncnst_tot = ndrop::ncnst_tot;
    const int nspec_max = mam4::ndrop::nspec_max;
    const int nvar_ptend_q = mam4::ndrop::nvar_ptend_q;

    const int pver = 72; // input.get_array("pver")[0];
    const auto state_q_db = input.get_array("state_q");

    const auto tair_db = input.get_array("temp");
    const auto pmid_db = input.get_array("pmid");
    const auto pint_db = input.get_array("pint");

    const auto pdel_db = input.get_array("pdel");
    const auto rpdel_db = input.get_array("rpdel");
    const auto zm_db = input.get_array("zm");
    const auto ncldwtr_db = input.get_array("ncldwtr");
    const auto kvh_db = input.get_array("kvh");
    const auto cldn_db = input.get_array("cldn");
    const auto wsub_db = input.get_array("wsub");
    const auto cldo_db = input.get_array("cldo");
    const auto qqcw_db = input.get_array("qqcw");

    // const int top_lev = 6;
    ColumnView state_q[pver];

    using ColumnHostView = typename HostType::view_1d<Real>;

    int count = 0;

    ColumnHostView state_host[pver];

    for (int kk = 0; kk < pver; ++kk) {
      state_q[kk] = haero::testing::create_column_view(nvars);
      state_host[kk] = ColumnHostView("state_host", nvars);
    } // kk

    for (int i = 0; i < nvars; ++i) {
      // input data is store on the cpu.
      for (int kk = 0; kk < pver; ++kk) {
        state_host[kk](i) = state_q_db[count];
        count++;
      }
    }

    for (int kk = 0; kk < pver; ++kk) {
      // transfer data to GPU.
      Kokkos::deep_copy(state_q[kk], state_host[kk]);
    }

    ColumnView qqcw[ncnst_tot];
    ColumnHostView qqcw_host[ncnst_tot];

    count = 0;
    for (int i = 0; i < ncnst_tot; ++i) {
      qqcw[i] = haero::testing::create_column_view(pver);
      qqcw_host[i] = ColumnHostView("qqcw_host", pver);
    }

    for (int kk = 0; kk < pver; ++kk) {
      for (int i = 0; i < ncnst_tot; ++i) {
        qqcw_host[i](kk) = qqcw_db[count];
        count++;
      }
    }

    // transfer data to GPU.
    for (int i = 0; i < ncnst_tot; ++i) {
      Kokkos::deep_copy(qqcw[i], qqcw_host[i]);
    }

    ColumnView tair;
    ColumnView pmid;
    ColumnView pint;
    ColumnView pdel;
    ColumnView rpdel;
    ColumnView zm;
    ColumnView ncldwtr;
    ColumnView kvh;
    ColumnView cldn;
    ColumnView wsub;
    ColumnView cldo;
    tair = haero::testing::create_column_view(pver);
    pmid = haero::testing::create_column_view(pver);
    pint = haero::testing::create_column_view(pver);
    pdel = haero::testing::create_column_view(pver);
    rpdel = haero::testing::create_column_view(pver);
    zm = haero::testing::create_column_view(pver);
    ncldwtr = haero::testing::create_column_view(pver);
    kvh = haero::testing::create_column_view(pver);
    cldn = haero::testing::create_column_view(pver);
    wsub = haero::testing::create_column_view(pver);
    cldo = haero::testing::create_column_view(pver);

    auto tair_host = Kokkos::create_mirror_view(tair);
    auto pmid_host = Kokkos::create_mirror_view(pmid);
    auto pint_host = Kokkos::create_mirror_view(pint);

    auto pdel_host = Kokkos::create_mirror_view(pdel);
    auto rpdel_host = Kokkos::create_mirror_view(rpdel);
    auto zm_host = Kokkos::create_mirror_view(zm);
    auto ncldwtr_host = Kokkos::create_mirror_view(ncldwtr);
    auto kvh_host = Kokkos::create_mirror_view(kvh);
    auto cldn_host = Kokkos::create_mirror_view(cldn);
    auto wsub_host = Kokkos::create_mirror_view(wsub);
    auto cldo_host = Kokkos::create_mirror_view(cldo);

    // // FIXME. Find a better way:
    for (int kk = 0; kk < pver; ++kk) {
      tair_host(kk) = tair_db[kk];
      pmid_host(kk) = pmid_db[kk];
      pint_host(kk) = pint_db[kk];
      pdel_host(kk) = pdel_db[kk];
      rpdel_host(kk) = rpdel_db[kk];
      zm_host(kk) = zm_db[kk];
      ncldwtr_host(kk) = ncldwtr_db[kk];
      kvh_host(kk) = kvh_db[kk];
      cldn_host(kk) = cldn_db[kk];
      wsub_host(kk) = wsub_db[kk];
      cldo_host(kk) = cldo_db[kk];
    }
    Kokkos::deep_copy(tair, tair_host);
    Kokkos::deep_copy(pmid, pmid_host);
    Kokkos::deep_copy(pint, pint_host);

    Kokkos::deep_copy(pdel, pdel_host);
    Kokkos::deep_copy(rpdel, rpdel_host);
    Kokkos::deep_copy(zm, zm_host);
    Kokkos::deep_copy(ncldwtr, ncldwtr_host);
    Kokkos::deep_copy(kvh, kvh_host);
    Kokkos::deep_copy(cldn, cldn_host);
    Kokkos::deep_copy(wsub, wsub_host);
    Kokkos::deep_copy(cldo, cldo_host);

    const Real dtmicro = input.get_array("dtmicro")[0];

    // const auto mam_idx_db = input.get_array("mam_idx");

    // count = 0;
    // int mam_idx[ntot_amode][nspec_max];
    // for (int i = 0; i < nspec_max; ++i) {
    //   for (int j = 0; j < ntot_amode; ++j) {
    //     mam_idx[j][i] = mam_idx_db[count];
    //     count++;
    //   }
    // }

    // const auto mam_cnst_idx_db = input.get_array("mam_cnst_idx");
    // count = 0;
    // int mam_cnst_idx[ntot_amode][nspec_max];
    // for (int i = 0; i < nspec_max; ++i) {
    //   for (int j = 0; j < ntot_amode; ++j) {
    //     mam_cnst_idx[j][i] = mam_cnst_idx_db[count];
    //     count++;
    //   }
    // }

    // output
    ColumnView qcld;
    ColumnView tendnd;
    ColumnView ndropcol;
    ColumnView ndropmix;
    ColumnView nsource;
    ColumnView wtke;
    qcld = haero::testing::create_column_view(pver);
    tendnd = haero::testing::create_column_view(pver);
    ndropcol = haero::testing::create_column_view(pver);
    ndropmix = haero::testing::create_column_view(pver);
    nsource = haero::testing::create_column_view(pver);
    wtke = haero::testing::create_column_view(pver);

    ColumnView ptend_q[nvar_ptend_q];

    count = 0;
    for (int i = 0; i < nvar_ptend_q; ++i) {
      ptend_q[i] = haero::testing::create_column_view(pver);
    }

    ColumnView factnum[pver];

    for (int i = 0; i < pver; ++i) {
      factnum[i] = haero::testing::create_column_view(ntot_amode);
    }

    ColumnView coltend[ncnst_tot];
    ColumnView coltend_cw[ncnst_tot];

    for (int i = 0; i < ncnst_tot; ++i) {
      coltend[i] = haero::testing::create_column_view(pver);
      coltend_cw[i] = haero::testing::create_column_view(pver);
    }

    ColumnView ccn[pver];

    for (int i = 0; i < pver; ++i) {
      ccn[i] = haero::testing::create_column_view(psat);
    }

    ColumnView raercol_cw[pver][2];
    ColumnView raercol[pver][2];
    for (int i = 0; i < pver; ++i) {
      raercol[i][0] = haero::testing::create_column_view(ncnst_tot);
      raercol[i][1] = haero::testing::create_column_view(ncnst_tot);
      raercol_cw[i][0] = haero::testing::create_column_view(ncnst_tot);
      raercol_cw[i][1] = haero::testing::create_column_view(ncnst_tot);
    }

    ColumnView nact[pver];
    ColumnView mact[pver];

    for (int i = 0; i < pver; ++i) {
      nact[i] = haero::testing::create_column_view(ntot_amode);
      mact[i] = haero::testing::create_column_view(ntot_amode);
    }

    ColumnView ekd;
    ekd = haero::testing::create_column_view(pver);

    ColumnView zn, csbot, zs, overlapp, overlapm, ekkp, ekkm, qncld, srcn,
        source;

    zn = haero::testing::create_column_view(pver);
    csbot = haero::testing::create_column_view(pver);
    zs = haero::testing::create_column_view(pver);
    overlapp = haero::testing::create_column_view(pver);
    overlapm = haero::testing::create_column_view(pver);
    ekkp = haero::testing::create_column_view(pver);
    ekkm = haero::testing::create_column_view(pver);
    qncld = haero::testing::create_column_view(pver);
    srcn = haero::testing::create_column_view(pver);
    source = haero::testing::create_column_view(pver);

    ColumnView dz, csbot_cscen;
    dz = haero::testing::create_column_view(pver);
    csbot_cscen = haero::testing::create_column_view(pver);

    // ColumnView qcldbrn[pver][maxd_aspectype];//[ntot_amode],
    ColumnView qcldbrn_num[pver]; // [ntot_amode]
    ColumnView raertend, qqcwtend;

    raertend = haero::testing::create_column_view(pver);
    qqcwtend = haero::testing::create_column_view(pver);

    for (int i = 0; i < pver; ++i) {
      qcldbrn_num[i] = haero::testing::create_column_view(ntot_amode);
    }
    auto team_policy = ThreadTeamPolicy(1u, Kokkos::AUTO);
    Kokkos::parallel_for(
        team_policy, KOKKOS_LAMBDA(const ThreadTeam &team) {
          int nspec_amode[ntot_amode];
          int lspectype_amode[maxd_aspectype][ntot_amode];
          int lmassptr_amode[maxd_aspectype][ntot_amode];
          Real specdens_amode[maxd_aspectype];
          Real spechygro[maxd_aspectype];
          int numptr_amode[ntot_amode];
          int mam_idx[ntot_amode][nspec_max];
          int mam_cnst_idx[ntot_amode][nspec_max];

          ndrop::get_e3sm_parameters(
              nspec_amode, lspectype_amode, lmassptr_amode, numptr_amode,
              specdens_amode, spechygro, mam_idx, mam_cnst_idx);

          Real exp45logsig[AeroConfig::num_modes()],
              alogsig[AeroConfig::num_modes()],
              num2vol_ratio_min_nmodes[AeroConfig::num_modes()],
              num2vol_ratio_max_nmodes[AeroConfig::num_modes()] = {};

          Real aten = zero;

          ndrop::ndrop_init(exp45logsig, alogsig, aten,
                            num2vol_ratio_min_nmodes,  // voltonumbhi_amode
                            num2vol_ratio_max_nmodes); // voltonumblo_amode

          ndrop::dropmixnuc(
              team, dtmicro, tair, pmid, pint, pdel, rpdel,
              zm, //  ! in zm[kk] - zm[kk+1], for pver zm[kk-1] - zm[kk]
              state_q, ncldwtr,
              kvh, // kvh[kk+1]
              cldn, lspectype_amode, specdens_amode, spechygro, lmassptr_amode,
              num2vol_ratio_min_nmodes, num2vol_ratio_max_nmodes, numptr_amode,
              nspec_amode, exp45logsig, alogsig, aten, mam_idx, mam_cnst_idx,
              qcld, //
              wsub,
              cldo, // in
              qqcw, // inout
              ptend_q, tendnd, factnum, ndropcol, ndropmix, nsource, wtke, ccn,
              coltend, coltend_cw, raercol_cw, raercol, nact, mact, ekd,
              // work arrays
              zn, csbot, zs, overlapp, overlapm, ekkp, ekkm, qncld, srcn,
              source, dz, csbot_cscen,
              /*qcldbrn*/ qcldbrn_num, raertend, qqcwtend);
        });

    auto host = Kokkos::create_mirror_view(tendnd);
    Kokkos::deep_copy(host, tendnd);
    std::vector<Real> v_host(pver);
    for (int kk = 0; kk < pver; ++kk) {
      v_host[kk] = host(kk);
    }

    output.set("tendnd", v_host);

    std::vector<Real> output_qqcw;

    // transfer data to host
    for (int i = 0; i < ncnst_tot; ++i) {
      Kokkos::deep_copy(qqcw_host[i], qqcw[i]);
    }

    for (int kk = 0; kk < pver; ++kk) {
      for (int i = 0; i < ncnst_tot; ++i) {
        output_qqcw.push_back(qqcw_host[i](kk));
      }
    }

    output.set("qqcw", output_qqcw);

    auto ptend_q_host = Kokkos::create_mirror_view(ptend_q[0]);
    std::vector<Real> output_ptend_q;
    for (int i = 0; i < nvar_ptend_q; ++i) {
      Kokkos::deep_copy(ptend_q_host, ptend_q[i]);
      for (int kk = 0; kk < pver; ++kk) {
        output_ptend_q.push_back(ptend_q_host(kk));
      }
    }
    output.set("ptend_q", output_ptend_q);

    std::vector<Real> output_factnum;
    ColumnHostView factnum_host[pver];
    for (int kk = 0; kk < pver; ++kk) {
      factnum_host[kk] = ColumnHostView("factnum_host", ntot_amode);
      Kokkos::deep_copy(factnum_host[kk], factnum[kk]);
    }

    for (int i = 0; i < ntot_amode; ++i) {
      for (int kk = 0; kk < pver; ++kk) {
        output_factnum.push_back(factnum_host[kk](i));
      }
    }

    output.set("factnum", output_factnum);
  });
}
