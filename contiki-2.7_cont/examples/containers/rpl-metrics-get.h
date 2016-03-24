#ifndef RPL_METRICS_GET_H
#define RPL_METRICS_GET_H

uint8_t rpl_metrics_get_NSA();
uint8_t rpl_metrics_get_energy();
uint32_t rpl_metrics_get_throughput();
uint32_t rpl_metrics_get_latency();
uint8_t rpl_metrics_get_LQL();
uint16_t rpl_metrics_get_ETX();
uint16_t rpl_metrics_get_LC();

void rpl_metrics_etx_refresh(rpl_parent_t *p, int status, int numtx);
#endif /* RPL_METRICS_GET_H */
