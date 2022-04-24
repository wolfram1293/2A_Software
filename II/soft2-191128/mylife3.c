#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep()関数を使う
#include <time.h>
#define N 256
int height;
int width;
void my_init_cells(int cell[N][N],FILE* fp);
void my_print_cells(FILE *fp, int gen, const int height, const int width, int cell[height][width]);
int my_count_adjacent_cells(int h, int w, const int height, const int width, int cell[height][width]);
void my_update_cells(const int height, const int width, int cell[height][width]);


int main(int argc, char **argv){
  int cell_temp[N][N];
  FILE *fp = stdout;
  /* ファイルを引数にとるか、ない場合はデフォルトの初期値を使う */
  if ( argc > 2 ) {
    fprintf(stderr, "usage: %s [filename for init]\n", argv[0]);
    return EXIT_FAILURE;
  }else if (argc == 2) {
    FILE *lgfile;
    if ( (lgfile = fopen(argv[1],"r")) != NULL ) {
      my_init_cells(cell_temp,lgfile); // ファイルによる初期化
    }
    else{
      fprintf(stderr,"cannot open file %s\n",argv[1]);
      return EXIT_FAILURE;
    }
    fclose(lgfile);
  }else{
      fprintf(stderr,"cannot open file\n"); 
  }

  int cell[height][width];
  for(int y = 0 ; y < height ; y++)for(int x = 0 ; x < width ; x++)cell[y][x] = 0;
  printf("x = %d, y = %d\n",width,height);
  for(int y = 0 ; y < height ; y++){
    for(int x = 0 ; x < width ; x++){
      cell[y][x] = cell_temp[y][x];
    }
  }

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

/*ファイルによるセルの初期化: 生きているセルの座標が記述されたファイルをもとに2次元配列の状態を初期化する
 fp = NULL のときは、関数内で適宜定められた初期状態に初期化する。関数内初期値はdefault.lif と同じもの*/
void my_init_cells(int cell[N][N],FILE* fp){
    
    char str[N];
    int l=0;
    while(fgets(str, N, fp) != NULL) {
        if(str[0]=='x'){
            break;
        }
        l++;
	}
    fseek(fp,0L,SEEK_SET);
    for(int i=0;i<l;i++){
        fgets(str, N, fp);
    }
    fscanf(fp,"x = %d, y = %d",&width,&height);

    fseek(fp,0L,SEEK_SET);
    for(int i=0;i<l+1;i++){
        fgets(str, N, fp);
    }
    fgets(str, N, fp);
    int j=0;
    int n=0;
    int y=0;
    int x=0;
    while(str[j]!='!'){
        if(str[j]>='0'&&str[j]<='9'){
            n=10*n+(str[j]-'0');
        }
        if(str[j]=='b'){
            if (n == 0) n = 1;
            for(int i=0;i<n;i++){
                cell[y][x]=0;
                x++;
            }
            n=0;
        }
        if(str[j]=='o'){
            if (n == 0) n = 1;
            for(int i=0;i<n;i++){
                cell[y][x]=1;
                x++;
            }
            n=0;
        }
        if(str[j]=='$'){
            y+=1;
            x=0;
        }
        j++;
    }
}

/*グリッドの描画: 世代情報とグリッドの配列等を受け取り、ファイルポインタに該当する出力にグリッドを描画する*/
void my_print_cells(FILE *fp,int gen,int height,int width,int cell[height][width]){
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
int my_count_adjacent_cells(int y,int x,int height,int width,int cell[height][width]){
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
void my_update_cells(int height,int width,int cell[height][width]){
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