#ifndef __PDT_PROTO_FORMAT__
#define __PDT_PROTO_FORMAT__

#define PROTO_NUM  "proto_num"
#define LAB        "lab"
#define LEN        "len"
#define COUNT      "count"
#define SLOT_INFO  "slot_info"
#define SYNC_INFO  "sync_info"
#define TACT_INFO  "tact_info"
#define SLC_INFO   "slc_info"
#define SLC_DATA   "slc_data"
#define SYNC_INDEX "sync_index"
#define ST_INFO    "st_info"
#define DC_INFO    "dc_info"
#define DC_DATA    "dc_data"
#define EMB_FLAG   "emb_flag"
#define EMB_INFO   "emb_info"
#define LC_INFO    "lc_info"
#define LC_DATA    "lc_data"
#define NO_NAME1   "no_name1"

int pfp_set_proto_format_3008(protocol_format_set_t *pfp_p);
int pfs_set_proto_format_3019(protocol_format_set_t *pfs_p);
int pfs_set_proto_format_3050(protocol_format_set_t *pfs_p);
int pfs_set_proto_format_3052(protocol_format_set_t *pfs_p);
void pfs_set_pdt_drv_proto_format(protocol_format_set_t *pfs_p);
#endif
