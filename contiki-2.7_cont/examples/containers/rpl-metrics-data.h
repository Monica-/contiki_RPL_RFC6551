#ifndef RPL_METRICS_DATA_H
#define RPL_METRICS_DATA_H


struct rpl_metric_object_NSA {
  uint8_t flags;
};
typedef struct rpl_metric_object_NSA rpl_metric_object_NSA_t;

struct rpl_metric_object_energy {
  uint8_t flags;
  uint8_t energy_est;
};
typedef struct rpl_metric_object_energy rpl_metric_object_energy_t;

struct rpl_metric_object_hop_count {
  uint8_t hop_count;
};
typedef struct rpl_metric_object_hop_count rpl_metric_object_hop_count_t;

struct rpl_metric_object_throughput {
  uint32_t throughput;
};
typedef struct rpl_metric_object_throughput rpl_metric_object_throughput_t;

struct rpl_metric_object_latency {
  uint32_t latency;
};
typedef struct rpl_metric_object_latency rpl_metric_object_latency_t;

struct rpl_metric_object_LQL {
  uint8_t value_and_counter;
};
typedef struct rpl_metric_object_LQL rpl_metric_object_LQL_t;

struct rpl_metric_object_ETX {
  uint16_t etx;
};
typedef struct rpl_metric_object_ETX rpl_metric_object_ETX_t;

struct rpl_metric_object_LC {
  uint16_t color_and_counter;
};
typedef struct rpl_metric_object_LC rpl_metric_object_LC_t;

 

struct rpl_metric_object {
  uint8_t type;
  uint16_t flags;
  uint8_t length;
  union {
    struct rpl_metric_object_NSA *nsa;
    struct rpl_metric_object_LQL *lql;
    struct rpl_metric_object_energy *energy;
    struct rpl_metric_object_hop_count *hop_count;
    struct rpl_metric_object_ETX *etx;
    struct rpl_metric_object_LC *lc;
    struct rpl_metric_object_throughput *throughput;
    struct rpl_metric_object_latency *latency;
    void *rpl_metric_object_data_pointer;
  } ;
};
typedef struct rpl_metric_object rpl_metric_object_t;



struct rpl_metric_container {

  uint8_t metric_and_const_obj;

  rpl_metric_object_t* metric_and_const_objects[NUMBER_OF_METRICS_AND_CONST_USED];
};
typedef struct rpl_metric_container rpl_metric_container_t;



struct rpl_metric_element {
  uint8_t type;
  uint8_t aggregation_mode;
  uint16_t data;
};
typedef struct rpl_metric_element rpl_metric_element_t;

#endif /* RPL_METRICS_DATA_H */
