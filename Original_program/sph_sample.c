/* Smoothed Particle Hydrodynamics for 2d */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "sph_sample.h"

/*
	1.	ini_pos
	2.	save_pre
	3.	kyoukai			-boundary function which moves particles from on side to another
	4. 	grouping
	5.	density
	6.	weighted_center
	7.	acceleration
	8.	smoothing
	9.	real_move
	10.	move
	11.	velocity_step
	12.	save_temp
	13.	load_temp
	14. 	save_file
	15.	read_file
	16.	viscosity_heat
	17.	compute_mean_density

	18.  	int main
*/

void ini_pos(pp,xp,yp,up,vp,tp,dp,mp,axp,ayp)
     
     int *pp;
     double xp[],yp[];
     double up[],vp[];
     double tp[],dp[];
     int mp[];
     double axp[],ayp[];
{
  int i,j,count,color;
   
  *pp=0;

  for(i=0;i<PXLMT;i++)
    for(j=0;j<PYLMT;j++)
      {
	xp[*pp]=(i+0.5)*P_SPACING;
	yp[*pp]=(j+0.5)*P_SPACING;
	up[*pp]=vp[*pp]=0.0;
	
	if(j<ATUMI)                     	  
	  {
	    dp[*pp]=M_WALL/(H*H);
	    mp[*pp]=100;
	  }

	
	if(j>PYLMT-ATUMI-1)                     	  
	  {
	    //up[*pp]=U;
	    dp[*pp]=M_WALL/(H*H);
	    mp[*pp]=100;
	    //if(i<10) mp[*pp] = 101;
	  }
	
	if(j>=ATUMI && j<=PYLMT-ATUMI-1)
	  {
	    dp[*pp]=RHO_WATER;
	    up[*pp]=U;
	    mp[*pp]=1;    
	  }
	
/*	if(i>PXLMT*4/10 && i<PXLMT*4/10+ATUMI) itse tehty
	{

		if(j<PYLMT/2-ATUMI && j>=ATUMI)  
		{
		
			dp[*pp]=M_WALL/(H*H);
	    		up[*pp]=0;
	    		mp[*pp]=100;
			//if(i<10) mp[*pp] = 101;
		}
	} /* itse tehty paattyy*/

	tp[*pp]=20;
	axp[*pp]=ayp[*pp]=0.0; 
	(*pp)++;	
      }


  /*   y axial marker  */
  count = 0;
  color = 2;
  for(i=0; i<(*pp); i++)
    {
      if(i>=count && i<3*PYLMT+count && mp[i]==1)
	mp[i] = color;
      if(i==3*PYLMT+count)
	{
	  count += 40*PYLMT;
	  color += 1;
	}
    }
  
  
  if(*pp>=PNLMT)
    {
      printf("Too many wall particles: straight!!\n");
      exit(1);
    }
      
}/* initial_position ends here*/



void save_pre(np,xp,yp,up,vp,tp,xpp,ypp,upp,vpp,tpp)
     int np;
     double *xp,*yp,*xpp,*ypp,*tp,*tpp,*up,*vp,*upp,*vpp;
{
  int i;
  
  for(i=0;i<np;i++)
    {
      xpp[i]=xp[i];
      ypp[i]=yp[i];
      tpp[i]=tp[i];
      upp[i]=up[i];
      vpp[i]=vp[i];
    }
  
}/* save_pre ends here */



void kyoukai(np,xp,yp,mp)
     int np,mp[];
     double xp[],yp[];
{
  int i;
  
  for(i=0;i<np;i++)
    {
      if(mp[i]!=-1)  
	{
	  
#if SHUUKIX
	  if(xp[i]<0.0)
	    xp[i]+=(double)(PXLMT*P_SPACING);
	  if(xp[i]>=(double)(PXLMT*P_SPACING))
	    xp[i]-=(double)(PXLMT*P_SPACING);
	  
	  if(xp[i]<0.0||xp[i]>=(double)(PXLMT*P_SPACING))
	    {
	      printf("(in move)particle%d in minus region at x=%lf,y=%lf\n",i,xp[i],yp[i]);
	      exit(1);
	    }
#endif   
	  
#if SHUUKIY
	  if(yp[i]<0.0)
	    yp[i]+=(double)(PYLMT*P_SPACING);
	  if(yp[i]>=(double)(PYLMT*P_SPACING))
	    yp[i]-=(double)(PYLMT*P_SPACING);
	  
	  if(yp[i]<0.0||yp[i]>=(double)(PYLMT*P_SPACING))
	    {
	      printf("(in move)particle%d in minus region at x=%lf,y=%lf\n",i,xp[i],yp[i]);
	      exit(1);
	    }
#endif   
	  
	}
    }
}/* kyoukai ends here */



void grouping(np,xp,yp,mp,pqz,pnz_pn) 
     int np;
     double xp[],yp[];
     int mp[],pqz[],pnz_pn[];
{
  int i;
  int zx,zy; /*zone x and y (zone ichi)*/
  int nzt; /* number in zone temp */
  double zlx,zly; /*zone length x and y*/
 
  zlx=P_SPACING*(double)PXLMT/(double)(ZXLMT);
  zly=P_SPACING*(double)PYLMT/(double)(ZYLMT);
  
  /* clear */
  for(i=0;i<ZXLMT*ZYLMT;i++)pqz[i]=0;                    /* particle in zone*/
  for(i=0;i<ZXLMT*ZYLMT*QPinZONE_MAX;i++)pnz_pn[i]=0;   /* zone where particle is*/
  
  /* start to separate zone*/
  for(i=0;i<np;i++)
    {
      if(mp[i]!=-1)                
	{
	  
	  zx=(int)floor(xp[i]/zlx);
	  if(zx<0)
	    {
	      printf("particle %d out of xzone %d\n",i,zx);
	      zx=0;
	    }
	  if(zx>=ZXLMT)
	    {
	      printf("particle %d out of xzone %d\n",i,zx);
	      zx=ZXLMT-1;
	    }
	  
	  zy=(int)floor(yp[i]/zly);                   
	  if(zy<0)
	    {
	      printf("particle %d out of yzone %d\n",i,zy);
	      zy=0;
	    }
	  
	  if(zy>=ZYLMT)
	    {
	      printf("particle %d out of yzone %d\n",i,zy);
	      zy=ZYLMT-1;
	    }
	  

	  nzt=pqz[zx*ZYLMT+zy];
	  pnz_pn[(zx*ZYLMT+zy)*QPinZONE_MAX+nzt]=i;
	  pqz[zx*ZYLMT+zy]++;
	  nzt++;
	  if(nzt>=QPinZONE_MAX)
	    {
	      printf("too many(%d)particle in zone(%d,%d)\n",nzt,zx,zy);
	      exit(1);
	    }
	}
    }
}/* grouping ends here */



void density(np,xp,yp,mp,dp,wcx,wcy,pqz,pnz_pn,pqzw,pnz_pnw)
     int np;
     double xp[],yp[],dp[],wcx[],wcy[];
     int mp[],pqz[],pnz_pn[],pqzw[],pnz_pnw[];
{
  int i,j;
  int zx1,zy1,zx2,zy2,zx2t,zy2t,p1,p2;
  double c1; /* constant numver*/
  double lx,hlx,ly,hly; /* length and half length */
  double q; /* q repressent distance . When q=1,  distance=H  */
  double dis,dissq,disx,disy;
  double kernel; /* kernel height */
  double dmax,dmin; /* density max and density min */
  double mass1,mass2;

  hlx=0.5*(double)PXLMT*P_SPACING;
  lx=(double)PXLMT*P_SPACING;
  hly=0.5*(double)PYLMT*P_SPACING;
  ly=(double)PYLMT*P_SPACING;

  c1=10.0/(7.0*M_PI*H*H); /* case 2d  */
  
  /* clear */
  for(i=0;i<np;i++)
    {
      dp[i]=0.0;
    }

  /*compute density*/
  for(zx1=0;zx1<ZXLMT;zx1++)
    for(zy1=0;zy1<ZYLMT;zy1++)      
      {       
	for(zy2t=zy1-1;zy2t<=zy1+1;zy2t++){ 
#if !SHUUKIY
	  if(zy2t>=0&&zy2t<ZYLMT){
#endif
	    zy2=zy2t;		
	    if(zy2<0)zy2+=ZYLMT;
	    if(zy2>=ZYLMT)zy2-=ZYLMT; 
	    
	    for(zx2t=zx1-1;zx2t<=zx1+1;zx2t++){
#if !SHUUKIX
	      if(zx2t>=0&&zx2t<ZXLMT){
#endif		   
		zx2=zx2t;
		if(zx2<0)zx2+=ZXLMT;
		if(zx2>=ZXLMT)zx2-=ZXLMT;
			
		for(i=0;i<pqzw[zx1*ZYLMT+zy1];i++)
		  {
		    p1=pnz_pnw[(zx1*ZYLMT+zy1)*QPinZONE_MAX+i];
		    
		    for(j=0;j<pqz[zx2*ZYLMT+zy2];j++)
		      {
			p2=pnz_pn[(zx2*ZYLMT+zy2)*QPinZONE_MAX+j];
			if(mp[p1]!=-1&&mp[p2]!=-1)
			  {
			 
			    disx=xp[p2]-wcx[p1];
#if SHUUKIX
			    if(disx>hlx)disx-=lx;
			    if(disx<-hlx)disx+=lx;
#endif
			    disy=yp[p2]-wcy[p1];
#if SHUUKIY
			    if(disy>hly)disy-=ly;
			    if(disy<-hly)disy+=ly;
#endif
			    dissq=sqr(disx)+sqr(disy);
			    dis=sqrt(dissq);
			    q=dis/H;
			    
			    if(q<2.0)
			      {
				if(q<1.0)kernel=c1*(1.0-1.5*q*q+0.75*q*q*q);
				else kernel=c1*(2.0-3.0*q+1.5*q*q-0.25*q*q*q);

				if(mp[p1]<100) mass1=M_LIQUID;
				if(mp[p1]>=100) mass1=M_WALL;
				if(mp[p2]<100) mass2=M_LIQUID;
				if(mp[p2]>=100) mass2=M_WALL;
			
				dp[p1]+=mass2*kernel;    
				
			      }
			  }
		      }
		    
		  }
#if !SHUUKIX
	      }
#endif
	    }
#if !SHUUKIY
	  }
#endif
	}
      }

  dmax=0.0;
  dmin=1.0e10;  
  for(i=0;i<np;i++){
      if(mp[i]!=-1&&mp[i]<100)
	{
	  if(dp[i]>dmax)dmax=dp[i];
	  if(dp[i]<dmin)dmin=dp[i];
	}
  }


}/* compute_density ends here */



void weighted_center(np,xp,yp,mp,dp,wcx,wcy,pqz,pnz_pn)
     int np;
     double xp[],yp[],dp[],wcx[],wcy[];
     int mp[],pqz[],pnz_pn[];
{
  int i,j;
  int zx1,zy1,zx2,zy2,zx2t,zy2t,p1,p2;
  double c1; /* constant number */
  double lx,hlx,ly,hly; /* length and half length */
  double q; /*q repressent distance . When q=1,  distance=H */
  double dis,dissq,disx,disy;
  double kernel; /* kernel height */
  double wet[PNLMT]; /* stands for weight */

  hlx=0.5*(double)PXLMT*P_SPACING;
  lx=(double)PXLMT*P_SPACING;
  hly=0.5*(double)PYLMT*P_SPACING;
  ly=(double)PYLMT*P_SPACING;

  c1=10.0/(7.0*M_PI*H*H); /* 2d no baai */
  
  /* clear */
  for(i=0;i<np;i++)
    {
      
      wcx[i]=c1*xp[i];
      wcy[i]=c1*yp[i];
      wet[i]=c1;
    }

#if 1

  /*compute weighted center*/
  for(zx1=0;zx1<ZXLMT;zx1++)
    for(zy1=0;zy1<ZYLMT;zy1++)
      {       
	for(zy2t=zy1-1;zy2t<=zy1+1;zy2t++){
#if !SHUUKIY
	  if(zy2t>=0&&zy2t<ZYLMT){
#endif
	    zy2=zy2t;		
	    if(zy2<0)zy2+=ZYLMT;
	    if(zy2>=ZYLMT)zy2-=ZYLMT;
	    
	    for(zx2t=zx1-1;zx2t<=zx1+1;zx2t++){
#if !SHUUKIX
	      if(zx2t>=0&&zx2t<ZXLMT){
#endif		   
		zx2=zx2t;
		if(zx2<0)zx2+=ZXLMT;
		if(zx2>=ZXLMT)zx2-=ZXLMT;
			
		for(i=0;i<pqz[zx1*ZYLMT+zy1];i++)
		  {
		    p1=pnz_pn[(zx1*ZYLMT+zy1)*QPinZONE_MAX+i];
		    
		    for(j=0;j<pqz[zx2*ZYLMT+zy2];j++)
		      {
			p2=pnz_pn[(zx2*ZYLMT+zy2)*QPinZONE_MAX+j];
			if(p1<p2&&mp[p1]!=-1&&mp[p2]!=-1)
			  {
			    disx=xp[p2]-xp[p1];
#if SHUUKIX
			    if(disx>hlx)disx-=lx;               /*  周期により -hlx <= disx <= hlx  */
			    if(disx<-hlx)disx+=lx;
#endif
			    disy=yp[p2]-yp[p1];
#if SHUUKIY
			    if(disy>hly)disy-=ly;               /*  周期により -hly <= disx <= hly  */
			    if(disy<-hly)disy+=ly;
#endif
			    dissq=sqr(disx)+sqr(disy);
			    dis=sqrt(dissq);
			    q=dis/H;
			    
			    if(q<2.0)
			      {
				if(q<1.0)
				  {
				    kernel=c1*(1.0-1.5*q*q+0.75*q*q*q);
				  } 
				else 
				  {
				    kernel=c1*(2.0-3.0*q+1.5*q*q-0.25*q*q*q);
				  } 
				
				wcx[p1]+=kernel*(xp[p1]+disx);
				wcy[p1]+=kernel*(yp[p1]+disy);
				wet[p1]+=kernel;

				wcx[p2]+=kernel*(xp[p2]-disx);
				wcy[p2]+=kernel*(yp[p2]-disy);
				wet[p2]+=kernel;
	
			      }
			  }
		      }
		    
		  }
#if !SHUUKIX
	      }
#endif
	    }
#if !SHUUKIY
	  }
#endif
	}
      }

  for(i=0;i<np;i++)
    {
      if(wet[i]!=0.0)
	{
	  wcx[i]/=wet[i];
	  wcy[i]/=wet[i];
	}
    }
#endif

}/* compute_weighted_center ends here */



void acceleration(np,xp,yp,mp,dp,axp,ayp,pqz,pnz_pn)
     int np;
     double xp[],yp[],dp[],axp[],ayp[];
     int mp[],pqz[],pnz_pn[];
{
  int i,j,p1,p2;
  int zx1,zy1,zx2,zy2,zx2t,zy2t;
  double kernel,der_k;
  double q;
  double c1; /* constant number */
  double dis,disx,disy;
  double lx,hlx,ly,hly;

  hlx=0.5*(double)PXLMT*P_SPACING;
  lx=(double)PXLMT*P_SPACING;
  hly=0.5*(double)PYLMT*P_SPACING;
  ly=(double)PYLMT*P_SPACING;

  c1=10.0/(7.0*M_PI*H*H); /* case 2d  */

  /* clear */
  for(i=0;i<np;i++)axp[i]=ayp[i]=0.0;

  /*compute acceleration*/
  for(zx1=0;zx1<ZXLMT;zx1++)
    for(zy1=0;zy1<ZYLMT;zy1++)
      {
	for(zy2t=zy1-1;zy2t<=zy1+1;zy2t++){
#if !SHUUKIY
	  if(zy2t>=0&&zy2t<ZYLMT){
#endif
	    zy2=zy2t;		
	    if(zy2<0)zy2+=ZYLMT;
	    if(zy2>=ZYLMT)zy2-=ZYLMT;
	    
	    for(zx2t=zx1-1;zx2t<=zx1+1;zx2t++){
#if !SHUUKIX
	      if(zx2t>=0&&zx2t<ZXLMT){
#endif		   
		zx2=zx2t;
		if(zx2<0)zx2+=ZXLMT;
		if(zx2>=ZXLMT)zx2-=ZXLMT;


		for(i=0;i<pqz[zx1*ZYLMT+zy1];i++)
		  {
		    p1=pnz_pn[(zx1*ZYLMT+zy1)*QPinZONE_MAX + i];
		    
		    for(j=0;j<pqz[zx2*ZYLMT+zy2];j++)
		      {
			p2=pnz_pn[(zx2*ZYLMT+zy2)*QPinZONE_MAX + j];
			
			if(p1<p2&&mp[p1]!=-1&&mp[p2]!=-1)
			  {
			    disx=xp[p2]-xp[p1];
#if SHUUKIX
			    if(disx>hlx)disx-=lx;
			    if(disx<-hlx)disx+=lx;
#endif
			    disy=yp[p2]-yp[p1];
#if SHUUKIY
			    if(disy>hly)disy-=ly;
			    if(disy<-hly)disy+=ly;
#endif
			    dis=sqrt(sqr(disx)+sqr(disy));
			    q=dis/H;
			    
			    if(q<2.0)
			      {
				if(q<1.0)
				  {
				    kernel=c1*(1.0-1.5*q*q+0.75*q*q*q);
				    der_k=c1*(-3.0*q+2.25*q*q)/H;
				  } 
				else 
				  {
				    kernel=c1*(2.0-3.0*q+1.5*q*q-0.25*q*q*q);
				    der_k=c1*(-3.0+3.0*q-0.75*q*q)/H;
				  } 
				
				axp[p1]+=M_LIQUID*(C_LIQUID*C_LIQUID*(dp[p1]-BG*RHO_LIQUID)/sqr(dp[p1])
						  +C_LIQUID*C_LIQUID*(dp[p2]-BG*RHO_LIQUID)/sqr(dp[p2]))*disx/dis*der_k;
				ayp[p1]+=M_LIQUID*(C_LIQUID*C_LIQUID*(dp[p1]-BG*RHO_LIQUID)/sqr(dp[p1])
						  +C_LIQUID*C_LIQUID*(dp[p2]-BG*RHO_LIQUID)/sqr(dp[p2]))*disy/dis*der_k;
				axp[p2]-=M_LIQUID*(C_LIQUID*C_LIQUID*(dp[p1]-BG*RHO_LIQUID)/sqr(dp[p1])
						  +C_LIQUID*C_LIQUID*(dp[p2]-BG*RHO_LIQUID)/sqr(dp[p2]))*disx/dis*der_k;
				ayp[p2]-=M_LIQUID*(C_LIQUID*C_LIQUID*(dp[p1]-BG*RHO_LIQUID)/sqr(dp[p1])
						  +C_LIQUID*C_LIQUID*(dp[p2]-BG*RHO_LIQUID)/sqr(dp[p2]))*disy/dis*der_k;
				
			      }/*if(q<2.0)*/		
			  } /* if(p1<p2) */
		      }  
		    
		  }		
#if !SHUUKIX
	      }
#endif
	    }
#if !SHUUKIY
	  }
#endif
	}
      }
  
}/* compute acceleration ends here */



void smoothing(np,xp,yp,mp,axp,ayp,pqz,pnz_pn)
     int np;
     double xp[],yp[],axp[],ayp[];
     int mp[],pqz[],pnz_pn[];
{
  int i,j,p1,p2;
  int zx1,zy1,zx2,zy2,zx2t,zy2t;
  double kernel;
  double q;
  double c1; /* teisuu */
  double dis,dissq,disx,disy;
  double lx,hlx,ly,hly;
  double axpt[PNLMT],aypt[PNLMT]; /* stands for acceleration temp */
  double wet[PNLMT]; /* stands for weight temp */

  hlx=0.5*(double)PXLMT*P_SPACING;
  lx=(double)PXLMT*P_SPACING;
  hly=0.5*(double)PYLMT*P_SPACING;
  ly=(double)PYLMT*P_SPACING;

  c1=10.0/(7.0*M_PI*H*H); /* case 2d */

  /* clear */
  for(i=0;i<np;i++)
    {
      axpt[i]=axp[i]*c1;
      aypt[i]=ayp[i]*c1;
      wet[i]=c1;
    }

  /*compute acceleration*/
  for(zx1=0;zx1<ZXLMT;zx1++)
    for(zy1=0;zy1<ZYLMT;zy1++)
      {
	for(zy2t=zy1-1;zy2t<=zy1+1;zy2t++){
#if !SHUUKIY
	  if(zy2t>=0&&zy2t<ZYLMT){
#endif
	    zy2=zy2t;		
	    if(zy2<0)zy2+=ZYLMT;
	    if(zy2>=ZYLMT)zy2-=ZYLMT;
	    
	    for(zx2t=zx1-1;zx2t<=zx1+1;zx2t++){
#if !SHUUKIX
	      if(zx2t>=0&&zx2t<ZXLMT){
#endif		   
		zx2=zx2t;
		if(zx2<0)zx2+=ZXLMT;
		if(zx2>=ZXLMT)zx2-=ZXLMT;

		for(i=0;i<pqz[zx1*ZYLMT+zy1];i++)
		  {
		    p1=pnz_pn[(zx1*ZYLMT+zy1)*QPinZONE_MAX+i];
		 
		    for(j=0;j<pqz[zx2*ZYLMT+zy2];j++)
		      {
			p2=pnz_pn[(zx2*ZYLMT+zy2)*QPinZONE_MAX+j];
			if(p1<p2&&mp[p1]==mp[p2])
			  {
			    disx=xp[p2]-xp[p1];
#if SHUUKIX
			    if(disx>hlx)disx-=lx;
			    if(disx<-hlx)disx+=lx;
#endif
			    disy=yp[p2]-yp[p1];
#if SHUUKIY
			    if(disy>hly)disy-=ly;
			    if(disy<-hly)disy+=ly;
#endif
			    dissq=sqr(disx)+sqr(disy);
			    dis=sqrt(dissq);
			    q=dis/H;
			    
			    if(q<2.0)
			      {
				if(q<1.0) kernel=c1*(1.0-1.5*q*q+0.75*q*q*q);
				else kernel=c1*(2.0-3.0*q+1.5*q*q-0.25*q*q*q);
				   
				axpt[p1]+=axp[p2]*kernel;
				aypt[p1]+=ayp[p2]*kernel;
				wet[p1]+=kernel;
				axpt[p2]+=axp[p1]*kernel;
				aypt[p2]+=ayp[p1]*kernel;
				wet[p2]+=kernel;
			      }
			  }
		      }
		  }
#if !SHUUKIX
	      }
#endif
	    }
#if !SHUUKIY
	  }
#endif
	}
      }

  for(i=0;i<np;i++)
    {
      if(mp[i]!=-1)
	{
	  axp[i]=(1.0-EX_A)*axp[i]+EX_A*axpt[i]/wet[i];
	  ayp[i]=(1.0-EX_A)*ayp[i]+EX_A*aypt[i]/wet[i];
	}
    }

}/* smoothing ends here */



void real_move(np,xp,yp,xpp,ypp,up,vp,mp,smallstep)
     
     int np;
     double xp[],yp[],xpp[],ypp[],up[],vp[];
     int mp[];
     double smallstep;
{
  int i;
  
  /* water particles move */
  for(i=0;i<np;i++)
    {
      if(mp[i]<100 && mp[i]!=-1)
	{
	  xp[i]=xpp[i]+up[i]*smallstep*DT;
	  yp[i]=ypp[i]+vp[i]*smallstep*DT;
	}

      if(mp[i]==100 || mp[i]==101)                                         /*  wall move  */
	{
	  xp[i]=xpp[i]+up[i]*smallstep*DT;
	  yp[i]=ypp[i];
	}
    }
}/* real_move ends here */



void move(np,xp,yp,axp,ayp,mp,step)
     int np;
     double xp[],yp[],axp[],ayp[];
     int mp[];
     double step;
{
  int i;

  /* water particles move */
  for(i=0;i<np;i++)
    {
      if(mp[i]<100&&mp[i]!=-1)
	{
	  xp[i]+=0.5*axp[i]*sqr(DT*step);
	  yp[i]+=0.5*ayp[i]*sqr(DT*step);
	}
    }
}/* move ends here */


void velocity_step(np,xp,yp,xpp,ypp,ups,vps,mp,step,buo)
     int np;
     double step;
     double xp[],yp[],xpp[],ypp[],ups[],vps[],buo[];
     int mp[];
{
  int i;

  for(i=0;i<np;i++)
    {
      if(mp[i]<100&&mp[i]!=-1)
	{
	  
#if SHUUKIX
	  if(xp[i]-xpp[i]>0.5*PXLMT*P_SPACING)xpp[i]+=PXLMT*P_SPACING;
	  if(xp[i]-xpp[i]<-0.5*PXLMT*P_SPACING)xpp[i]-=PXLMT*P_SPACING;
#endif    
	  
#if SHUUKIY
	  if(yp[i]-ypp[i]>0.5*PYLMT*P_SPACING)ypp[i]+=PYLMT*P_SPACING;
	  if(yp[i]-ypp[i]<-0.5*PYLMT*P_SPACING)ypp[i]-=PYLMT*P_SPACING;
#endif    
	  /*    初期プログラム 
	  ups[i]=(xp[i]-xpp[i])/(step*DT)+F*step*DT;
	  vps[i]=(yp[i]-ypp[i])/(step*DT)+buo[i]*step*DT-G*step*DT;
	  */
	  
	  ups[i]=(xp[i]-xpp[i])/(step*DT);
	  vps[i]=(yp[i]-ypp[i])/(step*DT);	
	}    
      
    }
  
}/* compute of velocity_step ends here */



void save_temp(np,xp,yp,dp,xpt,ypt,dpt)
     int np;
     double *xp,*yp,*dp,*xpt,*ypt,*dpt;
{
  int i;
  
  for(i=0;i<np;i++)
    {
      xpt[i]=xp[i];
      ypt[i]=yp[i];
      dpt[i]=dp[i];
    }

}/* save_temp ends here */



void load_temp(np,xp,yp,dp,xpt,ypt,dpt)
     int np;
     double *xp,*yp,*dp,*xpt,*ypt,*dpt;
{
  int i;

  for(i=0;i<np;i++)
    {
      xp[i]=xpt[i];
      yp[i]=ypt[i];
      dp[i]=dpt[i];
    }

}/* load_temp ends here */



void save_file(np,xp,yp,up,vp,rax,ray,tp,dp,mp,fname,time,fn)
     int np,time,fn;
     double *xp,*yp,*up,*vp,*rax,*ray,*tp,*dp;
     int *mp;
     char fname[];
{
  FILE *fp;
  int  i,k;
  char fname_full[200];
 
  sprintf(fname_full,"%s_nor.%05d",fname,fn);
  fp=fopen(fname_full,"wt");
  if(fp==NULL) {printf("\n Cannot create file1 %s \n",fname_full);exit(1);}
  
  fprintf(fp,"%d\n",np);
  for(k=0;k<np;k++)
    fprintf(fp,"%lf %lf %7.6e %7.6e %lf %lf %d\n",xp[k],yp[k],up[k],vp[k],tp[k],dp[k],mp[k]);
  
  if(fclose(fp)==EOF)
    {printf("\n Error in closing file %s \n",fname_full);exit(1);}
    
}/* save_file ends here */



void read_file(pp,xp,yp,up,vp,tp,dp,mp,fname,fn)
     int *pp,fn;
     double xp[],yp[],up[],vp[],tp[],dp[];
     int mp[];
     char fname[];
{
  FILE *fp;
  int i,m,np;
  double x,y,u,v,t,d,p;
  char fname_full[100];

  sprintf(fname_full,"%s_nor.%05d",fname,fn);
  fp=fopen(fname_full,"r");
  if(fp==NULL) {printf("\n Cannot open file %s \n",fname_full);exit(1);}
  
  fscanf(fp,"%d",&np);
  i=0;
  while(1)
    {
      //printf("%d\n",np);
      if(fscanf(fp,"%lf %lf %lf %lf %lf %lf %d",&x,&y,&u,&v,&t,&d,&m)!=7)break;
      xp[i]=x;
      yp[i]=y;
      up[i]=u;
      vp[i]=v;
      tp[i]=t;
      dp[i]=d;
      mp[i]=m;
      i++;
    }
  *pp=np;

  if(fclose(fp)==EOF)
    {printf("\n Error in closing file %s \n",fname_full);exit(1);}


}/* read_file ends here */



void viscosity_heat(np,xp,yp,mp,dp,up,vp,pqz,pnz_pn,step,tp,tpp)
     int np;
     double step;
     double xp[],yp[],dp[],up[],vp[],tp[],tpp[];
     int mp[],pqz[],pnz_pn[];
{
  int i,j,p1,p2;
  int zx1,zy1,zx2,zy2,zx2t,zy2t,count;
  double kernel,der_k,der2_k,q,c1,c2;
  double dis,dissq,disx,disy;
  double lx,hlx,ly,hly;
  double u1,u2,v1,v2;

  double ut[PNLMT],vt[PNLMT],rt[PNLMT],wet[PNLMT],tt[PNLMT],nu[PNLMT],a[PNLMT];

  hlx=0.5*(double)PXLMT*P_SPACING;
  lx=(double)PXLMT*P_SPACING;
  hly=0.5*(double)PYLMT*P_SPACING;
  ly=(double)PYLMT*P_SPACING;

  c1=10.0/(7.0*M_PI*H*H); /* 2d no baai */
  c2=1.0/M_PI;
  /*t_nu=NU_LIQUID/PR_LIQUID;    heat transfer*/            

  /* clear */
  for(i=0;i<np;i++)
    {
      ut[i]=0.0;
      vt[i]=0.0;
      rt[i]=0.0;
      wet[i]=0.0;
      tt[i]=0.0;

      nu[i]=nu_liquid(tp[i]);  /*dynamic viscosity*/
      a[i]=a_liquid(tp[i]);    /*eat diffusivity*/
    
    }

  /*compute velocity concavity*/
  for(zx1=0;zx1<ZXLMT;zx1++)
    for(zy1=0;zy1<ZYLMT;zy1++)
      {
	for(zy2t=zy1-1;zy2t<=zy1+1;zy2t++){
#if !SHUUKIY
	  if(zy2t>=0&&zy2t<ZYLMT){
#endif
	    zy2=zy2t;		
	    if(zy2<0)zy2+=ZYLMT;
	    if(zy2>=ZYLMT)zy2-=ZYLMT;
	    
	    for(zx2t=zx1-1;zx2t<=zx1+1;zx2t++){
#if !SHUUKIX
	      if(zx2t>=0&&zx2t<ZXLMT){
#endif		   
		zx2=zx2t;
		if(zx2<0)zx2+=ZXLMT;
		if(zx2>=ZXLMT)zx2-=ZXLMT;

		for(i=0;i<pqz[zx1*ZYLMT+zy1];i++)
		  {
		    p1=pnz_pn[(zx1*ZYLMT+zy1)*QPinZONE_MAX+i];
		    
		    for(j=0;j<pqz[zx2*ZYLMT+zy2];j++)
		      {
			p2=pnz_pn[(zx2*ZYLMT+zy2)*QPinZONE_MAX+j];
			if(p1<p2 && mp[p1]!=-1 && mp[p2]!=-1 && mp[p1]!=101 && mp[p2]!=101)
			  {
			    disx=xp[p2]-xp[p1];
#if SHUUKIX
			    if(disx>hlx)disx-=lx;
			    if(disx<-hlx)disx+=lx;
#endif
			    disy=yp[p2]-yp[p1];
#if SHUUKIY
			    if(disy>hly)disy-=ly;
			    if(disy<-hly)disy+=ly;
#endif
			    dissq=sqr(disx)+sqr(disy);
			    dis=sqrt(dissq);
			    q=dis/H;
			    
			    if(q<2.0)
			      {		
				if(q<1.0)
				  {
				    kernel=c1*(1.0-1.5*q*q+0.75*q*q*q);
				    der_k=c1*(-3.0*q+2.25*q*q)/H;
				    der2_k=c2*(-3.0+4.5*q)/(H*H);
				  }
				else
				  {
				    kernel=c1*(2.0-3.0*q+1.5*q*q-0.25*q*q*q);
				    der_k=c1*(-3.0+3.0*q-0.75*q*q)/H;
				    der2_k=c2*(3.0-1.5*q)/(H*H);
				  }		
				
				u1=up[p1];
				v1=vp[p1];
				u2=up[p2];
				v2=vp[p2];
				
			       
				if(mp[p1]==100||mp[p1]==200)
				  { 
				    u1-=up[p2];
				    v1-=vp[p2];
				  }
				
				if(mp[p2]==100||mp[p1]==200)
				  { 
				    u2-=up[p1];
				    v2-=vp[p1];
				  }
						
				if(mp[p1]==101)
				  { 
				    u1=up[p2];
				    v1=vp[p2];
				  }
				if(mp[p2]==101)
				  { 
				    u2=up[p1];
				    v2=vp[p1];
				  }
				
				ut[p1]-=(dp[p1]+dp[p2])*(u2-u1)*der_k/(dp[p1]*dp[p2]*dis);
				vt[p1]-=(dp[p1]+dp[p2])*(v2-v1)*der_k/(dp[p1]*dp[p2]*dis);
				
				ut[p2]-=(dp[p1]+dp[p2])*(u1-u2)*der_k/(dp[p1]*dp[p2]*dis);
				vt[p2]-=(dp[p1]+dp[p2])*(v1-v2)*der_k/(dp[p1]*dp[p2]*dis);
				
				
				tt[p1]-=(dp[p1]+dp[p2])*(tp[p2]-tp[p1])*der_k/(dp[p1]*dp[p2]*dis);
				tt[p2]-=(dp[p1]+dp[p2])*(tp[p1]-tp[p2])*der_k/(dp[p1]*dp[p2]*dis);	
			      }	       	    
			  }			    
		      }
		  }
#if !SHUUKIX
	      }
#endif
	    }
#if !SHUUKIY
	  }
#endif
	}
      }
  
  for(i=0;i<np;i++)
    {      
      if(mp[i]<100)
	{
	  up[i]+=M_LIQUID*nu[i]*ut[i]*step*DT;
	  vp[i]+=M_LIQUID*nu[i]*vt[i]*step*DT;
	  
	  tp[i]=tpp[i]+M_LIQUID*a[i]*tt[i]*step*DT; 
	}
    }
  
  count=0;
  for(i=0;i<PXLMT;++i)    /* wall temperature keep constant*/
    for(j=0;j<PYLMT;++j)
      {
	if(j<ATUMI)
	  tp[count]=20.;
	else if(j>=PYLMT-ATUMI)
	  tp[count]=20.;
	
	++count;
      }

}/* viscosity_heat ends here */



void compute_mean_density(np,dp,mp,mean)
     int np,mp[];
     double dp[];
     double *mean;
{
  double sum_d;
  int i,j;
		
  sum_d=0.0;
  j=0;
  for(i=0;i<np;i++)
    {
      if(mp[i]<100&&mp[i]!=-1)
	{
	  sum_d+=dp[i];
	  j++;
	}
    }
  
  *mean=sum_d/(double)j;

}



/***************************************************************************/
/*********************************  MAIN  **********************************/
/***************************************************************************/

int main()
{ 
  /* used by preview */  int i,j;
  int nu;

  double radius; /*particle radius*/
  double c;
 
  /* used for SPH */
  int time;
  int np; /* stands for number of particles */
 
  int d_loop,d_count; /* damp loop & damp count*/
  int over_p;
  int cstep; /* calculation step for short */

  int fn; /* stands for file number */
  char fname[100];
  /* file name root, data file name, vorticity file name root,
     vorticity data file name */
  double step,tstep; /* movement step length & test step length */
  double drms[2]; /* density rms & density rms max */
  double mean_d; /* sum density and mean density */

  double *xp,*yp,*xpp,*ypp,*up,*vp,*upp,*vpp,*dp,*axp,*ayp,*rax,*ray; 
  /*location,previous location,velocity,density,and acceleration*/
  
  double *tp,*tpp;
  /*heat*/

  double *xpt,*ypt,*dpt;
  /* location temp and density temp */

  double *xps,*yps,*dps;
  /* location step and density step */

  double *vor;  /* stands for vorticity */

  double *wcx,*wcy; /* weighted center x and y */

  int *mp;
  int *ml;

  int *pqz,*pnz_pn;
  /* number of particles in zone and particles in zone */

  int *pqzw,*pnz_pnw;
  /* number of particles in zone and particles in zone (for density) */

  double *buo;
  /*huryoku kankei*/

  double high,Gr1,Ra1;
  
  double dpmax,dpmin;

 
  /*****************************************************************/

  xp=(double *)malloc(sizeof(double)*PNLMT);
  yp=(double *)malloc(sizeof(double)*PNLMT);
  xpp=(double *)malloc(sizeof(double)*PNLMT);
  ypp=(double *)malloc(sizeof(double)*PNLMT);
  up=(double *)malloc(sizeof(double)*PNLMT);
  vp=(double *)malloc(sizeof(double)*PNLMT);
  upp=(double *)malloc(sizeof(double)*PNLMT);
  vpp=(double *)malloc(sizeof(double)*PNLMT);
  dp=(double *)malloc(sizeof(double)*PNLMT);
  axp=(double *)malloc(sizeof(double)*PNLMT);
  ayp=(double *)malloc(sizeof(double)*PNLMT);
  rax=(double *)malloc(sizeof(double)*PNLMT);
  ray=(double *)malloc(sizeof(double)*PNLMT);
  tp=(double *)malloc(sizeof(double)*PNLMT);
  tpp=(double *)malloc(sizeof(double)*PNLMT);

  xpt=(double *)malloc(sizeof(double)*PNLMT);
  ypt=(double *)malloc(sizeof(double)*PNLMT);
  dpt=(double *)malloc(sizeof(double)*PNLMT);
 
  xps=(double *)malloc(sizeof(double)*PNLMT);
  yps=(double *)malloc(sizeof(double)*PNLMT);
  dps=(double *)malloc(sizeof(double)*PNLMT);

  vor=(double *)malloc(sizeof(double)*PNLMT);

  wcx=(double *)malloc(sizeof(double)*PNLMT);
  wcy=(double *)malloc(sizeof(double)*PNLMT);

  mp=(int *)malloc(sizeof(int)*PNLMT);
  ml=(int *)malloc(sizeof(int)*PNLMT);

  pqz=(int *)malloc(sizeof(int)*(ZXLMT*ZYLMT));
  pnz_pn=(int *)malloc(sizeof(int)*(ZXLMT*ZYLMT*QPinZONE_MAX));

  pqzw=(int *)malloc(sizeof(int)*(ZXLMT*ZYLMT));
  pnz_pnw=(int *)malloc(sizeof(int)*(ZXLMT*ZYLMT*QPinZONE_MAX));

  buo=(double *)malloc(sizeof(double)*PNLMT);
   
  /*****************************************************************/
 
  c=0.2;

  np = 0;
  fn = 0;

  
  strcpy(fname,FILE_NAME);
   
  step=DAMPING_STEP;
  
  high=((PYLMT-ATUMI*2)*P_SPACING*2);
  Gr1=cube(high)*G*(100-20)*BETA_LIQUID/sqr(NU_LIQUID);
  Ra1=PR_LIQUID*Gr1;
  /* printf("%7.6e\n",Ra1);*/

  if(PXLMT!=2*ZXLMT || PYLMT!=2*ZYLMT)
    {printf("ZXLMT or ZYLMT is wrong!!\n");exit(1);}
 

  /**********************LOOP START*****************************/
  for(time=0;time<=ENDTIME+1;time++)
    {
      if(time == 0)
	{
#if RESTART   /* when you restart in midstream */
	  fn=RESTART;                                                   /*  final data number  */
	  read_file(&np,xp,yp,up,vp,tp,dp,mp,fname,fn); 
	  grouping(np,xp,yp,mp,pqz,pnz_pn);
	  
	  for(i=0;i<np;i++)
	    {
	      vor[i]=0.0;
	      wcx[i]=wcy[i]=0.0;
	    }
	  
	  time=fn*DTF + 1;
	  fn++;
	  printf("Restart timestep = %d\n",time);
#endif

	  
#if !RESTART     /* first prosses */ 
	  ini_pos(&np,xp,yp,up,vp,tp,dp,mp,axp,ayp);

	  for(i=0;i<np;i++)
	    {
	      
	      if(mp[i]<100)       
		{
		  xp[i]+=(1.0*rand())/(double)RAND_MAX*1.0e-6*P_SPACING;
		  yp[i]+=(1.0*rand())/(double)RAND_MAX*1.0e-6*P_SPACING;	
		}
	      
	      /*clear*/
	      vor[i]=0.0;
	      wcx[i]=wcy[i]=0.0;
	    }
	  
	  kyoukai(np,xp,yp,mp);
	  grouping(np,xp,yp,mp,pqz,pnz_pn); 		
	  weighted_center(np,xp,yp,mp,dp,wcx,wcy,pqz,pnz_pn);
	  kyoukai(np,wcx,wcy,mp);
	  grouping(np,wcx,wcy,mp,pqzw,pnz_pnw);
	  density(np,xp,yp,mp,dp,wcx,wcy,pqz,pnz_pn,pqzw,pnz_pnw);                         
	  
#endif  
		  
	}/* if(time==0) */
      
            
      save_pre(np,xp,yp,up,vp,tp,xpp,ypp,upp,vpp,tpp);
      
      /***********************4  precision  START*****************************/    
      
      for(cstep=0;cstep<4;cstep++)
	{
	  real_move(np,xp,yp,xpp,ypp,up,vp,mp,1.0/(double)(4-cstep));
	  
	  kyoukai(np,xp,yp,mp);
	  
	  grouping(np,xp,yp,mp,pqz,pnz_pn);
	  weighted_center(np,xp,yp,mp,dp,wcx,wcy,pqz,pnz_pn);
	  kyoukai(np,wcx,wcy,mp);
	  grouping(np,wcx,wcy,mp,pqzw,pnz_pnw);
	  density(np,xp,yp,mp,dp,wcx,wcy,pqz,pnz_pn,pqzw,pnz_pnw);
	  
	  compute_mean_density(np,dp,mp,&mean_d);


	  d_count=0;
 	  d_loop=1;
	 	  
	  /**********************DAMPING LOOP*********************/     /*  imaginary step  */
	  
	  while(d_loop&&d_count<DAMPLMT)
	    {
	   	 	 	     
	     if(d_count%5!=0) /* no step adjust */
	       { 
		 acceleration(np,xp,yp,mp,dp,axp,ayp,pqz,pnz_pn);
		 smoothing(np,xp,yp,mp,axp,ayp,pqz,pnz_pn);
		 move(np,xp,yp,axp,ayp,mp,step);
		 
		 kyoukai(np,xp,yp,mp);
		 
		 grouping(np,xp,yp,mp,pqz,pnz_pn);
		 weighted_center(np,xp,yp,mp,dp,wcx,wcy,pqz,pnz_pn);
		 kyoukai(np,wcx,wcy,mp);
		 grouping(np,wcx,wcy,mp,pqzw,pnz_pnw);
		 density(np,xp,yp,mp,dp,wcx,wcy,pqz,pnz_pn,pqzw,pnz_pnw);
 	       
		 compute_mean_density(np,dp,mp,&mean_d);
		
	       }/* no step adjust ends here */
	     
	     else /* step adjust */
	       {
		 tstep=step/cube(DAMPING_STEP_CHANGE);
		 
		 save_temp(np,xp,yp,dp,xpt,ypt,dpt);
		 
		 acceleration(np,xp,yp,mp,dp,axp,ayp,pqz,pnz_pn);
		 smoothing(np,xp,yp,mp,axp,ayp,pqz,pnz_pn);
		 move(np,xp,yp,axp,ayp,mp,tstep);

		 kyoukai(np,xp,yp,mp);
		 grouping(np,xp,yp,mp,pqz,pnz_pn);
		 weighted_center(np,xp,yp,mp,dp,wcx,wcy,pqz,pnz_pn);
		 kyoukai(np,wcx,wcy,mp);
		 grouping(np,wcx,wcy,mp,pqzw,pnz_pnw);
		 density(np,xp,yp,mp,dp,wcx,wcy,pqz,pnz_pn,pqzw,pnz_pnw);

		 compute_mean_density(np,dp,mp,&mean_d);
		 
		 drms[0]=0.0;
		 j=0;
		 for(i=0;i<np;i++)
		   {
		     if(mp[i]<100&&mp[i]!=-1)
		       {
			 drms[0]+=sqr(dp[i]-mean_d);
			 j++;
		       }
		   }
		 drms[0]=sqrt(drms[0]/(double)j);
		 
	
		 
		 tstep=step*DAMPING_STEP_CHANGE;
		 save_temp(np,xp,yp,dp,xps,yps,dps);
		 load_temp(np,xp,yp,dp,xpt,ypt,dpt);
		 
		 acceleration(np,xp,yp,mp,dp,axp,ayp,pqz,pnz_pn);
		 smoothing(np,xp,yp,mp,axp,ayp,pqz,pnz_pn);
		 move(np,xp,yp,axp,ayp,mp,tstep);

		 kyoukai(np,xp,yp,mp);
		 grouping(np,xp,yp,mp,pqz,pnz_pn);
		 weighted_center(np,xp,yp,mp,dp,wcx,wcy,pqz,pnz_pn);
		 kyoukai(np,wcx,wcy,mp);
		 grouping(np,wcx,wcy,mp,pqzw,pnz_pnw);
		 density(np,xp,yp,mp,dp,wcx,wcy,pqz,pnz_pn,pqzw,pnz_pnw);

		 compute_mean_density(np,dp,mp,&mean_d);
			
		 drms[1]=0.0;
		 j=0;
		 for(i=0;i<np;i++)
		   {
		     if(mp[i]<100&&mp[i]!=-1)
		       {
			 drms[1]+=sqr(dp[i]-mean_d);
			 j++;
		       }
		   }
		 drms[1]=sqrt(drms[1]/(double)j);
		 
	
		 
		 if(drms[0]<drms[1])
		   {
		     step/=cube(DAMPING_STEP_CHANGE);
		     load_temp(np,xp,yp,dp,xps,yps,dps);
		   }
		 else
		   {
		     step*=DAMPING_STEP_CHANGE;
		   }
	       }/* step adjust ends here */
	     
	     
	     /* judge density */
	     over_p=0;
	     dpmax=dpmin=mean_d;
	     
	     for(i=0;i<np;i++)
	       {
		 if(mp[i]<100&&mp[i]!=-1)
		   {
		     if(dp[i]>mean_d*D_MAX||dp[i]<mean_d*D_MIN)
		       {
			 over_p++;
			 if (dp[i]>dpmax) dpmax=dp[i]; 
			 else if (dp[i]<dpmin) dpmin=dp[i];
		       }
		   }
	       }
	     
	  
	     
	     if(over_p==0) d_loop=0;
	     else d_loop=1;
	     	   
	     /* end of judge density */
	     //printf("dampingcount=%d max=%lf min=%lf mean=%lf\n",d_count,dpmax,dpmin,mean_d);
	     
	     d_count++;
	   }/* end while(d_loop&&d_count<DAMPLMT)*/
	 

	  //if(d_count>=DAMPLMT) printf("count over damping limit, timestep = %d, fn = %d\n",time,fn);
	  
	  //printf("dampingcount=%d max=%lf min=%lf mean=%lf\n",d_count,dpmax,dpmin,mean_d);
	 
	 /**********************DAMPING LOOP END*******************/
     
	 velocity_step(np,xp,yp,xpp,ypp,up,vp,mp,1.0/(double)(4-cstep),buo);

	 viscosity_heat(np,xp,yp,mp,dp,up,vp,pqz,pnz_pn,1.0/(double)(4-cstep),tp,tpp);

       }/* end for(cstep=0;cstep<4;cstep++) */
     
     /***********************4 precision END**************************/
     for(i=0;i<np;++i)
       {
	 rax[i]=(up[i]-upp[i])/DT;
	 ray[i]=(vp[i]-vpp[i])/DT;
       }
   
     if(time%10==0)printf("timestep = %d, fn = %d\n",time,fn);  
     

#if SAVEFILE
     if(time%DTF==0)
       {
	 if (fn==100000) {printf("saving file number is over 100000\n"); break; }
	 save_file(np,xp,yp,up,vp,rax,ray,tp,dp,mp,fname,time,fn);
	 fn++;
       }
#endif

    }

 return(0);
} 



