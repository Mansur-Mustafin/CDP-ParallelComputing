#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <cstdlib>
#include <papi.h>
#include <math.h>
#include <chrono>

#ifdef _OPENMP
	#include <omp.h>
#endif


using namespace std;

#define SYSTEMTIME clock_t

void printMatrix(double * mat, int n){
	// display 10 elements of the result matrix tto verify correctness
	cout << "Result matrix: " << endl;
	for(int i = 0; i < 1; i++)
		for(int j = 0; j < min(10, n); j++)
			cout << mat[j] << " ";
	
	cout << endl;
}

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

	printMatrix(phc, m_br);

    free(pha);
    free(phb);
    free(phc);
	return time;
}

double OnMultLine(int m_ar, int m_br)
{
	SYSTEMTIME Time1, Time2;
	
	double time;
	int i, j, k;

	double *pha, *phb, *phc;
		
    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)calloc(m_ar * m_ar, sizeof(double));	// use calloc?

	for(i=0; i<m_ar; i++)
		for(j=0; j<m_ar; j++)
			pha[i*m_ar + j] = (double)1.0;

	for(i=0; i<m_br; i++)
		for(j=0; j<m_br; j++)
			phb[i*m_br + j] = (double)(i+1);

    Time1 = clock();

	for(i=0; i<m_ar; i++)
	{	for( k=0; k<m_ar; k++)
		{   for( j=0; j<m_br; j++)
			{	
				phc[i*m_ar+j] += pha[i*m_ar+k] * phb[k*m_br+j];
			}
		}
	}

    Time2 = clock();

	time = (double)(Time2 - Time1) / CLOCKS_PER_SEC;

	printMatrix(phc, m_br);

    free(pha);
    free(phb);
    free(phc);
	return time;
}

double OnMultBlock(int m_ar, int m_br, int bkSize)
{
	SYSTEMTIME Time1, Time2;
	
	double time;
	int i, j, k;
	int iBlock, kBlock, jBlock;

	double *pha, *phb, *phc;
	
    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)calloc(m_ar * m_ar, sizeof(double));

	for(i=0; i<m_ar; i++)
		for(j=0; j<m_ar; j++)
			pha[i*m_ar + j] = (double)1.0;

	for(i=0; i<m_br; i++)
		for(j=0; j<m_br; j++)
			phb[i*m_br + j] = (double)(i+1);


    Time1 = clock();

	for(iBlock=0; iBlock<m_ar; iBlock+=bkSize)
	{	for( kBlock=0; kBlock<m_ar; kBlock+=bkSize)
		{   for( jBlock=0; jBlock<m_br; jBlock+=bkSize)
			{	
				int iEndBlock = iBlock + bkSize;
				int jEndBlock = jBlock + bkSize;
				int kEndBlock = kBlock + bkSize;

				for(i=iBlock;i<iEndBlock;i++)
				{	for(k=kBlock;k<kEndBlock;k++)
					{	for(j=jBlock;j<jEndBlock;j++)
						{
							phc[i*m_ar+j] += pha[i*m_ar+k] * phb[k*m_ar+j];
						}
					}
				}

			}
		}
	}

    Time2 = clock();

	time = (double)(Time2 - Time1) / CLOCKS_PER_SEC;

	printMatrix(phc, m_br);

    free(pha);
    free(phb);
    free(phc);
	return time;
}

double OnMultLineV1(int m_ar, int m_br, int n_threads)
{
	double time;
	int i, j, k;

	double *pha, *phb, *phc;
	
    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)calloc(m_ar * m_ar, sizeof(double));

	for(i=0; i<m_ar; i++)
		for(j=0; j<m_ar; j++)
			pha[i*m_ar + j] = (double)1.0;

	for(i=0; i<m_br; i++)
		for(j=0; j<m_br; j++)
			phb[i*m_br + j] = (double)(i+1);

    auto Time1 = chrono::high_resolution_clock::now();

	#pragma omp parallel for num_threads(n_threads) private(k, j)
	for(i=0; i<m_ar; i++)
	{	for( k=0; k<m_ar; k++)
		{   for( j=0; j<m_br; j++)
			{	
				phc[i*m_ar+j] += pha[i*m_ar+k] * phb[k*m_br+j];
			}
		}
	}
	
    auto Time2 = chrono::high_resolution_clock::now();

	time = chrono::duration_cast<chrono::microseconds>(Time2 - Time1).count() / 1e6;

	printMatrix(phc, m_br);

    free(pha);
    free(phb);
    free(phc);
	return time;
}

double OnMultLineV2(int m_ar, int m_br, int n_threads)
{
	
	double time;
	int i, j, k;

	double *pha, *phb, *phc;
	
    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)calloc(m_ar * m_ar, sizeof(double));

	for(i=0; i<m_ar; i++)
		for(j=0; j<m_ar; j++)
			pha[i*m_ar + j] = (double)1.0;

	for(i=0; i<m_br; i++)
		for(j=0; j<m_br; j++)
			phb[i*m_br + j] = (double)(i+1);

    auto Time1 = chrono::high_resolution_clock::now();
	
	#pragma omp parallel num_threads(n_threads) private(i, k)
	for(i=0; i<m_ar; i++)
	{	for( k=0; k<m_ar; k++)
		{   
			#pragma omp for
			for( j=0; j<m_br; j++)
			{	
				phc[i*m_ar+j] += pha[i*m_ar+k] * phb[k*m_br+j];
			}
		}
	}

    auto Time2 = chrono::high_resolution_clock::now();

	time = chrono::duration_cast<chrono::microseconds>(Time2 - Time1).count() / 1e6;

	printMatrix(phc, m_br);

    free(pha);
    free(phb);
    free(phc);
	return time;
}

double OnMultBlockParallel(int m_ar, int m_br, int bkSize, int n_threads)
{
	double time;
	int i, j, k;
	int iBlock, kBlock, jBlock;
	int iEndBlock, jEndBlock, kEndBlock;

	double *pha, *phb, *phc;
	
    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)calloc(m_ar * m_ar, sizeof(double));

	for(i=0; i<m_ar; i++)
		for(j=0; j<m_ar; j++)
			pha[i*m_ar + j] = (double)1.0;

	for(i=0; i<m_br; i++)
		for(j=0; j<m_br; j++)
			phb[i*m_br + j] = (double)(i+1);


    auto Time1 = chrono::high_resolution_clock::now();

	#pragma omp parallel for num_threads(n_threads) private(kBlock, jBlock, iEndBlock, jEndBlock, kEndBlock, i, j, k)
	for(iBlock=0; iBlock<m_ar; iBlock+=bkSize)
	{	for( kBlock=0; kBlock<m_ar; kBlock+=bkSize)
		{   for( jBlock=0; jBlock<m_br; jBlock+=bkSize)
			{	
				iEndBlock = iBlock + bkSize;
				jEndBlock = jBlock + bkSize;
				kEndBlock = kBlock + bkSize;

				for(i=iBlock;i<iEndBlock;i++)
				{	for(k=kBlock;k<kEndBlock;k++)
					{	for(j=jBlock;j<jEndBlock;j++)
						{
							phc[i*m_ar+j] += pha[i*m_ar+k] * phb[k*m_ar+j];
						}
					}
				}

			}
		}
	}

    auto Time2 = chrono::high_resolution_clock::now();

	time = chrono::duration_cast<chrono::microseconds>(Time2 - Time1).count() / 1e6;

	printMatrix(phc, m_br);

    free(pha);
    free(phb);
    free(phc);
	return time;
}

void handle_error (int retval){
  printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
  exit(EXIT_FAILURE);
}

void init_papi() {
  int retval = PAPI_library_init(PAPI_VER_CURRENT);
  if (retval != PAPI_VER_CURRENT && retval < 0) {
    printf("PAPI library version mismatch!\n");
    exit(EXIT_FAILURE);
  }
  if (retval < 0) handle_error(retval);

  std::cout << "PAPI Version Number: MAJOR: " << PAPI_VERSION_MAJOR(retval)
            << " MINOR: " << PAPI_VERSION_MINOR(retval)
            << " REVISION: " << PAPI_VERSION_REVISION(retval) << "\n";
}

int main (int argc, char *argv[])
{
	if (argc < 4 || argc > 6) {
        printf("Usage: %s <operation> <dimention> <result_file> [block_size] [n_threads]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

	int op = std::atoi(argv[1]);
	int lin = std::atoi(argv[2]);
	int col = lin;
	int blockSize = (argc == 5 && op == 3) ? std::atoi(argv[4]) : (argc == 6 && op == 6 ? std::atoi(argv[4]) : 0);
	double time, gflops;
	int n_threads = (argc == 5 && (op == 4 || op == 5)) ? std::atoi(argv[4]) : (argc == 6 && op == 6 ? std::atoi(argv[5]) : 1);

	std::ofstream resultFile(argv[3], std::ios::out | std::ios::app);
	if (!resultFile.is_open()) {
        printf("[FAIL] Unable to open file %s\n", argv[3]);
		return EXIT_FAILURE;
    } 
	
	int EventSet = PAPI_NULL;
  	long long values[2];
	
	if ( PAPI_library_init( PAPI_VER_CURRENT ) != PAPI_VER_CURRENT ) std::cout << "FAIL" << endl;
	if ( PAPI_create_eventset(&EventSet) != PAPI_OK) cout << "ERROR: create eventset" << endl;
	if ( PAPI_add_event(EventSet,PAPI_L1_DCM ) != PAPI_OK) cout << "ERROR: PAPI_L1_DCM" << endl;
	if ( PAPI_add_event(EventSet,PAPI_L2_DCM) != PAPI_OK) cout << "ERROR: PAPI_L2_DCM" << endl;


	if ( PAPI_start(EventSet) != PAPI_OK) cout << "ERROR: Start PAPI" << endl;		// Start counting
	switch (op){
		case 1: time = OnMult(lin, col); break;
		case 2: time = OnMultLine(lin, col); break;
		case 3: time = OnMultBlock(lin, col, blockSize); break;
		case 4: time = OnMultLineV1(lin, col, n_threads); break;
		case 5: time = OnMultLineV2(lin, col, n_threads); break;
		case 6: time = OnMultBlockParallel(lin, col, blockSize, n_threads); break;
		default: printf("[ERROR] Invalid operation code: %d\n", op); return EXIT_FAILURE;
	}
	if ( PAPI_stop(EventSet, values) != PAPI_OK) cout << "ERROR: Stop PAPI" << endl;	// stop counting
	if ( PAPI_reset( EventSet ) != PAPI_OK ) std::cout << "FAIL reset" << endl; 

	gflops = (2 * pow(lin, 3) / time) / 1e9;
	resultFile 	<< op << "," 
	       		<< lin << "," 
				<< blockSize << "," 
				<< time << "," 
				<< values[0] << "," 
				<< values[1] << ","
				<< fixed << setprecision(4) << gflops << ","
				<< n_threads << "\n";
	resultFile.close();

	if ( PAPI_remove_event( EventSet, PAPI_L1_DCM ) != PAPI_OK ) std::cout << "FAIL remove event" << endl; 
	if ( PAPI_remove_event( EventSet, PAPI_L2_DCM ) != PAPI_OK ) std::cout << "FAIL remove event" << endl; 
	if ( PAPI_destroy_eventset( &EventSet ) != PAPI_OK ) std::cout << "FAIL destroy" << endl;

	return EXIT_SUCCESS;
}
