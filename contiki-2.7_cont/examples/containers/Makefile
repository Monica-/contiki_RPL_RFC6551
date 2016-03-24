CONTIKI = ../..


ifndef TARGET
TARGET=z1
endif

#OPTI=0 for debugging
ifndef OPTI
OPTI=0
endif


APPS = powertrace collect-view
CONTIKI_PROJECT = server client
PROJECT_SOURCEFILES += collect-common.c rpl-metrics.c rpl-metrics-get.c rpl-metrics-containers-OF.c


WITH_UIP6=1
UIP_CONF_IPV6=1
UIP_CONF_TCP=0
PERIOD=30
#RPL_CONF_DAG_MC=2 
SMALL=1

#to debug a file add to CFLAGS -Dtag with tag=
#DEBUG_RPL_ICMP6
#DEBUG_RPL_DAG
#DEBUG_RPL_TIMERS
#DEBUG_RPL_METRICS
#DEBUG_RPL_METRICS_GET
#DEBUG_RPL_OF

#use LEDS to show when there is a default parent
#USE_LEDS_DEF_PARENT

#use button to simulate loss of communication from/to a node
#BUTTON_INTERFERENCE

#-gdwarf-4 for debugging
CFLAGS+= #-DDEBUG_RPL_METRICS  -DDEBUG_RPL_OF 
CFLAGS+= -DSTATISTICS -DUIP_CONF_IPV6_RPL  -DUSE_LEDS_DEF_PARENT -DBUTTON_INTERFERENCE  -DPROJECT_CONF_H=\"project-conf.h\"

#ifdef PERIOD
#CFLAGS=-DPERIOD=$(PERIOD)
#endif

all: $(CONTIKI_PROJECT)

include $(CONTIKI)/Makefile.include

