///2018331085

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <bits/stdc++.h>
#define ld long double
#define f first
#define s second
#define mnn 1e-6
using namespace std;

void setPixel(float x, float y){
    glVertex3f(x, y, 0.0);
}

ld x_mn, x_mx, y_mn, y_mx;
int n;
queue< pair< pair<ld,ld>,pair<ld,ld> > > q;



int get_mask(ld x,ld y)
{
    int tmp=0;
    if(x<x_mn) tmp|=1;
    if(x>x_mx) tmp|=2;
    if(y<y_mn) tmp|=4;
    if(y>y_mx) tmp|=8;
    return tmp;
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);


    int msk1,msk2;

    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    setPixel(x_mn,y_mn);
    setPixel(x_mx,y_mn);
    setPixel(x_mx,y_mx);
    setPixel(x_mn,y_mx);


    glEnd();



    glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 0.0);
    setPixel(-2000,0);
    setPixel(2000,0);
    setPixel(0,-2000);
    setPixel(0,2000);
    glEnd();
    glBegin(GL_LINES);
    pair< pair<ld,ld>,pair<ld,ld> > pt,p1,p2;
    while(!q.empty())
    {
        pt = q.front();
        q.pop();
        msk1 = get_mask(pt.f.f, pt.f.s);
        msk2 = get_mask(pt.s.f, pt.s.s);
        if(msk1 == 0 && msk2 == 0) {
            setPixel(pt.f.f, pt.f.s);
            setPixel(pt.s.f, pt.s.s);
            continue;
        }
        if(msk1&msk2) continue;
        ld x=pt.f.f+pt.s.f;
        x/=2.;
        ld y=pt.f.s+pt.s.s;
        y/=2.;

        if(abs(pt.f.f-x) < mnn && abs(pt.f.s-y) < mnn) ;
        else
            q.push({pt.f,{x,y}});
        if(abs(pt.s.f-x) < mnn && abs(pt.s.s-y)< mnn ) ;
        else
            q.push({{x,y},pt.s});
    }



    glEnd();
    glFlush();

}

void pre()
{
    ld a,b,c,d;
    cin >> x_mn >> y_mn >> x_mx >> y_mx >> n;
    for(int i=0;i<n;i++)
    {
        cin >> a >> b >> c >> d;
        q.push({{a,b},{c,d}});
    }
}

void init(){
    pre();
    glClearColor(0.0,0.0,0.0,0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-20.0, 20.0, -20.0, 20.0, -2000.0, 2000.0);
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
