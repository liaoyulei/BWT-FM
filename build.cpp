#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define N 938452060
#define LN 134217727 //留一个结束符#
#define LO 8388608 //LN>>4
#define LC 256
char s0[N], s[LN + 1];
int a[LN + 1], c[5], occ[4][LO];

int compare(const void *a, const void *b) {//比较函数比较s[x...n),s[y...n)的字典序
	return strcmp(s + *(int *)a, s + *(int *)b);
}

int chartoint(char c) {//将ACGT映射到0123
	switch(c) {
		case 'A': return 0;
		case 'C': return 1;
		case 'G': return 2;
		case 'T': return 3;
	}
	printf("error character %c\n", c);
	exit(0);
}

void bwt(int begin, int end) {//s[0]...s[n-2]中存储字符，s[n-1]中存储虚拟结束符
	int ch, n = end - begin + 1;
	for(int i = 0; i < n; i++) {
		a[i] = i;
	}
	qsort(a, n, sizeof(int), compare);
	printf("Finish building suffix array from %d to %d\n", begin, end);
	if(a[0]) {//若A[i]=0表示该处最后一列字符为终结符
		ch = chartoint(s[a[0] - 1]);
		c[ch + 1]++;
		occ[ch][0]++;
	}
	for(int i = 1; i <= (n - 1) >> 4; i++) {//此处为下取整
		for(int j = 0; j < 4; j++) {
			occ[j][i] = occ[j][i - 1];
		}
		for(int j = ((i - 1) << 4) + 1; j <= i << 4; j++) {
			if(a[j]) {
				ch = chartoint(s[a[j] - 1]);
				c[ch + 1]++;
				occ[ch][i]++;
			}
		}
	}
	for(int i = ((n - 1) >> 4 << 4) + 1; i < n; i++) {
		if(a[i]) {
			ch = chartoint(s[a[i] - 1]);
			c[ch + 1]++;
		}
	}
	for(int i = c[0] = 1; i < 5; i++) {
		c[i] += c[i - 1];
	}
	printf("Finish building index from %d to %d\n", begin, end);
}

int main(int argc, char **argv) {
    char filename[35] = {"data/"};
	int n, begin, end = LC;
	clock_t timebegin, timeend;
	FILE *fp, *fpb;
 	strcat(filename, argv[1]);
	strcat(filename, "_rows_ATCG.txt");
	fp = fopen(filename, "r");
	if(fp) {
        printf("Building index based file %s\n", filename);
	}
	else{
		printf("No file named %s\n", filename);
		exit(0);
	}
	fscanf(fp, "%d%s", &n, s0);
	fclose(fp);
	if(strlen(s0) <= n) { //修正错误数据
		n = strlen(s0) - 1;
	}
	printf("The length of template is %d\n", n);
	strcpy(filename, argv[1]);
	strcat(filename, ".dat");
	fpb = fopen(filename, "wb");
	if(fpb) {
		printf("Saving index in file %s\n", filename);
	}
	else{
		printf("Cannot create file %s\n", filename);
		exit(0);
	}
	timebegin = clock();
	while(end < n) {
		begin = end - LC;
		end = begin + LN < n ? begin + LN : n;
		memset(c, 0, sizeof(int) * 5);
		memset(occ, 0, sizeof(int) * LO << 2);
		strncpy(s, s0 + begin, end - begin);
		s[end - begin] = '\0';
		bwt(begin, end);
		fwrite(a, sizeof(int), LN + 1, fpb);
		fwrite(c, sizeof(int), 5, fpb);
		fwrite(occ, sizeof(int), LO << 2, fpb);
		printf("Finish saving index from %d to %d\n", begin, end);
	}
	timeend = clock();
	fclose(fpb);
	printf("Using %fs to build and save index\n", (float)(timeend - timebegin) / CLK_TCK);
	return 0;
}
