/* to see & save the results of SPH */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <math.h>
#include "GLbitmap.h"


/*  load file  */
#define PATH  "./result/data"
#define SPLIT 1        /* interval to load data */
#define FILE_INIT 0
#define FILE_END 500
#define PNOLMT 75000
//#define PNOLMT 65535 /* (24*80*80) 34796*/
//#define PNOLMT 200000 /* (400*200) 80000*/


/*  save image  */
#define SAVE 1
#define SAVE_PATH "./result/image"
#define EXTENSION "bmp"                                          /*  jpg,eps,bmp  */ 


/*  particle shape  */
#define SPHERE 0                                            /*  1.draw ball    0.circle    */
#define P_SPACING 1.0e-4                                    /*  distance of particles             */
#define P_R      0.5                                        /*  particle diameter         (sample = 0.5)  */
#define P_SHAPE1 6.0                                        /*  particle shape _xy (sample = 6.0)  */
#define P_SHAPE2 2.0                                        /*  particle shape _z  (sample = 6.0)  */



/*******************  GL setting  ********************/
#define ESC 27
#define WINDOW_NAME "sph"
#define WINDOW_WIDTH 1000                                        /*** 1000 ***/
#define WINDOW_HEIGHT 500                                        /*** 500 ***/
#define WINDOW_X 0 
#define WINDOW_Y 0
#define MOUSE 0

#define EYEX 75.0   /*****************/                        /*** 150.0  / 250.0  ***/        
#define EYEY 15.0   /* camera position  */                         /*** 15.0   / 20.0   ***/
#define EYEZ 2000.0 /*****************/                         /*** 3500.0 / 6000.0 ***/
#define CENTERX (EYEX)   /*****************/
#define CENTERY (EYEY)  /* lens angle  */
#define CENTERZ 0.0    /*****************/

#define UPX 0.0   /******************/
#define UPY 1.0  /* upper vector */
#define UPZ 0.0 /******************/

#define SCOPE 5.0 /* L<->R [0:180] */                                /*  magnification ratio (5.0) */
#define ASPECT 2.0
#define ZNEAR 1.0
#define ZFAR 10000.0

#define STATE_DOWN 1
#define STATE_UP 1
/*****************************************************/



int t,fnum;
int stop,back,f_split;


/* light source */
static GLfloat light0_position[] = {4.0, 2.0, 0.0, 1.0};/*light position*/
static GLfloat light0_ambient[] = {1.0, 1.0, 1.0, 1.0}; /*light ambient RGB luminance*/
static GLfloat light0_diffuse[] = {0.7, 0.7, 0.7, 1.0}; /*light diffuse RGB luminance*/
static GLfloat light0_specular[] = {0.5, 0.5, 0.5, 1.0};/*light specular RGB luminance*/


/* material red */
static GLfloat material_ambient_r[4] = {0.6, 0.0, 0.0, 1.0};   
static GLfloat material_diffuse_r[4] = {0.6, 0.3, 0.3, 1.0};
static GLfloat material_specular_r[4] = {0.6, 0.3, 0.3, 1.0};
static GLfloat material_shininess_r = {50.0};

/* material yellow */
static GLfloat material_ambient_y[4] = {0.5, 0.5, 0.0, 1.0};
static GLfloat material_diffuse_y[4] = {0.3, 0.3, 0.2, 1.0};
static GLfloat material_specular_y[4] = {0.3, 0.3, 0.2, 1.0};
static GLfloat material_shininess_y = {50.0};

/* material blue */
static GLfloat material_ambient_b[4] = {0.0, 0.0, 1.0, 1.0};
static GLfloat material_diffuse_b[4] = {0.7, 0.7, 1.0, 1.0};
static GLfloat material_specular_b[4] = {0.7, 0.7, 1.0, 1.0};
static GLfloat material_shininess_b = {50.0};

/* material light blue */
static GLfloat material_ambient_lb[4] = {0.0, 0.5, 1.0, 1.0};
static GLfloat material_diffuse_lb[4] = {0.7, 0.7, 1.0, 1.0};
static GLfloat material_specular_lb[4] = {0.7, 0.7, 1.0, 1.0};
static GLfloat material_shininess_lb = {50.0};

/* material white */
static GLfloat material_ambient_wh[4] = {1.0, 1.0, 1.0, 1.0};
static GLfloat material_diffuse_wh[4] = {0.7, 0.7, 1.0, 1.0};
static GLfloat material_specular_wh[4] = {0.7, 0.7, 1.0, 1.0};
static GLfloat material_shininess_wh = {50.0};

/* material green */
static GLfloat material_ambient_g[4] = {0.0, 0.5, 0.0, 1.0};
static GLfloat material_diffuse_g[4] = {0.3, 0.5, 0.3, 1.0};
static GLfloat material_specular_g[4] = {0.3, 0.5, 0.3, 1.0};
static GLfloat material_shininess_g = {50.0};

/* material cyan */ 
static GLfloat material_ambient_c[4] = {0.4, 0.4, 1.0, 1.0};
static GLfloat material_diffuse_c[4] = {0.4, 0.4, 1.0, 1.0};
static GLfloat material_specular_c[4] = {0.4, 0.4, 1.0, 1.0};
static GLfloat material_shininess_c = {50.0};

/* material grey */
static GLfloat material_ambient_gr[4] = {0.35, 0.35, 0.35, 1.0};
static GLfloat material_diffuse_gr[4] = {0.2, 0.2, 0.2, 1.0};
static GLfloat material_specular_gr[4] = {0.2, 0.2, 0.2, 1.0};
static GLfloat material_shininess_gr = {50.0};

/* material magenta */
static GLfloat material_ambient_m[4] = {0.4, 0.0, 0.4, 1.0};
static GLfloat material_diffuse_m[4] = {0.4, 0.3, 0.4, 1.0};
static GLfloat material_specular_m[4] = {0.4, 0.3, 0.4, 1.0};
static GLfloat material_shininess_m = {50.0};

/* material water */
static GLfloat material_ambient_a[4] = {0.5, 0.0, 0.0, 1.0};
static GLfloat material_diffuse_a[4] = {1.0, 0.5, 0.5, 1.0};
static GLfloat material_specular_a[4] = {1.0, 0.5, 0.5, 1.0};
static GLfloat material_shininess_a = {50.0};

/* material wall */
static GLfloat material_ambient_w[4] = {0.0, 0.1, 0.0, 1.0};
static GLfloat material_diffuse_w[4] = {0.1, 1.0, 0.1, 1.0};
static GLfloat material_specular_w[4] = {0.1, 1.0, 0.1, 1.0};
static GLfloat material_shininess_w = {50.0};

static GLfloat material_ambient_w2[4] = {0.0, 0.25, 0.0, 1.0};
/*  static GLfloat material_ambient_w2[4] = {0.0, 0.2, 0.0, 1.0};  */  
static GLfloat material_diffuse_w2[4] = {0.2, 1.0, 0.2, 1.0};
static GLfloat material_specular_w2[4] = {0.2, 1.0, 0.2, 1.0};
static GLfloat material_shininess_w2 = {50.0};

/* material surface */
static GLfloat material_ambient_s[4] = {0.3, 0.0, 0.0, 1.0};
static GLfloat material_diffuse_s[4] = {1.0, 0.5, 0.5, 1.0};
static GLfloat material_specular_s[4] = {1.0, 0.5, 0.5, 1.0};
static GLfloat material_shininess_s = {50.0};

/* clear */
static GLfloat material_ambient_cl[4] = {0.0, 0.0, 0.0, 0.0};
static GLfloat material_diffuse_cl[4] = {0.0, 0.0, 0.0, 0.0};
static GLfloat material_specular_cl[4] = {0.0, 0.0, 0.0, 0.0};
static GLfloat material_shininess_cl = {0.0};


/*  circle colors  */
static double circle_r[3]  = {0.8, 0.0, 0.0};
static double circle_y[3]  = {0.8, 0.8, 0.0};
static double circle_b[3]  = {0.0, 0.0, 1.0};
static double circle_lb[3] = {0.0, 0.8, 1.0};
static double circle_wh[3] = {1.0, 1.0, 1.0};
static double circle_g[3]  = {0.0, 1.0, 0.0};
static double circle_c[3]  = {0.6, 0.6, 1.0};
static double circle_gr[3] = {0.6, 0.6, 0.6};
static double circle_m[3]  = {0.6, 0.0, 0.6};
static double circle_a[3]  = {0.5, 0.0, 0.0};
static double circle_w[3]  = {0.0, 0.5, 0.0};
static double circle_w2[3] = {0.0, 0.3, 0.0};
static double circle_s[3]  = {0.3, 0.0, 0.0};





static GLdouble spin_x=0.;
static GLdouble spin_y=0.;
static GLint dragFlag;
static GLint x_mem1=0.;
static GLint y_mem1=0.;

static GLint x_mem2=0.;
static GLint y_mem2=0.;




void initialize_light(void)
{
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

  glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);

  glEnable(GL_DEPTH_TEST);
} /* end of initialize_light */



void initialize(void)
{
  double scope_rad,fovy_rad,fovy_deg; 

  scope_rad=SCOPE*M_PI/180.0;
  fovy_rad=2.0*atan(tan(scope_rad/2.0)/ASPECT);
  fovy_deg=fovy_rad*180.0/M_PI;

  glClearColor(0.0, 0.0, 0.0, 0.0);
  //glClearColor(1.0, 1.0, 1.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glMatrixMode(GL_PROJECTION);   /* set up projection matrix */
  glLoadIdentity();  /* reset coordinate transform matrix */

  gluPerspective(fovy_deg, ASPECT, ZNEAR, ZFAR);  /* set up projection range */
} /* end of initialize */



void keyboard(unsigned char key, int x, int y)
{
  if (key == ESC)                                       /*  ESC      : end  */
    exit(0);
  if (key == '\040')                                    /*  スペース : stop、start  */
    stop *= -1;                                       
  if (key == 98)                                        /*  b        : back  */
    back *= -1;
  if (key == 102)                                       /*  f        : fast  */
    f_split += SPLIT;
  if (key == 115 && f_split>SPLIT)                      /*  s        : slow  */
    f_split -= SPLIT;
} /* end of keybord */



void mouse(int button, int state, int x,int y)
{
  switch(button)
    {
    case GLUT_LEFT_BUTTON:
      if(state == GLUT_DOWN)
				fnum+=SPLIT;
    
      glutPostRedisplay();   /* request to redraw screen */
      break;

    default:
      break;
    }
} /* end of mouse */



void motion(int x, int y)
{
  if(dragFlag==STATE_DOWN)
    {
      if(x_mem1!=x)
				spin_x += 300.0*2.0*(x_mem1-x)/500;
      if(y_mem1!=y)
				spin_y += 300.0*2.0*(y_mem1-y)/500;
      if(spin_x>360.0)
				spin_x -= 360.0;
      else if(spin_x<-360.0)
				spin_x += 360.0;
      if(spin_y>369.0)
				spin_y -= 360.0;

      else if(spin_y<-360.0)
				spin_y += 360.0;
      glutPostRedisplay();
      x_mem2=x_mem1;
      y_mem2=y_mem1;
      x_mem1=x;
      y_mem1=y;
    }
} /* end of motion */



void drawParticle(void)
{
  int    *mark;
  double *xloc,*yloc,*zloc,*rloc,*tloc; 
  double *uloc,*vloc,*w,*density,*another; 

  int    pno,pnomax;
  int    i,j,k,n;
  int    tempint1,tempint2; 
  double temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp9,temp10;
  char   filename[100],temp8;
  FILE   *fp;
  
  double dis_x,dis_y,dis_z;
   
  double d_value,value;
  double rad;


  mark    = (int *)malloc(sizeof(int)*PNOLMT);
  xloc    = (double *)malloc(sizeof(double)*PNOLMT);
  yloc    = (double *)malloc(sizeof(double)*PNOLMT);
  zloc    = (double *)malloc(sizeof(double)*PNOLMT);
  rloc    = (double *)malloc(sizeof(double)*PNOLMT);
  tloc    = (double *)malloc(sizeof(double)*PNOLMT);
  uloc    = (double *)malloc(sizeof(double)*PNOLMT);
  vloc    = (double *)malloc(sizeof(double)*PNOLMT);
  w       = (double *)malloc(sizeof(double)*PNOLMT);
  density = (double *)malloc(sizeof(double)*PNOLMT);
  another = (double *)malloc(sizeof(double)*PNOLMT);

#if !SPHERE
  n = 8;
  rad = 2.0*M_PI/n;
#endif

 
  sprintf(filename,"%s_nor.%05d",PATH,fnum);              
  fp=fopen(filename,"r");
  if(fp==NULL){printf("\n Cannot find the file %s\n",filename);exit(1);}  
  

  fscanf(fp,"%d",&pnomax);
  
  if (fnum == 0)
    printf("reading file  %s\nnumber of particles = %d\n",PATH,pnomax);
  if (fnum%10 == 0)
    printf("file number = %05d\n",fnum);
  
  
  pno=0;
  while(1)
    {
      if(fscanf(fp,"%lf %lf %lf %lf %lf %lf %d",
								&temp1,&temp2,&temp3,&temp4,&temp5,&temp6,&tempint1)!=7)
				break;


      xloc[pno]    = temp1;
      yloc[pno]    = temp2;
      zloc[pno]    = 0.0;       
      uloc[pno]    = temp3;
      vloc[pno]    = temp4;
      tloc[pno]    = temp5;
      density[pno] = temp6;
      mark[pno]    = tempint1;
      /*  rloc[pno]   = 0.0; */      
      /*  w[pno]=temp6;      */
        
      pno++;
    }
  
  fclose(fp);



  pnomax=pno;
  for (pno=0;pno<pnomax;pno++)
    {
      dis_x=xloc[pno]/P_SPACING;
      dis_y=yloc[pno]/P_SPACING;
      dis_z=zloc[pno]/P_SPACING;
      
      /*  when you draw ball  */
#if SPHERE
      if(mark[pno]>=-1 && mark[pno]<=300)
				{
					glPushMatrix();
	  
					glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient_lb);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_lb);
					glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular_lb);
					glMaterialf(GL_FRONT, GL_SHININESS, material_shininess_lb);
	  
	  
					if(mark[pno]==-1)
						{
							glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient_w);
							glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_w);
							glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular_w);
							glMaterialf(GL_FRONT, GL_SHININESS, material_shininess_w);
						}
	  
					if(mark[pno]==1)
						{ 
							glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient_lb);
							glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_lb);
							glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular_lb);
							glMaterialf(GL_FRONT, GL_SHININESS, material_shininess_lb);
						}
	  
					if(mark[pno]>=2 && mark[pno]<100 && mark[pno]%2==0)
						{
							glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient_y);
							glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_y);
							glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular_y);
							glMaterialf(GL_FRONT, GL_SHININESS, material_shininess_y);
						}
	  
					if(mark[pno]>=2 && mark[pno]<100 && mark[pno]%2==1)
						{
							glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient_r);
							glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_r);
							glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular_r);
							glMaterialf(GL_FRONT, GL_SHININESS, material_shininess_r);
						}
	  
					/*
						if(mark[pno]==4)
						{
						glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient_y);
						glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_y);
						glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular_y);
						glMaterialf(GL_FRONT, GL_SHININESS, material_shininess_y);
						}
					*/	  

					if(mark[pno]==100)
						{
							glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient_w);
							glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_w);
							glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular_w);
							glMaterialf(GL_FRONT, GL_SHININESS, material_shininess_w);
						}
	  
					if(mark[pno]==101)
						{
							glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient_gr);
							glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_gr);
							glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular_gr);
							glMaterialf(GL_FRONT, GL_SHININESS, material_shininess_gr);
						}
	  
					if(mark[pno]==150)
						{
							glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient_w);
							glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_w);
							glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular_w);
							glMaterialf(GL_FRONT, GL_SHININESS, material_shininess_w);
						}
	  
					if(mark[pno]==200)
						{
							glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient_gr);
							glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_gr);
							glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular_gr);
							glMaterialf(GL_FRONT, GL_SHININESS, material_shininess_gr);
						}
	  
	  
					/* else if(mark[pno]==0)
						 {
						 glPushMatrix();
						 glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient_c);
						 glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_c);
						 glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular_c);
						 glMaterialf(GL_FRONT, GL_SHININESS, material_shininess_c);
						 glPopMatrix();	    
	     
						 glTranslatef(dis_x,dis_y,dis_z);
						 glutSolidSphere(P_R, P_SHAPE1, P_SHAPE2);  
						 glTranslatef(-dis_x,-dis_y,-dis_z);
						 }
					*/
	  
					glPopMatrix();	    
	  
					glTranslatef(dis_x,dis_y,dis_z);
					glutSolidSphere(P_R, P_SHAPE1, P_SHAPE2);    /*  particle diameter, shape  */
					glTranslatef(-dis_x,-dis_y,-dis_z);
				}
#endif



      /*  when you draw circle  */
#if !SPHERE
      glDisable(GL_LIGHTING);
      glDisable(GL_DEPTH_TEST) ;
      glPushAttrib(GL_CURRENT_BIT);   


      if(mark[pno]%2==0) glColor3d(circle_r[0],  circle_r[1],  circle_r[2]);
      if(mark[pno]%2==1) glColor3d(circle_y[0],  circle_y[1],  circle_y[2]);

      if(mark[pno]==-1)  glColor3d(circle_w[0],  circle_w[1],  circle_w[2]);
      if(mark[pno]==1)   glColor3d(circle_lb[0], circle_lb[1], circle_lb[2]);
      if(mark[pno]==100) glColor3d(circle_w[0],  circle_w[1],  circle_w[2]);
      if(mark[pno]==101) glColor3d(circle_wh[0],  circle_wh[1],  circle_wh[2]);
      if(mark[pno]==200) glColor3d(circle_gr[0], circle_gr[1], circle_gr[2]);
      
           
      glBegin(GL_POLYGON);
      for(i=0; i<=n; i++)
				glVertex2d(dis_x+P_R*cos(rad*i) ,dis_y+P_R*sin(rad*i));
      glEnd();
	  

      glPopAttrib();
      glEnable(GL_DEPTH_TEST) ;     
      glEnable(GL_LIGHTING);
        
#endif


    }
     
}



void drawObject(void)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  /*
    glRotatef(spin_x,0.0,1.0,0.0);
    glRotatef(spin_y,1.0,0.0,0.0);
    glColor3f(1.0,1.0,1.0);
  */
  
  glPushMatrix();
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  glPopMatrix();
  
  glPushMatrix();
  
  gluLookAt(EYEX,EYEY,EYEZ,CENTERX,CENTERY,CENTERZ,UPX,UPY,UPZ);

  // glTranslatef(-1.0, -0.5, 1.0);
 
  glPushMatrix();
  drawParticle();
  glPopMatrix();
} /* end of drawObject */



void import(void)
{
  char command[200],command_b[100];
  char name_bmp[100],name_ext[100];
 
  sprintf(name_bmp,"%s%04d.bmp",SAVE_PATH,(int)(fnum/SPLIT));
  sprintf(name_ext,"%s%04d.%s",SAVE_PATH,(int)(fnum/SPLIT),EXTENSION);

 
  WriteBitmap(name_bmp,WINDOW_WIDTH,WINDOW_HEIGHT);           /*  make bmp  */
  if(EXTENSION!="bmp")
    {
      sprintf(command,"convert %s %s",name_bmp,name_ext);     /*   convert    */
      system(command); 
      sprintf(command,"rm %s",name_bmp);                      /*  clear bmp  */
      system(command);
    }

  /*   
			 sprintf(command_b,"xset b off");
			 system(command_b);
			 sprintf(command,"/usr/bin/import %s -window %s",name_jpg);
			 system(command);
			 sprintf(command_b,"xset b on");
			 system(command_b);
  */
  
} /* end of import */



void display(void)
{    
  if(stop==-1)
    {
      glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT); 
      
      drawObject();
      
      glutSwapBuffers();
      
#if SAVE
      import();
#endif
      
      if(back==-1)
				fnum+=f_split;
      if(back==1)
				fnum-=f_split;
      
      glutPostRedisplay();
    }
}


void idle(void)
{
  if (fnum>FILE_END){
    printf("file (%d) end\n",fnum);
    exit(1);
  }

  if(stop==-1)
    display();
  /* if(dragFlag==STATE_UP)
     {
     spin_x += 300.0*2.0*(x_mem2-x_mem1)/500;
     spin_y += 300.0*2.0*(y_mem2-y_mem1)/500;
     glutPostRedisplay();
     }*/
}


int main(int argc, char** argv)
{
  stop = -1;
  back = -1;
  f_split = SPLIT;

  fnum=(int)FILE_INIT;

  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
  glutInitWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT);              /* max window  */
  glutInitWindowPosition( WINDOW_X, WINDOW_Y );
  glutCreateWindow(WINDOW_NAME);
  
  initialize_light();
  initialize();
 
  glutKeyboardFunc(keyboard);
  
#if MOUSE
  glutMouseFunc(mouse);
#endif
  /* glutMotionFunc(motion);*/
  glutDisplayFunc(display);
 
  glutIdleFunc(idle);  
 
  glutMainLoop();   
  return 0;
}
