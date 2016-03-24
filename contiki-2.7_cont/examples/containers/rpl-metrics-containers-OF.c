
#include "net/rpl/rpl-private.h"
#include "net/nbr-table.h"
#include "rpl-metrics.h"
#include "rpl-metrics-get.h"

#ifdef DEBUG_RPL_OF
#include <stdio.h>
#define DEBUG DEBUG_PRINT

#else /*DEBUG_RPL_OF*/
#define DEBUG DEBUG_NONE
#endif /*DEBUG_RPL_OF*/

#include "net/uip-debug.h"


static void reset(rpl_dag_t *);
#if defined (RPL_DAG_MC_USE_ETX) || defined (RPL_DAG_MC_CONST_USE_ETX)
static void neighbor_link_callback(rpl_parent_t *, int, int);
#endif /* defined (RPL_DAG_MC_USE_ETX) || defined (RPL_DAG_MC_CONST_USE_ETX) */
static rpl_parent_t *best_parent(rpl_parent_t *, rpl_parent_t *);
static rpl_dag_t *best_dag(rpl_dag_t *, rpl_dag_t *);
static rpl_rank_t calculate_rank(rpl_parent_t *, rpl_rank_t);
static rpl_parent_t *check_optional_constraints (rpl_parent_t *, rpl_parent_t *);
static void update_metric_container(rpl_instance_t *);


rpl_of_t rpl_metrics_containers_OF = {
  reset,
#if defined (RPL_DAG_MC_USE_ETX) || defined (RPL_DAG_MC_CONST_USE_ETX)
  neighbor_link_callback,
#endif /* defined (RPL_DAG_MC_USE_ETX) || defined (RPL_DAG_MC_CONST_USE_ETX) */
  best_parent,
  best_dag,
  calculate_rank,
  update_metric_container,
  1
};

typedef uint16_t rpl_path_metric_t;
extern uint8_t rpl_leaf;
static uint8_t etx_weight=1;


static void
reset(rpl_dag_t *sag)
{
  PRINTF("RPL: Reset OF\n");
}

/**********************************************************************/

/*
 * Function called when a packet is sent, in order to refresh
 * etx of the link
 */

#if defined (RPL_DAG_MC_USE_ETX) || defined (RPL_DAG_MC_CONST_USE_ETX)

static void
neighbor_link_callback(rpl_parent_t *p, int status, int numtx)
{
  rpl_metrics_etx_refresh(p,status, numtx);
}

#endif /* defined (RPL_DAG_MC_USE_ETX) || defined (RPL_DAG_MC_CONST_USE_ETX) */


/**********************************************************************/

static rpl_rank_t
calculate_rank(rpl_parent_t *p, rpl_rank_t base_rank)
{
  rpl_rank_t new_rank= 0;
  rpl_metric_element_t array_of_metrics[NUMBER_OF_METRICS_AND_CONST_USED];
  uint8_t number_of_elems;
  uint8_t i,j;

  if(p == NULL) {
    if(base_rank == 0) {
      return INFINITE_RANK;
    }
    new_rank = RPL_INIT_LINK_METRIC + base_rank;
  } 
  else {

    number_of_elems= rpl_metrics_array_of_metrics (&(p->dag->instance->mc), array_of_metrics);


#ifndef LEXIC_COMPOSITION /*then  WEIGHTED_COMPOSITION */
{
#if defined(METRICS_WEIGHTS)
    uint8_t weights_array[]= METRICS_WEIGHTS;
#else /* defined(METRICS_WEIGHTS) */
    uint8_t weights_array[NUMBER_OF_METRICS_AND_CONST_USED];
    for (i=0, j=number_of_elems; j>0;  j--)
      weights_array[i++]= j;
#endif /* defined(METRICS_WEIGHTS) */

#ifdef DEBUG_RPL_OF
    PRINTF("RPL: METRICS: OF: weighted composition of weights[ ");     
    for (i=0; i<number_of_elems; i++){
      PRINTF("%u ",weights_array[i]);
    }
    PRINTF("] and metrics[ ");
#endif /*#ifdef DEBUG_RPL_OF */
    
    for (i=0; i<number_of_elems; i++){
      PRINTF("%u ",array_of_metrics[i].data);
      new_rank+= weights_array[i] * array_of_metrics[i].data;
#if defined( RPL_DAG_MC_USE_ETX)
      if(array_of_metrics[i].type== RPL_DAG_MC_ETX)
	 etx_weight= weights_array[i];
#endif /* defined( RPL_DAG_MC_USE_ETX) */
    }

    PRINTF("] - ");
}
#else /* LEXIC_COMPOSITION */

    new_rank= base_rank + array_of_metrics[0].data;

#endif /* LEXIC_COMPOSITION */
 }

  if(new_rank>INFINITE_RANK)
    new_rank= INFINITE_RANK;

  PRINTF("RPL: METRICS: OF: new rank: %u\n",new_rank);

  return new_rank;
}


/**********************************************************************/


static rpl_dag_t *
best_dag(rpl_dag_t *d1, rpl_dag_t *d2)
{
  if(d1->grounded != d2->grounded) {
    return d1->grounded ? d1 : d2;
  }

  if(d1->preference != d2->preference) {
    return d1->preference > d2->preference ? d1 : d2;
  }

  return d1->rank < d2->rank ? d1 : d2;
}



/**********************************************************************/



static rpl_parent_t *
best_parent(rpl_parent_t *p1, rpl_parent_t *p2)
{
  rpl_parent_t *p_parent;

#ifndef LEXIC_COMPOSITION /*then  WEIGHTED_COMPOSITION */

  rpl_dag_t *dag;
  rpl_path_metric_t p1_metric;
  rpl_path_metric_t p2_metric;

  uint16_t etx1, etx2;

  dag = p1->dag; /* Both parents are in the same DAG. */

  p1_metric= p1->rank;
  p2_metric= p2->rank;

#if defined( RPL_DAG_MC_USE_ETX)
  /*
   * ETX is the only metric of the node that doesn't remain the  *
   * same for both parents. LQL and LC not implemented yet.      *
   */

  etx1= p1->etx;
  etx2= p2->etx;

  p1_metric+= etx1 * etx_weight;
  p2_metric+= etx2 * etx_weight;
#endif /* defined( RPL_DAG_MC_USE_ETX) */

  /* check optional constraints if difference between metrics smaller than OPTIONAL_CONSTRAINTS_THRESHOLD*/

  if(p1_metric < p2_metric + OPTIONAL_CONSTRAINTS_THRESHOLD &&
     p1_metric >(p2_metric>OPTIONAL_CONSTRAINTS_THRESHOLD?(p2_metric-OPTIONAL_CONSTRAINTS_THRESHOLD):0)) {
    p_parent= check_optional_constraints (p1, p2);
    if(p_parent!= NULL){
      return p_parent;
    }
  }

  /* Maintain stability of the preferred parent in case of similar ranks. */
  if(p1 == dag->preferred_parent || p2 == dag->preferred_parent) {
    if(p1_metric < p2_metric + WEIGHTED_DIFF_THRESHOLD &&
       p1_metric >(p2_metric>WEIGHTED_DIFF_THRESHOLD?(p2_metric-WEIGHTED_DIFF_THRESHOLD):0)) {
      PRINTF("RPL: MRHOF hysteresis: %u <= %u <= %u\n",
             p2_metric>WEIGHTED_DIFF_THRESHOLD?(p2_metric-WEIGHTED_DIFF_THRESHOLD):0,
             p1_metric,
             p2_metric + WEIGHTED_DIFF_THRESHOLD);

      return dag->preferred_parent;
    }
  }
  return p1_metric < p2_metric ? p1 : p2;

#else /* LEXIC_COMPOSITION */

  rpl_metric_element_t array_of_metrics_1[NUMBER_OF_METRICS_AND_CONST_USED];
  rpl_metric_element_t array_of_metrics_2[NUMBER_OF_METRICS_AND_CONST_USED];
  uint32_t lexic_diff_thres[]= LEXIC_DIFF_THRESHOLD;
  uint8_t number_of_elems_1, number_of_elems_2;
  uint8_t maximization_types[]=MAXIMIZED_METRICS;
  int16_t diff;
  uint8_t i,j;

  number_of_elems_1= rpl_metrics_array_of_metrics (&(p1->mc), array_of_metrics_1);
  number_of_elems_2= rpl_metrics_array_of_metrics (&(p2->mc), array_of_metrics_2);

  if(number_of_elems_1 != number_of_elems_2){
    PRINTF("RPL: METRICS: OF: Parents have different metric containers to compare\n");
    return NULL;
  }
  else {
    for (i= 0; i< number_of_elems_1; i++){
      if(array_of_metrics_1[i].type != array_of_metrics_2[i].type){
	PRINTF("RPL: METRICS: OF: Different metric objects to compare\n");
	return NULL;
      }
      diff= array_of_metrics_1[i].data- array_of_metrics_2[i].data;

      /*
       * ETX is the only metric of the node that doesn't remain the  *
       * same for both parents. LQL and LC not implemented yet.      *
       */
      if(array_of_metrics_1[i].type ==  RPL_DAG_MC_ETX)
	diff+= p1->etx - p2->etx;

      if( (diff > lexic_diff_thres[i]) || (diff < -lexic_diff_thres[i]) ){
	/*check if type is in maximization metrics array*/
	for(j=0; j<(sizeof(maximization_types)/ sizeof(maximization_types[0])); j++){
	  if(array_of_metrics_1[i].type == maximization_types[j])
	    return(diff>0 ? p1 : p2);
	}
	return(diff<0 ? p1 : p2);
      }
    }
    return p1->dag->preferred_parent;  /* Both parents are in the same DAG. */
  }

#endif /* LEXIC_COMPOSITION */

}



/**********************************************************************/


static rpl_parent_t *
check_optional_constraints (rpl_parent_t *p1, rpl_parent_t *p2){

  rpl_metric_object_t *p_obj, *opt_const[NUMBER_OF_METRICS_AND_CONST_USED>>1];
  uint8_t i, j, imin, num_objects, num_opt_const;
  uint16_t flags;
  uint8_t p1_const, p2_const;
 
  /*identify optional constraints*/
  num_opt_const= 0;
  num_objects= p1->mc.metric_and_const_obj;
  for(i=0; i<num_objects; i++){
    p_obj= p1->mc.metric_and_const_objects[i];
    flags= p_obj->flags;
    if( READ_BIT(flags, RPL_DAG_MC_POS_C_BIT) ) 
      if( READ_BIT(flags, RPL_DAG_MC_POS_O_BIT))
	  opt_const[num_opt_const++]= p_obj;
  }

  if(num_opt_const){

    /* sort by precedence */
    for (i=0; i< num_opt_const-1; i++){
      imin= i;
      for(j=i+1; j< num_opt_const; j++){
	if( (RPL_METRIC_READ_PREC_FIELD((opt_const[j]->flags))) < (RPL_METRIC_READ_PREC_FIELD((opt_const[imin]->flags))) )
	  imin= j;
      }
      p_obj= opt_const[i];
      opt_const[i]= opt_const[imin];
      opt_const[imin]= p_obj;
    }

    /*compare candidates*/
    p1_const= p2_const= 0;
    for(i=0; i<num_opt_const; i++){
      p1_const= rpl_metrics_satisfies_constraint (opt_const[i], &(p1->mc));
      p2_const= rpl_metrics_satisfies_constraint (opt_const[i], &(p2->mc));
      if(p1_const!=p2_const)
	return (p1_const ? p1 : p2);
    }
  }
    return NULL;
}


/**********************************************************************/


static void
update_metric_container(rpl_instance_t *instance)
{
  rpl_metrics_update_metric_container(instance);
}

