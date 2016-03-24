#ifndef RPL_METRICS_H
#define RPL_METRICS_H

#include "net/rpl/rpl.h"
#include "rpl-metrics-data.h"


#define READ_BIT(flags, pos) ((((flags) & (1<<pos)) == 0)?0:1)
#define WRITE_BIT(flags,pos,val) (val ?  ((flags) |= (1<<pos)):((flags) &= ~(1<<pos)))
#define READ_FIELD(flags, pos, mask) (((flags) & mask) >> pos)

#define RPL_METRIC_READ_A_FIELD(flags) READ_FIELD((flags), RPL_DAG_MC_POS_A_FIELD, 0x70)
#define RPL_METRIC_READ_PREC_FIELD(flags) ((flags) & 0xF)
#define RPL_METRIC_WRITE_A_FIELD(flags,val) ((flags)= (((flags) & 0xFF8F) | (val<<4)))
#define RPL_METRIC_WRITE_PREC_FIELD(flags,val) ((flags)= (((flags) & 0xFFF0) | val))

#define RPL_METRIC_OBJECT_HEADER_TYPE(header) READ_FIELD(header,24,0xFF000000)
#define RPL_METRIC_OBJECT_HEADER_FLAGS(header) READ_FIELD(header,8,0x00FFFF00)
#define RPL_METRIC_OBJECT_HEADER_LENGTH(header) READ_FIELD(header,0,0x000000FF)

uint8_t rpl_metrics_create_object (rpl_metric_container_t *p_mc, uint8_t  type, uint8_t pos_obj);
uint8_t rpl_metrics_create_root_container(rpl_metric_container_t *mc);
uint8_t  rpl_metrics_copy_mc (rpl_metric_container_t *p_mc_dest, rpl_metric_container_t *p_mc_orig);
uint8_t rpl_metrics_write_object_to_metric_container (rpl_metric_container_t *mc, uint8_t *buffer, uint8_t obj_cont);
uint8_t rpl_metrics_read_from_metric_container (rpl_metric_object_t *obj, uint8_t *buffer);
uint8_t rpl_metrics_array_of_metrics (rpl_metric_container_t *p_mc, rpl_metric_element_t *array_of_metrics);
uint32_t rpl_metrics_aggregated(uint8_t metric_comb_type, uint32_t parent_metric, uint32_t new_metric);
void rpl_metrics_update_metric_container(rpl_instance_t *instance);
uint8_t rpl_metrics_satisfies_constraint(rpl_metric_object_t *p_obj, rpl_metric_container_t *p_instance_container);

#endif /* RPL_METRICS_H */

