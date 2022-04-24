#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "physics2.h"

int main(int argc, char **argv){
  const Condition cond = {
		    .width  = 75,
		    .height = 40,
		    .G = 1.0,
		    .dt = 0.8,
		    .cor = 0.8,
            .e=2.0
  };
  
  size_t objnum = 4;
  Object objects[objnum];

  // objects[1] は巨大な物体を画面外に... 地球のようなものを想定
  objects[0] = (Object){ .m = 10.0, .y = -19.9,.x=-10.0,.vy = 2.0,.vx=2.0};
  objects[1] = (Object){ .m = 10.0, .y =  19.0,.x=0.0,.vy = 2.0,.vx=0.0};
  objects[2] = (Object){ .m = 10.0, .y =  -19.0,.x=10.0,.vy = 2.0,.vx=0.0};
  objects[3] = (Object){ .m = 10.0, .y =  -19.0,.x=0.0,.vy = 2.0,.vx=0.0};

  // シミュレーション. ループは整数で回しつつ、実数時間も更新する
  const double stop_time = 400;
  double t = 0;
  for (int i = 0 ; t <= stop_time ; i++){
    t = i * cond.dt;
    my_update_velocities(objects, objnum, cond);
    my_update_positions(objects, objnum, cond);
    my_bounce(objects, objnum, cond);
    fusion(objects,objnum,cond);
    // 表示の座標系は width/2, height/2 のピクセル位置が原点となるようにする
    my_plot_objects(objects, objnum, t, cond);
    
    usleep(100 * 1000); // 200 x 1000us = 200 ms ずつ停止
    printf("\e[%dA", cond.height+3);// 壁とパラメータ表示分で3行
  }
  return EXIT_SUCCESS;
}

// 実習: 以下に my_ で始まる関数を実装する
// 最終的に phisics2.h 内の事前に用意された関数プロトタイプをコメントアウト
void my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond){
    int height=cond.height;
    int width=cond.width;
    int y0[numobj],x0[numobj];
    for(int i=0;i<numobj;i++){
        y0[i]=objs[i].y;
    }
    for(int i=0;i<numobj;i++){
        x0[i]=objs[i].x;
    }
    int yx[height][width];
    
    printf("+");
    for(int i=0;i<width;i++){
        printf("-");
    }
    printf("+\n");
    for(int i=0;i<height;i++){
        printf("|");
        for(int j=0;j<width;j++){
            int y=i-height/2;
            int x=j-width/2;
            for(int k=0;k<numobj;k++){
                if(y==y0[k]&&x==x0[k]){
                    yx[i][j]=1;
                    break;
                }else{
                    yx[i][j]=0;
                }
            }
            if(yx[i][j]==1){
                    printf("o");
            }else{
                    printf(" ");
            }
        }
        printf("|\n");
    }
    printf("+");
    for(int i=0;i<width;i++){
        printf("-");
    }
    printf("+\n");
    printf("t =  %f\n",t);
}
void my_update_velocities(Object objs[], const size_t numobj, const Condition cond){
    double ax;
    double ay;
    for(int i=0;i<numobj;i++){
        ax=0;
        ay=0;
        for(int j=0;j<numobj;j++){
            if(i!=j){
                double denom=pow(((objs[j].x-objs[i].x)*(objs[j].x-objs[i].x)+(objs[j].y-objs[i].y)*(objs[j].y-objs[i].y)),1.5);
                if(denom!=0){
                    ax+=cond.G*objs[j].m*(objs[j].x-objs[i].x)/denom;
                    ay+=cond.G*objs[j].m*(objs[j].y-objs[i].y)/denom;
                }
            }
        }
        objs[i].vx=objs[i].vx+ax*cond.dt;
        objs[i].vy=objs[i].vy+ay*cond.dt;
    }
}
void my_update_positions(Object objs[], const size_t numobj, const Condition cond){
    for(int i=0;i<numobj;i++){
        objs[i].prev_x=objs[i].x;
        objs[i].prev_y=objs[i].y;
        objs[i].x=objs[i].x+objs[i].vx*cond.dt;
        objs[i].y=objs[i].y+objs[i].vy*cond.dt;
    }
}
void my_bounce(Object objs[], const size_t numobj, const Condition cond){
    int height=cond.height;
    int width=cond.width;
    for(int i=0;i<numobj;i++){
        if (objs[i].x>=width/2.0||objs[i].x<=-width/2.0){
            objs[i].x=objs[i].prev_x;
            objs[i].vx=-cond.cor*objs[i].vx;
        }
        if (objs[i].y>=height/2.0||objs[i].y<=-height/2.0){
            objs[i].y=objs[i].prev_y;
            objs[i].vy=-cond.cor*objs[i].vy;
        }
    }

}

void fusion(Object objs[], const size_t numobj, const Condition cond){
    for(int i=0;i<numobj;i++){
        for(int j=0;j<numobj;j++){
            if(i!=j){
                if(objs[i].y-objs[j].y!=0.0&&objs[i].x-objs[j].x!=0.0){
                    if(fabs(objs[i].y-objs[j].y)<cond.e&&fabs(objs[i].x-objs[j].x)<cond.e){
                        objs[i].x=(objs[i].x+objs[j].x)/2.0;
                        objs[j].x=0.0;
                        objs[i].y=(objs[i].y+objs[j].y)/2.0;
                        objs[j].y=1000.0;

                        objs[i].vx=(objs[i].m*objs[i].vx+objs[j].m*objs[j].vx)/(objs[i].m+objs[j].m);
                        objs[j].vx=0.0;
                        objs[i].vy=(objs[i].m*objs[i].vy+objs[j].m*objs[j].vy)/(objs[i].m+objs[j].m);
                        objs[j].vy=0.0;

                        objs[i].m=objs[i].m+objs[j].m;
                        objs[j].m=0.0;

                    }

                }
                
            }
        }
    }
}