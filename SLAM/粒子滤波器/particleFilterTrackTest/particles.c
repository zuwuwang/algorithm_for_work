/*
  Functions for object tracking with a particle filter
  
  @author Rob Hess
  @version 1.0.0-20060310
*/


#include "defs.h"
#include "utils.h"
#include "particles.h"



/*************************** Function Definitions ****************************/

/*
  Creates an initial distribution of particles at specified locations
  
  
  @param regions an array of regions describing player locations around
    which particles are to be sampled
  @param histos array of histograms describing regions in \a regions
  @param n the number of regions in \a regions
  @param p the total number of particles to be assigned
  
  @return Returns an array of \a p particles sampled from around regions in
    \a regions
*/
// 在指定位置创建粒子的初始分布，n表示要跟踪的目标个数，p表示初始设置的粒子数
// 初始化粒子的时候给定了一个histos特征
particle* init_distribution( CvRect* regions, histogram** histos, int n, int p)
{
  particle* particles;
  int np;
  float x, y;
  int i, j, width, height, k = 0;
  
  particles = malloc( p * sizeof( particle ) );
  np = p / n; //平均每个区域放置的粒子数

  /* create particles at the centers of each of n regions */
  //在每个区域的中心创建指定书目的粒子
  //也就是说，初始时在目标中心放置100个框，并设置权重为0，提取区域的色调直方图特征向量
  for( i = 0; i < n; i++ )
    {
      width = regions[i].width;
      height = regions[i].height;
      x = regions[i].x + width / 2; // x,y表示选择的第i个目标区域的中心点
      y = regions[i].y + height / 2;
	  // 该目标处放置100个粒子
      for( j = 0; j < np; j++ )
		{
		  // k用来表示实际创建的粒子的个数，当前粒子原点坐标与上一坐标
		  particles[k].x0 = particles[k].xp = particles[k].x = x;
		  particles[k].y0 = particles[k].yp = particles[k].y = y;
		  particles[k].sp = particles[k].s = 1.0;
		  // 每个粒子其实就是一个目标区域的大小
		  particles[k].width = width;
		  particles[k].height = height;
		  particles[k].histo = histos[i];
		  particles[k++].w = 0;
		}
    }

  /* make sure to create exactly p particles */
  //确保初始化了指定数量的粒子
  // 为什么还需要这样再去确认一下，粒子在上一过程中会放少了吗？
  i = 0;
  while( k < p )
    {
      width = regions[i].width;
      height = regions[i].height;
      x = regions[i].x + width / 2;
      y = regions[i].y + height / 2;
      particles[k].x0 = particles[k].xp = particles[k].x = x;
      particles[k].y0 = particles[k].yp = particles[k].y = y;
      particles[k].sp = particles[k].s = 1.0;
      particles[k].width = width;
      particles[k].height = height;
      particles[k].histo = histos[i];
      particles[k++].w = 0;
      i = ( i + 1 ) % n;
    }

  return particles;
}



/*
  Samples a transition model for a given particle
  
  @param p a particle to be transitioned
  @param w video frame width
  @param h video frame height
  @param rng a random number generator from which to sample

  @return Returns a new particle sampled based on <EM>p</EM>'s transition
    model
*/
// 估计目标移动后粒子的位置
particle transition( particle p, int w, int h, gsl_rng* rng )
{
  float x, y, s;
  particle pn;
  
  /* sample new state using second-order autoregressive dynamics */
  // 使用二阶动态回归来自动更新粒子状态 
  //回归模型的参数即A1、A2、B0等Rob Hess在代码中已设定（我不知道是怎么来的？）
  x = A1 * ( p.x - p.x0 ) + A2 * ( p.xp - p.x0 ) +
	  B0 * gsl_ran_gaussian( rng, TRANS_X_STD ) + p.x0;
  pn.x = MAX( 0.0, MIN( (float)w - 1.0, x ) );
  
  y = A1 * ( p.y - p.y0 ) + A2 * ( p.yp - p.y0 ) +
    B0 * gsl_ran_gaussian( rng, TRANS_Y_STD ) + p.y0;
  pn.y = MAX( 0.0, MIN( (float)h - 1.0, y ) );
  
  s = A1 * ( p.s - 1.0 ) + A2 * ( p.sp - 1.0 ) +
    B0 * gsl_ran_gaussian( rng, TRANS_S_STD ) + 1.0;
  
  pn.s = MAX( 0.1, s );
  pn.xp = p.x;
  pn.yp = p.y;
  pn.sp = p.s;
  pn.x0 = p.x0;
  pn.y0 = p.y0;
  pn.width = p.width;
  pn.height = p.height;
  pn.histo = p.histo;
  pn.w = 0;

  return pn;
}



/*
  Normalizes particle weights so they sum to 1
  
  @param particles an array of particles whose weights are to be normalized
  @param n the number of particles in \a particles
*/
void normalize_weights( particle* particles, int n )
{
  float sum = 0;
  int i;

  for( i = 0; i < n; i++ )
    sum += particles[i].w;
  for( i = 0; i < n; i++ )
    particles[i].w /= sum;
}



/*
  Re-samples a set of particles according to their weights to produce a
  new set of unweighted particles
  
  @param particles an old set of weighted particles whose weights have been
    normalized with normalize_weights()
  @param n the number of particles in \a particles
  
  @return Returns a new set of unweighted particles sampled from \a particles
*/
// 粒子重采样
// 就是说目标运动到新的区域之后，原有的不在目标中的粒子权重降低，需要抛弃，设置权重阈值
// 低于该阈值时抛弃该粒子，并找新的粒子来补充，用最大权值来补充。
// 这就是重采样的过程
particle* resample( particle* particles, int n )
{
  particle* new_particles;
  int i, j, np, k = 0;
  // 这里n的值指的是定义的粒子总数，n = 100
  // 对粒子按权值排序，排序好的粒子仍然以particles表示
  qsort( particles, n, sizeof( particle ), &particle_cmp ); //根据权重进行排序，自定义比较排序函数
  new_particles = malloc( n * sizeof( particle ) ); //重新分配100个粒子的空间
  // 对于新的0-99个粒子
  for( i = 0; i < n; i++ )
    {
	  // 按照权重大小依次取出，权重×100，即为重采样之后新放的粒子数
	  // 这里其实没有阈值的概念，当按权值重新放置之后，满100即退出
      np = cvRound( particles[i].w * n ); // 淘汰弱权值样本？怎么淘汰的？保留阈值以上样本？
      for( j = 0; j < np; j++ )
		{
		  new_particles[k++] = particles[i];  //新的粒子直接复制权重最高的例子，但会降低粒子的多样性。表示目标落在这个区域的可能性越大。
		  // 如何解决上述步骤中的粒子的多样性降低的问题，可继续研究改进
		  if( k == n )  // 如果放满了100个粒子就退出，返回新构建的100个粒子
			goto exit;
		}
    }
  while( k < n )
    new_particles[k++] = particles[0];  //复制最大权值样本以填充满样本空间

 exit:
  return new_particles;
}



/*
  Compare two particles based on weight.  For use in qsort.

  @param p1 pointer to a particle
  @param p2 pointer to a particle

  @return Returns -1 if the \a p1 has lower weight than \a p2, 1 if \a p1
    has higher weight than \a p2, and 0 if their weights are equal.
*/
// 由qsort函数调用
int particle_cmp( void* p1, void* p2 )
{
  particle* _p1 = (particle*)p1;
  particle* _p2 = (particle*)p2;

  if( _p1->w > _p2->w )
    return -1;
  if( _p1->w < _p2->w )
    return 1;
  return 0;
}



/*
  Displays a particle on an image as a rectangle around the region specified
  by the particle
  
  @param img the image on which to display the particle
  @param p the particle to be displayed
  @param color the color in which \a p is to be displayed
*/
void display_particle( IplImage* img, particle p, CvScalar color )
{
  int x0, y0, x1, y1;

  x0 = cvRound( p.x - 0.5 * p.s * p.width );
  y0 = cvRound( p.y - 0.5 * p.s * p.height );
  x1 = x0 + cvRound( p.s * p.width );
  y1 = y0 + cvRound( p.s * p.height );
  
  cvRectangle( img, cvPoint( x0, y0 ), cvPoint( x1, y1 ), color, 1, 8, 0 );
}
