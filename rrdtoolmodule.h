#ifndef __RRDTOOLMODULE_H_
#define __RRDTOOLMODULE_H_

#include <rrd.h>

/* for some reason, some declarations are not available on librrd
   distributions, so we re-define them here. */

enum cf_en { CF_AVERAGE = 0,    /* data consolidation functions */
    CF_MINIMUM,
    CF_MAXIMUM,
    CF_LAST,
    CF_HWPREDICT,
    /* An array of predictions using the seasonal 
     * Holt-Winters algorithm. Requires an RRA of type
     * CF_SEASONAL for this data source. */
    CF_SEASONAL,
    /* An array of seasonal effects. Requires an RRA of
     * type CF_HWPREDICT for this data source. */
    CF_DEVPREDICT,
    /* An array of deviation predictions based upon
     * smoothed seasonal deviations. Requires an RRA of
     * type CF_DEVSEASONAL for this data source. */
    CF_DEVSEASONAL,
    /* An array of smoothed seasonal deviations. Requires
     * an RRA of type CF_HWPREDICT for this data source.
     * */
    CF_FAILURES,
    /* HWPREDICT that follows a moving baseline */
    CF_MHWPREDICT
        /* new entries must come last !!! */
};

typedef int (*rrd_fetch_cb_t)(
    const char     *filename,  /* name of the rrd */
    enum cf_en     cf_idx, /* consolidation function */
    time_t         *start,
    time_t         *end,       /* which time frame do you want?
                                * will be changed to represent reality */
    unsigned long  *step,      /* which stepsize do you want? 
                                * will be changed to represent reality */
    unsigned long  *ds_cnt,    /* number of data sources in file */
    char           ***ds_namv, /* names of data_sources */
    rrd_value_t    **data      /* two dimensional array containing the data */
);

int rrd_fetch_cb_register(rrd_fetch_cb_t cb);

#endif /* __RRDTOOLMODULE_H_ */
