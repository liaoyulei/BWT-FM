#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <iostream>
#define N 938452060
#define LN 134217727 //留一个结束符#
#define LO 8388608 //LN>>4
#define LC 256
#define M 74
using namespace std;
char s0[N];
int a[LN + 1], c[5], occ[4][LO];

int chartoint(char c) {
	switch(c) {
		case 'A': return 0;
		case 'C': return 1;
		case 'G': return 2;
		case 'T': return 3;
	}
	printf("error character %c\n", c);
	exit(0);
}

int getocc(int ch, int p, int begin) {//得到ch在[0, p]行最后一列出现的次数
	int ans = occ[ch][p >> 4];
	for(int i = (p >> 4 << 4) + 1; i <= p; i++) {
		if(a[i] && chartoint(s0[begin + a[i] - 1]) == ch) {
			ans++;
		}
	}
	return ans;
}

void fm(char *s, int begin, set<int> &ans) {//S[0]...S[m-1]中存储字符
	int ch = chartoint(s[strlen(s) - 1]);
	int sp = c[ch], ep = c[ch + 1] - 1;
	for(int i = strlen(s) - 2; i >= 0 && sp <= ep; i--) {
		ch = chartoint(s[i]);
		sp = c[ch] + getocc(ch, sp - 1, begin);
		ep = c[ch] + getocc(ch, ep, begin) - 1;
	}
	for(int i = sp; i <= ep; i++) {
		ans.insert(a[i] + begin);
	}
}

int main(int argc, char **argv) {
	char filename[35] = {"data/"}, s[LC];
	int n, begin, end = LC;
	clock_t timebegin, timeend;
	FILE *fp, *fpb;
	set<int> ans[M];
	strcat(filename, argv[1]);
	strcat(filename, "_rows_ATCG.txt");
	fp = fopen(filename, "r");
	if(fp) {
		printf("Getting template in file %s\n", filename);
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
	fpb = fopen(filename, "rb");
	if(fpb) {
		printf("Getting index in file %s\n", filename);
	}
	else {
		printf("No file named %s\n", filename);
		exit(0);
	}
	strcpy(filename, "data/search_");
	strcat(filename, argv[1]);
	strcat(filename, ".txt");
	timebegin = clock();
	while(end < n) {
		begin = end - LC;
		end = begin + LN < n ? begin + LN : n;
		fread(a, sizeof(int), LN + 1, fpb);
		fread(c, sizeof(int), 5, fpb);
		fread(occ, sizeof(int), LO << 2, fpb);
		fp = fopen(filename, "r");
		for(int i = 0; i < M; i++) {
			fscanf(fp, "%s", s);
			fm(s, begin, ans[i]);
		}
		fclose(fp);
	}
	timeend = clock();
	fclose(fpb);
	for(int i = 0; i < M; i++) {
		printf("Finding %1d POS in string %2d:", ans[i].size(), i);
		for(set<int>::iterator it = ans[i].begin(); it != ans[i].end(); it++) {
            printf(" %5d", *it);
		}
		printf("\t");
	}
	printf("Using %fs to search %d strings, ", (float)(timeend - timebegin) / CLK_TCK, M);
	printf("The average cost is %fs\n", (float)(timeend - timebegin) / M / CLK_TCK);
	return 0;
}
