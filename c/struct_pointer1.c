/*ポインタによる構造体型変数へのアクセス
構造体を指すポインタ -> 構造体のメンバ名 
（* 構造体を指すポインタ）.構造体のメンバ名　と同じ
*/

typedef struct 
{
    double re;
    double im;
};complex_t;

int main(void){
    complex_t c;
    complex_t* cp;

    cp = &c; //cのｐアドレスをcpに代入
}

