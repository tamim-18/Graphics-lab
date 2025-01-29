///2018331085

#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <bits/stdc++.h>
using namespace std;

//Edge with end points
struct E_points{
    int x1, y1, x2, y2;
    E_points(int xx1, int yy1, int xx2, int yy2) {
        if(yy1 > yy2) {
            swap(xx1, xx2);
            swap(yy1, yy2);
        }
        x1 = xx1;
        y1 = yy1;
        x2 = xx2;
        y2 = yy2;
    }
};

struct Edge{
    int y_min, y_max, x, id;
    double m_inv;

    Edge(E_points e) {
        y_min = e.y1;
        y_max = e.y2;
        x = e.x1;
        m_inv = (e.x2 - e.x1) / (e.y2 - e.y1);
    }
};

vector< pair<int, int> > points;
vector< Edge > EdgeList;
vector< E_points > original_edges;
set<int> active; // id

bool cmp(Edge a,Edge b){
    return a.y_min<b.y_min;
}

void setPixel(float x, float y){
    glVertex3f(x, y, 0.0);
}

void input() {
    /*
    points.push_back({0, -4});
    points.push_back({-4, -2});
    points.push_back({-4, 0});
    points.push_back({-2, 2});
    points.push_back({0, 0});
    points.push_back({4, 4});
    points.push_back({4, -4});
    */

    original_edges.push_back(E_points(0, -4, -4, -2));
    original_edges.push_back(E_points(-4, -2, -4, 0));
    original_edges.push_back(E_points(-4, 0, -2, 2));
    original_edges.push_back(E_points(-2, 2, 0, 0));
    original_edges.push_back(E_points(0, 0, 4, 4));
    original_edges.push_back(E_points(4, 4, 4, -4));
    original_edges.push_back(E_points(4, -4, 0, -4));

    //testing
    glColor3f(0.0, 1.0, 0.0);
    setPixel(0, -4); setPixel(-4, -2);
    setPixel(-4, -2); setPixel(-4, 0);
    setPixel(-4, 0); setPixel(-2, 2);
    setPixel(-2, 2); setPixel(0, 0);
    setPixel(0, 0); setPixel(4, 4);
    setPixel(4, 4); setPixel(4, -4);
    setPixel(4, -4); setPixel(0, -4);
}
void scan_polygon() {
    input();

    glColor3f(1.0, 1.0, 1.0);
    int y_min = 100000, y_max = -10000;
    for(int i = 0; i < original_edges.size(); i++) {
        E_points e = original_edges[i];
        if(e.y1 == e.y2) continue;
        EdgeList.push_back(Edge(e));

        //-1?
        if(EdgeList.size() > 1){
            Edge e1 = EdgeList.back();
            Edge e2 = EdgeList[ EdgeList.size() - 2 ];

            if(e1.y_max <= e2.y_max && e1.y_max == e2.y_min) {
                //cout << "hello \n";
                EdgeList[ EdgeList.size() - 1 ].y_max--;
            }

            if(e2.y_max <= e1.y_max && e2.y_max == e1.y_min) {
                //cout << "hello \n";
                EdgeList[ EdgeList.size() - 2 ].y_max--;
            }
        }

        y_min = min(y_min, EdgeList.back().y_min);
        y_max = max(y_max, EdgeList.back().y_max);
    }

    sort(EdgeList.begin(), EdgeList.end(), cmp);
    int last_taken = -1;
    for(int y = y_min; y <= y_max; y++) {
        //add
        for(int i = last_taken + 1; i < EdgeList.size(); i++){
            if(EdgeList[i].y_min == y) {
                active.insert(i);
                last_taken = i;
            } else {
                break;
            }
        }

        //process
        vector<int> x_val, del;
        for(set<int>::iterator it = active.begin(); it != active.end(); it++){
            x_val.push_back(EdgeList[*it].x);
            EdgeList[*it].x += EdgeList[*it].m_inv;
            if(EdgeList[*it].y_max == y){
                del.push_back(*it);
            }
        }
        sort(x_val.begin(), x_val.end());
        //cout << y <<" : "<< x_val.size() << endl;
        for(int i = 0; i < x_val.size(); i += 1) {
            setPixel(x_val[i], y);
        }

        //remove
        for(int i = 0; i < del.size(); i++) {
            active.erase(active.find(del[i]));
        }
    }

}
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_LINES);

    scan_polygon();

    glEnd();
    glFlush();

}

void init(){
    glClearColor(0.0,0.0,0.0,0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-20.0, 20.0, -20.0, 20.0, -10.0, 10.0);
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(1024,1024);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

    glutCreateWindow("Demo");
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return EXIT_SUCCESS;
}
