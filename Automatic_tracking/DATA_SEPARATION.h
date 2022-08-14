unsigned char data_sepa[8];			
data_separation(long unsigned int sepa_data);	


data_separation(long unsigned int sepa_data)
{  
	unsigned char i;			
	i=0;
	while(sepa_data/10)      		      
           { 
             data_sepa[i]=sepa_data%10; 	
             sepa_data=sepa_data/10; 		
             i++; 				
           } 
	data_sepa[i]=sepa_data;
	data_sepa[i+1]='\0';
	i++;			
	return i;	
}

DATA_transfer_separation(long unsigned int sepa_data)
{  
	unsigned char i=0;			
	unsigned char j;
    for(j=0;j<8;j++)
	{data_sepa[j]=0;}
	while(sepa_data/10)      		      
           { 
             data_sepa[i]=sepa_data%10+0X30;   	
             sepa_data=sepa_data/10; 		
             i++; 				
           } 
	data_sepa[i]=sepa_data+0X30;	
	if(i!=0)    {data_sepa[i+1]='\0';}
	else        {data_sepa[0]='\0';}	
	return i;	
}

