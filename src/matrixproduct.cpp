#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <cstdlib>
#include <papi.h>

using namespace std;

#define SYSTEMTIME clock_t

 
double OnMult(int m_ar, int m_br) 
{
	
	SYSTEMTIME Time1, Time2;
	
	double temp, time;
	int i, j, k;

	double *pha, *phb, *phc;
	

		
    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

	for(i=0; i<m_ar; i++)
		for(j=0; j<m_ar; j++)
			pha[i*m_ar + j] = (double)1.0;



	for(i=0; i<m_br; i++)
		for(j=0; j<m_br; j++)
			phb[i*m_br + j] = (double)(i+1);



    Time1 = clock();

	for(i=0; i<m_ar; i++)
	{	for( j=0; j<m_br; j++)
		{	temp = 0;
			for( k=0; k<m_ar; k++)
			{	
				temp += pha[i*m_ar+k] * phb[k*m_br+j];
			}
			phc[i*m_ar+j]=temp;
		}
	}


    Time2 = clock();

	time = (double)(Time2 - Time1) / CLOCKS_PER_SEC;

	// display 10 elements of the result matrix tto verify correctness
	cout << "Result matrix: " << endl;
	for(i=0; i<1; i++)
	{	for(j=0; j<min(10,m_br); j++)
			cout << phc[j] << " ";
	}
	cout << endl;

    free(pha);
    free(phb);
    free(phc);
	return time;
}

// TODO add code here for line x line matriz multiplication
double OnMultLine(int m_ar, int m_br){}

// TODO add code here for block x block matriz multiplication
double OnMultBlock(int m_ar, int m_br, int bkSize){}



void handle_error (int retval)
{
  printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
  exit(1);
}

void init_papi() {
  int retval = PAPI_library_init(PAPI_VER_CURRENT);
  if (retval != PAPI_VER_CURRENT && retval < 0) {
    printf("PAPI library version mismatch!\n");
    exit(1);
  }
  if (retval < 0) handle_error(retval);

  std::cout << "PAPI Version Number: MAJOR: " << PAPI_VERSION_MAJOR(retval)
            << " MINOR: " << PAPI_VERSION_MINOR(retval)
            << " REVISION: " << PAPI_VERSION_REVISION(retval) << "\n";
}


int main (int argc, char *argv[])
{
	
	if (argc < 4) {
        printf("Usage: %s <operation> <dimention> <result_file> <block_size>\n", argv[0]);
        exit(-1);
    }

	// char c;
	double time;
	int lin = atoi(argv[2]);
	int col = lin; 
	int blockSize = argc == 5 ? atoi(argv[4]) : 0;
	int op = atoi(argv[1]);
	ofstream resultFile; 
	resultFile.open(argv[3], ios::out | ios::app);;

	if (resultFile.is_open()) {
        resultFile << op << "," << lin << "," << blockSize << ",";
    } else {
        std::cout << "[FAIL] Unable to open file" << argv[3] << std::endl;
    }
	
	int EventSet = PAPI_NULL;
  	long long values[2];
  	int ret;
	

	ret = PAPI_library_init( PAPI_VER_CURRENT );
	if ( ret != PAPI_VER_CURRENT )
		std::cout << "FAIL" << endl;


	ret = PAPI_create_eventset(&EventSet);
		if (ret != PAPI_OK) cout << "ERROR: create eventset" << endl;


	ret = PAPI_add_event(EventSet,PAPI_L1_DCM );
	if (ret != PAPI_OK) cout << "ERROR: PAPI_L1_DCM" << endl;


	ret = PAPI_add_event(EventSet,PAPI_L2_DCM);
	if (ret != PAPI_OK) cout << "ERROR: PAPI_L2_DCM" << endl;


	// Start counting
	ret = PAPI_start(EventSet);
	if (ret != PAPI_OK) cout << "ERROR: Start PAPI" << endl;

	switch (op){
		case 1: 
			time = OnMult(lin, col);
			break;
		case 2:
			time = OnMultLine(lin, col);  
			break;
		case 3:
			time = OnMultBlock(lin, col, blockSize);  
			break;
		default:
			break;
	}

	// stop counting
	ret = PAPI_stop(EventSet, values);
	if (ret != PAPI_OK) cout << "ERROR: Stop PAPI" << endl;
	resultFile << time << "," << values[0] << "," << values[1] << "\n";


	ret = PAPI_reset( EventSet );
	if ( ret != PAPI_OK )
		std::cout << "FAIL reset" << endl; 

	resultFile.close();


	ret = PAPI_remove_event( EventSet, PAPI_L1_DCM );
	if ( ret != PAPI_OK )
		std::cout << "FAIL remove event" << endl; 

	ret = PAPI_remove_event( EventSet, PAPI_L2_DCM );
	if ( ret != PAPI_OK )
		std::cout << "FAIL remove event" << endl; 

	ret = PAPI_destroy_eventset( &EventSet );
	if ( ret != PAPI_OK )
		std::cout << "FAIL destroy" << endl;

}