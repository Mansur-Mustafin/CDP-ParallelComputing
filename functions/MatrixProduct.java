import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;
import java.lang.Math;

public class MatrixProduct {

    public static void printMatrix(double[] mat, int n){
        System.out.println("Result matrix:");
        for (int i = 0; i < 1; i++) {
            for (int j = 0; j < Math.min(10, n); j++) {
                System.out.print(mat[j] + " ");
            }
            System.out.println();
        }
    }

    public static double OnMult(int m_ar, int m_br){
        long Time1, Time2;
        
        double temp, time;
        int i, j, k;

        double[] pha = new double[m_ar * m_ar];
        double[] phb = new double[m_br * m_br];
        double[] phc = new double[m_ar * m_br];

        for(i=0; i<m_ar; i++)
            for(j=0; j<m_ar; j++)
                pha[i*m_ar + j] = (double)1.0;

        for(i=0; i<m_br; i++)
            for(j=0; j<m_br; j++)
                phb[i*m_br + j] = (double)(i+1);
        
        Time1 = System.nanoTime();  // TODO nao temos que calcular numero de clicks?

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

        Time2 = System.nanoTime();

        time = (Time2 - Time1) / 1.0e9;

        printMatrix(phc, m_br);
        
        return time;
    }

    public static double OnMultLine(int m_ar, int m_br){
        long Time1, Time2;
        
        double time;
        int i, j, k;

        double[] pha = new double[m_ar * m_ar];
        double[] phb = new double[m_br * m_br];
        double[] phc = new double[m_ar * m_br];

        for(i=0; i<m_ar; i++)
            for(j=0; j<m_ar; j++)
                pha[i*m_ar + j] = (double)1.0;

        for(i=0; i<m_br; i++)
            for(j=0; j<m_br; j++)
                phb[i*m_br + j] = (double)(i+1);

        Arrays.fill(phc, 0.0);
        
        Time1 = System.nanoTime(); 

        for(i=0; i<m_ar; i++)
        {	for( k=0; k<m_ar; k++)
            {   for( j=0; j<m_br; j++)
                {	
                    phc[i*m_ar+j] += pha[i*m_ar+k] * phb[k*m_br+j];
                }
            }
        }

        Time2 = System.nanoTime();

        time = (Time2 - Time1) / 1.0e9;

        printMatrix(phc, m_br);
        
        return time;
    }

    public static double OnMultBlock(int m_ar, int m_br, int bkSize){
        long Time1, Time2;
        
        double time;
        int i, j, k;
        int iBlock, kBlock, jBlock;

        double[] pha = new double[m_ar * m_ar];
        double[] phb = new double[m_br * m_br];
        double[] phc = new double[m_ar * m_br];

        for(i=0; i<m_ar; i++)
            for(j=0; j<m_ar; j++)
                pha[i*m_ar + j] = (double)1.0;

        for(i=0; i<m_br; i++)
            for(j=0; j<m_br; j++)
                phb[i*m_br + j] = (double)(i+1);

        Arrays.fill(phc, 0.0);
        
        Time1 = System.nanoTime(); 

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

        Time2 = System.nanoTime();

        time = (Time2 - Time1) / 1.0e9;

        printMatrix(phc, m_br);
        
        return time;
    }

    public static void main(String[] args) {
        if (args.length < 3 || args.length > 4) {
            System.out.println("Usage: java MatrixOperations <operation> <dimension> <result_file> [block_size]");
            System.exit(-1);
        }

        try {
            int op = Integer.parseInt(args[0]);
            int lin = Integer.parseInt(args[1]);
            int col = lin;
            int blockSize = (args.length == 4 && op == 3) ? Integer.parseInt(args[3]) : 0;
            double time = 0, gflops = 0;

            BufferedWriter resultFile = new BufferedWriter(new FileWriter(args[2], true));

            switch (op) {
                case 1: time = OnMult(lin, col); break;
                case 2: time = OnMultLine(lin, col); break;
                case 3: time = OnMultBlock(lin, col, blockSize); break;
                default: break;
            }
            
            gflops = (2 * Math.pow(lin, 3) / time) / 1e9;
            resultFile.write(op + "," + lin + "," + blockSize + "," + time + "," + gflops + "\n");
            resultFile.close();
        } catch (NumberFormatException e) {
            System.out.println("Error parsing integer arguments");
        } catch (IOException e) {
            System.out.println("[FAIL] Unable to open file: " + args[2]);
        }
    }
}
