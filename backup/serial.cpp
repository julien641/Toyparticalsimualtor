#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "common.h"
#include <vector> 
#include<bits/stdc++.h>
#define density 0.0005
double squares = 10;
double sub_space;
double lsize;
double deadzone=.01;

//
//  benchmarking program
//
//hello
std::vector< std::vector < std::vector<particle_t> > > sort(std::vector<std::vector<std::vector<particle_t> > >old);

int withindead(particle_t x);
/*  
       
    3__4__5
    |     |
    |     |
   2| 0   | 6
    |_____|
    1  8  7



*/






int withindead(particle_t p,int x,int y){
        if(p.x<(sub_space *x)+deadzone){//left
            if(p.y<(sub_space *(y))+deadzone){
                return 3;
            }else if(p.y<(sub_space *(y+1))-deadzone){
                return 1
            }else{
            return 2;
            }
        }else if(p.y<(sub_space *y)+deadzone){//top
            if(p.x<(sub_space *(x+1))-deadzone){
                return 5;
            }else{
            return 4;
            }
        }else if(p.x>(sub_space *(x+1))-deadzone){//right
            if(p.y<(sub_space *(y+1))-deadzone){
             return 6;
            }
            return 7;
        }else if(p.y<(sub_space *(y+1))-deadzone){//bottom
            return 8;
        }else{
            return 0;
        }
    
}

std::vector< std::vector < std::vector<particle_t> > > sort(std::vector<std::vector<std::vector<particle_t> > >old) {
	std::vector< std::vector < std::vector<particle_t> > > new_n;
	//  printf("new_o:");
	for (int new_o = 0; new_o < old.size(); new_o++) {

		std::vector<std::vector<particle_t> > vectvectparttemp;

		for (int new_m = 0; new_m < old.at(0).size(); new_m++) {

			std::vector<particle_t> vecpartical;

			for (int old_o = 0; old_o < old.size(); old_o++) {

				for (int old_m = 0; old_m < old.at(old_o).size(); old_m++) {

					for (int old_i = 0; old_i < old.at(old_o).at(old_m).size(); old_i++) {

						
							if (old.at(old_o).at(old_m).at(old_i).x > sub_space * (new_o) && old.at(old_o).at(old_m).at(old_i).x <= sub_space * (new_o + 1)) {
                                if (old.at(old_o).at(old_m).at(old_i).y > ((sub_space)*(new_m)) && old.at(old_o).at(old_m).at(old_i).y <= ((sub_space)*(new_m + 1))) {

								vecpartical.push_back(old.at(old_o).at(old_m).at(old_i));
							}
						}

					}
				}
			}
			vectvectparttemp.push_back(vecpartical);
		}
		new_n.push_back(vectvectparttemp);
	}

	return new_n;

}
void set_local(int n);
void set_local(int n) {
	lsize = sqrt(density * n);
	sub_space = lsize / squares;



}

int main(int argc, char **argv)
{
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

	char *savename = read_string(argc, argv, "-o", NULL);
	char *sumname = read_string(argc, argv, "-s", NULL);

	FILE *fsave = savename ? fopen(savename, "w") : NULL;
	FILE *fsum = sumname ? fopen(sumname, "a") : NULL;

	particle_t *particles = (particle_t*)malloc(n * sizeof(particle_t));

	set_local(n);

	set_size(n);

	init_particles(n, particles);
	bool end = false;
	std::vector< std::vector < std::vector<particle_t> > > x;

	for (int d = 0; d < squares; d++) {
		std::vector<std::vector<particle_t> > vectvectparttemp;
		for (int a = 0; a < squares; a++) {
			std::vector<particle_t> vecpartical;
			for (int p = 0 ; (p < n); p++) {
				if (particles[p].y > sub_space * a && particles[p].y <= ((sub_space)*(a + 1))) {

					if (particles[p].x > sub_space * d && particles[p].x <= sub_space * (d + 1)) {
						//count++;
                       // printf("n:%d,x:%d,y:%d \n",particles[p].x,particles[p].y);
						vecpartical.push_back(particles[p]);
					}
				}

			}

			vectvectparttemp.push_back(vecpartical);
		}

		x.push_back(vectvectparttemp);
	}

	//    printf("%d",count);

   //  printf("%n",count);
	//
	//  simulate a number of time steps
	//
	double simulation_time = read_timer();

	for (int step = 0; step < NSTEPS; step++)
	{
	    /*
	    int cot=0;
	    printf("start\n");
	   	for (int dude = 0; dude < x.size(); dude++)
		{

			for (int hello = 0; hello < x.at(dude).size(); hello++) {

            for(int adf=0;adf<x.at(dude).at(hello).size();adf++){
                
                 printf("n:%d,x:%f,y:%f \n",cot,x.at(dude).at(hello).at(adf).x,x.at(dude).at(hello).at(adf).x);
                cot++;
                
            }
  
			}
	        
	        
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
		for (int o = 0; o < x.size(); o++)
		{

			for (int m = 0; m < x.at(o).size(); m++) {



				for (int i = 0; i < x.at(o).at(m).size(); i++) {
                    
					x.at(o).at(m).at(i).ax = x.at(o).at(m).at(i).ay = 0;
				int xdead=withindead(x.at(o).at(m).at(i));
					for (int j = 0; j < x.at(o).at(m).size(); j++) {
        					apply_force(x.at(o).at(m).at(i), x.at(o).at(m).at(j), &dmin, &davg, &navg);
                        switch(xdead){
                            case 0:
                            break;
                            case 1:
                                if( o>0 ){
                                    if(x.at(o-1).at(m).size()>j){
                                    apply_force(x.at(o).at(m).at(i), x.at(o-1).at(m).at(j), &dmin, &davg, &navg);
                                    }
                                    if(m<x.at(o).size()){
                                        if(x.at(o-1).at(m+1).size()>j){
                                            apply_force(x.at(o).at(m).at(i), x.at(o-1).at(m+1).at(j), &dmin, &davg, &navg);
                                        }
                                    }
                                 }
                                 if(m<x.size()){
                                     if(j<x.at(o).at(m+1)size()){
                                         apply_force(x.at(o).at(m).at(i), x.at(o).at(m+1).at(j), &dmin, &davg, &navg);
                                     }
                                 }
                            break;
                            case 2:
                             if( o>0 ){
                                      if(x.at(o-1).at(m).size()>j){
                                           apply_force(x.at(o).at(m).at(i), x.at(o-1).at(m).at(j), &dmin, &davg, &navg);
                                      }
                             }
                             break;
                            case 3:
                                    if(o>0){
                                        if(x.at(o-1).at(m).size()>j){
                                            apply_force(x.at(o).at(m).at(i), x.at(o-1).at(m).at(j), &dmin, &davg, &navg);
                                        }
                                          if(m>0){
                                                  if(x.at(o-1).at(m-1).size()>j){
                                                    apply_force(x.at(o).at(m).at(i), x.at(o-1).at(m-1).at(j), &dmin, &davg, &navg);
                                                  }
                                              
                                          }
                                    }
                                    if(m>0){
                                  if(x.at(o).at(m-1).size()>j){
                                            apply_force(x.at(o).at(m).at(i), x.at(o).at(m-1).at(j), &dmin, &davg, &navg);
                                        }
                                     }
                             break;
                            case 4:
                            if(0<m){
                                if(x.at(o).at(m-1).size()){
                                      apply_force(x.at(o).at(m).at(i), x.at(o).at(m-1).at(j), &dmin, &davg, &navg);
                                }
                            }
                             break;
                            case 5:
                              if(m>0){
                                    if(x.at(o).at(m-1).size()>j){
                                            apply_force(x.at(o).at(m).at(i), x.at(o).at(m-1).at(j), &dmin, &davg, &navg);
                                        }
                                          if(o<x.size()){
                                                  if(x.at(o-1).at(m+1).size()>j){
                                                    apply_force(x.at(o).at(m).at(i), x.at(o-1).at(m+1).at(j), &dmin, &davg, &navg);
                                                  }
                                              
                                          }
                                    }
                                    if(m>0){
                                         if(x.at(o).at(m-1).size()>j){
                                            apply_force(x.at(o).at(m).at(i), x.at(o).at(m-1).at(j), &dmin, &davg, &navg);
                                        }
                                     }
                            
                            
                            
                             break;
                            case 6:
                             break;
                            case 7:
                             break;
                            case 8:
                             break;

                        }
                        

        					if (o > 0) {
        					    
        						if (j < x.at(o - 1).at(m).size()) {
        							apply_force(x.at(o).at(m).at(i), x.at(o - 1).at(m).at(j), &dmin, &davg, &navg);
        						}
        						
        						
        						if (m > 0) {
        						    
        						    
        							if (j < x.at(o - 1).at(m - 1).size()) {
        								apply_force(x.at(o).at(m).at(i), x.at(o - 1).at(m - 1).at(j), &dmin, &davg, &navg);
        							}
        
        						}
        					
        					
        				    	if (m < x.at(o).size()-1) {
        						    if (j < x.at(o - 1).at(m + 1).size()) {
        						    	apply_force(x.at(o).at(m).at(i), x.at(o - 1).at(m + 1).at(j), &dmin, &davg, &navg);
        						    }
        
        				    	}
        					}
        
        				if (m > 0) {
        					if (j < x.at(o).at(m-1).size()) {
        					    
        						apply_force(x.at(o).at(m).at(i), x.at(o).at(m-1 ).at(j), &dmin, &davg, &navg);
        					}
        
        				}
        				if (m < x.at(o).size()-1) {
        					if (j < x.at(o).at(m + 1).size()) {
        						apply_force(x.at(o).at(m).at(i), x.at(o).at(m + 1).at(j), &dmin, &davg, &navg);
        					}
        				}
        				if (o < x.at(o).size()-1) {
        					if (j < x.at(o + 1).at(m).size()) {
        					    printf("h");
        						apply_force(x.at(o).at(m).at(i), x.at(o + 1).at(m).at(j), &dmin, &davg, &navg);
        					}
        					if (m < x.at(o).size()-1) {
        						if (j < x.at(o + 1).at(m + 1).size()) {
        							apply_force(x.at(o).at(m).at(i), x.at(o + 1).at(m + 1).at(j), &dmin, &davg, &navg);
        						}
        					}
        					if (m > 0) {
        						if (j < x.at(o + 1).at(m - 1).size()) {
        							apply_force(x.at(o).at(m).at(i), x.at(o + 1).at(m - 1).at(j), &dmin, &davg, &navg);
        						}
        
        
        					}
        				}
				}

			}
		}
	}

	for (int roo = 0; roo < x.size(); roo++)
	{

		for (int bar= 0; bar < x.at(roo).size(); bar++) {


			for (int foo = 0; foo < x.at(roo).at(bar).size(); foo++) {
				move(x.at(roo).at(bar).at(foo));
			}
		}

	}

	x = sort(x);


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

return 0;
}
