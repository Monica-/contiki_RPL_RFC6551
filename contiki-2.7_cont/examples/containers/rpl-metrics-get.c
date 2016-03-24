#include "net/rpl/rpl-private.h"
#include "sys/energest.h"
#include "dev/leds.h"


#ifdef DEBUG_RPL_METRICS_GET
#define DEBUG DEBUG_PRINT
#else
#define DEBUG DEBUG_NONE
#endif
#include "net/uip-debug.h"
#include <stdio.h>

static uint16_t max_charge;
uint8_t rpl_metrics_power_refresh();


/********************************************************************/

uint8_t rpl_metrics_get_NSA()
{
  //  if((AM_I_NODE(2))||(AM_I_NODE(3)))
  //    return ((1<<1)+1);
  return ((RPL_DAG_MC_NSA_A_AGGREGATION<<1)+RPL_DAG_MC_NSA_O_OVERLOAD);
}


/********************************************************************/

#if defined(RPL_DAG_MC_USE_ENERGY) || defined(RPL_DAG_MC_CONST_USE_ENERGY)
uint8_t rpl_metrics_get_energy()
{
  uint8_t power;
  static uint8_t initialized= 0;

  if(!initialized){
    initialized++;
    //    if(AM_I_NODE(6))
    //      max_charge= (100L*3600)>>16; 
    //    else
      max_charge= (BATT_CHARGE*3600)>>16; 
  }
power= rpl_metrics_power_refresh();

#ifdef RPL_DAG_MC_ENERGY_DIVISOR
 power>>= RPL_DAG_MC_ENERGY_DIVISOR;
#endif

 return( power==0 ? 1 : power ); 

}
#endif /*defined(RPL_DAG_MC_USE_ENERGY) || defined(RPL_DAG_MC_CONST_USE_ENERGY)*/


/********************************************************************/

uint8_t rpl_metrics_get_hop_count()
{
  rpl_dag_t *dag;
  rpl_metric_container_t *p_container;
  uint8_t i;

  /* Let's suppose we have only one instance */
  dag = rpl_get_any_dag();

  if(dag->preferred_parent != NULL) {
    p_container=&(dag->preferred_parent->mc);
    for(i=0; i< p_container->metric_and_const_obj; i++){
      if(p_container->metric_and_const_objects[i]->type == RPL_DAG_MC_HOPCOUNT)
	return(p_container->metric_and_const_objects[i]->hop_count->hop_count + 1);
    }
  }

  return 0;

}


/********************************************************************/

uint32_t rpl_metrics_get_throughput()
{

  if(AM_I_NODE(3))
    return (150*65536);
  return (100*65536);
}

/********************************************************************/

uint32_t rpl_metrics_get_latency()
{

  if(AM_I_NODE(3))
    return (100*65536);
  return (150*65536);
}

/********************************************************************/

uint8_t rpl_metrics_get_LQL()
{

  //  if((AM_I_NODE(2))||(AM_I_NODE(3)))
  //  return (6 <<5);
  return ((RPL_DAG_MC_LQL_INITIALIZATION & 0x7)<<5);
}


/********************************************************************/

#if defined(RPL_DAG_MC_USE_ETX) || defined(RPL_DAG_MC_CONST_USE_ETX)

uint16_t rpl_metrics_get_ETX()
{
  rpl_dag_t *dag;

  /* Let's suppose we have only one instance */
  dag = rpl_get_any_dag();

  if(dag->preferred_parent != NULL) 
    return (dag->preferred_parent->etx);
  else
    return 0  ;
}
#endif /* defined(RPL_DAG_MC_USE_ETX) || defined(RPL_DAG_MC_CONST_USE_ETX)*/

/********************************************************************/

uint16_t rpl_metrics_get_LC()
{
  //  if((AM_I_NODE(2))||(AM_I_NODE(3)))
  //    return (2 <<6);
  return (RPL_DAG_MC_LC_INITIALIZATION<<6);
}


/********************************************************************/

#if defined(RPL_DAG_MC_USE_ETX) || defined(RPL_DAG_MC_CONST_USE_ETX)

void rpl_metrics_etx_refresh(rpl_parent_t *p, int status, int numtx)
{
  uint16_t recorded_etx = p->etx;
  uint16_t packet_etx = numtx * RPL_DAG_MC_ETX_DIVISOR;
  uint16_t new_etx;

/* When the radio was explicitly turned off, mac returns MAC_TX_ERR_FATAL.
 * To simulate loss of communication with the parent, it is changed for
 * no Ack received
 */ 
#ifdef BUTTON_INTERFERENCE
  if(status== MAC_TX_ERR_FATAL) 
    status= MAC_TX_NOACK;
#endif /*BUTTON_INTERFERENCE*/

  /* Do not penalize the ETX when collisions or transmission errors occur. */
  if(status == MAC_TX_OK || status == MAC_TX_NOACK) {
    if(status == MAC_TX_NOACK) {
      packet_etx = RPL_DAG_MC_ETX_MAX * RPL_DAG_MC_ETX_DIVISOR;
    }

    new_etx = ((uint32_t)recorded_etx * ETX_ALPHA +
               (uint32_t)packet_etx * (ETX_SCALE - ETX_ALPHA)) / ETX_SCALE;

    PRINTF("RPL: METRICS: ETX old: %u,new: %u (packet ETX = %u)\n",
        recorded_etx, new_etx, packet_etx);
    p->etx = new_etx;
  }

#ifdef DEBUG_RPL_METRICS_GET
  if(status == MAC_TX_OK)  //The MAC layer transmission was OK.
    printf("- OK\n");
  else if (status == MAC_TX_NOACK)  //The MAC layer did not get an acknowledgement for the packet.
    printf("- NO ACK\n");
  else 
    printf("- PROBLEM\n");
#endif /* DEBUG_RPL_METRICS_GET */

}
#endif /* defined(RPL_DAG_MC_USE_ETX) || defined(RPL_DAG_MC_CONST_USE_ETX) */



/********************************************************************/

#if defined(RPL_DAG_MC_USE_ENERGY) || defined(RPL_DAG_MC_CONST_USE_ENERGY)

uint8_t rpl_metrics_power_refresh()
{
  uint32_t ticks_cpu, ticks_lpm, ticks_transmit, ticks_listen;
  uint8_t  batt_used;
  uint32_t power32;

  energest_flush();
  
  ticks_lpm = energest_type_time(ENERGEST_TYPE_LPM)>>8;
  ticks_cpu = energest_type_time(ENERGEST_TYPE_CPU)>>8;
  ticks_transmit = energest_type_time(ENERGEST_TYPE_TRANSMIT)>>8;
  ticks_listen = energest_type_time(ENERGEST_TYPE_LISTEN)>>8;

  power32= (ticks_lpm * (uint16_t)CURRENT_LPM) + (ticks_cpu * (uint16_t)CURRENT_CPU) + (ticks_transmit * (uint16_t)CURRENT_TRANSMIT) + (ticks_listen * (uint16_t)CURRENT_LISTEN);

  power32>>=(TICKS_PER_SECOND-8);

  if(power32 >= ((uint32_t)max_charge<<16)){
    batt_used= 0xFF;
#ifdef STATISTICS
    if(!depleted_battery)
      printf("30 Battery used: 100%%\n");
#endif /*STATISTICS*/

    depleted_battery= 1;
    leds_on(1);//BLUE


  }
  else
    batt_used= (uint8_t) ((power32/max_charge)>>8);

#ifdef STATISTICS
  {
    uint8_t perc;
    static uint8_t perc_25=0;
    static uint8_t perc_50=0;
    static uint8_t perc_75=0;

    perc= (batt_used*100)/0xFF;
    if( (perc>= 75) && (!depleted_battery)){
      if(!perc_75){
	perc_75++;
      printf("30 Battery used: 75%%\n");
      }
    }
    else if(perc>= 50){
      if(!perc_50){
	perc_50++;
      printf("30 Battery used: 50%%\n");
      }
    }
    else if(perc>= 25){
      if(!perc_25){
	perc_25++;
      printf("30 Battery used: 25%%\n");
      }
    }
  }
#endif /*STATISTICS*/

  
  PRINTF("RPL:METRICS:ticks_lpm %lu-0x%08lx\n",ticks_lpm<<8,ticks_lpm<<8);
  PRINTF("RPL:METRICS:ticks_cpu %lu-0x%08lx\n",ticks_cpu<<8,ticks_cpu<<8);
  PRINTF("RPL:METRICS:ticks_trans %lu-0x%08lx\n",ticks_transmit<<8,ticks_transmit<<8);
  PRINTF("RPL:METRICS:ticks_rx %lu-0x%08lx\n",ticks_listen<<8,ticks_listen<<8);  

  PRINTF("RPL:METRICS:consumed= %lu-0x%08lx (uAs) total= %lu\n", power32,power32, (uint32_t)max_charge<<16);
  PRINTF("RPL:METRICS:% bat= %u\n",batt_used);


  return batt_used;
}
#endif /*defined(RPL_DAG_MC_USE_ENERGY) || defined(RPL_DAG_MC_CONST_USE_ENERGY)*/
