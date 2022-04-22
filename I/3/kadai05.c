#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define K 5 // スロットマシンの数
#define N 1000000 // コインの数．採点時には1,000,000などの大きな数にする．
int bet(int k); //変更不可

int main() { /*手持ちのコインの半数をK台のマシンに均等に賭け、その結果最も稼げた台に残額を全額投資するアルゴリズム*/
             /*(当選金)÷(掛け金N)の値10回の平均は1.075091となった*/
    srand((unsigned) time(NULL));
    int credit = N, reward=0, sum=0;
    int testM = N/1000; /*それぞれの台の儲けやすさを調べるのに賭ける額*/
    int testresult[K]; /*K台のマシンそれぞれのテスト時の結果を入れる配列*/

    for(int i=1;i<testM ; i+=K){ /*限度額までそれぞれの台の儲け額を調べる*/
        for(int k=0;k<K;k++){
            testresult[k] = bet(k);
            reward = bet(k);
            sum += reward;
            credit = credit-1;
        }
    }

    int l=0; /*最も稼げる台の番号*/
    int testmax=0; /*最も稼いだ額*/
    for(int j=0;j<K;j++){ /*最も稼げる台の番号を抽出*/
        if(testresult[j]>testmax){
            testmax = testresult[j];
            l=j;
        } 
    }

    while(credit-- > 0){ /*残額をすべて最も稼げる台に賭ける*/
        reward = bet(l);
        sum += reward;
    }

    printf("ratio = %f\n",(double) sum / N );
        
    return 0;
}
// 以下の関数は変更不可
int bet(int k){ // 入力：スロットマシンの番号 出力：当選金
    // これは例であり，採点時には当選金と確率が変化する
    switch(k){
        case 0:
            if(rand() % 2 == 0){ // 2/2
                return 2;
            }else{
                return 0;
            }
            break;
        case 1:
            if(rand() % 7 == 0){ // 5/7
                return 5;
            }else{
                return 0;
            }
            break;
        case 2:
            if(rand() % 8 == 0){ // 10/8
                return 10;
            }else{
                return 0;
            }
            break;
        case 3:
            if(rand() % 40 == 0){ // 50/4０
                return 50;
            }else{
                return 0;
            }
            break;
        case 4:
            if(rand() % 200 == 0){  // 100/200
                return 100;
            }else{
                return 0;
            }
            break;
        default:
            return 0;
    }
}