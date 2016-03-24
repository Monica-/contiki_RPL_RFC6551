#awk -f script-data.awk COOJA.testlog


BEGIN { 
    minute= 0;
    rec_pack_min= 0;
    parent_change_min= 0;
    sent_pack_min= 0;
    DIS_min= 0;
    DIO_min= 0;
    DAO_min=0;
    rec_pack= 0;
    parent_change= 0;
    batt_25= 0;
    batt_50= 0;
    batt_75= 0;
    batt_100= 0;
    sent_pack= 0;
    DIS_tot= 0;
    DIO_tot= 0;
    DAO_tot=0;
    latency=0;
    hops=0;
}
{
    if( NR == 1 )
    {
	print "minute\tsent\tsent_m\trec\trec_m\tp_ch\tp_ch_m\tDIS\tDIS_m",
	    "\tDIO\tDIO_m\tDAO\tDAO_m\tbat_25\tbat_50\tbat_75",
	    "\tbat_100\tlat\thops" > "results.txt"
	next;
    }
    if( minute != $2 )
    {
	rec_pack+= rec_pack_min;
	parent_change+= parent_change_min; 
	sent_pack+= sent_pack_min; 
	DIS_tot+= DIS_min;
	DIO_tot+= DIO_min;
	DAO_tot+= DAO_min;
	if(rec_pack_min)
	{
	    latency/= rec_pack_min;
	    hops/= rec_pack_min;
	}

	print minute,"\t",sent_pack,"\t",sent_pack_min,"\t",rec_pack,"\t",rec_pack_min\
	    ,"\t",parent_change,"\t",parent_change_min,"\t",DIS_tot,"\t",DIS_min,"\t",\
	    DIO_tot,"\t",DIO_min,"\t",DAO_tot,"\t",DAO_min,"\t",batt_25,"\t",batt_50 \
	    ,"\t",batt_75,"\t",batt_100,"\t",latency,"\t",hops > "results.txt"
	
	rec_pack_min= 0;  parent_change_min= 0;
	sent_pack_min= 0; DIS_min= 0;
	DIO_min= 0;       DAO_min= 0;
	latency= 0;       hops= 0;

	minute= $2;
    }

    if( $4== 0 ) #packet received
    {
	rec_pack_min ++;
	latency+= ($1-time_sent[$7]);
	hops+= $9;
	next;
    }
    if( $4 == "Parent" )
    {
	parent_change_min++;
	next;
    }
    if( $4 == "Battery" )
    {
	if( $6 == "25%" )
	{
	    batt_25 ++;
	    next;
	}
	if( $6 == "50%" )
	{
	    batt_25 --;
	    batt_50 ++;
	    next;
	}
	if( $6 == "75%" )
	{
	    batt_50 --;
	    batt_75 ++;
	    next;
	}
	if( $6 == "100%" )
	{
	    batt_75 --;
	    batt_100 ++;
	    next;
	}
    }
    if( $4 == "Packet" ) 
    {
	sent_pack_min++;
	split($3, node, "#");
	time_sent[node[2]]= $1;
	next;
    }
    if( $5 == "DIS:" ) 
    {
	DIS_min++;
	next;
    }
    if( $5 == "DIO:" ) 
    {
	DIO_min++;
	next;
    }
    if( $5 == "DAO:" ) 
    {
	DAO_min++;
	next;
    }

}
END {  print "END" }



