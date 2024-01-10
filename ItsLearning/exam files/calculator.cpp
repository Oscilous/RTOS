#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#define EVER ;; 


int mul_number_1, mul_number_2, mul_result ;
int div_number_1, div_number_2, div_result ;
int add_number_1, add_number_2, add_result ;
int sub_number_1, sub_number_2, sub_result ;

HANDLE mul_operands_ready, mul_result_ready ;
HANDLE div_operands_ready, div_result_ready ;
HANDLE add_operands_ready, add_result_ready ;
HANDLE sub_operands_ready, sub_result_ready ;

void multiplicator( void )
{ 
	for( EVER )
	{
		WaitForSingleObject( mul_operands_ready, INFINITE ) ;
		mul_result = mul_number_1 * mul_number_2 ;
		ResetEvent( mul_operands_ready ) ;
		SetEvent( mul_result_ready ) ;
	}
}

void divider( void )
{ 
	for( EVER )
	{
		WaitForSingleObject( div_operands_ready, INFINITE ) ;
		div_result = div_number_1 / div_number_2 ;
		ResetEvent( div_operands_ready ) ;
		SetEvent( div_result_ready ) ;
	}
}

void adder( void )
{ 
	for( EVER )
	{
		WaitForSingleObject( add_operands_ready, INFINITE ) ;
		add_result = add_number_1 + add_number_2 ;
		ResetEvent( add_operands_ready ) ;
		SetEvent( add_result_ready ) ;
	}
}

void subtractor( void )
{ 
	for( EVER )
	{
		WaitForSingleObject( sub_operands_ready, INFINITE ) ;
		sub_result = sub_number_1 - sub_number_2 ;
		ResetEvent( sub_operands_ready ) ;
		SetEvent( sub_result_ready ) ;
	}
}
void calculator( void )
{ 
	for( EVER )
	{    
		mul_number_1 = 4;
		mul_number_2 = 3;
        SetEvent( mul_operands_ready) ;

		div_number_1 = 8;
		div_number_2 = 2;
        SetEvent( div_operands_ready) ;


		WaitForSingleObject( mul_result_ready, INFINITE ) ;
		ResetEvent( mul_result_ready ) ;

		WaitForSingleObject( div_result_ready, INFINITE ) ;
		ResetEvent( div_result_ready ) ;

		sub_number_1 = mul_result;
		sub_number_2 = 2;
        SetEvent( sub_operands_ready) ;
        WaitForSingleObject( sub_result_ready, INFINITE ) ;
		ResetEvent( sub_result_ready ) ;

        add_number_1 = sub_result;
		add_number_2 = div_result;
		SetEvent( add_operands_ready) ;
        WaitForSingleObject( add_result_ready, INFINITE ) ;
		ResetEvent( add_result_ready ) ;

		printf("%d ",add_result);

        
		
	}
}

void main( void )
{   
	
	HANDLE calc_thread[5];
      
   mul_operands_ready = CreateEvent(NULL,TRUE,FALSE,NULL);
   mul_result_ready   = CreateEvent(NULL,TRUE,FALSE,NULL);

   div_operands_ready = CreateEvent(NULL,TRUE,FALSE,NULL);
   div_result_ready   = CreateEvent(NULL,TRUE,FALSE,NULL);  
  
   add_operands_ready = CreateEvent(NULL,TRUE,FALSE,NULL);
   add_result_ready   = CreateEvent(NULL,TRUE,FALSE,NULL);

   sub_operands_ready = CreateEvent(NULL,TRUE,FALSE,NULL);
   sub_result_ready   = CreateEvent(NULL,TRUE,FALSE,NULL);

   

	
	calc_thread[0] = CreateThread( NULL, 0 , (LPTHREAD_START_ROUTINE)multiplicator, NULL,0, NULL) ;
	calc_thread[1] = CreateThread( NULL, 0 , (LPTHREAD_START_ROUTINE)divider, NULL,0, NULL) ;
    calc_thread[2] = CreateThread( NULL, 0 , (LPTHREAD_START_ROUTINE)adder, NULL,0, NULL) ;
	calc_thread[3] = CreateThread( NULL, 0 , (LPTHREAD_START_ROUTINE)subtractor, NULL,0, NULL) ;
	calc_thread[4] = CreateThread( NULL, 0 , (LPTHREAD_START_ROUTINE)calculator, NULL,0, NULL) ;
	
    WaitForMultipleObjects(5,calc_thread,true,INFINITE);

}
