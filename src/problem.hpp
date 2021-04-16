//
// Created by George MacKay-Shore on 15/04/2021.
//

#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "glpk/glpk.h"

#define min(a,b) ((a < b) ? a : b)

using namespace emscripten;

namespace glpk {
    class Problem {
    private:
        glp_prob *this_problem;
        glp_smcp simplex_solver;

    public:
        Problem() { this_problem = glp_create_prob(); }
        Problem(std::string name)
        {
            this_problem = glp_create_prob();
            glp_set_prob_name(this_problem, name.c_str());
        }
        ~Problem() { glp_delete_prob(this_problem); }

        void set_prob_name(const std::string& name) { glp_set_prob_name(this_problem, name.c_str()); }
        std::string get_prob_name() { return glp_get_prob_name(this_problem); }

        void set_obj_dir(const uint8_t dir) { glp_set_obj_dir(this_problem, dir); }
        int get_obj_dir() { return glp_get_obj_dir(this_problem); }

        int add_rows(const int rows) { return glp_add_rows(this_problem, rows); }
        void set_row_name(const int i, std::string name) { glp_set_row_name(this_problem, i, name.c_str()); }
        std::string get_row_name(const int i) { return glp_get_row_name(this_problem, i); }
        void set_row_bounds(const int i, const uint8_t type, double lb, double ub)
        {
            glp_set_row_bnds(this_problem, i, type, lb, ub);
        }

        int add_cols(const int cols) { return glp_add_cols(this_problem, cols); }
        void set_column_name(const int i, std::string name) { glp_set_col_name(this_problem, i, name.c_str()); }
        std::string get_column_name(const int i) { return glp_get_col_name(this_problem, i); }
        void set_column_bounds(const int i, const uint8_t type, double lb, double ub)
        {
            glp_set_col_bnds(this_problem, i, type, lb, ub);
        }
        double get_col_prim(const int j) { return glp_get_col_prim(this_problem, j); }

        void set_obj_coef(const int j, double coef) { glp_set_obj_coef(this_problem, j, coef); }
        double get_obj_coef(const int j) { return glp_get_obj_coef(this_problem, j); }

        void load_matrix(val ia, val ja, val ar)
        {
            std::vector<int> _ia = convertJSArrayToNumberVector<int>(ia);
            std::vector<int> _ja = convertJSArrayToNumberVector<int>(ja);
            std::vector<double> _ar = convertJSArrayToNumberVector<double>(ar);

            const int ne = _ia.size() - 1;
            glp_load_matrix(this_problem, ne, _ia.data(), _ja.data(), _ar.data());
        }
        void set_matrix_row(const int i, val ind, val values)
        {
            std::vector<int> _ind = convertJSArrayToNumberVector<int>(ind);
            std::vector<double> _val = convertJSArrayToNumberVector<double>(values);

            const int len = _ind.size();
            glp_set_mat_row(this_problem, i, len, _ind.data(), _val.data());
        }
        void set_matrix_col(const int j, val ind, val values)
        {
            std::vector<int> _ind = convertJSArrayToNumberVector<int>(ind);
            std::vector<double> _val = convertJSArrayToNumberVector<double>(values);

            const int len = _ind.size() - 1;
            glp_set_mat_col(this_problem, j, len, _ind.data(), _val.data());
        }

#define UPDATE_MEMBER(js, c, t) \
    do { \
        if (!param[#js].isUndefined()) { \
            simplex_solver.c = param[#js].as<t>(); \
        } \
    } while(0)

        int simplex(val param)
        {
            glp_init_smcp(&simplex_solver);

            glp_smcp *solver = nullptr;

            if (!(param.isUndefined() || param.isNull())) {
                UPDATE_MEMBER(msgLev, msg_lev, int);
                UPDATE_MEMBER(meth, meth, int);
                UPDATE_MEMBER(pricing, pricing, int);
                UPDATE_MEMBER(rTest, r_test, int);
                UPDATE_MEMBER(tolBnd, tol_bnd, double);
                UPDATE_MEMBER(tolDj, tol_dj, double);
                UPDATE_MEMBER(tolPiv, tol_piv, double);
                UPDATE_MEMBER(objUl, obj_ul, double);
                UPDATE_MEMBER(objLl, obj_ll, double);
                UPDATE_MEMBER(itLim, it_lim, int);
                UPDATE_MEMBER(tmLim, tm_lim, int);
                UPDATE_MEMBER(outFrq, out_frq, int);
                UPDATE_MEMBER(outDly, out_dly, int);
                UPDATE_MEMBER(presolve, presolve, int);
                UPDATE_MEMBER(excl, excl, int);
                UPDATE_MEMBER(shift, shift, int);
                UPDATE_MEMBER(aorn, aorn, int);

                solver = &simplex_solver;
            }

            return glp_simplex(this_problem, solver);
        }

        double get_obj_val() { return glp_get_obj_val(this_problem); }
    };

    EMSCRIPTEN_BINDINGS(glpk_problem) {
        class_<Problem>("Problem")
                .constructor()
                .constructor<std::string>()
                .function("setProbName", &Problem::set_prob_name)
                .function("getProbName", &Problem::get_prob_name)

                .function("setObjDir", &Problem::set_obj_dir)
                .function("getObjDir", &Problem::get_obj_dir)
                .function("setObjCoef", &Problem::set_obj_coef)
                .function("getObjCoef", &Problem::get_obj_coef)
                .function("getObjVal", &Problem::get_obj_val)

                .function("addRows", &Problem::add_rows)
                .function("setRowName", &Problem::set_row_name)
                .function("getRowName", &Problem::get_row_name)
                .function("setRowBnds", &Problem::set_row_bounds)

                .function("addCols", &Problem::add_cols)
                .function("setColName", &Problem::set_column_name)
                .function("getColName", &Problem::get_column_name)
                .function("setColBnds", &Problem::set_column_bounds)
                .function("getColPrim", &Problem::get_col_prim)

                .function("loadMatrix", &Problem::load_matrix)
                .function("setMatRow", &Problem::set_matrix_row)
                .function("setMatCol", &Problem::set_matrix_col)

                .function("simplexSync", &Problem::simplex)
                ;

        value_array<std::array<double, 33>>("foo_bar_33");

        value_object<glp_smcp>("glp_smcp")
                .field("msgLev", &glp_smcp::msg_lev)
                .field("meth", &glp_smcp::meth)
                .field("pricing", &glp_smcp::pricing)
                .field("rTest", &glp_smcp::r_test)
                .field("tolBnd", &glp_smcp::tol_bnd)
                .field("tolDj", &glp_smcp::tol_dj)
                .field("tolPiv", &glp_smcp::tol_piv)
                .field("objLl", &glp_smcp::obj_ll)
                .field("objUl", &glp_smcp::obj_ul)
                .field("itLim", &glp_smcp::it_lim)
                .field("tmLim", &glp_smcp::tm_lim)
                .field("outFrq", &glp_smcp::out_frq)
                .field("outDly", &glp_smcp::out_dly)
                .field("presolve", &glp_smcp::presolve)
                .field("excl", &glp_smcp::excl)
                .field("shift", &glp_smcp::shift)
                .field("aorn", &glp_smcp::aorn)
                .field("foobar", &glp_smcp::foo_bar)
                ;
    }
}
