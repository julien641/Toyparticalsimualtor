#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "common.h"
#include "omp.h"
#include <vector> 
static double density =0.0005;
static int squares;
static double cutoff = .01;
static double sub_space;
static double lsize;
static double deadzone= .01;
static int area;




//
//  benchmarking program
//
void set_local(int n);
void set_local(int n) {

	lsize = sqrt(density * n);
	squares=11;
	area=squares*squares;
	sub_space=lsize/squares;


}
void printvector(std::vector<std::vector<particle_t> > x);
void printvector(std::vector<std::vector<particle_t> > x){
	printf("subspace:%f ",sub_space);
	int count=0;
	for(int i=0;i<x.size();i++){
		printf("box: %i size:%i\n",i,x[i].size());
		count+=x[i].size();
		printf("m:%i \n",x[i].size());
		for(int zsd=0;zsd<x[i].size();zsd++){
			printf("x:%f y:%f  |",x[i][zsd].x,x[i][zsd].y);

		}
		printf("\n");
	}
	printf("count:%i",count);
	printf("\n\n");


}
int withindead(particle_t& p,int m);
int withindead(particle_t& p,int m){
	double x=(m%squares);

	double y= floor((m/squares));
	if((p.x>=(sub_space *  x))&&(p.x<=(sub_space *  x)+deadzone)){//left
		// printf(" 3/1/2 ");
		if(p.y>=(sub_space * (y)) && p.y<=(sub_space * (y))+deadzone){
			//   printf("3"); 
			return 3;
		}else if((p.y)<=(sub_space *  (y+1))&&p.y>=(sub_space * (y+1))-deadzone){
			// printf("1");
			return 1;
		}else{
			//   printf("2");
			return 2;
		}
	}else if(p.y>=(sub_space * y)&&p.y<=(sub_space * y)+deadzone){//top
		//   printf(" 5/4 ");
		if((p.x>=((sub_space * (x+1))-deadzone)) && (p.x<=(sub_space *(x+1)))){
			//    printf("5");
			return 5; 
		}else{

			//      printf("4");
			return 4;
		}
	}else if((p.x>=(sub_space * (x+1))-deadzone)&&  (p.x<=(sub_space * (x+1)))){//right
		// printf("7/6");
		if((p.y>=(sub_space * (y+1))-deadzone)&&(p.y<=(sub_space * (y+1)))){
			//       printf("7");
			return 7;
		}else{
			//   printf("6");
			return 6;
		}
	}else if(p.y>=(sub_space * (y+1))-deadzone && p.y<=(sub_space * (y+1))){//bottom
		//  printf("8");
		return 8;
	}else{
		//  printf("0");
		return 0;
	}

}
std::vector<std::vector<particle_t> > initvector(int size){
	std::vector<std::vector<particle_t> > vector;
	vector.reserve(size);
	for(int i=0;i<size;i++){
		std::vector<particle_t> dude;
		dude.reserve(1.3*squares);
		vector.push_back(dude);

	}

	return vector;


}
int main( int argc, char **argv )
{   



	int navg,nabsavg=0,numthreads; 
	double dmin, absmin=1.0,davg,absavg=0.0;

	if( find_option( argc, argv, "-h" ) >= 0 )
	{
		printf( "Options:\n" );
		printf( "-h to see this help\n" );
		printf( "-n <int> to set number of particles\n" );
		printf( "-o <filename> to specify the output file name\n" );
		printf( "-s <filename> to specify a summary file name\n" ); 
		printf( "-no turns off all correctness checks and particle output\n");   
		return 0;
	}

	int n = read_int( argc, argv, "-n", 1000 );
	char *savename = read_string( argc, argv, "-o", NULL );
	char *sumname = read_string( argc, argv, "-s", NULL );

	FILE *fsave = savename ? fopen( savename, "w" ) : NULL;
	FILE *fsum = sumname ? fopen ( sumname, "a" ) : NULL;      

	particle_t *particles = (particle_t*) malloc( n * sizeof(particle_t) );
	set_size( n );

	set_local(n);



	std::vector<std::vector<particle_t> > x=initvector(area);



	init_particles( n, particles );


	/*
	   std::vector<std::vector<particle_t> > x;
	   x.reserve(area);
	   for(int i=0;i<area;i++){
	   std::vector<particle_t> dude;
	   x.push_back(dude);

	   }
	   */
	//seperating particles depending on their y and x into bins[][][]][][][][]....
	for (int p = 0 ; p < n; p++) {
		int xs=(particles[p].x/sub_space);
		int y=(particles[p].y/sub_space);

		int sup= ((y * squares)+xs);
		//    printf("%f",p.x/subspace+p.y/subspace);
		//  printf("sup: %i",sup);
		x[sup].push_back(particles[p]);

		//printf ("size: %i\n",x[sup].size());
	}
	//
	//  simulate a number of time steps
	//

	double simulation_time = read_timer( );
	//printvector(x);

	std::vector<std::vector<particle_t> > new_n;


	omp_lock_t writelock[area];
	for(int i =0;i<area;i++){


		omp_init_lock(&writelock[i]);


	}







#pragma omp parallel private(dmin)
	{



		numthreads = omp_get_num_threads();
		for( int step = 0; step < 1000; step++ )
		{
			navg = 0;
			davg = 0.0;
			dmin = 1.0;

			//
			//  compute all forces
			//

			int m,i,j, xdead,case1;
			//  printvector(x);

			dmin=1;
#pragma omp single
			{
				new_n=initvector(area);    



			}

#pragma omp  for private(i,j,xdead,case1)  reduction (+:navg) reduction(+:davg) 
			for ( m = 0; m < x.size(); m++) {


				for ( i = 0; i < x[m].size(); i++) {
					x[m][i].ax = x[m][i].ay = 0;
					xdead=withindead(x[m][i],m);
					// #pragma omp parallel  for reduction (+:navg) reduction(+:davg)
					for ( j = 0; j < x[m].size(); j++) {
						apply_force(x[m][i], x[m][j], &dmin, &davg, &navg);

					}

					switch(xdead){

						case 1:

							if( m%squares>0 ){
								//     #pragma omp parallel  for
								for( case1=0;case1<x[m-1].size();case1++){

									apply_force(x[m][i], x[m-1][case1], &dmin, &davg, &navg);

								}
								//y
								if(m/squares<squares-1){
									//    #pragma omp parallel  for
									for( case1=0;x[m+squares-1].size()>case1;case1++){

										apply_force(x[m][i], x[m+squares-1][case1], &dmin, &davg, &navg);

									}
								}
							}
							//-1
							if(m/squares<squares-1){
								//      #pragma omp parallel  for
								for( case1=0;x[m+squares-1].size()>case1;case1++){

									apply_force(x[m][i], x[m+squares-1][case1], &dmin, &davg, &navg);

								}
							}
							break;
						case 2:
							if(  m%squares>0 ){
								//  #pragma omp parallel  for
								for( case1=0;x[m-1].size()>case1;case1++){

									apply_force(x[m][i], x[m-1][case1], &dmin, &davg, &navg);

								}             
							}
							break;
						case 3:
							if( m%squares>0){
								//     #pragma omp parallel  for
								for(case1=0;x[m-1].size()>case1;case1++){

									//        deady=withindead(xo-1mcase3),o-1,m);
									//      if((deady==5)||(deady==6)||(deady==4)||(deady==7)){
									apply_force(x[m][i], x[m-1][case1], &dmin, &davg, &navg);


									//    }
								}

								if(m/squares>0){
									//         #pragma omp parallel  for
									for(case1=0;x[m-squares-1].size()>case1;case1++){
										//         deady=withindead( xo-1m-1case3),o-1,m-1);
										//        if((deady==6)||(deady==7)||(deady==8)){
										apply_force(x[m][i], x[m-squares-1][case1], &dmin, &davg, &navg);
										//     }

									}
								}
							}

							if(m/squares>0){
								//     #pragma omp parallel  for
								for(case1=0;x[m-squares].size()>case1;case1++){

									//      deady=withindead( xom-1case3),o,m-1);
									//          if((deady==1)||(deady==2)||(deady==8)){
									apply_force(x[m][i], x[m-squares][case1], &dmin, &davg, &navg);
									//  }
								}
							}
							break;
						case 4:
							if(m/squares>0){
								//      #pragma omp parallel  for
								for( case1=0;x[m-squares].size()>case1;case1++){

									apply_force(x[m][i], x[m-squares][case1], &dmin, &davg, &navg);

								}
							}
							break;
						case 5:
							if(m/squares>0){
								//  #pragma omp parallel  for
								for( case1=0;x[m-squares].size()>case1;case1++){
									//    deady=withindead( xom-1case4),o,m-1);
									//  if((deady==7)||(deady==8)||(deady==6)){

									apply_force(x[m][i], x[m-squares][case1], &dmin, &davg, &navg);
									//    }
								}

								if(m%squares<squares-1){
									//         #pragma omp parallel  for
									for( case1=0;x[m+1-squares].size()>case1;case1++){

										//  deady= withindead( xo+1m-1case4),o+1,m-1);
										//    if((deady==1)||(deady==2)||(deady==8)){
										apply_force(x[m][i], x[m+1-squares][case1], &dmin, &davg, &navg);
										//    }
									}
								}
							}
							if(m%squares<squares-1){
								//    #pragma omp parallel  for
								for( case1=0;x[m+1].size()>case1;case1++){

									//        deady=withindead( xo+1mcase4),o+1,m);
									//         if((deady==3)||(deady==2)||(1==deady)){
									apply_force(x[m][i], x[m+1][case1], &dmin, &davg, &navg);
									//        }
								}
							}

							break;
						case 6:
							if(m%squares<squares-1){
								///     #pragma omp parallel  for
								for( case1=0;x[m+1].size()>case1;case1++){

									//       deady=withindead( xo+1mcase4),o+1,m);
									//    if((deady==3)||(deady==2)||(deady==1)||(deady==7)){
									apply_force(x[m][i], x[m+1][case1], &dmin, &davg, &navg);
									//     }                      

								}
							}
							break;
							//    
						case 7:
							//  printf("hiii");
							if(m/squares<squares-1){
								//      printf("hiii");
								//   #pragma omp parallel  for
								for( case1=0;x[m+squares].size()>case1;case1++){
									//   if(withindead( xom+1case4),o,m+1)==){
									//           deady=withindead(xom+1case4),o,m+1);
									//            if((deady==3)||(deady==4)||(deady==5)){
									apply_force(x[m][i], x[m+squares][case1], &dmin, &davg, &navg);
									//           }
								}
								if(m%squares<squares-1){
									//   printf("hello\n");
									//     #pragma omp parallel  for
									for( case1=0;x[m+squares+1].size()>case1;case1++){
										//            deady=withindead(xo+1m+1case4),m+1,o+1);
										//          if((deady==1)||(deady==2)||(deady==3)||(deady==4)){
										//      if(withindead( xo+1m+1case4),o+1,m+1)==3){
										apply_force(x[m][i], x[m+squares+1][case1], &dmin, &davg, &navg);
										//                }   
									}

									}
								}
								if(m%squares<squares-1){
									//      printf("hello");
									//   #pragma omp parallel  for
									for( case1=0;x[m+1].size()>case1;case1++){
										//        deady=withindead(xo+1mcase4),o+1,m);
										//        if((deady==3)||(deady==5)||(deady==4)){
										apply_force(x[m][i], x[m+1][case1], &dmin, &davg, &navg);
										//      }
									}
								}
								break;
									case 8:
								if(m/squares<squares-1){
									//    #pragma omp parallel  for
									for( case1=0;x[m+squares].size()>case1;case1++){
										//    deady=withindead(xom+1case4),o,m+1);
										//    if((deady==4)||(deady==3)||(deady==5)){
										apply_force(x[m][i], x[m+squares][case1], &dmin, &davg, &navg);

										//         }
									}
								}
								break;


								}
								//       printf("\n%i",numthreads);

							}
					}


					//
					//  move particles
					//
					int foo;
#pragma omp   for private(foo)
					for (int bar= 0; bar < x.size(); bar++) {
						for ( foo = 0; foo < x[bar].size(); foo++) {
							move(x[bar][foo]);
						}
					}
					int old_i,xs,y,sup;

					//new_n.reserve(area);
					//  printf("new_o:");



					int ip;


#pragma omp for private(old_i,y,sup,xs)
					for ( int old_m = 0;  old_m< x.size(); old_m++) {
						for ( old_i = 0; old_i < x[old_m].size(); old_i++) {
							// ip=omp_get_num_threads();
							xs=(x[old_m][old_i].x/sub_space);
							y=(x[old_m][old_i].y/sub_space);
							//printf("%i",ip);
							sup= floor(( y* squares)+xs);
							//    printf("sup: %i",sup);
							omp_set_lock(&writelock[sup]);
							new_n[sup].push_back(x[old_m][old_i]);
							omp_unset_lock(&writelock[sup]);
						}

					}  


#pragma omp single
					{
						x=new_n;

					}


					if( find_option( argc, argv, "-no" ) == -1 ) 
					{
						//
						//  compute statistical data
						//
#pragma omp master
						if (navg) { 
							absavg += davg/navg;
							nabsavg++;
						}

#pragma omp critical
						if (dmin < absmin) absmin = dmin; 

						//
						//  save if necessary
						//
#pragma omp master
						if( fsave && (step%SAVEFREQ) == 0 )
							save( fsave, n, particles );
					}
				}
			}
			simulation_time = read_timer( ) - simulation_time;

			printf( "n = %d,threads = %d, simulation time = %g seconds", n,numthreads, simulation_time);

			if( find_option( argc, argv, "-no" ) == -1 )
			{
				if (nabsavg) absavg /= nabsavg;
				// 
				//  -the minimum distance absmin between 2 particles during the run of the simulation
				//  -A Correct simulation will have particles stay at greater than 0.4 (of cutoff) with typical values between .7-.8
				//  -A simulation were particles don't interact correctly will be less than 0.4 (of cutoff) with typical values between .01-.05
				//
				//  -The average distance absavg is ~.95 when most particles are interacting correctly and ~.66 when no particles are interacting
				//
				printf( ", absmin = %lf, absavg = %lf", absmin, absavg);
				if (absmin < 0.4) printf ("\nThe minimum distance is below 0.4 meaning that some particle is not interacting");
				if (absavg < 0.8) printf ("\nThe average distance is below 0.8 meaning that most particles are not interacting");
			}
			printf("\n");

			//
			// Printing summary data
			//
			if( fsum)
				fprintf(fsum,"%d %d %g\n",n,numthreads,simulation_time);

			//
			// Clearing space
			//
			if( fsum )
				fclose( fsum );

			free( particles );
			if( fsave )
				fclose( fsave );

			return 0;
		}
