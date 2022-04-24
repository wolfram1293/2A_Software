#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <errno.h> // for error catch

// Structure for canvas
typedef struct
{
  int width;
  int height;
  char **canvas;
  char pen;
} Canvas;

// Structure for history (2-D array)
typedef struct
{
  struct node *begin;
  size_t max_history;
  size_t bufsize;
  char **commands;
  size_t hsize;
} History;
const int maxlen = 1000;

struct node 
{
  char *str;
  struct node *next;
};

typedef struct node Node;

// functions for Canvas type
Canvas *init_canvas(int width, int height, char pen);
void reset_canvas(Canvas *c);
void print_canvas(FILE *fp, Canvas *c);
void free_canvas(Canvas *c);

// display functions
void rewind_screen(FILE *fp,unsigned int line);
void clear_command(FILE *fp);
void clear_screen(FILE *fp);

// enum for interpret_command results
typedef enum res{ EXIT, NORMAL, COMMAND} Result;

int max(const int a, const int b);
void draw_line(Canvas *c, const int x0, const int y0, const int x1, const int y1);
void draw_rectangle(Canvas *c, const int x0, const int y0, const int x1, const int y1);
void draw_circle(Canvas *c, const int x0, const int y0, const int r);
Result interpret_command(const char *command, History *his, Canvas *c);
void save_history(const char *filename, History *his);
void load_history(const char *filename, History *his, Canvas *c);
void undo(History *his, Canvas *c);



int main(int argc, char **argv)
{
  const int bufsize = 1000;
  History his = (History){.bufsize = bufsize, .hsize = 0};
  int width;
  int height;
  if (argc != 3){
    fprintf(stderr,"usage: %s <width> <height>\n",argv[0]);
    return EXIT_FAILURE;
  } else{
    long w = strtol(argv[1],NULL,10);
    if(errno == ERANGE){
      fprintf(stderr, "%s: %s\n",argv[1],strerror(errno));
      return EXIT_FAILURE;
    }
    long h = strtol(argv[2],NULL,10);
    if(errno == ERANGE){
      fprintf(stderr, "%s: %s\n",argv[2],strerror(errno));
      return EXIT_FAILURE;
    }
    width = (int)w;
    height = (int)h;    
  }
  char pen = '*';
  printf("default pen \"%c\"\n",pen);

  FILE *fp;
  char buf[his.bufsize];
  fp = stdout;
  Canvas *c = init_canvas(width,height, pen);

  fprintf(fp,"\n"); // required especially for windows env
  while (1==1) {
    size_t hsize = his.hsize;
    size_t bufsize = his.bufsize;
    print_canvas(fp,c);
    printf("%zu > ", hsize);
    fgets(buf, bufsize, stdin);

    const Result r = interpret_command(buf, &his,c);
    if (r == EXIT) break;
    if (r == NORMAL) {
      Node *N=(Node*)malloc(sizeof(Node));
      char *L=(char*)malloc(sizeof(char)*(his.bufsize));
      strcpy(L,buf);
      *N= (Node){.str=L};
      if (his.begin == NULL) { 
        his.begin=N;
      }else{
        Node *p = his.begin;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next=N;
      }
      his.hsize++;
    }
    printf("pen:\"%c\"\n",c->pen);
    rewind_screen(fp,2); // command results
    clear_command(fp); // command itself
    rewind_screen(fp, height+2); // rewind the screen to command input
  }
  clear_screen(fp);
  free_canvas(c);
  fclose(fp);

  return 0;
}

Canvas *init_canvas(int width,int height, char pen)
{
  Canvas *new = (Canvas *)malloc(sizeof(Canvas));
  new->width = width;
  new->height = height;
  new->canvas = (char **)malloc(width * sizeof(char *));

  char *tmp = (char *)malloc(width*height*sizeof(char));
  memset(tmp, ' ', width*height*sizeof(char));
  for (int i = 0 ; i < width ; i++){
    new->canvas[i] = tmp + i * height;
  }
  
  new->pen = pen;
  return new;
}

void reset_canvas(Canvas *c)
{
  const int width = c->width;
  const int height = c->height;
  memset(c->canvas[0], ' ', width*height*sizeof(char));
}


void print_canvas(FILE *fp, Canvas *c)
{
  const int height = c->height;
  const int width = c->width;
  char **canvas = c->canvas;
  // 上の壁
  fprintf(fp,"+");
  for (int x = 0 ; x < width ; x++)
    fprintf(fp, "-");
  fprintf(fp, "+\n");

  // 外壁と内側
  for (int y = 0 ; y < height ; y++) {
    fprintf(fp,"|");
    for (int x = 0 ; x < width; x++){
      const char c = canvas[x][y];
      fputc(c, fp);
    }
    fprintf(fp,"|\n");
  }
  
  // 下の壁
  fprintf(fp, "+");
  for (int x = 0 ; x < width ; x++)
    fprintf(fp, "-");
  fprintf(fp, "+\n");
  fflush(fp);
}

void free_canvas(Canvas *c)
{
  free(c->canvas[0]); //  for 2-D array free
  free(c->canvas);
  free(c);
}

void rewind_screen(FILE *fp,unsigned int line)
{
  fprintf(fp,"\e[%dA",line);
}

void clear_command(FILE *fp)
{
  fprintf(fp,"\e[2K");
}

void clear_screen(FILE *fp)
{
  fprintf(fp, "\e[2J");
}

int max(const int a, const int b)
{
  return (a > b) ? a : b;
}

void draw_line(Canvas *c, const int x0, const int y0, const int x1, const int y1)
{
  const int width = c->width;
  const int height = c->height;
  char pen = c->pen;
  
  const int n = max(abs(x1 - x0), abs(y1 - y0));
  c->canvas[x0][y0] = pen;
  for (int i = 1; i <= n; i++) {
    const int x = x0 + i * (x1 - x0) / n;
    const int y = y0 + i * (y1 - y0) / n;
    if ( (x >= 0) && (x< width) && (y >= 0) && (y < height))
      c->canvas[x][y] = pen;
  }
  //printf("1 line drawn\n");
}

void draw_rectangle(Canvas *c, const int x0, const int y0, const int x1, const int y1)
{
  draw_line(c,x0, y0, x1, y0);
  draw_line(c,x0, y0, x0, y1);
  draw_line(c,x1, y1, x1, y0);
  draw_line(c,x1, y1, x0, y1);
}

void draw_circle(Canvas *c, const int x0, const int y0, const int r)
{
  const int width = c->width;
  const int height = c->height;
  char pen = c->pen;
  double d;
  double maxd;
  double e=0.5;
  for (int i=y0-r; i<=y0+r; i++) {
    for(int j=x0-r;j<=x0+r;j++){
      d=sqrt((i-y0)*(i-y0)+(j-x0)*(j-x0));
      if(fabs(d-r)<e){
        if ( (j>= 0) && (j< width) && (i>= 0) && (i< height)){
          c->canvas[j][i] = pen;
        }
      }
    }
  }
}

void save_history(const char *filename, History *his)
{
  const char *default_history_file = "history.txt";
  if (filename == NULL)
    filename = default_history_file;
  
  FILE *fp;
  if ((fp = fopen(filename, "w")) == NULL) {
    fprintf(stderr, "error: cannot open %s.\n", filename);
    return;
  }

  Node *p = his->begin;
  while (p!= NULL) {
    fprintf(fp, "%s", p->str);
    p = p->next;
  }
  fclose(fp);
}

void load_history(const char *filename, History *his, Canvas *c)
{
  const char *default_history_file = "history.txt";
  if (filename == NULL)
    filename = default_history_file;
  FILE *fp;
  if ( (fp = fopen(filename,"r")) != NULL ) {
    while(his->begin != NULL){
      undo(his,c);
    }
    reset_canvas(c);
    char buf[his->bufsize];
    while(fgets(buf, his->bufsize, fp)!=NULL){
      const Result r = interpret_command(buf, his,c);
      if (r == NORMAL) {
        Node *N=(Node*)malloc(sizeof(Node));
        char *L=(char*)malloc(sizeof(char)*(his->bufsize));
        strcpy(L,buf);
        *N= (Node){.str=L};
        if (his->begin == NULL) { 
          his->begin=N;
        }else{
          Node *p = his->begin;
          while (p->next != NULL) {
            p = p->next;
          }
          p->next=N;
        }
        his->hsize++;
      }
    }
    printf("load \"%s\"\n",filename);
  }
  else{
    printf("cannot open file \"%s\"\n",filename);
  }
  fclose(fp);
}

void undo(History *his, Canvas *c)
{
  if (his->begin != NULL){
    Node *p = his->begin;
    c->pen='*';
    if(p->next== NULL){
      free(p->str);
      free(p);
      his->begin = NULL;
    }else{
      while (p->next->next != NULL) {
        interpret_command(p->str, his, c);
        p = p->next;
      }
      interpret_command(p->str, his, c);
      free(p->next->str);
      free(p->next);
      p->next=NULL;
    }
    his->hsize--;
  }
}

Result interpret_command(const char *command, History *his, Canvas *c)
{
  char buf[his->bufsize];
  strcpy(buf, command);
  buf[strlen(buf) - 1] = 0; // remove the newline character at the end

  const char *s = strtok(buf, " ");

  // The first token corresponds to command
  if (strcmp(s, "line") == 0) {
    int x0, y0, x1, y1;
    x0 = 0; y0 = 0; x1 = 0; y1 = 0; // initialize
    char *b[4];
    for (int i = 0 ; i < 4; i++){
      b[i] = strtok(NULL, " ");
      if (b[i] == NULL){
        printf("the number of point is not enough.\n");
        return COMMAND;
      }
    }
    x0 = strtol(b[0],NULL,10);
    y0 = strtol(b[1],NULL,10);
    x1 = strtol(b[2],NULL,10);
    y1 = strtol(b[3],NULL,10);

    draw_line(c,x0, y0, x1, y1);
    printf("1 line drawn\n");
    return NORMAL;
  }
  
  if (strcmp(s, "rectangle") == 0) {
    int x0, y0, x1, y1;
    x0 = 0; y0 = 0; x1 = 0; y1 = 0;
    char *b[4];
    for (int i = 0 ; i < 4; i++){
      b[i] = strtok(NULL, " ");
      if (b[i] == NULL){
        printf("the number of point is not enough.\n");
        return COMMAND;
      }
    }
    x0 = strtol(b[0],NULL,10);
    y0 = strtol(b[1],NULL,10);
    x1 = strtol(b[2],NULL,10);
    y1 = strtol(b[3],NULL,10);

    draw_rectangle(c,x0, y0, x1, y1);
    printf("1 rectangle drawn\n");
    return NORMAL;
  }

  if (strcmp(s, "circle") == 0) {
    int x0, y0, r;
    x0 = 0; y0 = 0; r = 0;
    char *b[3];
    for (int i = 0 ; i < 3; i++){
      b[i] = strtok(NULL, " ");
      if (b[i] == NULL){
        printf("the number of point is not enough.\n");
        return COMMAND;
      }
    }
    x0 = strtol(b[0],NULL,10);
    y0 = strtol(b[1],NULL,10);
    r = strtol(b[2],NULL,10);

    draw_circle(c,x0, y0, r);
    printf("1 circle drawn\n");
    return NORMAL;
  }

  if (strcmp(s, "save") == 0) {
    s = strtok(NULL, " ");
    save_history(s, his);
    printf("saved as \"%s\"\n",(s==NULL)?"history.txt":s);
    return COMMAND;
  }

  if (strcmp(s, "load") == 0) {
    s = strtok(NULL, " ");
    load_history(s,his,c);
    return COMMAND;
  }

  if (strcmp(s, "undo") == 0) {
    reset_canvas(c);
    undo(his,c);
    return COMMAND;
  }

  if (strcmp(s, "pen") == 0) {
    s = strtok(NULL, " ");
    if(s!=NULL){
      int i;
      for(i=0; s[i]!='\0'; ++i);
      if(i==1){
        char pen=(char)s[0];
        c->pen=pen;
      }else{
        printf("usage: pen <character>\n");
        return COMMAND;
      }
    }else{
      printf("usage: pen <character>\n");
      return COMMAND;
    }
    return NORMAL;
  }

  if (strcmp(s, "quit") == 0) {
    return EXIT;
  }

  printf("error: unknown command.\n");

  return COMMAND;
}