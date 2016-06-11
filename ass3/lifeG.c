 #include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#define size 500
#define rate 500
      int *arr;
      int *tmp;
      int length,width;
	  int k,counter,maxGen;
	  int stop;
  void readFromFile(FILE *f)
   {
	int i,j;
	int c;
	
	for(i=0;i<length;i++)
	{
	  for(j=0;j<width;j++)
	  {
	    c=fgetc(f);
	    
	    if(c=='1')
	      arr[i*width+j]=1;
	    else arr[i*width+j]=0;
	    
	  }
	  
	  c=fgetc(f);
	  if(c==EOF)
	    break;
	}
	
  }

      void print()
       {
	  int i,j;
	  char c;
	  for(i=0;i<length;i++)
	  {
	    for(j=0;j<width;j++)
	    {
	      if(arr[i*width+j]==0)
	      printf(" ");
	      else printf("%d",arr[i*width+j]);
	    
	    }
	    printf("\n");
	}
	counter=0;
      }
      
       void printTmp()
       {
	  int i,j;
	  char c;
	  for(i=0;i<length;i++)
	  {
	    for(j=0;j<width;j++)
	    {
	      if(tmp[i*width+j]==0)
	      printf(" ");
	      else printf("%d",tmp[i*width+j]);
	      
	    }
	    printf("\n");
	}
	
      }
      
      void calc()
      {
	 int i,j,k1,k2,l1,l2;
	 for(i=0;i<length;i++)
	  {
	    if(i==0)
		k1=length-1;
	    else k1=i-1;
	    if(i==length-1)
		k2=0;
	    else k2=i+1;
	    for(j=0;j<width;j++)
	    {      
		  if(counter==k)
			   print();
		  counter++;
	      if(j==width-1)
		l2=0;
	      else l2=j+1;
	      if(j==0)
		l1=width-1;
	      else l1=j-1;
	      tmp[i*width+j]=arr[k1*width+l1]>0;
	      tmp[i*width+j]+=arr[k1*width+j]>0;
	      tmp[i*width+j]+=arr[k1*width+l2]>0;
	      tmp[i*width+j]+=arr[i*width+l1]>0;
	      tmp[i*width+j]+=arr[i*width+l2]>0;
	      tmp[i*width+j]+=arr[k2*width+l1]>0;
	      tmp[i*width+j]+=arr[k2*width+l2]>0;
	      tmp[i*width+j]+=arr[k2*width+j]>0;
	     // printf("i=%d j=%d\n",i,j);
	    }
	  }
		
      }
      
      void update()
       {
	 int i,j;
	  for(i=0;i<length;i++)
	  {
	    for(j=0;j<width;j++)
	    {
		 if(counter==k)
			print();
	     counter++; 
		 if(tmp[i*width+j]==3 || (tmp[i*width+j]==2 && arr[i*width+j]>0))
		 {	
			 if( arr[i*width+j]<9)
				arr[i*width+j]++;
		 }
		 else arr[i*width+j]=0;
		 
		}
	}
	  
      }
      
int init( int argc, char** argv )
      {
	FILE *f;
	int t=3;
	k=150;
	counter=0;
	if(argc>3)
	{
	  int i;
	  f=fopen(argv[1],"r");
	  length=atoi(argv[2]);
	  width=atoi(argv[3]);
	  if(argc>=5)
	  {
	    t=atoi(argv[4]);
	    k=atoi(argv[5]);
	  }
	  arr=(int*)malloc(sizeof(int)*length*width);
	  tmp=(int*)malloc(sizeof(int)*length*width);
	  readFromFile(f);
	  printf("finish loading\n");
	  fclose(f);
	  print();
	 
	  maxGen=t;
	 // getchar();
	  return t;
	}
	else return 0;
 }

void initGL()
{
	glClearColor(0,0,0,1);

	glMatrixMode(GL_PROJECTION); /* switch matrix mode */
	glLoadIdentity();
	//defines view mode
	glOrtho(-width/2,width/2,-length/2,length/2,-1,1);


	glMatrixMode(GL_MODELVIEW); /* switch matrix mode */
	glLoadIdentity();
	glTranslatef(-width/2.0,length/2.0,0);
	stop=0;
}

void draw()
{
	int i,j;
	glClear(GL_COLOR_BUFFER_BIT);
	for(i=0;i<length;i++)
	 {
	    for(j=0;j<width;j++)
	    {
			if(arr[i*width+j]>0)
			{
				glPushMatrix();
				glTranslatef(j,-i-1,0);
				glColor4f(0,arr[i*width+j]/12.0+0.25,0,0.2);
				glRectf(0,0,1,1);
				glPopMatrix();
			}
		}
	 }
	glFlush();
}

void generation(int t)
{
	int i;
	printf("start %d\n",maxGen-t);
	calc();
	update();
	glutPostRedisplay();
	t--;
	if(t>0 && !stop)
		glutTimerFunc(rate,generation,t);
}

void readKey(unsigned char key, int x,int y)
{
	switch(key)
	{
		case ' ':
			stop=1-stop;
			if(!stop)
			{
				generation(50);
			}
		break;
	}
}

int main(int  argc,  char** argv) 
{
   int t;
   glutInit (& argc, argv) ;
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB) ;
   glutInitWindowSize (size,size) ;
   glutCreateWindow("Game of Life") ;
   t=init(argc,argv);
   initGL();
   glutDisplayFunc(draw);
   glutKeyboardFunc(readKey);
   glutTimerFunc(rate*2,generation,t);
  // glutIdleFunc(game); 
	glutMainLoop () ; 
	free(arr);
	free(tmp);
}