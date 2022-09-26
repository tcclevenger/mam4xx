#include "kohler.hpp"

#include <iomanip>
#include <sstream>
#include <fstream>

namespace mam4 {

std::string KohlerVerification::mathematica_verification_program() const {
  std::ostringstream ss;
  /* mathematica doesn't like exponential notation, so we use setprecision
   * instead.*/
  ss << "kelvinCoeff = " << std::fixed << std::setprecision(16)
     << kelvin_coefficient<Real>() << ";\n";
  ss << "rhMin = " << rhmin << ";\n";
  ss << "rhMax = " << KohlerPolynomial<Real>::rel_humidity_max << ";\n";
  ss << "hygMin = " << hmin << ";\n";
  ss << "hygMax = " << KohlerPolynomial<Real>::hygro_max << ";\n";
  ss << "radMin = " << rmin << ";\n";
  ss << "radMax = " << KohlerPolynomial<Real>::dry_radius_max_microns << ";\n";
  ss << "nn = " << n << ";\n";
  ss << "drh = (rhMax - rhMin)/(nn-1);\n";
  ss << "dhyg = (hygMax - hygMin)/(nn-1);\n";
  ss << "drad = (radMax - radMin)/(nn-1);\n";
  ss << "kinputs = Flatten[Table[{rhMin + i*drh, hygMin + j*dhyg, radMin + "
        "k*drad}, {i, 0, nn - 1}, {j, 0, nn - 1}, {k, 0, nn - 1}], 2];\n";
  ss << "kroots = Flatten[Table[NSolve[Log[kinputs[[i]][[1]]] r^4 + "
        "kelvinCoeff*(kinputs[[i]][[3]]^3 - r^3) + (kinputs[[i]][[2]] - "
        "Log[kinputs[[i]][[1]]]) kinputs[[i]][[3]]^3 r == 0 && r > 0, r, "
        "Reals], {i, Length[kinputs]}]];\n";
  ss << "kout = Table[r /. kroots[[i]], {i, Length[kroots]}];\n";
  ss << "(* Uncomment to overwrite data file: Export[\"" << HAERO_TEST_DATA_DIR
     << "/mm_kohler_roots.txt\", kout];*)\n";
  return ss.str();
}

std::string KohlerVerification::matlab_verification_program() const {
  std::ostringstream ss;
  ss << "clear; format long;\n";
  ss << "%% parameter bounds\n";
  ss << "kelvinCoeff = " << std::fixed << std::setprecision(16)
     << kelvin_coefficient<Real>() << ";\n";
  ss << "rhMin = " << rhmin << ";\n";
  ss << "rhMax = " << KohlerPolynomial<Real>::rel_humidity_max << ";\n";
  ss << "hygMin = " << hmin << ";\n";
  ss << "hygMax = " << KohlerPolynomial<Real>::hygro_max << ";\n";
  ss << "radMin = " << rmin << ";\n";
  ss << "radMax = " << KohlerPolynomial<Real>::dry_radius_max_microns << ";\n";
  ss << "nn = " << n << ";\n";
  ss << "%% parameter inputs\n";
  ss << "relh = rhMin:(rhMax - rhMin)/(nn-1):rhMax;\n";
  ss << "hygro = hygMin:(hygMax-hygMin)/(nn-1):hygMax;\n";
  ss << "dry_rad = radMin:(radMax - radMin)/(nn - 1):radMax;\n";
  ss << "%% kohler polynomial companion matrix\n";
  ss << "cmat = zeros(4);\n";
  ss << "for i=1:3\n";
  ss << "  cmat(i+1,i) = 1;\n";
  ss << "end\n";
  ss << "%% solves: companion matrix eigenvalues\n";
  ss << "wet_rad_sol = zeros(1,nn^3);\n";
  ss << "idx = 1;\n";
  ss << "for i=1:nn\n";
  ss << "  logrh=log(relh(i));\n";
  ss << "  for j=1:nn\n";
  ss << "    hyg = hygro(j);\n";
  ss << "    for k=1:nn\n";
  ss << "      dradcube = dry_rad(k)^3;\n";
  ss << "      cmat(1,4) = -kelvinCoeff*dradcube/logrh;\n";
  ss << "      cmat(2,4) = (logrh-hyg)*dradcube/logrh;\n";
  ss << "      cmat(4,4) = kelvinCoeff/logrh;\n";
  ss << "      e = eig(cmat');\n";
  ss << "      wet_rad_sol(idx) = max(real(e));\n";
  ss << "      idx = idx + 1;\n";
  ss << "    end\n";
  ss << "  end\n";
  ss << "end\n";
  ss << "%% output: uncomment to overwrite data file\n";
  ss << "% writematrix(wet_rad_sol', \"" << HAERO_TEST_DATA_DIR
     << "/matlab_kohler_roots.txt\");\n";
  return ss.str();
}

void KohlerVerification::load_true_sol_from_file() {
  std::string data_file = HAERO_TEST_DATA_DIR;
  data_file += "/mm_kohler_roots.txt";
  std::cout << "reading true solutions from data file: " << data_file << "\n";
  std::ifstream mm_sols(data_file);
  EKAT_REQUIRE(mm_sols.is_open());

  auto h_result = Kokkos::create_mirror_view(true_sol);
  Real mmroot;
  int idx = 0;
  while (mm_sols >> mmroot) {
    const int pack_idx = PackInfo::pack_idx(idx);
    const int vec_idx = PackInfo::vec_idx(idx++);
    h_result(pack_idx)[vec_idx] = mmroot;
  }
  mm_sols.close();
  Kokkos::deep_copy(true_sol, h_result);
}

void KohlerVerification::generate_input_data() {
  const auto md_policy = Kokkos::MDRangePolicy<Kokkos::Rank<3>>({0,0,0},{n,n,n});
  Kokkos::parallel_for("KohlerVerification::generate_input_data",
    md_policy, KOKKOS_CLASS_LAMBDA (const int i, const int j, const int k) {
      const int ind = haero::square(n)*i + n*j + k;
      const auto pack_idx = PackInfo::pack_idx(ind);
      const auto vec_idx = PackInfo::vec_idx(ind);
      relative_humidity(pack_idx)[vec_idx] = rhmin + i*drh;
      hygroscopicity(pack_idx)[vec_idx] = hmin + j*dhyg;
      dry_radius(pack_idx)[vec_idx] = rmin + k*ddry;
    });
}



// ETI
// double precison is required by the KohlerPolynomial class, so we intstantiate
// the two most common types here.
template struct KohlerPolynomial<PackType>;
template struct KohlerPolynomial<double>;

}  // namespace haero
