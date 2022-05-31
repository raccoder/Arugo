#include <stdio.h>
#include <string.h>
#include <limits.h>
#define MAX 10
#define NAME_LEN 20
/*--- KMP 法による文字列探索 ---*/
#define PSC 1 /* Position of the Second Character */
#define MaxPatt 1024 /* パターンの最大文字数 */
/*--- 身体データ型 ---*/
typedef struct{
   double vision; /* 視力 */
  int height; /* 身長 */
} Body ;
/*--- 身体検査データ型 ---*/
typedef struct{
   char name[NAME_LEN]; /* 氏名 */
  Body body; /* 身体データ型 ---*/
} PhysCheck ;
/*--- 身体検査データ型スタックを実現する構造体 ---*/
typedef struct {
  int max; /* スタックの容量 */
  int ptr; /* スタックポインタ */
  PhysCheck stk[MAX]; /* スタック本体*/
} PhysCheckStack;
/* KMP 法による文字列探索 ---*/
char *kmp_match(char *pat, char *txt){
  char *pt ; /* txt をなぞるカーソル */
  char *pp ; /* pat をなぞるカーソル */
  char *skip[MaxPatt]; /* スキップテーブル */
  pp = pat;
  skip[PSC] = pp;
  pt = pp + PSC;
  while (*pt != '\0') {
    if (*pt == *pp)
      skip[++pt-pat] = ++pp;
    else if (pp == pat)
      skip[++pt-pat] = pp;
    else
      pp = skip[pp - pat];
 }
 for(pp = pat, pt = txt; *pt != '\0' && *pp != '\0';){
  if (*pt == *pp) {
    pt++;
    pp++;
 }
  else if (pp == pat)
    pt++;
   else
    pp = skip[pp - pat];
 }
    if (*pp == '\0') return pt - strlen(pat);
    return NULL;
}
/*--- スタックの初期化 ---*/
int Initialize(PhysCheckStack *s, int max){
 s->ptr = 0;
 s->max = max;
 return 0;
}
/*--- スタックにデータをプッシュ ---*/
int Push(PhysCheckStack *s, PhysCheck x){
 if (s->ptr >= s->max) return -1; /* スタック満杯 */
    s->stk[s->ptr] = x;
    s->ptr++;
 return 0;
}
/*--- スタックからデータをポップ ---*/
int Pop(PhysCheckStack *s, PhysCheck *x){
 if (s->ptr <= 0) return -1; /* スタックは空 */
    s->ptr--;
    *x = s->stk[s->ptr];
 return 0;
}
/*--- スタックからデータをピーク ---*/
int Peek(PhysCheckStack *s, PhysCheck *x){
 if (s->ptr <= 0) return -1;
    *x = s->stk[s->ptr - 1];
 return 0;
}
/*--- スタックの容量 ---*/
int Capacity(const PhysCheckStack *s){
 return s->max;
}
/*--- スタックに積まれているデータ数 ---*/
int Size(const PhysCheckStack *s){
 return s->ptr;
}
/*--- 身体検査データを１つ表示 ---*/
void OneDataPrint(const PhysCheck *x){
 printf("%-18.18s%4d%5.1f\n", x->name, x->body.height, x-> body.vision);
}
/*--- スタックの全データの表示 ---*/
void Print(const PhysCheckStack *s){
 int i;

 for(i = 0; i < s->ptr; i++)
 OneDataPrint(s->stk+i);
 putchar('\n');
}
/*--- スタックのデータにあるパターンの表示とその数を数える ---*/
int Search(PhysCheckStack *s, PhysCheck *x){
 int i, last, num = 0;
 char *txt, *pat = x->name;
for(i = s->ptr -1; i >= 0; i--){ /* 頂上から底へ */
  txt = s->stk[i].name;
  while((txt = kmp_match(pat, txt)) != NULL){
  num ++;
  txt ++;
  last = i;
 }
 }
 if (num > 0) OneDataPrint(s->stk + last);
    return num;
}
int main(void){
 PhysCheckStack s;
 Initialize(&s, MAX);
 while (1) {
 int menu, num;
 PhysCheck x;

 printf("現在のデータ数：%d/%d\n",Size(&s), Capacity(&s));
 printf("(1) プッシュ (2) ポップ (3) ピーク (4) 表示 (5)探索 (0) 終了：");
 scanf("%d", &menu);
 if (menu == 0) break;
  switch (menu) {
    case 1: /* プッシュ */
    printf("名前: "); scanf("%s", x.name);
    printf("身長: "); scanf("%d", &(x.body.height));
    printf("視力: "); scanf("%lf", &(x.body.vision));
 if (Push(&s, x) == -1)
 puts("\a エラー：プッシュに失敗しました。\n");
 break;
 case 2: /* ポップ */
 if (Pop(&s, &x) == -1)
    puts("\a エラー：ポップに失敗しました。\n");
 else{
    printf("ポップしたデータは，");
    OneDataPrint(&x);
 }
 break;
 case 3: /* ピーク */
 if (Peek(&s, &x) == -1)
    puts("\a エラー：ピークに失敗しました。\n");
 else{
    printf("ピークしたデータは，");
    OneDataPrint(&x);
 }
 break;
 case 4: /* 表示 */
 Print(&s);
 break;
 case 5: /* 検索 */
 printf("探す名前:"); scanf("%s", x.name);
 if ((num = Search(&s, &x)) >0)
      printf("名前に含まれるパターン数は，%d 個です，\n",num);
 else{
      puts("\a エラー：パターンは存在しません。\n ");
 }
 break;
 }
 }
 return 0;
}