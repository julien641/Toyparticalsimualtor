#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "common.h"
#include <vector> 
#include<bits/stdc++.h>
double density =0.0005;
int squares;
double cutoff = .01;
double sub_space;
double lsize;
double deadzone= .01;
int area;
//
//  benchmarking program
//
//hello

int withindead(particle_t& p,int m);
/*  
       
    3__4__5
    |     |
    |     |
   2| 0   | 6
    |_____|
    1  8  7



*/
void printvector(std::vector<std::vector<particle_t> > x);
void printvector(std::vector<std::vector<particle_t> > x){
    printf("subspace:%f ",sub_space);
    int count=0;
    for(int i=0;i<x.size();i++){
        //printf("box: %i size:%i\n",i,x[i].size());
        count+=x[i].size();
        printf("m:%i \n",x[i].size());
        for(int zsd=0;zsd<x[i].size();zsd++){
       // printf("x:%f y:%f  |",x[i][zsd].x,x[i][zsd].y);
        
        }
        printf("\n");
    }
    printf("count:%i",count);
            printf("\n\n");
    
    
}




int withindead(particle_t& p,int m){
   // printf("x:%f y:%f",p.x,p.y );
   double x=(m%squares);
  
   double y= floor((m/squares));
 //  printf("x:%i y:%i",x,y);
   
  //  double x =floor(p.x/sub_space);
    //double y=floor(p.y/sub_space);
    
    
    
    
    
    
    
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
            
        }else
            
            
    if(p.y>=(sub_space * y)&&p.y<=(sub_space * y)+deadzone){//top
      //   printf(" 5/4 ");
            if((p.x>=((sub_space * (x+1))-deadzone)) && (p.x<=(sub_space *(x+1)))){
             //    printf("5");
                return 5; 
            }else{
            
          //      printf("4");
            return 4;
            }
        }
        
        if((p.x>=(sub_space * (x+1))-deadzone)&&  (p.x<=(sub_space * (x+1)))){//right
       // printf("7/6");
            if((p.y>=(sub_space * (y+1))-deadzone)&&(p.y<=(sub_space * (y+1)))){
           //       printf("7");
             return 7;
            }else{
            //   printf("6");
            return 6;
            }
        }else
        
        if(p.y>=(sub_space * (y+1))-deadzone && p.y<=(sub_space * (y+1))){//bottom
      //  printf("8");
            return 8;
        }else{
          //  printf("0");
            return 0;
        }
    
}
void set_local(int n);
void set_local(int n) {

	lsize = sqrt(density * n);
    squares=16;
    area=squares*squares;
    sub_space=lsize/squares;

}

int main(int argc, char **argv)
{
    
    
  // for(int squares=1;squares<20;squares++){

	int navg, nabsavg = 0;
	double davg, dmin, absmin = 1.0, absavg = 0.0;

	if (find_option(argc, argv, "-h") >= 0)
	{
		printf("Options:\n");
		printf("-h to see this help\n");
		printf("-n <int> to set the number of particles\n");
		printf("-o <filename> to specify the output file name\n");
		printf("-s <filename> to specify a summary file name\n");
		printf("-no turns off all correctness checks and particle output\n");
		return 0;
	}
	//30


	int n = read_int(argc, argv, "-n", 1000);
//squares=ceil(1.0524*pow(n,0.2527));
//squares=ceil(0.0003*n + 7.1471);
//squares=ceil(2.1853 * log(n) - 9.2132);

	char *savename = read_string(argc, argv, "-o", NULL);
	char *sumname = read_string(argc, argv, "-s", NULL);
 
	FILE *fsave = savename ? fopen(savename, "w") : NULL;
	FILE *fsum = sumname ? fopen(sumname, "a") : NULL;

	particle_t *particles = (particle_t*)malloc(n * sizeof(particle_t));


	set_local(n);

	set_size(n);

 //printf("squares = %i, ",area);
	init_particles(n, particles);

  //int ncount=0;
  std::vector<std::vector<particle_t> > x(area, std::vector<particle_t>());
  /*
  std::vector<std::vector<particle_t> > x;
  x.reserve(area);
for(int i=0;i<area;i++){
std::vector<particle_t> dude;
x.push_back(dude);

}

*/


     // volatile double sup=sqrt((pow(particles[p].x/sub_space,2)+pow(particles[p].y/sub_space,2)));
 for (int p = 0 ; p < n; p++) {
     int xs=(particles[p].x/sub_space);
     int y=(particles[p].y/sub_space);
     
      int sup= ((y * squares)+xs);
//    printf("%f",p.x/subspace+p.y/subspace);
  //  printf("sup: %i",sup);
    x[sup].push_back(particles[p]);
    x.reserve(1.3*(n/area));
   //printf ("size: %i\n",x[sup].size());
}
    			  
//printvector( x);
//printvector( x);
    	
    
	//    printf("%d",count);

   //  printf("%n",count);
	//
	//  simulate a number of time steps
	//
	double simulation_time = read_timer();
	
	for (int step = 0; step < NSTEPS; step++)
	{
	 //   printvector(x);
	    /*
	    int count[squares][squares];
	    
	    int cot=0;


	   	for (int dude = 0; dude < x.size(); dude++)
		{

			for (int hello = 0; hello < x[dude].size(); hello++) {

            for(int adf=0;adf<x[dude][hello].size();adf++){
                count[dude][hello]++;
                
                
                
                
                
               //  printf("n:%d,x:%f,y:%f \n",cot,xdudehelloadf).x,xdudehelloadf).x);
                cot++;
                
            }
  
			}
	        
	        
	        
	    }
	   
	    
	    for(int asd=0;asd<squares;asd++){
	        for(int ds=0;ds<squares;ds++){
	            printf("%i ",count[asd][ds]);
	            count[asd][ds]=0;
	        }
	        printf("\n");
	        
	    }
	    
	    
	    
	    printf("%d \n",cot);
	    printf("end\n");
	    */
	    
		navg = 0;
		davg = 0.0;
		dmin = 1.0;
		//
		//  compute forces
		//



		//outer vector -- size/squares
for (int m = 0; m < x.size(); m++) {

	for (int i = 0; i < x[m].size(); i++) {
                         
					x[m][i].ax = x[m][i].ay = 0;
				int xdead=withindead(x[m][i],m);

					for (int j = 0; j < x[m].size(); j++) {
        					apply_force(x[m][i], x[m][j], &dmin, &davg, &navg);
					}
			

/*  
       
    3__4__5
    |     |
    |     |
   2| 0   | 6
    |_____|
    1  8  7



*/


//printf("\nx:%i y:%i",xss,yss);
                       switch(xdead){
                           case 0:
                            break;
                           case 1:
                      
                                if( m%squares>0 ){
                                    for(int case1=0;case1<x[m-1].size();case1++){
                                            apply_force(x[m][i], x[m-1][case1], &dmin, &davg, &navg);
                                    }
                                    //y
                                    if(m/squares<squares-1){
                                        
                                        for(int case1_2=0;x[m+squares-1].size()>case1_2;case1_2++){

                                                apply_force(x[m][i], x[m+squares-1][case1_2], &dmin, &davg, &navg);

                                        }
                                    }
                                 }
                                //-1
                                 if(m/squares<squares-1){
                                     
                                      for(int case1_3=0;x[m+squares-1].size()>case1_3;case1_3++){

                                         apply_force(x[m][i], x[m+squares-1][case1_3], &dmin, &davg, &navg);

                                    }
                                 }
                            break;
                            case 2:
                             if(  m%squares>0 ){
                                 for(int case2=0;x[m-1].size()>case2;case2++){

                                           apply_force(x[m][i], x[m-1][case2], &dmin, &davg, &navg);

                                        }             
                                         }
                             break;
                            case 3:
                                    if( m%squares>0){
                                            for(int case3=0;x[m-1].size()>case3;case3++){

                                    //        deady=withindead(xo-1mcase3),o-1,m);
                                      //      if((deady==5)||(deady==6)||(deady==4)||(deady==7)){
                                            apply_force(x[m][i], x[m-1][case3], &dmin, &davg, &navg);
                                            
                                                
                                        //    }
                                            }
                                    
                                          if(m/squares>0){
                                               for(int case3=0;x[m-squares-1].size()>case3;case3++){
                                          //         deady=withindead( xo-1m-1case3),o-1,m-1);
                                            //        if((deady==6)||(deady==7)||(deady==8)){
                                                    apply_force(x[m][i], x[m-squares-1][case3], &dmin, &davg, &navg);
                                              //     }
                                                        
                                                    }
                                    }
                                    }
                                    
                                    if(m/squares>0){
                                          for(int case3=0;x[m-squares].size()>case3;case3++){
                            
                                    //      deady=withindead( xom-1case3),o,m-1);
                                      //          if((deady==1)||(deady==2)||(deady==8)){
                                            apply_force(x[m][i], x[m-squares][case3], &dmin, &davg, &navg);
                                        //  }
                                     }
                                    }
                           break;
                           case 4:
                                  if(m/squares>0){
                                         for(int case4=0;x[m-squares].size()>case4;case4++){

                                      apply_force(x[m][i], x[m-squares][case4], &dmin, &davg, &navg);
                               
                                         }
                                     }
                            break;
                            case 5:
                              if(m/squares>0){
                                  for(int case4=0;x[m-squares].size()>case4;case4++){
                                //    deady=withindead( xom-1case4),o,m-1);
                                  //  if((deady==7)||(deady==8)||(deady==6)){
                                        
                                            apply_force(x[m][i], x[m-squares][case4], &dmin, &davg, &navg);
                                    //    }
                                  }
                                  
                                if(m%squares<squares-1){
                                              
                                    for(int case4=0;x[m+1-squares].size()>case4;case4++){

                                  //  deady= withindead( xo+1m-1case4),o+1,m-1);
                                //    if((deady==1)||(deady==2)||(deady==8)){
                                        apply_force(x[m][i], x[m+1-squares][case4], &dmin, &davg, &navg);
                                    //    }
                                    }
                              }
                              }
                                if(m%squares<squares-1){
                                    for(int case4=0;x[m+1].size()>case4;case4++){

                                  //        deady=withindead( xo+1mcase4),o+1,m);
                                    //         if((deady==3)||(deady==2)||(1==deady)){
                                            apply_force(x[m][i], x[m+1][case4], &dmin, &davg, &navg);
                                      //        }
                                }
                                }
                              
                             break;
                            case 6:
                                if(m%squares<squares-1){
                                      for(int case4=0;x[m+1].size()>case4;case4++){
                                          
                                      //       deady=withindead( xo+1mcase4),o+1,m);
                                        //    if((deady==3)||(deady==2)||(deady==1)||(deady==7)){
                                            apply_force(x[m][i], x[m+1][case4], &dmin, &davg, &navg);
                                          //     }                      
                
                                        }
                                }
                            break;
                       //    
                         case 7:
                          //  printf("hiii");
                                if(m/squares<squares-1){
                              //      printf("hiii");
                                    for(int case4=0;x[m+squares].size()>case4;case4++){
                                                 //   if(withindead( xom+1case4),o,m+1)==){
                                      //           deady=withindead(xom+1case4),o,m+1);
                                        //            if((deady==3)||(deady==4)||(deady==5)){
                                            apply_force(x[m][i], x[m+squares][case4], &dmin, &davg, &navg);
                                         //           }
                                        }
                                          if(m%squares<squares-1){
                                              //   printf("hello\n");
                                                for(int case4=0;x[m+squares+1].size()>case4;case4++){
                                        //            deady=withindead(xo+1m+1case4),m+1,o+1);
                                          //          if((deady==1)||(deady==2)||(deady==3)||(deady==4)){
                                              //      if(withindead( xo+1m+1case4),o+1,m+1)==3){
                                                    apply_force(x[m][i], x[m+squares+1][case4], &dmin, &davg, &navg);
                                    //                }   
                                                  }
                                              
                                          }
                                    }
                                if(m%squares<squares-1){
                              //      printf("hello");
                              for(int case4=0;x[m+1].size()>case4;case4++){
                                //        deady=withindead(xo+1mcase4),o+1,m);
                                  //        if((deady==3)||(deady==5)||(deady==4)){
                                            apply_force(x[m][i], x[m+1][case4], &dmin, &davg, &navg);
                                    //      }
                                }
                                }
                             break;
                                  case 8:
                                if(m/squares<squares-1){
                                      for(int case4=0;x[m+squares].size()>case4;case4++){
                                        //    deady=withindead(xom+1case4),o,m+1);
                                      //    if((deady==4)||(deady==3)||(deady==5)){
                                            apply_force(x[m][i], x[m+squares][case4], &dmin, &davg, &navg);
                                    
                                //         }
                                          }
                                }
                             break;

                        
                                }
                    }
}
        		
		

		for (int bar= 0; bar < area; bar++) {


			for (int foo = 0; foo < x[bar].size(); foo++) {
				move(x[bar][foo]);
			}
		}
/*
std::vector < std::vector<particle_t> > new_n;
new_n.reserve(area);
//new_n.reserve(area);
	//  printf("new_o:");

      for (int a = 0; a < area; a++) {
          		std::vector<particle_t> vecpartical;
          		vecpartical.reserve(1.3*(n/area));
             //   vecpartical.reserve((n/area)*1.3);
          	new_n.push_back(vecpartical);
      }
*/

  std::vector<std::vector<particle_t> > new_n(area, std::vector<particle_t>());

  
  
//printf("sort");
for (int old_m = 0;  old_m< x.size(); old_m++) {
  
    for (int old_i = 0; old_i < x[old_m].size(); old_i++) {
          int xs=(x[old_m][old_i].x/sub_space);
     int y=(x[old_m][old_i].y/sub_space);
     
      int sup= floor(( y* squares)+xs);
      //    printf("sup: %i",sup);
        new_n[sup].push_back(x[old_m][old_i]);
			}

		}

	
x= new_n;  
//printvector( x);

	if (find_option(argc, argv, "-no") == -1)
	{
		//
		// Computing statistical data
		//
		if (navg) {
			absavg += davg / navg;
			nabsavg++;
		}
		if (dmin < absmin) absmin = dmin;

		//
		//  save if necessary
		//
		if (fsave && (step%SAVEFREQ) == 0)
			save(fsave, n, particles);
	}
}
simulation_time = read_timer() - simulation_time;

printf("n = %d, simulation time = %g seconds", n, simulation_time);

if (find_option(argc, argv, "-no") == -1)
{
	if (nabsavg) absavg /= nabsavg;
	// 
	//  -the minimum distance absmin between 2 particles during the run of the simulation
	//  -A Correct simulation will have particles stay at greater than 0.4 (of cutoff) with typical values between .7-.8
	//  -A simulation were particles don't interact correctly will be less than 0.4 (of cutoff) with typical values between .01-.05
	//
	//  -The average distance absavg is ~.95 when most particles are interacting correctly and ~.66 when no particles are interacting
	//
	printf(", absmin = %f, absavg = %f", absmin, absavg);
	if (absmin < 0.4) printf("\nThe minimum distance is below 0.4 meaning that some particle is not interacting");
	if (absavg < 0.8) printf("\nThe average distance is below 0.8 meaning that most particles are not interacting");
}
printf("\n");

//
// Printing summary data
//
if (fsum)
fprintf(fsum, "%d %g\n", n, simulation_time);

//
// Clearing space
//
if (fsum)
fclose(fsum);
free(particles);
if (fsave)
fclose(fsave);

//return 0;

}


/*
squares = 11, n = 500, simulation time = 0.442159 seconds
squares = 11, n = 1000, simulation time = 0.882489 seconds
squares = 11, n = 2000, simulation time = 1.88728 seconds
squares = 11, n = 3000, simulation time = 3.0277 seconds
squares = 11, n = 4000, simulation time = 4.30358 seconds
squares = 11, n = 5000, simulation time = 5.74068 seconds
squares = 11, n = 6000, simulation time = 7.12722 seconds
squares = 11, n = 7000, simulation time = 8.81123 seconds
squares = 11, n = 8000, simulation time = 10.5104 seconds
squares = 11, n = 9000, simulation time = 12.3447 seconds
squares = 11, n = 10000, simulation time = 14.6201 seconds
squares = 11, n = 11000, simulation time = 16.6252 seconds
squares = 11, n = 12000, simulation time = 18.8365 seconds
Serial code is O(N^slope)
Slope estimates are : 0.997013 1.096658 1.165739 1.222335 1.291234 1.186605 1.375964 1.320
575 1.365745 1.605634 1.348464 1.435179
Slope estimate for line fit is: 1.191961
Serial Grade =  100.00

*/