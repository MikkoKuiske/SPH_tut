/* used for preview */
#define RESTART   0                                        /*  first file number to restart calculation  */

#define DIM 100000   

#define SHUUKIX 1
#define SHUUKIY 0 /* 0=din't make periodic boundary  1=make periodic boundary */


/***************************************  parameter of sph  **************************************/
#define SAVEFILE 1
#define FILE_NAME  "./result/data"

#define ENDTIME 500                                                  
#define DT 1.0e-3                 /*  1.0e-3  */                         /*it take for 1step [s]  */
#define DTF 1 /* How often do you save data? */ 


#define PNLMT 100000 /* particle number limit */
#define PXLMT 150                                                       /*  x axial particle number changed  */
#define PYLMT 30                                                              /*  y axial particle number  */
#define ATUMI 3                                                            /*  wall thickness(particle number)  */

#define ZXLMT 75                                             /*  x axial ZONE number(=PXLMT/2)  */
#define ZYLMT 15                                                    /*  y axial ZONE number(=PYLMT/2)  */

#define QPinZONE_MAX 20 

#define P_SPACING 1.0e-4 /*in meter */                            /* also particle diameter 粒子の直径みたいなもの  */
#define H 1.0e-4                                                           /*  kernel size カーネルの大きさ  */


#define U 0.3                                                       /*  wall velosity (Couette flow)  */   

#define F 0                                                                         /*  a force of friction?  */
#define G 0                                                                            /*  gravity 重力  */

#define BG 1.0

#define ATM_PRESSURE 101300.0 /* atmospheric pressure Pa=N/m2 */

#define HIGH 15

#define EX_A  0.487 /* stands for exchange of acceleration */

#define DAMPLMT 300                                                      
#define DAMPING_STEP (1.0*P_SPACING/(C_WATER*DT))
#define DAMPING_STEP_CHANGE 1.05

#define D_MAX 1.01
#define D_MIN 0.99

#define E 2.718281828459  /* natural log */

#define KEISUU 0.487 /* KEISUU mean coefficient*/

#define sqr(x) ((x)*(x))
#define cube(x) ((x)*(x)*(x))

#define DIF_X 1.5e-1
#define DIF_Y 1.5e-1

/*************WATER****************/
#define RHO_WATER    1000.0/*water weight per meter on atmospheric pressure  kg m3*/
#define M_WATER      (RHO_WATER*P_SPACING*P_SPACING) /* mass for water particle */ 
#define C_WATER      1.453e3 /* sonic speed in water */
#define NU_WATER     1.0038e-6 /*dynamic viscosity  in nature 1.01e-6(about 20C ) */
#define PR_WATER     7./*prantdle viscosity/diffusivity of heat(about 20C )*/
#define BETA_WATER   0.21e-3 /*coefficient of cubic thermal expansion 20C */


#define nu_water(x) 1.7818e-6-5.5703e-8*(x)+1.0109e-9*((x)*(x))-9.4499e-12*((x)*(x)*(x))+3.4292e-14*((x)*(x)*(x)*(x))  
/* dynamic viscosity calculated by least-square method*/
#define a_water(x) 1.3331e-7+5.5969e-10*(x)-2.8479e-12*((x)*(x))+6.954e-15*((x)*(x)*(x))
/*diffusivity of heat*/

#define rho_water_high(x) 1.0002e3+3.0641e-3*(x)-5.6149e-3*(x)*(x)+1.4121e-5*(x)*(x)*(x)  
#define rho_water_low(x) 9.9984e2+7.342e-2*(x)-1.355e-2*(x)*(x)+8.2906e-4*(x)*(x)*(x)

/**************SUIGIN*****************/
#define RHO_HG       13.59e3 
#define M_HG         (RHO_HG*P_SPACING*P_SPACING)
#define C_HG         1.45e3     /*23~27C*/
#define NU_HG        1.258e-5 /*(20C)*/
#define PR_HG        0.0028 
#define BETA_HG      0.181e-3    /*20C*/

/*************HIMASI YU*********************/
#define RHO_HI       0.965e3  /*25C*/
#define M_HI         (RHO_HI*P_SPACING*P_SPACING)
#define C_HI         1390 /*zaisitu ondo humei*/
#define NU_HI        1.29e-4 /*25C big change by heat*/
#define PR_HI        0.99e4  /*25C*/
#define BETA_HI      0.72e-3 /*orive oil 20C*/


#define M_WALL       M_WATER /* mass for wall particle */
#define M_LIQUID     M_WATER  
#define C_LIQUID     C_WATER
#define RHO_LIQUID   RHO_WATER
#define NU_LIQUID    NU_WATER
#define nu_liquid(x) nu_water(x)
#define a_liquid(x)  a_water(x)
#define PR_LIQUID    PR_WATER
#define BETA_LIQUID  BETA_WATER
