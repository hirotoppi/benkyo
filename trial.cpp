//standard lib
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

#define MAP_PIXEL 0.1L /* 単位はm */

typedef struct {
	int w, h, wo, ho; //w:wide, h:height, woとhoは地図の原点？
	double p; //ピクセル
	double **mz, **sig; //z軸の磁気強度mzと、分散σ。この分散はどこから来たなんのための分散？
} Map;


Map map;
double minx, maxx, miny, maxy;
int u,v;

char *filepath = NULL;



void
localize_map_init(char *f, Map *m, double *minx, double *maxx, double *miny, double *maxy)
{
	int u, v, wl, hl;
	double x, y, mz, sig;
	char buf[256];
	FILE *fp;

	sprintf(buf, "%szmagmap.txt", f); //filepathがfに代入されて、それが%sに入る。
	fp = fopen(buf, "r"); //読み込み専用でファイルを開く
	if (fp == NULL)	perror("file open"), exit(1);
	*minx = *miny =  1000.0L;
	*maxx = *maxy = -1000.0L;
	while ((fscanf(fp, "%d %d %lf %lf %lf %lf", &u, &v, &x, &y, &mz, &sig)) != EOF) {
		if (*minx > x)	*minx = x;
		if (*maxx < x)	*maxx = x;
		if (*miny > y)	*miny = y;
		if (*maxy < y)	*maxy = y;
	}
	fclose(fp);
	m->w   = u + 1;
	m->h   = v + 1;
	m->p   = MAP_PIXEL;
	wl     = abs((int)(*maxx / m->p)); //xの最大値が何ピクセルかを算出
	hl     = abs((int)(*miny / m->p)); 
	m->wo  = m->w - wl; 
	m->ho  = m->h - hl;
	m->mz  = (double **)malloc(m->w * sizeof(double *));
	m->sig = (double **)malloc(m->w * sizeof(double *));
	if (m->mz == NULL || m->sig == NULL)	perror("memory get"), exit(1);
	for (u = 0; u < m->w; u++) {
		m->mz[u]  = (double *)malloc(m->h * sizeof(double));
		m->sig[u] = (double *)malloc(m->h * sizeof(double));
		if (m->mz[u] == NULL || m->sig[u] == NULL)	perror("memory get"), exit(1);
	}
}


void
localize_read_mapdata(char *f, Map *m)
{
	int u, v;
	double x, y, mz, sig;
	char buf[256];
	FILE *fp;

	sprintf(buf, "%szmagmap.txt", f); //f:file path
	fp = fopen(buf, "r"); //読み込み専用でファイルを開く
	if (fp == NULL)	perror("file open"), exit(1);

	/*読み込んだファイルから磁気強度mzをMap構造体のオブジェクトmの中のmzに(u,v)を指定して代入*/
	/*fscanf : 読み取りに成功した項目の個数を返す*/
	/*while(fscanf(~) != EOF) : 引数で指定した項目が読み取れなくなるまで繰り返し処理を行う*/
	/*fpで先頭アドレスを指定したファイルに格納されてる情報を、Map構造体のオブジェクトに代入するをループ*/
	while ((fscanf(fp, "%d %d %lf %lf %lf %lf", &u, &v, &x, &y, &mz, &sig)) != EOF) {
		m->mz[u][v]  = mz;
		m->sig[u][v] = sig; //std?var?どっちなの？
		//printf("%f %f\n", mz, sig);
	}
	//printf("%f %f\n", mz, sig);

	fclose(fp);
}

void
localize_xy_to_uv(Map *m, double *x, double *y, int *u, int *v)
{
	*u = m->wo + (int)(x / m->p); //ポインタを決めてる？
	*v = m->ho - (int)(y / m->p);
	if (*u < 0)		*u = 0;
	if (*u >= m->w)	*u = m->w - 1;
	if (*v < 0)		*v = 0;
	if (*v >= m->h)	*v = m->h - 1;
}

void
main(void)
{
    localize_map_init(filepath, &map, &minx, &maxx, &miny, &maxy);
	localize_read_mapdata(filepath, &map); //mapdataを読み込む関数
    localize_xy_to_uv(&map, &minx, &maxx, &u, &maxy);
}
