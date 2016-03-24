
#ifndef __PROJECT_CONF_H__
#define __PROJECT_CONF_H__

#define USE_METRIC_CONTAINERS 

#ifdef USE_METRIC_CONTAINERS
#define RPL_CONF_DAG_MC RPL_DAG_MC_ENERGY
#endif /* USE_METRIC_CONTAINERS*/

/* objective function */
#define RPL_CONF_OF rpl_metrics_containers_OF

/* DIOs with rank lower than this will be ignored.*
 * It must be selected in function of the metrics *
 */
#define RPL_CONF_MIN_HOPRANKINC 128
/*by default, RPL_CONF_MAX_RANKINC = 7*RPL_CONF_MIN_HOPRANKINC*/
//#define RPL_CONF_MAX_HOPRANKINC 1000

/* By default used weighted composition */ 
#define LEXIC_COMPOSITION

/********************weighted composition****************************/
/*
 * The maximum difference between metrics to not change the parent *
 */

#define WEIGHTED_DIFF_THRESHOLD 64

/* (optional)
 * The weight of each metric, with the precedence order        *
 * The number of elements must be the same as                  *
 * NUMBER_OF_METRICS_AND_CONST_USED                            *
 */

//#define METRICS_WEIGHTS {1,0}

/*******************************************************************/

/*******************lexicographic composition***********************/
/*
* Which metrics to maximize (the bigger, the better); the others *
* are minimized. The last element must be 0                      *
*/ 

#define MAXIMIZED_METRICS {RPL_DAG_MC_THROUGHPUT,\
                           0}

/*
 * The maximum difference between each metric pair to consider *
 * them equal and compare the next pair. It follows the        *
 * precedence order.                                           *
 * The number of elements must be the same as                  *
 * NUMBER_OF_METRICS_AND_CONST_USED                            *
 */

#define LEXIC_DIFF_THRESHOLD {64, 0}


/********************************************************************/
/********************************************************************/


/* number of path metrics and constraint objects used. */
#define NUMBER_OF_METRICS_AND_CONST_USED 2

/* difference between metrics to check optional constraints */
#define OPTIONAL_CONSTRAINTS_THRESHOLD 0


/* select containers used and add the routing metric object *
 * header as described in RFC6551 (in hexadecimal)          *
 *
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Routing-MC-Type|Res Flags|P|C|O|R| A   | Prec  | Length (bytes)|
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *         |       |       |       |       |       |       |
 */

/*
* Headers ONLY for path metrics.     *
* Uncomment only the used ones.      *
* Length and type cannot be changed. *
*/

//#define RPL_DAG_MC_USE_NSA        0x01000102
#define RPL_DAG_MC_USE_ENERGY     0x02001102
//#define RPL_DAG_MC_USE_HOPCOUNT   0x03000002
//#define RPL_DAG_MC_USE_THROUGHPUT 0x04002204
//#define RPL_DAG_MC_USE_LATENCY    0x05001304
//#define RPL_DAG_MC_USE_LQL        0x06000402
#define RPL_DAG_MC_USE_ETX        0x07000002
//#define RPL_DAG_MC_USE_LC         0x08000303

/* 
* Headers ONLY for path constraints  *
* Uncomment only the used ones.      *
* Length and type cannot be changed. *
* For each constraint it is mandatory a metric of the same type! *
*/

//#define RPL_DAG_MC_CONST_USE_NSA         0x01030102
//#define RPL_DAG_MC_CONST_USE_ENERGY      0x02031102
//#define RPL_DAG_MC_CONST_USE_HOPCOUNT	   0x03030202
//#define RPL_DAG_MC_CONST_USE_THROUGHPUT
//#define RPL_DAG_MC_CONST_USE_LATENCY	
//#define RPL_DAG_MC_CONST_USE_LQL         0x06030202
//#define RPL_DAG_MC_CONST_USE_ETX	
//#define RPL_DAG_MC_CONST_USE_LC          0x08020303


/******************************NSA ******************************************/

/* metric */
#define RPL_DAG_MC_NSA_A_AGGREGATION  0
#define RPL_DAG_MC_NSA_O_OVERLOAD     0

/* constraint */
#define RPL_DAG_MC_CONST_NSA_A_AGGREGATION  0
#define RPL_DAG_MC_CONST_NSA_O_OVERLOAD     0

#define RPL_DAG_MC_NSA_AGGREGATION_BIT 2
#define RPL_DAG_MC_NSA_OVERLOAD_BIT 1
/***************************************************************************/


/*****************************energy ***************************************/
/*between 20L (minimum resolution) and 7200L (overflow of ticks)*/
#define BATT_CHARGE      250L   /*uAh*/

/*simplify a division by shifting ( 32768 = 2^15 )*/
#define TICKS_PER_SECOND 15 

#define CURRENT_CPU      4000   /*uA*/
#define CURRENT_LPM      20     /*uA*/
#define CURRENT_TRANSMIT 17400  /*uA*/
#define CURRENT_LISTEN   18800  /*uA*/

/***************************************************/
#define RPL_DAG_MC_ENERGY_TYPE_MAINS		0
#define RPL_DAG_MC_ENERGY_TYPE_BATTERY		1
#define RPL_DAG_MC_ENERGY_TYPE_SCAVENGING	2

#define RPL_DAG_MC_ENERGY_TYPE_MYSELF  RPL_DAG_MC_ENERGY_TYPE_BATTERY

/* metric */
/*in additive mode, avoids overflow of the 8-bit variable*/   
//#define RPL_DAG_MC_ENERGY_DIVISOR               3 /*>> bits*/

#define RPL_DAG_MC_ENERGY_I_INCLUDED            1
#define RPL_DAG_MC_ENERGY_T_TYPE                RPL_DAG_MC_ENERGY_TYPE_BATTERY
#define RPL_DAG_MC_ENERGY_E_ESTIMATION          1
#define RPL_DAG_MC_ENERGY_INITIALIZATION        1

/* constraint */
#define RPL_DAG_MC_CONST_ENERGY_I_INCLUDED      1
#define RPL_DAG_MC_CONST_ENERGY_T_TYPE          RPL_DAG_MC_ENERGY_TYPE_BATTERY
#define RPL_DAG_MC_CONST_ENERGY_E_ESTIMATION    1
#define RPL_DAG_MC_CONST_ENERGY_THRESHOLD       175
/**************************************************************************/


/****************************hop count ************************************/

#define RPL_DAG_MC_HOPCOUNT_MULTIPLIER        256

/* metric */
#define RPL_DAG_MC_HOPCOUNT_INITIALIZATION    1
/* constraint */
#define RPL_DAG_MC_CONST_HOPCOUNT_THRESHOLD   4
/*************************************************************************/

/***************************throughput ***********************************/

/*throughput is 4 Bytes, but Rank is only 2 Bytes long      */
/*value is then divided by 2^RPL_DAG_MC_THROUGHPUT_DIVISOR */

#define RPL_DAG_MC_THROUGHPUT_DIVISOR          16

/* metric */
#define RPL_DAG_MC_THROUGHPUT_INITIALIZATION   6553600
/* constraint */
#define RPL_DAG_MC_CONST_THROUGHPUT_THRESHOLD  3276800
/*************************************************************************/

/***************************latency **************************************/

/*latency is 4 Bytes, but Rank is only 2 Bytes long      */
/*value is then divided by 2^ RPL_DAG_MC_LATENCY_DIVISOR */
 
#define RPL_DAG_MC_LATENCY_DIVISOR           16

/* metric */
#define RPL_DAG_MC_LATENCY_INITIALIZATION    6533600
/* constraint */
#define RPL_DAG_MC_CONST_LATENCY_THRESHOLD   655360
/*************************************************************************/

/***************************LQL ******************************************/

/* metric */
#define RPL_DAG_MC_LQL_INITIALIZATION    3
/* constraint */
#define RPL_DAG_MC_CONST_LQL_THRESHOLD   5
/*************************************************************************/

/***************************etx ******************************************/

/* metric */
#define RPL_DAG_MC_ETX_INITIALIZATION    5
/* constraint */
#define RPL_DAG_MC_CONST_ETX_THRESHOLD   7
/*****/

//#define RPL_CONF_INIT_LINK_METRIC       RPL_DAG_MC_ETX_INITIALIZATION
/*ETX when no Ack from the node is received*/
#define RPL_DAG_MC_ETX_MAX		10
#define RPL_DAG_MC_ETX_DIVISOR		128
/*weight of the old ETX when the new is calculated*/
#define ETX_ALPHA   90
#define ETX_SCALE   100


/*************************************************************************/

/***************************LC *******************************************/

#define RPL_DAG_MC_LC_I_INCLUDED    0

/* don't change this */
#define RPL_DAG_MC_LC_MASK_BIT_1    0x001
#define RPL_DAG_MC_LC_MASK_BIT_2    0x002
#define RPL_DAG_MC_LC_MASK_BIT_3    0x004
#define RPL_DAG_MC_LC_MASK_BIT_4    0x008
#define RPL_DAG_MC_LC_MASK_BIT_5    0x010
#define RPL_DAG_MC_LC_MASK_BIT_6    0x020
#define RPL_DAG_MC_LC_MASK_BIT_7    0x040
#define RPL_DAG_MC_LC_MASK_BIT_8    0x080
#define RPL_DAG_MC_LC_MASK_BIT_9    0x100
#define RPL_DAG_MC_LC_MASK_BIT_10   0x200

/* metric */
#define RPL_DAG_MC_LC_INITIALIZATION     0 //10 bits
/* constraint */
#define RPL_DAG_MC_CONST_LC_THRESHOLD    1 //10 bits
/*************************************************************************/



/* DAG Metric Container Object Types by IANA. */
#define RPL_DAG_MC_NONE			0 /* Local identifier for empty MC */
#define RPL_DAG_MC_NSA                  1 /* Node State and Attributes */
#define RPL_DAG_MC_ENERGY               2 /* Node Energy */
#define RPL_DAG_MC_HOPCOUNT             3 /* Hop Count */
#define RPL_DAG_MC_THROUGHPUT           4 /* Throughput */
#define RPL_DAG_MC_LATENCY              5 /* Latency */
#define RPL_DAG_MC_LQL                  6 /* Link Quality Level */
#define RPL_DAG_MC_ETX                  7 /* Expected Transmission Count */
#define RPL_DAG_MC_LC                   8 /* Link Color */

/* DAG Metric Container aggregation mode. */
#define RPL_DAG_MC_AGGR_ADDITIVE        0
#define RPL_DAG_MC_AGGR_MAXIMUM         1
#define RPL_DAG_MC_AGGR_MINIMUM         2
#define RPL_DAG_MC_AGGR_MULTIPLICATIVE  3


extern uint8_t depleted_battery;
extern unsigned char node_mac[8];
#define AM_I_NODE(node) (node_mac[7]==node)

#endif /* __PROJECT_CONF_H__ */
