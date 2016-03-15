#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

#define MAX_H 100
#define MAX_W 100
#define CELL_SIZE 2

struct Point{
    int x,y;
    Point(int X,int Y){
       x = X;
       y = Y;
    }
    Point(){};
};

struct KeyPoint{
    int x,y,r;
};

int dir[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
int mapW,mapH,level;
char Map[MAX_H][MAX_W];
char visited[MAX_H][MAX_W];
int key;
int longest,deadends,longestp[2];

Point startPoint;
KeyPoint doorPosition,keyPosition;
vector<Point>starList;

char gMap[MAX_H*CELL_SIZE][MAX_W*CELL_SIZE];

int dfs(int x,int y,int l)
{
    visited[y][x]=1;
    ++ l;
    vector<int>dirIn;
    for(int i=0;i<4;i++)
    {
        dirIn.push_back(i);
    }
    random_shuffle(dirIn.begin(),dirIn.end());
    int deadend = 1;
    for(vector<int>::iterator it = dirIn.begin();it!=dirIn.end();it++){
        int idx = *it;
        int dx = x + dir[idx][0];
        int dy = y + dir[idx][1];

        if(dx<=0||dy<=0||dy>=mapH-1||dx>=mapW-1)
            continue;
        int dx2 = dx + dir[idx][0];
        int dy2 = dy + dir[idx][1];
        if(visited[dy2][dx2])
            continue;

        Map[dy][dx] = ' ';

        dfs(dx2,dy2,l);
        deadend = 0;
    }
    if(deadend && l > 3){
        deadends ++;
        Map[y][x] = '?';
        Point p(x,y);
        starList.push_back(p);
        if(l > longest){
            longest = l;
            longestp[0] = x;
            longestp[1] = y;
        }
    }
    return 0;
}

void initMap(){
    starList.clear();
    longest = deadends = 0;
    memset(Map,' ',sizeof(Map));
    memset(visited,0,sizeof(visited));
    for(int i=0;i<mapH;i++)
    {
        for(int j=0;j<mapW;j++){
            if(i%2 == 0)
                Map[i][j] = '*';
            else if(j%2 == 0)
                Map[i][j] = '*';
        }
    }
}

void printMap(){
    cout << endl;
    for(int i=0;i<mapH;i++){
        for(int j=0;j<mapW;j++){
            cout << Map[i][j];
        }
    }
    cout << endl;
}

void initGMap(){
    memset(gMap,' ',sizeof(gMap));
    for(int i=0;i<mapH;i++)
    {
        for(int j=0;j<mapW;j++){
            int gx = j*CELL_SIZE + CELL_SIZE/2;
            int gy = i*CELL_SIZE + CELL_SIZE/2;
            if(Map[i][j]!='*'){
                gMap[gy][gx] = Map[i][j];
                continue;
            }
            for(int k = 0;k<4;k++){
                int dx = j + dir[k][0];
                int dy = i + dir[k][1];
                if(dx < 0|| dy <0||dx >= mapW||dy>=mapH)
                    continue;
                if(Map[dy][dx]!='*')
                    continue;
                int gx2 = gx;
                int gy2 = gy;
                do{
                    if(i==0||j==0||(i>=3&&i<=7)||(j>=17&&j<=21)||(i>=33&&i<36)||
                       (j<=47&&j>=45)||(i<=59&&i>=62)||(j<=71&&j>=74)||(i>=89&&i<=92)||i==mapH-1||j==mapW-1)
                        gMap[gy2][gx2] = '*';
                    else gMap[gy2][gx2] = '#';
                    if(gMap[gy2][gx2] == '#'){
                        switch(rand()%15){
                      case 0:
                        gMap[gy2][gx2] = 'f';
                        break;
                      case 2:
                        gMap[gy2][gx2] = 't';
                        }
                    }
                    gx2 += dir[k][0];
                    gy2 += dir[k][1];
                }while(gx2/CELL_SIZE == j && gy2/CELL_SIZE == i);
            }
        }
    }
}

void printGMap(){
    cout << endl;
    for(int i=0;i<mapH*CELL_SIZE;i++)
    {
        for(int j=0;j<mapW*CELL_SIZE;j++)
        {
            cout << gMap[i][j];
        }
        cout << endl;
    }
    ofstream outFile;
    outFile.open("map5.map");
    outFile << endl;
    for(int i=0;i<mapH*CELL_SIZE;i++)
    {
        for(int j=0;j<mapW*CELL_SIZE;j++)
        {
            outFile << gMap[i][j];
        }
        outFile << endl;
    }
    outFile.close();
}
void addKey(int x,int y)
{
    if(Map[y][x] == '?'){
        if((rand()%100)<90){
            keyPosition.x = x;
            keyPosition.y = y;
        }
    }

    vector<int> dirIN;
    for(int i=0;i<4;i++)
        dirIN.push_back(i);
    random_shuffle(dirIN.begin(),dirIN.end());
    for(vector<int>::iterator it=dirIN.begin();it!=dirIN.end();it++){
        int idx = *it;
        int dx = x + dir[idx][0];
        int dy = y + dir[idx][1];

        if(dx <= 0|| dy <=0 || dy >= mapH - 1 || dx >= mapW - 1)
            continue;
        if(visited[dy][dx] || Map[dy][dx] == '*')
            continue;
        addKey(dx,dy);
    }
}

void addObjects(int sx, int sy)
{
	const int queueSize = 200;

	int q[queueSize][2];
	int qi = 0, qj = 0;
	int thingsCount = rand()%12+level*4;

	memset(visited, 0, sizeof(visited));
	q[0][0] = sx;
	q[0][1] = sy;
	qj = 1;
	while(qj > qi && thingsCount>0){
		int x, y, i;
		x = q[qi][0];
		y = q[qi][1];
		qi++;
		for(i=0; i<4; i++){
			int dx = x + dir[i][0];
			int dy = y + dir[i][1];
			if(visited[dy][dx] || gMap[dy][dx]!=' ')
				continue;

			visited[dy][dx] = 1;
			if(qj >= queueSize)
				break;

            switch(rand()%11){
           case 0:
              gMap[dy][dx] = 'r';
              break;
           case 1:
              gMap[dy][dx] = '+';
              break;
           /*case 2:case 11:
              gMap[dy][dx] = 'b';
              break;*/
           case 3:case 4:case 5:
              gMap[dy][dx] = '$';
              break;
           case 6:case 7:case 8:
              gMap[dy][dx] = 'm';
              break;
           case 9:case 10:
              gMap[dy][dx] = 'g';
              break;
            }
            thingsCount --;

            q[qj][0] = dx;
            q[qj][1] = dy;
            qj++;
        }
    }
}
void decorate()
{
    int x2,y2 = CELL_SIZE/2;
    do{
        x2 = (rand() % (mapW-1))*CELL_SIZE + CELL_SIZE/2;
        if(gMap[y2+1][x2]!='*')
            break;
    }while(true);
    gMap[y2][x2] = 'o';

    for(int i=0;i<starList.size();i++){
        const Point& p = starList[i];
        addObjects(p.x*CELL_SIZE + CELL_SIZE/2 , p.y*CELL_SIZE + CELL_SIZE/2);
    }
    while(gMap[y2][x2] !=' '){
        x2 = (rand()%(mapW - 1))*CELL_SIZE + CELL_SIZE/2;
        y2 = (rand()%(mapH - 1))*CELL_SIZE + CELL_SIZE/2;
    }
    gMap[y2][x2] = 'k';
    for(int i = 3; i <= 3 + level * 2 ; i++){
        while(gMap[y2][x2] !=' '){
        x2 = (rand()%(mapW - 1))*CELL_SIZE + CELL_SIZE/2;
        y2 = (rand()%(mapH - 1))*CELL_SIZE + CELL_SIZE/2;
    }
        gMap[y2][x2] = 'b';
    }
}
#define rdtsc(lowpart,highpart)__asm__("rdtsc":"=a"(lowpart),"=d"(highpart))
int main(int argc,char **argv)
{
    mapW = 41;
    mapH = 21;
    level = 1;

    for(int i = 1;i<argc;i++){
        if(strcmp(argv[i],"-w")==0)
            mapW = atoi(argv[++i]);
        else if(strcmp(argv[i],"-h")==0)
            mapH = atoi(argv[++i]);
        else if(strcmp(argv[i],"-l")==0)
            level = atoi(argv[++i]);
    }
        if(mapW%2==0)
            mapW++;
        if(mapH%2==0)
            mapH++;

        int low,high;
        rdtsc(low,high);
        srand(low);

        initMap();
        startPoint.x = (rand()%(mapW/2))*2+1;
        startPoint.y = (rand()%(mapH/2))*2+1;
        Map[startPoint.y][startPoint.x] = 's';

        dfs(startPoint.x,startPoint.y,0);

        addKey(startPoint.x,startPoint.y);

        initGMap();

        decorate();

        printGMap();
        return 0;
}















