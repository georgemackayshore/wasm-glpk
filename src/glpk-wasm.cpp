//
// Created by George MacKay-Shore on 13/04/2021.
//

#include <emscripten/bind.h>
#include <emscripten/html5.h>
#include "glpk/glpk.h"

#include "problem.hpp"

using namespace emscripten;

extern "C" {
    void error_hook(void *s) {
        emscripten_throw_string((const char *) s);
    }

    int main(int argc, char **argv) {
        emscripten_console_log("Initialising GLPK");
        glp_error_hook(error_hook, nullptr);
    }
}

namespace glpk {
// This will be exported as Module.glpk
    struct glpk_consts {
        // Version
        uint8_t major_version;
        uint8_t minor_version;

        // Optimisation
        uint8_t min;
        uint8_t max;

        // Structural variable kind
        uint8_t cv;
        uint8_t iv;
        uint8_t bv;

        // Auxiliary/structural variable kind
        uint8_t fr;
        uint8_t lo;
        uint8_t up;
        uint8_t db;
        uint8_t fx;

        // Auxiliary/structural variable kind
        uint8_t bs;
        uint8_t nl;
        uint8_t nu;
        uint8_t nf;
        uint8_t ns;

        // Scaling options
        uint8_t sf_gm;
        uint8_t sf_eq;
        uint8_t sf_2n;
        uint8_t sf_skip;
        uint8_t sf_auto;

        // Solution indicator
        uint8_t sol;
        uint8_t ipt;
        uint8_t mip;

        // Solution status
        uint8_t undef;
        uint8_t feas;
        uint8_t infeas;
        uint8_t nofeas;
        uint8_t opt;
        uint8_t unbnd;

        // Simplex message levels
        uint8_t msg_off;
        uint8_t msg_err;
        uint8_t msg_on;
        uint8_t msg_all;
        uint8_t msg_dbg;

        // Simplex methods
        uint8_t primal;
        uint8_t dualp;
        uint8_t dual;

        // Simplex pricing
        uint8_t pt_std;
        uint8_t pt_pse;

        // Simplex radio test
        uint8_t rt_std;
        uint8_t rt_har;
        uint8_t rt_flip;

        // Simplex a or n
        uint8_t use_at;
        uint8_t use_nt;
    } _glpk_consts = {
            .major_version = GLP_MAJOR_VERSION,
            .minor_version = GLP_MINOR_VERSION,

            .min = GLP_MIN,
            .max = GLP_MAX,

            .cv = GLP_CV,
            .iv = GLP_IV,
            .bv = GLP_BV,

            .fr = GLP_FR,
            .lo = GLP_LO,
            .up = GLP_UP,
            .db = GLP_DB,
            .fx = GLP_FX,

            .bs = GLP_BS,
            .nl = GLP_NL,
            .nu = GLP_NU,
            .nf = GLP_NF,
            .ns = GLP_NS,

            .sf_gm = GLP_SF_GM,
            .sf_eq = GLP_SF_EQ,
            .sf_2n = GLP_SF_2N,
            .sf_skip = GLP_SF_SKIP,
            .sf_auto = GLP_SF_AUTO,

            .sol = GLP_SOL,
            .ipt = GLP_IPT,
            .mip = GLP_MIP,

            .undef = GLP_UNDEF,
            .feas = GLP_FEAS,
            .infeas = GLP_INFEAS,
            .nofeas = GLP_NOFEAS,
            .opt = GLP_OPT,
            .unbnd = GLP_UNBND,

            .msg_off = GLP_MSG_OFF,
            .msg_err = GLP_MSG_ERR,
            .msg_on = GLP_MSG_ON,
            .msg_all = GLP_MSG_ALL,
            .msg_dbg = GLP_MSG_DBG,

            .primal = GLP_PRIMAL,
            .dualp = GLP_DUALP,
            .dual = GLP_DUAL,

            .pt_std = GLP_PT_STD,
            .pt_pse = GLP_PT_PSE,

            .rt_std = GLP_RT_STD,
            .rt_har = GLP_RT_HAR,
            .rt_flip = GLP_RT_FLIP,

            .use_at = GLP_USE_AT,
            .use_nt = GLP_USE_NT,
    };

    EMSCRIPTEN_BINDINGS(glpk_consts) {
        // Register the struct
        value_object<glpk_consts>("glpk_consts")
                .field("MAJOR_VERSION", &glpk_consts::major_version)
                .field("MINOR_VERSION", &glpk_consts::minor_version)

                .field("MIN", &glpk_consts::min)
                .field("MAX", &glpk_consts::max)

                .field("CV", &glpk_consts::cv)
                .field("IV", &glpk_consts::iv)
                .field("BV", &glpk_consts::bv)

                .field("FR", &glpk_consts::fr)
                .field("LO", &glpk_consts::lo)
                .field("UP", &glpk_consts::up)
                .field("DB", &glpk_consts::db)
                .field("FX", &glpk_consts::fx)

                .field("BS", &glpk_consts::bs)
                .field("NL", &glpk_consts::nl)
                .field("NU", &glpk_consts::nu)
                .field("NF", &glpk_consts::nf)
                .field("NS", &glpk_consts::ns)

                .field("SF_GM", &glpk_consts::sf_gm)
                .field("SF_EQ", &glpk_consts::sf_eq)
                .field("SF_2N", &glpk_consts::sf_2n)
                .field("SF_SKIP", &glpk_consts::sf_skip)
                .field("SF_AUTO", &glpk_consts::sf_auto)

                .field("SOL", &glpk_consts::sol)
                .field("IPT", &glpk_consts::ipt)
                .field("MIP", &glpk_consts::mip)

                .field("UNDEF", &glpk_consts::undef)
                .field("FEAS", &glpk_consts::feas)
                .field("INFEAS", &glpk_consts::infeas)
                .field("NOFEAS", &glpk_consts::nofeas)
                .field("OPT", &glpk_consts::opt)
                .field("UNBND", &glpk_consts::unbnd);

        constant("glpk", _glpk_consts);
    }
}