#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep()関数を使う
#include <time.h>
#define N 256
void my_init_cells(const int height, const int width, int cell[height][width]);
void my_print_cells(FILE *fp, int gen, const int height, const int width, int cell[height][width]);
int my_count_adjacent_cells(int h, int w, const int height, const int width, int cell[height][width]);
void my_update_cells(const int height, const int width, int cell[height][width]);


int main(int argc, char **argv)
{
  FILE *fp = stdout;
  const int height = 40;
  const int width = 70;

  int cell[height][width];
  for(int y = 0 ; y < height ; y++){
    for(int x = 0 ; x < width ; x++){
      cell[y][x] = 0;
    }
  }
  my_init_cells(height, width, cell);
  my_print_cells(fp, 0, height, width, cell); // 表示する
  sleep(1); // 1秒休止

  /* 世代を進める*/
  for (int gen = 1 ;; gen++) {
    my_update_cells(height, width, cell); // セルを更新
    my_print_cells(fp, gen, height, width, cell);  // 表示する
    sleep(1); //1秒休止する
    fprintf(fp,"\e[%dA",height+3);//height+3 の分、カーソルを上に戻す(壁2、表示部1)
  }

  return EXIT_SUCCESS;
}

void my_init_cells(const int height, const int width, int cell[height][width]){
    double random;
    unsigned int now=(unsigned int)time(0);
    srand(now);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            random=(double)rand()/(double)RAND_MAX;
            if(random<0.1){
                cell[i][j]=1;
            }else{
                cell[i][j]=0;
            }
        }
    }
}

/*グリッドの描画: 世代情報とグリッドの配列等を受け取り、ファイルポインタに該当する出力にグリッドを描画する*/
void my_print_cells(FILE *fp, int gen, const int height, const int width, int cell[height][width]){
    fprintf(fp,"generateion = %d\t",gen);
    double alive=0;
    double dead=0;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            if(cell[i][j]==1){
                alive+=1;
            }else{
                dead+=1;
            }
        }
    }
    double ratio=alive/dead;
    fprintf(fp,"alive/dead= %f\n",ratio);
    fprintf(fp,"+");
    for(int i=0;i<width;i++){
        fprintf(fp,"-");
    }
    fprintf(fp,"+\n");
    for(int y=0;y<height;y++){
        fprintf(fp,"|");
        for(int x=0;x<width;x++){
            if(cell[y][x]==1){
            fprintf(fp,"\e[31m#\e[0m");
            }else{
                fprintf(fp," ");
            }
        }
        fprintf(fp,"|\n");
    }
    fprintf(fp,"+");
    for(int i=0;i<width;i++){
        fprintf(fp,"-");
    }
    fprintf(fp,"+\n");
}

/*着目するセルの周辺の生きたセルをカウントする関数*/
int my_count_adjacent_cells(int y, int x, const int height, const int width, int cell[height][width]){
    int count=0;

    for(int i=y-1;i<=y+1;i++){
        for(int j=x-1;j<=x+1;j++){
            if(i>=0&&i<height&&j>=0&&j<width){
                count+=cell[i][j];
            }
        }
    }
    count-=cell[y][x];
    return count;
}

/*ライフゲームのルールに基づいて2次元配列の状態を更新する*/
void my_update_cells(const int height, const int width, int cell[height][width]){
    int temp[height][width];
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            temp[i][j]=cell[i][j];
        }
    }
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            int celln=my_count_adjacent_cells(i,j,height,width,temp);
            if(temp[i][j]==1){
                if(celln==2||celln==3){
                    cell[i][j]=1;
                }else{
                    cell[i][j]=0;
                }

            }else{
                if(celln==3){
                    cell[i][j]=1;
                }else{
                    cell[i][j]=0;
                }

            }
        }
    }
}