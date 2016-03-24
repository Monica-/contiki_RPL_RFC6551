
#include "rpl-metrics.h"
#include "net/rpl/rpl-private.h"
#include "rpl-metrics-get.h"
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG_RPL_METRICS
#define DEBUG DEBUG_PRINT
#else
#define DEBUG DEBUG_NONE
#endif
#include "net/uip-debug.h"

extern uint8_t rpl_leaf;
extern uint8_t leaf_dio;

uint8_t 
rpl_metrics_create_object (rpl_metric_container_t *p_mc, uint8_t  type, uint8_t pos_obj)
{
  rpl_metric_object_t *p_obj;
  uint8_t error;

  if( NULL == (p_mc->metric_and_const_objects[pos_obj]= malloc (sizeof(rpl_metric_object_t))))
    return 1;

  error= 0;
  p_obj= p_mc->metric_and_const_objects[pos_obj];

  switch (type){
#if defined( RPL_DAG_MC_USE_NSA) || defined (RPL_DAG_MC_CONST_USE_NSA)
  case	 RPL_DAG_MC_NSA:
    if( NULL == (p_obj->nsa= malloc (sizeof(rpl_metric_object_NSA_t))))
      error= 1;
    break;
#endif /* defined( RPL_DAG_MC_USE_NSA) || defined (RPL_DAG_MC_CONST_USE_NSA) */
#if defined( RPL_DAG_MC_USE_ENERGY) || defined( RPL_DAG_MC_CONST_USE_ENERGY)
  case RPL_DAG_MC_ENERGY:
    if( NULL == (p_obj->energy= malloc (sizeof(rpl_metric_object_energy_t))))
      error= 1;
    break;    
#endif /*defined( RPL_DAG_MC_USE_ENERGY) || defined( RPL_DAG_MC_CONST_USE_ENERGY)*/
#if defined( RPL_DAG_MC_USE_HOPCOUNT) || defined( RPL_DAG_MC_CONST_USE_HOPCOUNT)
  case RPL_DAG_MC_HOPCOUNT:
    if( NULL == (p_obj->hop_count= malloc (sizeof(rpl_metric_object_hop_count_t))))
      error= 1;
    break;  
#endif /*defined( RPL_DAG_MC_USE_HOPCOUNT) || defined( RPL_DAG_MC_CONST_USE_HOPCOUNT)*/
#if defined( RPL_DAG_MC_USE_THROUGHPUT) || defined( RPL_DAG_MC_CONST_USE_THROUGHPUT)
  case RPL_DAG_MC_THROUGHPUT:
    if( NULL == (p_obj->throughput= malloc (sizeof(rpl_metric_object_throughput_t))))
      error= 1;
    break;
#endif /*defined( RPL_DAG_MC_USE_THROUGHPUT) || defined( RPL_DAG_MC_CONST_USE_THROUGHPUT)*/
#if defined( RPL_DAG_MC_USE_LATENCY) || defined( RPL_DAG_MC_CONST_USE_LATENCY)
  case RPL_DAG_MC_LATENCY:
    if( NULL == (p_obj->latency= malloc (sizeof(rpl_metric_object_latency_t))))
      error= 1;
    break;   
#endif /*defined( RPL_DAG_MC_USE_LATENCY) || defined( RPL_DAG_MC_CONST_USE_LATENCY)*/
#if defined( RPL_DAG_MC_USE_LQL) || defined( RPL_DAG_MC_CONST_USE_LQL)
  case RPL_DAG_MC_LQL:
    if( NULL == (p_obj->lql= malloc (sizeof(rpl_metric_object_LQL_t))))
      error= 1;
    break;	      
#endif /*defined( RPL_DAG_MC_USE_LQL) || defined( RPL_DAG_MC_CONST_USE_LQL)*/
#if defined( RPL_DAG_MC_USE_ETX) || defined( RPL_DAG_MC_CONST_USE_ETX)
  case RPL_DAG_MC_ETX:
    if( NULL == (p_obj->etx= malloc (sizeof(rpl_metric_object_ETX_t))))
      error= 1;
    break;	      
#endif /*defined( RPL_DAG_MC_USE_ETX) || defined( RPL_DAG_MC_CONST_USE_ETX)*/
#if defined( RPL_DAG_MC_USE_LC) || defined( RPL_DAG_MC_CONST_USE_LC)
  case RPL_DAG_MC_LC:
    if( NULL == (p_obj->lc= malloc (sizeof(rpl_metric_object_LC_t))))
      error= 1;
    break;	      
#endif /*defined( RPL_DAG_MC_USE_LC) || defined( RPL_DAG_MC_CONST_USE_LC)*/
  default:
    /* unknown metric object */
    error= 2;
  }
  if(error){
    free(p_obj);
    PRINTF("RPL: METRICS: ERROR creating an object\n");
  }

  return error;
}



/*****************************************************************************************************/


uint8_t 
rpl_metrics_create_root_container(rpl_metric_container_t *p_mc)
{
  uint8_t objects_created;
  uint32_t header;
  rpl_metric_object_t **p_p_obj;
  uint32_t object_headers[NUMBER_OF_METRICS_AND_CONST_USED];
  uint8_t i;
  
  if (NUMBER_OF_METRICS_AND_CONST_USED){   
    objects_created= 0;
    p_p_obj= p_mc->metric_and_const_objects;
#ifdef RPL_DAG_MC_USE_NSA
    if( rpl_metrics_create_object (p_mc, RPL_DAG_MC_NSA, objects_created) )
      return 0;
    object_headers[objects_created++]= RPL_DAG_MC_USE_NSA;
    (*p_p_obj++)->nsa->flags= RPL_DAG_MC_NSA_O_OVERLOAD|(RPL_DAG_MC_NSA_A_AGGREGATION<<1); 
#endif /* RPL_DAG_MC_USE_NSA */
    
#ifdef RPL_DAG_MC_USE_ENERGY     
    if( rpl_metrics_create_object (p_mc, RPL_DAG_MC_ENERGY, objects_created) )
      return 0;
    object_headers[objects_created++]= RPL_DAG_MC_USE_ENERGY;
    (*p_p_obj)->energy->flags= (RPL_DAG_MC_ENERGY_I_INCLUDED<<3)|(RPL_DAG_MC_ENERGY_T_TYPE<<1)|RPL_DAG_MC_ENERGY_E_ESTIMATION;
    (*p_p_obj++)->energy->energy_est= RPL_DAG_MC_ENERGY_INITIALIZATION;
#endif /* RPL_DAG_MC_USE_ENERGY */     
    
#ifdef RPL_DAG_MC_USE_HOPCOUNT  
    if( rpl_metrics_create_object (p_mc, RPL_DAG_MC_HOPCOUNT, objects_created) )
      return 0;
    object_headers[objects_created++]= RPL_DAG_MC_USE_HOPCOUNT;
    (*p_p_obj++)->hop_count->hop_count= RPL_DAG_MC_HOPCOUNT_INITIALIZATION;
#endif /* RPL_DAG_MC_USE_HOPCOUNT */
    
#ifdef RPL_DAG_MC_USE_THROUGHPUT
    if( rpl_metrics_create_object (p_mc, RPL_DAG_MC_THROUGHPUT, objects_created) )
      return 0;
    object_headers[objects_created++]= RPL_DAG_MC_USE_THROUGHPUT;
    (*p_p_obj++)->throughput->throughput= RPL_DAG_MC_THROUGHPUT_INITIALIZATION;
#endif /* RPL_DAG_MC_USE_THROUGHPUT */
    
#ifdef RPL_DAG_MC_USE_LATENCY   
    if( rpl_metrics_create_object (p_mc, RPL_DAG_MC_LATENCY, objects_created) )
      return 0;
    object_headers[objects_created++]= RPL_DAG_MC_USE_LATENCY;
    (*p_p_obj++)->latency->latency= RPL_DAG_MC_LATENCY_INITIALIZATION;
#endif /* RPL_DAG_MC_USE_LATENCY */
    
#ifdef RPL_DAG_MC_USE_LQL
    if( rpl_metrics_create_object (p_mc, RPL_DAG_MC_LQL, objects_created) )
      return 0;
    object_headers[objects_created++]= RPL_DAG_MC_USE_LQL;
    (*p_p_obj++)->lql->value_and_counter= RPL_DAG_MC_LQL_INITIALIZATION<<5;
#endif /* RPL_DAG_MC_USE_LQL */
    
#ifdef RPL_DAG_MC_USE_ETX       
    if( rpl_metrics_create_object (p_mc, RPL_DAG_MC_ETX, objects_created) )
      return 0;
    object_headers[objects_created++]= RPL_DAG_MC_USE_ETX;
    (*p_p_obj++)->etx->etx= RPL_DAG_MC_ETX_INITIALIZATION * RPL_DAG_MC_ETX_DIVISOR;
#endif /* RPL_DAG_MC_USE_ETX */
    
#ifdef RPL_DAG_MC_USE_LC
    if( rpl_metrics_create_object (p_mc, RPL_DAG_MC_LC, objects_created) )
      return 0;
    object_headers[objects_created++]= RPL_DAG_MC_USE_LC;
    (*p_p_obj++)->lc->color_and_counter= RPL_DAG_MC_LC_INITIALIZATION<<6;
#endif /* RPL_DAG_MC_USE_LC */
    
#ifdef RPL_DAG_MC_CONST_USE_NSA
    if( rpl_metrics_create_object (p_mc, RPL_DAG_MC_NSA, objects_created) )
      return 0;
    object_headers[objects_created++]= RPL_DAG_MC_CONST_USE_NSA;
    (*p_p_obj++)->nsa->flags= RPL_DAG_MC_CONST_NSA_O_OVERLOAD|(RPL_DAG_MC_CONST_NSA_A_AGGREGATION<<1); 
#endif /* RPL_DAG_MC_CONST_USE_NSA */
    
#ifdef RPL_DAG_MC_CONST_USE_ENERGY   
    if( rpl_metrics_create_object (p_mc, RPL_DAG_MC_ENERGY, objects_created) )
      return 0;
    object_headers[objects_created++]= RPL_DAG_MC_CONST_USE_ENERGY;
    (*p_p_obj)->energy->flags= (RPL_DAG_MC_CONST_ENERGY_I_INCLUDED<<3)|(RPL_DAG_MC_CONST_ENERGY_T_TYPE<<1)|RPL_DAG_MC_CONST_ENERGY_E_ESTIMATION;
    (*p_p_obj++)->energy->energy_est= RPL_DAG_MC_CONST_ENERGY_THRESHOLD;
#endif /* RPL_DAG_MC_CONST_USE_ENERGY */     
    
#ifdef RPL_DAG_MC_CONST_USE_HOPCOUNT  
    if( rpl_metrics_create_object (p_mc, RPL_DAG_MC_HOPCOUNT, objects_created) )
      return 0;
    object_headers[objects_created++]= RPL_DAG_MC_CONST_USE_HOPCOUNT;
    (*p_p_obj++)->hop_count->hop_count= RPL_DAG_MC_CONST_HOPCOUNT_THRESHOLD;
#endif /* RPL_DAG_MC_CONST_USE_HOPCOUNT */
    
#ifdef RPL_DAG_MC_CONST_USE_THROUGHPUT
    if( rpl_metrics_create_object (p_mc, RPL_DAG_MC_THROUGHPUT, objects_created) )
      return 0;
    object_headers[objects_created++]= RPL_DAG_MC_CONST_USE_THROUGHPUT;
    (*p_p_obj++)->throughput->throughput= RPL_DAG_MC_CONST_THROUGHPUT_THRESHOLD;
#endif /* RPL_DAG_MC_CONST_USE_THROUGHPUT */
    
#ifdef RPL_DAG_MC_CONST_USE_LATENCY   
    if( rpl_metrics_create_object (p_mc, RPL_DAG_MC_LATENCY, objects_created) )
      return 0;
    object_headers[objects_created++]= RPL_DAG_MC_CONST_USE_LATENCY;
    (*p_p_obj++)->latency->latency= RPL_DAG_MC_CONST_LATENCY_THRESHOLD;
#endif /* RPL_DAG_MC_CONST_USE_LATENCY */
    
#ifdef RPL_DAG_MC_CONST_USE_LQL
    if( rpl_metrics_create_object (p_mc, RPL_DAG_MC_LQL, objects_created) )
      return 0;
    object_headers[objects_created++]= RPL_DAG_MC_CONST_USE_LQL;
    (*p_p_obj++)->lql->value_and_counter= RPL_DAG_MC_CONST_LQL_THRESHOLD<<5;
#endif /* RPL_DAG_MC_CONST_USE_LQL */
    
#ifdef RPL_DAG_MC_CONST_USE_ETX       
    if( rpl_metrics_create_object (p_mc, RPL_DAG_MC_ETX, objects_created) )
      return 0;
    object_headers[objects_created++]= RPL_DAG_MC_CONST_USE_ETX;
    (*p_p_obj++)->etx->etx= RPL_DAG_MC_CONST_ETX_THRESHOLD;
#endif /* RPL_DAG_MC_CONST_USE_ETX */
    
#ifdef RPL_DAG_MC_CONST_USE_LC
    if( rpl_metrics_create_object (p_mc, RPL_DAG_MC_LC, objects_created) )
      return 0;
    object_headers[objects_created++]= RPL_DAG_MC_CONST_USE_LC;
    (*p_p_obj++)->lc->color_and_counter= (RPL_DAG_MC_CONST_LC_THRESHOLD<<6) | RPL_DAG_MC_LC_I_INCLUDED;
#endif /* RPL_DAG_MC_CONST_USE_LC */
    
    p_mc->metric_and_const_obj= objects_created;
    
#ifdef DEBUG_RPL_METRICS
    if(NUMBER_OF_METRICS_AND_CONST_USED != objects_created)
      printf("RPL: METRICS: NUMBER_OF_CONSTRAINTS_USED is not the same as objects created\n");
#endif /* DEBUG_RPL_METRICS */
    
    p_p_obj= p_mc->metric_and_const_objects;
    for (i= 0; i< objects_created; i++){
      header= object_headers[i];
      (*p_p_obj)->type= RPL_METRIC_OBJECT_HEADER_TYPE(header);
      (*p_p_obj)->flags= RPL_METRIC_OBJECT_HEADER_FLAGS(header);
      (*p_p_obj++)->length= RPL_METRIC_OBJECT_HEADER_LENGTH(header);
    }
  }
return 1;

}


/**************************************************************************************************/


uint8_t 
rpl_metrics_copy_mc (rpl_metric_container_t *p_mc_dest, rpl_metric_container_t *p_mc_orig)
{
  uint8_t i;
  uint8_t ok_ret_value;
  uint8_t num_dest_objects, num_orig_objects;
  rpl_metric_object_t *p_obj_dest, *p_obj_orig;

  num_dest_objects= p_mc_dest->metric_and_const_obj;
  num_orig_objects= p_mc_orig->metric_and_const_obj;
  ok_ret_value = 1;
  if(num_dest_objects < num_orig_objects){ 
    for (i= num_dest_objects; i< num_orig_objects; i++){
      if( rpl_metrics_create_object(p_mc_dest,p_mc_orig->metric_and_const_objects[i]->type, i)){
	PRINTF("RPL: METRICS: Error creating an object\n");
	num_orig_objects= i;
	ok_ret_value= 0;
      }
      else{
	p_mc_dest->metric_and_const_obj++;
	p_mc_dest->metric_and_const_objects[i]->length =  p_mc_orig->metric_and_const_objects[i]->length;
      }
    }
  }
  else if(num_dest_objects > num_orig_objects){
    for(i= num_orig_objects; i< num_dest_objects; i++){
      free( p_mc_dest->metric_and_const_objects[i]->rpl_metric_object_data_pointer);
      free( p_mc_dest->metric_and_const_objects[i]);
	p_mc_dest->metric_and_const_obj--;
    }
  }

  for (i=0; i< num_orig_objects; i++){
    p_obj_dest= p_mc_dest->metric_and_const_objects[i];
    p_obj_orig= p_mc_orig->metric_and_const_objects[i];
    if(p_obj_dest->length != p_obj_orig->length ){
      free(p_obj_dest->rpl_metric_object_data_pointer);
      if( NULL == (p_obj_dest->rpl_metric_object_data_pointer= malloc (sizeof(p_obj_orig->length * sizeof(uint8_t))))){
	PRINTF("RPL: METRICS: Error allocating memory for a metric object\n");
	return 0;
      }
    }
    p_obj_dest->type = p_obj_orig->type;
    p_obj_dest->flags = p_obj_orig->flags;
    p_obj_dest->length = p_obj_orig->length;
    memcpy(p_obj_dest->rpl_metric_object_data_pointer,p_obj_orig->rpl_metric_object_data_pointer, p_obj_dest->length * sizeof(uint8_t));
  }
  p_mc_dest->metric_and_const_obj= num_orig_objects;

  return ok_ret_value;
}


/*************************************************************************************************/


uint8_t 
rpl_metrics_write_object_to_metric_container (rpl_metric_container_t *mc, uint8_t *buffer, uint8_t obj_cont){
  
  rpl_metric_object_t* p_obj= NULL;
  uint8_t error_code;
  uint8_t type;
  uint8_t number_objects;
  uint8_t i;
  uint16_t temp16;
  uint32_t temp32;
  
  number_objects= mc->metric_and_const_obj;
  if ( (obj_cont >= NUMBER_OF_METRICS_AND_CONST_USED) || (obj_cont > number_objects)){
    PRINTF("RPL: METRICS: DIO has too many metric objects or not memory enough\n");
    return 0;
  }

  i=0;
  type= buffer[i++];

  if ( ((obj_cont < number_objects)&&(mc->metric_and_const_objects[obj_cont]->type != type)) || (obj_cont == number_objects) ){
    if(obj_cont != number_objects)
      free(mc->metric_and_const_objects[obj_cont]);
    error_code= rpl_metrics_create_object (mc, type, obj_cont);
    if (error_code==1){
      PRINTF("RPL: METRICS: Unable to allocate mem for an object\n");
      return 0;
    }
    if (error_code==2){
      PRINTF("RPL: METRICS: Unknown metric object\n");
      return 0;
    }
    if (obj_cont == number_objects)
      (mc->metric_and_const_obj)++;
  }
  
  p_obj= mc->metric_and_const_objects[obj_cont];  
  p_obj->type= type;
  temp16= buffer[i++];
  temp16= (temp16<<8)+buffer[i++];
  p_obj->flags= temp16;
  p_obj->length= buffer[i++];

  switch (type){
#if defined (RPL_DAG_MC_USE_NSA) || defined (RPL_DAG_MC_CONST_USE_NSA)
  case RPL_DAG_MC_NSA:
    i++; //reserved flags
    p_obj->nsa->flags= buffer[i++];
    PRINTF("RPL: METRICS: DIO had an NSA object\n");
    break;
#endif /* defined (RPL_DAG_MC_USE_NSA) || defined (RPL_DAG_MC_CONST_USE_NSA) */
#if defined(RPL_DAG_MC_USE_ENERGY) || defined(RPL_DAG_MC_CONST_USE_ENERGY)
  case RPL_DAG_MC_ENERGY:
    p_obj->energy->flags= buffer[i++];
    p_obj->energy->energy_est= buffer[i++];
    PRINTF("RPL: METRICS: DIO had an energy object with flags:0x%02x, value:%u\n",p_obj->energy->flags, p_obj->energy->energy_est);
    break;    
#endif /* defined(RPL_DAG_MC_USE_ENERGY) || defined(RPL_DAG_MC_CONST_USE_ENERGY) */
#if defined(RPL_DAG_MC_USE_HOPCOUNT) || defined(RPL_DAG_MC_CONST_USE_HOPCOUNT)
  case RPL_DAG_MC_HOPCOUNT:
    i++; //reserved flags
    p_obj->hop_count->hop_count= buffer[i++];
    PRINTF("RPL: METRICS: DIO had a hop-count object with value:%u\n",buffer[i-1]);
    break;  
#endif /* defined(RPL_DAG_MC_USE_HOPCOUNT) || defined(RPL_DAG_MC_CONST_USE_HOPCOUNT) */
#if defined(RPL_DAG_MC_USE_THROUGHPUT) || defined(RPL_DAG_MC_CONST_USE_THROUGHPUT)
  case RPL_DAG_MC_THROUGHPUT:
    temp32= buffer[i++];
    temp32= (temp32<<8)+buffer[i++];
    temp32= (temp32<<8)+buffer[i++];
    temp32= (temp32<<8)+buffer[i++];
    p_obj->throughput->throughput= temp32;
    PRINTF("RPL: METRICS: DIO had a throughput object with value:%lu\n",p_obj->throughput->throughput);
    break;
#endif /* defined(RPL_DAG_MC_USE_THROUGHPUT) || defined(RPL_DAG_MC_CONST_USE_THROUGHPUT) */
#if defined(RPL_DAG_MC_USE_LATENCY) || defined(RPL_DAG_MC_CONST_USE_LATENCY)
  case RPL_DAG_MC_LATENCY:
    temp32= buffer[i++];
    temp32= (temp32<<8)+buffer[i++];
    temp32= (temp32<<8)+buffer[i++];
    temp32= (temp32<<8)+buffer[i++];
    p_obj->latency->latency= temp32;
    PRINTF("RPL: METRICS: DIO had a latency object with value:%lu\n",p_obj->latency->latency);
    break;   
#endif /* defined(RPL_DAG_MC_USE_LATENCY) || defined(RPL_DAG_MC_CONST_USE_LATENCY) */
#if defined(RPL_DAG_MC_USE_LQL) || defined(RPL_DAG_MC_CONST_USE_LQL)
  case RPL_DAG_MC_LQL:
    i++; //reserved flags
    p_obj->lql->value_and_counter= buffer[i++];
    PRINTF("RPL: METRICS: DIO had an LQL object\n");
    break;	      
#endif /* defined(RPL_DAG_MC_USE_LQL) || defined(RPL_DAG_MC_CONST_USE_LQL) */
#if defined(RPL_DAG_MC_USE_ETX) || defined(RPL_DAG_MC_CONST_USE_ETX)
  case RPL_DAG_MC_ETX:
    temp16= buffer[i++];
    temp16= (temp16<<8)+buffer[i++];
    p_obj->etx->etx= temp16;
    PRINTF("RPL: METRICS: DIO had an ETX object with value:%u\n",p_obj->etx->etx);
    break;	      
#endif /* defined(RPL_DAG_MC_USE_ETX) || defined(RPL_DAG_MC_CONST_USE_ETX) */
#if defined(RPL_DAG_MC_USE_LC) || defined(RPL_DAG_MC_CONST_USE_LC)
  case RPL_DAG_MC_LC:
    i++; //reserved flags
    temp16= buffer[i++];
    temp16= (temp16<<8)+buffer[i++];
    p_obj->lc->color_and_counter= temp16;
    PRINTF("RPL: METRICS: DIO had an LC object\n");
    break;	      
#endif /* defined(RPL_DAG_MC_USE_LC) || defined(RPL_DAG_MC_CONST_USE_LC) */
  }
  return i; 

}


/********************************************************************************************/


uint8_t 
rpl_metrics_read_from_metric_container (rpl_metric_object_t *obj, uint8_t *buffer)
{

  uint8_t type, length;
  uint16_t temp16;
  uint32_t temp32;

  /* metric container object header */
  type= obj->type;
  length= obj->length;

  *buffer++ = type;
  *buffer++ = obj->flags >> 8;
  *buffer++ = obj->flags & 0xFF;
  *buffer++ = length;

  switch (type){
#if defined(RPL_DAG_MC_USE_NSA) || defined(RPL_DAG_MC_CONST_USE_NSA) 
    case RPL_DAG_MC_NSA:
      *buffer++= 0; //reserved flags
      *buffer= obj->nsa->flags;
      PRINTF("RPL: METRICS: add in the DIO an NSA object with flags:0x%04x\n",obj->nsa->flags);
    break;
#endif /*defined(RPL_DAG_MC_USE_NSA) || defined(RPL_DAG_MC_CONST_USE_NSA)  */
#if defined(RPL_DAG_MC_USE_ENERGY) || defined(RPL_DAG_MC_CONST_USE_ENERGY) 
    case RPL_DAG_MC_ENERGY:
      *buffer++= obj->energy->flags;
      *buffer= obj->energy->energy_est;
      PRINTF("RPL: METRICS: add in the DIO an energy object with value:%u\n",obj->energy->energy_est);
    break;    
#endif /*defined(RPL_DAG_MC_USE_ENERGY) || defined(RPL_DAG_MC_CONST_USE_ENERGY)  */
#if defined(RPL_DAG_MC_USE_HOPCOUNT) || defined(RPL_DAG_MC_CONST_USE_HOPCOUNT) 
    case RPL_DAG_MC_HOPCOUNT:
      *buffer++= 0; //reserved flags
      *buffer= obj->hop_count->hop_count;
      PRINTF("RPL: METRICS: add in the DIO a hop-count object with value:%u\n",obj->hop_count->hop_count);
    break;  
#endif /*defined(RPL_DAG_MC_USE_HOPCOUNT) || defined(RPL_DAG_MC_CONST_USE_HOPCOUNT)  */
#if defined(RPL_DAG_MC_USE_THROUGHPUT) || defined(RPL_DAG_MC_CONST_USE_THROUGHPUT) 
    case RPL_DAG_MC_THROUGHPUT:
      temp32= obj->throughput->throughput;
      *buffer++= READ_FIELD(temp32, 24, 0xFF000000);
      *buffer++= READ_FIELD(temp32, 16, 0x00FF0000);
      *buffer++= READ_FIELD(temp32, 8, 0x0000FF00);
      *buffer= READ_FIELD(temp32, 0, 0x000000FF);
      PRINTF("RPL: METRICS: add in the DIO a throughput object with value:%lu\n",temp32);
    break;
#endif /*defined(RPL_DAG_MC_USE_THROUGHPUT) || defined(RPL_DAG_MC_CONST_USE_THROUGHPUT)  */
#if defined(RPL_DAG_MC_USE_LATENCY) || defined(RPL_DAG_MC_CONST_USE_LATENCY) 
    case RPL_DAG_MC_LATENCY:
      temp32= obj->latency->latency;
      *buffer++= READ_FIELD(temp32, 24, 0xFF000000);
      *buffer++= READ_FIELD(temp32, 16, 0x00FF0000);
      *buffer++= READ_FIELD(temp32, 8, 0x0000FF00);
      *buffer= READ_FIELD(temp32, 0, 0x000000FF);
      PRINTF("RPL: METRICS: add in the DIO a latency object with value:%lu\n",temp32);
    break;   
#endif /*defined(RPL_DAG_MC_USE_LATENCY) || defined(RPL_DAG_MC_CONST_USE_LATENCY)  */
#if defined(RPL_DAG_MC_USE_LQL) || defined(RPL_DAG_MC_CONST_USE_LQL) 
    case RPL_DAG_MC_LQL:
      *buffer++= 0; //reserved flags
      *buffer= obj->lql->value_and_counter;
      PRINTF("RPL: METRICS: add in the DIO an LQL object\n");
    break;	      
#endif /*defined(RPL_DAG_MC_USE_LQL) || defined(RPL_DAG_MC_CONST_USE_LQL)  */
#if defined(RPL_DAG_MC_USE_ETX) || defined(RPL_DAG_MC_CONST_USE_ETX) 
    case RPL_DAG_MC_ETX:
      temp16= obj->etx->etx;
      *buffer++= READ_FIELD(temp16, 8, 0xFF00);
      *buffer= READ_FIELD(temp16, 0, 0x00FF);
      PRINTF("RPL: METRICS: add in the DIO an ETX object with value:%u\n",temp16);
    break;	      
#endif /*defined(RPL_DAG_MC_USE_ETX) || defined(RPL_DAG_MC_CONST_USE_ETX)  */
#if defined(RPL_DAG_MC_USE_LC) || defined(RPL_DAG_MC_CONST_USE_LC) 
    case RPL_DAG_MC_LC:
      *buffer++= 0; //reserved flags
      temp16= obj->lc->color_and_counter;
      *buffer++= READ_FIELD(temp16, 8, 0xFF00);
      *buffer= READ_FIELD(temp16, 0, 0x00FF);
      PRINTF("RPL: METRICS: add in the DIO an LC object\n");
    break;	      
#endif /*defined(RPL_DAG_MC_USE_LC) || defined(RPL_DAG_MC_CONST_USE_LC)  */
  }

return (length+4); // length object body + header (bytes)
}


/********************************************************************************/


uint8_t 
rpl_metrics_array_of_metrics (rpl_metric_container_t *p_mc, rpl_metric_element_t *array_of_metrics)
{

  uint8_t type;
  uint16_t flags;
  uint8_t agg_mode;
  uint8_t number_of_elem= 0;
  rpl_metric_object_t *p_obj;
  rpl_metric_element_t temp_elem;
  uint8_t precedence[NUMBER_OF_METRICS_AND_CONST_USED];
  uint8_t i, j, imin;

    for(i=0; i< NUMBER_OF_METRICS_AND_CONST_USED; i++){
      p_obj= p_mc->metric_and_const_objects[i];
      flags= p_obj->flags;

      /* if not a constraint */
      if(  !READ_BIT(flags, RPL_DAG_MC_POS_C_BIT)){
	type= p_obj->type;
	agg_mode= RPL_METRIC_READ_A_FIELD(flags);
	precedence[number_of_elem]= RPL_METRIC_READ_PREC_FIELD(flags);

	array_of_metrics[number_of_elem].type= type;
	array_of_metrics[number_of_elem].aggregation_mode= agg_mode;

	switch (type){
#ifdef RPL_DAG_MC_USE_NSA
	case RPL_DAG_MC_NSA:
	  //array_of_metrics[number_of_elem++].data= p_obj->nsa->flags;
	  break;
#endif /* RPL_DAG_MC_USE_NSA */
#ifdef RPL_DAG_MC_USE_ENERGY
	case RPL_DAG_MC_ENERGY:
	  array_of_metrics[number_of_elem++].data=  p_obj->energy->energy_est;
	  break;    
#endif /* RPL_DAG_MC_USE_ENERGY */
#ifdef RPL_DAG_MC_USE_HOPCOUNT
	case RPL_DAG_MC_HOPCOUNT:
	  array_of_metrics[number_of_elem++].data=  (p_obj->hop_count->hop_count) * RPL_DAG_MC_HOPCOUNT_MULTIPLIER;
	  break;  
#endif /* RPL_DAG_MC_USE_HOPCOUNT */
#ifdef RPL_DAG_MC_USE_THROUGHPUT
	case RPL_DAG_MC_THROUGHPUT:
	  array_of_metrics[number_of_elem++].data= (p_obj->throughput->throughput)>>RPL_DAG_MC_THROUGHPUT_DIVISOR;
	  break;
#endif /* RPL_DAG_MC_USE_THROUGHPUT */
#ifdef RPL_DAG_MC_USE_LATENCY
	case RPL_DAG_MC_LATENCY:
	  array_of_metrics[number_of_elem++].data= (p_obj->latency->latency)>>RPL_DAG_MC_LATENCY_DIVISOR;
	  break;   
#endif /* RPL_DAG_MC_USE_LATENCY */
#ifdef RPL_DAG_MC_USE_LQL
	case RPL_DAG_MC_LQL:
	  //array_of_metrics[number_of_elem++].data= p_obj->lql->value_and_counter;
	  break;	      
#endif /* RPL_DAG_MC_USE_LQL */
#ifdef RPL_DAG_MC_USE_ETX
	case RPL_DAG_MC_ETX:
	  array_of_metrics[number_of_elem++].data= p_obj->etx->etx;
	  break;	      
#endif /* RPL_DAG_MC_USE_ETX */
#ifdef RPL_DAG_MC_USE_LC
	case RPL_DAG_MC_LC:
	  //array_of_metrics[number_of_elem++].data= p_obj->lc->color_and_counter;
	  break;	      
#endif /* RPL_DAG_MC_USE_LC */
	}
      }
    }

    /* sort by precedence */
    for (i=0; i< number_of_elem-1; i++){
      imin= i;
      for(j=i+1; j< number_of_elem; j++){
	if(precedence[j] < precedence[imin])
	  imin= j;
      }
      temp_elem= array_of_metrics[i];
      array_of_metrics[i]=  array_of_metrics[imin];
      array_of_metrics[imin]=  temp_elem;
    }

    return number_of_elem;
}


/**********************************************************************************************/


uint32_t rpl_metrics_aggregated(uint8_t metric_comb_type, uint32_t parent_metric, uint32_t new_metric)
{

  switch (metric_comb_type){
  case RPL_DAG_MC_AGGR_ADDITIVE:
    return (parent_metric + new_metric);
    break;

  case RPL_DAG_MC_AGGR_MAXIMUM:
    return (parent_metric > new_metric ? parent_metric : new_metric);
    break;

  case RPL_DAG_MC_AGGR_MINIMUM:
    return (parent_metric < new_metric ? parent_metric : new_metric);
    break;

  case RPL_DAG_MC_AGGR_MULTIPLICATIVE:
    return (parent_metric * new_metric);
    break;

  default:
    PRINTF("RPL: METRICS: Error aggregation type: %u\n", metric_comb_type);
    return 0;
    break;
  }
}


/**********************************************************************************************/


void
rpl_metrics_update_metric_container(rpl_instance_t *instance)
{

  rpl_metric_container_t *p_parent_container, *p_instance_container;
  rpl_metric_object_t *p_obj_parent, *p_obj_instance;
  uint8_t i, num_constraints,constraints[5];
  uint8_t type, metric_comb_type;
  uint16_t flags;
  uint32_t new_metric, parent_metric;
  uint8_t copy_const= 0;


  if( instance->current_dag->rank == ROOT_RANK(instance) )
    rpl_metrics_create_root_container(&(instance->mc));
  else{
    p_instance_container= &(instance->mc);
    p_parent_container= &(instance->current_dag->preferred_parent->mc);
    num_constraints=0;

    for(i=0; i< p_parent_container->metric_and_const_obj; i++){
      p_obj_instance= p_instance_container->metric_and_const_objects[i];
      p_obj_parent= p_parent_container->metric_and_const_objects[i];
      
      flags= p_obj_parent->flags;

      copy_const= 0;
      /* constraint */

      if( READ_BIT(flags, RPL_DAG_MC_POS_C_BIT) ) {
	if( !READ_BIT(flags, RPL_DAG_MC_POS_O_BIT)){
	  constraints[num_constraints]= i;
	  num_constraints++;
	}
	copy_const=1;
      }

      metric_comb_type= RPL_METRIC_READ_A_FIELD(flags);
      type= p_obj_parent->type;

      switch (type){
#if defined( RPL_DAG_MC_USE_NSA) || defined( RPL_DAG_MC_CONST_USE_NSA)
      case RPL_DAG_MC_NSA:
	if(copy_const)
	  p_obj_instance->nsa->flags= p_obj_parent->nsa->flags;
	else
	  p_obj_instance->nsa->flags= rpl_metrics_get_NSA();
	break;
#endif /*defined( RPL_DAG_MC_USE_NSA) || defined( RPL_DAG_MC_CONST_USE_NSA)*/
	
#if defined( RPL_DAG_MC_USE_ENERGY) || defined( RPL_DAG_MC_CONST_USE_ENERGY)
      case RPL_DAG_MC_ENERGY:
	p_obj_instance->energy->flags= p_obj_parent->energy->flags;
	if(copy_const)
	  p_obj_instance->energy->energy_est= p_obj_parent->energy->energy_est;
	else{
	  new_metric= rpl_metrics_get_energy();
	  parent_metric= (uint32_t)p_obj_parent->energy->energy_est;
	  new_metric= rpl_metrics_aggregated(metric_comb_type, parent_metric, new_metric);
	  p_obj_instance->energy->energy_est= (new_metric>255?255:new_metric);
	}
	break;    
#endif /*defined( RPL_DAG_MC_USE_ENERGY) || defined( RPL_DAG_MC_CONST_USE_ENERGY)*/
	
#if defined( RPL_DAG_MC_USE_HOPCOUNT) || defined( RPL_DAG_MC_CONST_USE_HOPCOUNT)
      case RPL_DAG_MC_HOPCOUNT:
	p_obj_instance->hop_count->hop_count= p_obj_parent->hop_count->hop_count;
	if(!copy_const)
	  (p_obj_instance->hop_count->hop_count)++;
	break;  
#endif /*defined( RPL_DAG_MC_USE_HOPCOUNT) || defined( RPL_DAG_MC_CONST_USE_HOPCOUNT)*/
	
#if defined( RPL_DAG_MC_USE_THROUGHPUT) || defined( RPL_DAG_MC_CONST_USE_THROUGHPUT)
      case RPL_DAG_MC_THROUGHPUT:
	if(copy_const)
	  new_metric= p_obj_parent->throughput->throughput;
	else{
	  new_metric= rpl_metrics_get_throughput();
	  parent_metric= p_obj_parent->throughput->throughput;
	  new_metric= rpl_metrics_aggregated(metric_comb_type, parent_metric, new_metric);
	}
	p_obj_instance->throughput->throughput= new_metric;
	break;
#endif /*defined( RPL_DAG_MC_USE_THROUGHPUT) || defined( RPL_DAG_MC_CONST_USE_THROUGHPUT)*/2
#if defined( RPL_DAG_MC_USE_LATENCY) || defined( RPL_DAG_MC_CONST_USE_LATENCY)
      case RPL_DAG_MC_LATENCY:
	if(copy_const)
	  new_metric= p_obj_parent->latency->latency;
	else{
	  new_metric= rpl_metrics_get_latency();
	  parent_metric= p_obj_parent->latency->latency;
	  new_metric= rpl_metrics_aggregated(metric_comb_type, parent_metric, new_metric);
	}
	p_obj_instance->latency->latency= new_metric;
	break;   
#endif /*defined( RPL_DAG_MC_USE_LATENCY) || defined( RPL_DAG_MC_CONST_USE_LATENCY)*/
	
#if defined( RPL_DAG_MC_USE_LQL) || defined( RPL_DAG_MC_CONST_USE_LQL)
      case RPL_DAG_MC_LQL:
	if(copy_const)
	  p_obj_instance->lql->value_and_counter=p_obj_parent->lql->value_and_counter;
	else
	  p_obj_instance->lql->value_and_counter= rpl_metrics_get_LQL();
	break;	      
#endif /*defined( RPL_DAG_MC_USE_LQL) || defined( RPL_DAG_MC_CONST_USE_LQL)*/
	
#if defined( RPL_DAG_MC_USE_ETX) || defined( RPL_DAG_MC_CONST_USE_ETX)
      case RPL_DAG_MC_ETX:
	if(copy_const)
	  new_metric= p_obj_parent->etx->etx;
	else{
	  new_metric= rpl_metrics_get_ETX();
	  parent_metric= p_obj_parent->etx->etx;
	  new_metric= rpl_metrics_aggregated(metric_comb_type, parent_metric, new_metric);
	}
	p_obj_instance->etx->etx= new_metric;
	break;	      
#endif /*defined( RPL_DAG_MC_USE_ETX) || defined( RPL_DAG_MC_CONST_USE_ETX)*/
	
#if defined( RPL_DAG_MC_USE_LC) || defined( RPL_DAG_MC_CONST_USE_LC)
      case RPL_DAG_MC_LC:
	if(copy_const)
	  p_obj_instance->lc->color_and_counter=p_obj_parent->lc->color_and_counter;
	else
	  p_obj_instance->lc->color_and_counter= rpl_metrics_get_LC();
	break;	      
#endif /*defined( RPL_DAG_MC_USE_LC) || defined( RPL_DAG_MC_CONST_USE_LC)*/
      }
    }

    /*check mandatory constraints*/
    for(i=0; i<num_constraints; i++){
      p_obj_instance= p_instance_container->metric_and_const_objects[constraints[i]];
      flags= p_obj_instance->flags;

      if(!rpl_metrics_satisfies_constraint(p_obj_instance, p_instance_container)){
	PRINTF("RPL: METRICS: does not satisfy a mandatory constraint and become a leaf node\n");
	if(!rpl_leaf){
	  rpl_leaf= 1;
	  leaf_dio= 1;
	}
	return;
      }
    }
  }
}


/************************************************************************************************/


uint8_t
rpl_metrics_satisfies_constraint(rpl_metric_object_t *p_obj_const, rpl_metric_container_t *p_instance_container)
{
  uint8_t return_value;
  uint8_t i, type, metric_found, num_objects;
  uint8_t temp_8;
  uint16_t temp_16_const,temp_16_inst, flags;
  uint8_t energy_type,bit_I;
  rpl_metric_object_t *p_obj_inst;

  metric_found= 0;
  type= p_obj_const->type;
  num_objects= p_instance_container->metric_and_const_obj;

  for(i=0; (i<num_objects) && (!metric_found); i++){
    p_obj_inst= p_instance_container->metric_and_const_objects[i];
    if(p_obj_inst->type == type){
      flags= p_obj_inst->flags;
      if( !READ_BIT(flags, RPL_DAG_MC_POS_C_BIT) ) 
	metric_found++;
    }
  }

  return_value= 1;
  if(metric_found){
    switch (type){
      
#ifdef RPL_DAG_MC_CONST_USE_NSA
    case RPL_DAG_MC_NSA:
      if( ! ((p_obj_const->nsa->flags) & RPL_DAG_MC_NSA_OVERLOAD_BIT))
	if( (p_obj_inst->nsa->flags) & RPL_DAG_MC_NSA_OVERLOAD_BIT)
	  return_value= 0;

      if((p_obj_const->nsa->flags) & RPL_DAG_MC_NSA_AGGREGATION_BIT)
	if( ! ((p_obj_inst->nsa->flags) & RPL_DAG_MC_NSA_AGGREGATION_BIT))
	  return_value= 0;
      break;
#endif /* RPL_DAG_MC_CONST_USE_NSA */
#ifdef RPL_DAG_MC_CONST_USE_ENERGY
    case RPL_DAG_MC_ENERGY:
      
      bit_I= ((p_obj_const->energy->flags) >> 3) & 0x1;
      energy_type= ((p_obj_const->energy->flags) >> 1) & 0x3;
      
      if(bit_I){
	if(energy_type< RPL_DAG_MC_ENERGY_TYPE_MYSELF)
	  return_value = 0;
      }
      else{
	if(energy_type<= RPL_DAG_MC_ENERGY_TYPE_MYSELF)
	  return_value = 0;
      }
      if(p_obj_inst->energy->energy_est > p_obj_const->energy->energy_est)
	return_value = 0;
      break;
#endif /* RPL_DAG_MC_CONST_USE_ENERGY */
#ifdef RPL_DAG_MC_CONST_USE_HOPCOUNT
    case RPL_DAG_MC_HOPCOUNT:
      if(p_obj_const->hop_count->hop_count < p_obj_inst->hop_count->hop_count)
	return_value= 0;
      break;
#endif /* RPL_DAG_MC_CONST_USE_HOPCOUNT */
#ifdef RPL_DAG_MC_CONST_USE_THROUGHPUT
    case RPL_DAG_MC_THROUGHPUT:
      if(p_obj_const->throughput->throughput > p_obj_inst->throughput->throughput)
	return_value= 0;
      break;
#endif /* RPL_DAG_MC_CONST_USE_THROUGHPUT */
#ifdef RPL_DAG_MC_CONST_USE_LATENCY
    case RPL_DAG_MC_LATENCY:
      if(p_obj_const->latency->latency < p_obj_inst->latency->latency)
	return_value= 0;
      break;
#endif /* RPL_DAG_MC_CONST_USE_LATENCY */
#ifdef RPL_DAG_MC_CONST_USE_LQL
    case RPL_DAG_MC_LQL:
      temp_8= (p_obj_const->lql->value_and_counter >> 5) & 0x07;
      if(temp_8 < ((p_obj_inst->lql->value_and_counter >> 5) & 0x07))
	return_value= 0;
      break;
#endif /* RPL_DAG_MC_CONST_USE_LQL */
#ifdef RPL_DAG_MC_CONST_USE_ETX
    case RPL_DAG_MC_ETX:
      if(p_obj_const->etx->etx < p_obj_inst->etx->etx);
	return_value= 0;
      break;
#endif /* RPL_DAG_MC_CONST_USE_ETX */
#ifdef RPL_DAG_MC_CONST_USE_LC
    case RPL_DAG_MC_LC:
      /* If 'I'=1, 1's of constraint and instance must be equal */
      /* If 'I'=0, 0's of constraint and instance must be equal */
      temp_16_const= (p_obj_const->lc->color_and_counter >> 6) & 0x3FF;;
      temp_16_inst= (p_obj_inst->lc->color_and_counter >> 6) & 0x3FF;;
      if( ! (p_obj_const->lc->color_and_counter & 0x1)){
	temp_16_const= (~temp_16_const) & 0x3FF;;
	temp_16_inst=  (~temp_16_inst) & 0x3FF;;
      }
      if(temp_16_const != (temp_16_const & temp_16_inst))
	  return_value= 0;
      break;
#endif /* RPL_DAG_MC_CONST_USE_LC */
    }
  }
  else
    PRINTF("RPL: METRICS: not found a metric for a constraint of type %u\n",type);

  return return_value;
}
