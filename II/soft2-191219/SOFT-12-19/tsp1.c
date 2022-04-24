#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h> // strtol のエラー判定用

//都市数n=20の時　total distance = 159.265841　で最小

// 町の構造体（今回は2次元座標）を定義
typedef struct
{
  int x;
  int y;
} City;

// 描画用
typedef struct
{
  int width;
  int height;
  char **dot;
} Map;

// 整数最大値をとる関数
int max(const int a, const int b)
{
  return (a > b) ? a : b;
}

// プロトタイプ宣言
// draw_line: 町の間を線で結ぶ
// draw_route: routeでの巡回順を元に移動経路を線で結ぶ
// plot_cities: 描画する
// distance: 2地点間の距離を計算
// solve(): TSPをといて距離を返す/ 引数route に巡回順を格納

void draw_line(Map map, City a, City b);
void draw_route(Map map, City *city, int n, const int *route);
void plot_cities(FILE* fp, Map map, City *city, int n, const int *route);
double distance(City a, City b);
double solve(const City *city, int n, int *route, int *visited);
Map init_map(const int width, const int height);
void free_map_dot(Map m);
City *load_cities(const char* filename,int *n);

double sumdistance(const City *city, int n, int *route);

Map init_map(const int width, const int height)
{
  char **dot = (char**) malloc(width * sizeof(char*));
  char *tmp = (char*)malloc(width*height*sizeof(char));
  for (int i = 0 ; i < width ; i++)
    dot[i] = tmp + i * height;
  return (Map){.width = width, .height = height, .dot = dot};
}
void free_map_dot(Map m)
{
  free(m.dot[0]);
  free(m.dot);
}

City *load_cities(const char *filename, int *n)
{
  City *city;
  FILE *fp;
  if ((fp=fopen(filename,"rb")) == NULL){
    fprintf(stderr, "%s: cannot open file.\n",filename);
    exit(1);
  }
  fread(n,sizeof(int),1,fp);
  city = (City*)malloc(sizeof(City) * *n);
  for (int i = 0 ; i < *n ; i++){
    fread(&city[i].x, sizeof(int), 1, fp);
    fread(&city[i].y, sizeof(int), 1, fp);
  }
  fclose(fp);
  return city;
}
int main(int argc, char**argv)
{
  // const による定数定義
  const int width = 70;
  const int height = 40;
  //const int max_cities = 100;

  Map map = init_map(width, height);
  
  FILE *fp = stdout; // とりあえず描画先は標準出力としておく
  if (argc != 2){
    fprintf(stderr, "Usage: %s <city file>\n", argv[0]);
    exit(1);
  }
  int n;
  City *city = load_cities(argv[1],&n);

  // 町の初期配置を表示
  plot_cities(fp, map, city, n, NULL);
  sleep(1);

  // 訪れる順序を記録する配列を設定
  int *route = (int*)calloc(n, sizeof(int));
  // 訪れた町を記録するフラグ
  int *visited = (int*)calloc(n, sizeof(int));

  const double d = solve(city,n,route,visited);
  plot_cities(fp, map, city, n, route);
  printf("total distance = %f\n", d);
  for (int i = 0 ; i < n ; i++){
    printf("%d -> ", route[i]);
  }
  printf("0\n");

  // 動的確保した環境ではfreeをする
  free(route);
  free(visited);
  free(city);
  
  return 0;
}

// 繋がっている都市間に線を引く
void draw_line(Map map, City a, City b)
{
  const int n = max(abs(a.x - b.x), abs(a.y - b.y));
  for (int i = 1 ; i <= n ; i++){
    const int x = a.x + i * (b.x - a.x) / n;
    const int y = a.y + i * (b.y - a.y) / n;
    if (map.dot[x][y] == ' ') map.dot[x][y] = '*';
  }
}

void draw_route(Map map, City *city, int n, const int *route)
{
  if (route == NULL) return;

  for (int i = 0; i < n; i++) {
    const int c0 = route[i];
    const int c1 = route[(i+1)%n];// n は 0に戻る必要あり
    draw_line(map, city[c0], city[c1]);
  }
}

void plot_cities(FILE *fp, Map map, City *city, int n, const int *route)
{
  fprintf(fp, "----------\n");

  memset(map.dot[0], ' ', map.width * map.height); 

  // 町のみ番号付きでプロットする
  for (int i = 0; i < n; i++) {
    char buf[100];
    sprintf(buf, "C_%d", i);
    for (size_t j = 0; j < strlen(buf); j++) {
      const int x = city[i].x + j;
      const int y = city[i].y;
      map.dot[x][y] = buf[j];
    }
  }
  draw_route(map, city, n, route);

  for (int y = 0; y < map.height; y++) {
    for (int x = 0; x < map.width; x++) {
      const char c = map.dot[x][y];
      fputc(c, fp);
    }
    fputc('\n', fp);
  }
  fflush(fp);
}

double distance(City a, City b)
{
  const double dx = a.x - b.x;
  const double dy = a.y - b.y;
  return sqrt(dx * dx + dy * dy);
}

double solve(const City *city, int n, int *route, int *visited)
{
  route[0] = 0; // 循環した結果を避けるため、常に0番目からスタート
  visited[0] = 1;

  // default
  for (int i = 1 ; i < n ; i++){
      int l=rand()%n;
      while(visited[l]==1){
          l=rand()%n;
      }
      route[i] =l;
      visited[l] = 1; // 訪問済みかチェック
  }
  for (int j= 1 ;j<n ;j++){
      visited[j] =0;
  }
  double summin2=sumdistance(city,n,route);

  //N個の初期状態をランダムに生成
  int N=pow(n,3);
  for(int k=0;k<N;k++){
    srand(k);
    for (int i = 1 ; i < n ; i++){
        int l=rand()%n;
        while(visited[l]==1){
            l=rand()%n;
        }
        route[i] =l;
        visited[l] = 1; // 訪問済みかチェック
    }
    for (int j=1;j<n;j++){
      visited[j] =0;
    }
    double prevsum =sumdistance(city,n,route);
    int ri,fi,fj;
    double sum,summin=prevsum;

    //初期状態から解の改良ができなくなるまで2都市の入れ替えを繰り返す
    while(1==1){
        for(int i=1;i<n-1;i++){
            for(int j=i+1;j<n;j++){
                if(i!=j){
                    ri=route[i];
                    route[i]=route[j];
                    route[j]=ri;
                    sum=sumdistance(city,n,route);
                    if(sum<summin){
                        summin=sum;
                        fi=i;
                        fj=j;
                    }
                    ri=route[i];
                    route[i]=route[j];
                    route[j]=ri;
                }
            }
        }
        //printf("prev=%f,min=%f\n",prevsum,summin);
        if(summin<prevsum){
            prevsum=summin;
            ri=route[fi];
            route[fi]=route[fj];
            route[fj]=ri;
            //printf("change %d,%d\n",fi,fj);
        }else{
            break;
        }
    }

    //最適解がそれ以前の最適解より小さければ更新

    //printf("k=%d,prev=%f,min=%f\n",k,prevsum,summin2);
    if(prevsum<summin2){
        summin2=prevsum;
    }
  }
  return summin2;
}

double sumdistance(const City *city, int n, int *route)
{
    double sum_d = 0;
    for (int i = 0 ; i < n ; i++){
        const int c0 = route[i];
        const int c1 = route[(i+1)%n];
        sum_d += distance(city[c0],city[c1]);
    }
    return sum_d;
}