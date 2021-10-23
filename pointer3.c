# include <stdio.h>

//int型の変数aとint型のポインタ変数*bを宣言
//aも*bも値が入っておらず初期化されていないため、アドレスが不定
//そのため、ポインタの指す先に値を代入する前に、必ず適切なアドレスの値でポインタを初期化する必要あり。

 
int a;
int *b;

int a = 1;

void ptr_1(int *a);

void ptr_1(int *a){
    *a = 10;
    printf("a: %d\n", *a);
}

void ptr_2(int a, int *b){
    b = &a; //*bのアドレスの値が不定なので、aのアドレスを使って初期化(*bのアドレスとaのアドレスが一致)
    *b = 300;

    printf("b= %d\n", *b);
    printf("address of a: %p\n", &a);
    printf("address of b: %p\n", b);

}

int main(void){
    ptr_1(&a);
    ptr_2(a, b);
}