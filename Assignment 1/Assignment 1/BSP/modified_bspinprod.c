#include "bspedupack.c"
#include <math.h>

/*  This program computes the sum of the first n squares, for n>=0,
        sum = 1*1 + 2*2 + ... + n*n
    by computing the inner product of x=(1,2,...,n)^T and itself.
    The output should equal n*(n+1)*(2n+1)/6.
    The distribution of x is cyclic.
*/

int P; /* number of processors requested */ 
int n; //length of input

int nloc(int p, int s, int n){
    /* Compute number of local components of processor s for vector
       of length n distributed cyclically over p processors. */

    return  (n+p-s-1)/p ; 

} /* end nloc */

double bspip(int p, int s, int n, double *x, double *y){
    /* Compute inner product of vectors x and y of length n>=0 */

    int nloc(int p, int s, int n);
    double inprod, *Inprod, alpha;
    int i, t;
  
    Inprod= vecallocd(p); bsp_push_reg(Inprod,p*SZDBL);
    bsp_sync();

    inprod= 0.0;
    for (i=0; i<nloc(p,s,n); i++){
        inprod += x[i]*y[i];
    }
    for (t=0; t<p; t++){
        bsp_put(t,&inprod,Inprod,s*SZDBL,SZDBL);
    }
    bsp_sync();

	//ADDED CODE FOR SUMMING IN PARALLEL
   	for(i=0; 1;i++) { 
		int k = pow(2,i);
		if(k > p) { //all processors finished working
			break;
		}
		
		int startIndex = s*2; 
		int nextIndex = (s*2) + k;
		if(s%k == 0 && startIndex < p) {
			Inprod[startIndex] = Inprod[startIndex]+Inprod[nextIndex]; //sum two pairs of partial sums
			if(s > 0) {
				bsp_put(s-k,&(Inprod[startIndex]),Inprod,startIndex*SZDBL,SZDBL); //send this components result to the process that needs the result
			}
		}
		
		bsp_sync(); //sync at the end of each iteration
	}
	
	if(s == 0) { //if process 0, send the final result to all processors
		for(i=1; i<p;i++) {
			bsp_put(i,&(Inprod[0]),Inprod,0,SZDBL);
		}
	}
	
	bsp_sync();
	alpha = Inprod[0]; //final sum
	
    bsp_pop_reg(Inprod); vecfreed(Inprod);

    return alpha;

} /* end bspip */

void bspinprod(){
    
    double bspip(int p, int s, int n, double *x, double *y);
    int nloc(int p, int s, int n);
    double *x, alpha, time0, time1;
    int p, s, nl, i, iglob;
    
    bsp_begin(P);
    p= bsp_nprocs(); /* p = number of processors obtained */ 
    s= bsp_pid();    /* s = processor number */ 

    bsp_push_reg(&n,SZINT);
    bsp_sync();

    bsp_get(0,&n,0,&n,SZINT);
    bsp_sync();
    bsp_pop_reg(&n);

    nl= nloc(p,s,n);
    x= vecallocd(nl);
    for (i=0; i<nl; i++){
        iglob= i*p+s;
        x[i]= iglob+1;
    }
    bsp_sync(); 
    time0=bsp_time();

    alpha= bspip(p,s,n,x,x);
    bsp_sync();  
    time1=bsp_time();

    printf("Processor %d: sum of squares up to %d*%d is %.lf\n",
            s,n,n,alpha); fflush(stdout);
    if (s==0){
        printf("This took only %.6lf seconds.\n", time1-time0);
        fflush(stdout);
    }

    vecfreed(x);
    bsp_end();

} /* end bspinprod */

int main(int argc, char **argv){
	sscanf(argv[2], "%d", &n);
    bsp_init(bspinprod, argc, argv);

    /* sequential part */
    //printf("How many processors do you want to use?\n"); fflush(stdout);
    //scanf("%d",&P);
    sscanf(argv[1], "%d", &P);
    if (P > bsp_nprocs()){
        printf("Sorry, not enough processors available.\n"); fflush(stdout);
        exit(1);
    }

    /* SPMD part */
    bspinprod();

    /* sequential part */
    exit(0);

} /* end main */
