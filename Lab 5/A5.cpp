#include <GL/gl.h>
#include <GL/glut.h>
#include<bits/stdc++.h>
#define ld long double
using namespace std;
ld x_mn,y_mn,x_mx,y_mx;

vector<pair<ld,ld> > st,ed;
void display(void)
{
/* clear all pixels */
    glClear (GL_COLOR_BUFFER_BIT);
/* draw white polygon (rectangle) with corners at
* (0.25, 0.25, 0.0) and (0.75, 0.75, 0.0)
*/





        glBegin(GL_POLYGON); //Begin quadrilateral coordinates
        glColor3f (1.0, 1.0, 1.0);

        //Trapezoid
        glVertex3f(x_mn, y_mn, 0.0f);
        glVertex3f(x_mx, y_mn, 0.0f);
        glVertex3f(x_mx, y_mx, 0.0f);
        glVertex3f(x_mn, y_mx, 0.0f);
        glEnd(); //End quadrilateral coordinates

        glBegin(GL_LINES);


        for(int32_t i=0;i<st.size();i++)
        {
            glColor3f (1.0, 0.0, 0.0);
            glVertex3f(st[i].first, st[i].second, 0.0f);
            glVertex3f(ed[i].first, ed[i].second, 0.0f);
        }

        glEnd();

glFlush ();
}
void init (void)
{
/* select clearing (background) color */
    glClearColor (0.0, 0.0, 0.0, 0.0);
    /* initialize viewing values */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-20.0, 20.0, -20.0, 20.0, -20.0, 20.0);
}
/*
* Declare initial window size, position, and display mode
* (single buffer and RGBA). Open window with "hello"
* in its title bar. Call initialization routines.
* Register callback function to display graphics.
* Enter main loop and process events.
*/

int32_t get_mask(ld x, ld y)
{
    int32_t tmp=0;
    if(x<x_mn) tmp|=1;
    if(x>x_mx) tmp|=2;
    if(y<y_mn) tmp|=4;
    if(y>y_mx) tmp|=8;
    return tmp;
}

pair<ld,ld> work(ld x,ld y, ld m, int32_t msk)
{
    int cnt=0;
    ld tmp_x, tmp_y, im=1./m;
    while(msk)
    {
        cnt++;
        if(cnt>10) break;
        if(msk&1)
        {
            tmp_x= x_mn;
            tmp_y= y+m*(tmp_x-x);
            x=tmp_x;
            y=tmp_y;

            msk= get_mask(x,y);
        }
        if(msk&2)
        {
            tmp_x= x_mx;
            tmp_y= y+m*(tmp_x-x);
            x=tmp_x;
            y=tmp_y;
            msk= get_mask(x,y);
        }
        if(msk&4)
        {
            tmp_y= y_mn;
            tmp_x= im*(tmp_y-y)+x;
            x=tmp_x;
            y=tmp_y;
            msk= get_mask(x,y);
        }
        if(msk&8)
        {
            tmp_y= y_mx;
            tmp_x= im*(tmp_y-y)+x;
            x=tmp_x;
            y=tmp_y;
            msk= get_mask(x,y);
        }
    }
    return {x,y};
}


void solve()
{
    ld a,b,c,d,e,f,g,h,n,x,y,xx,yy,m;
    cin >> a >> b >> c >> d >> e >> f >> g >> h;
    x_mn=a;
    y_mn=b;
    x_mx=e;
    y_mx=f;
    cin >> n;
    for(int i=0;i<n;i++)
    {
        cin >> x >> y >> xx >> yy;
        m=(yy-y)/(xx-x);
        int32_t msk1= get_mask(x,y);
        int32_t msk2= get_mask(xx,yy);

        if(msk1==0 && msk2==0)
        {
            st.push_back({x,y});
            ed.push_back({xx,yy});
            continue;
        }
        if(msk1&msk2) continue;
        pair<ld,ld> newP1 = work(x,y,m,msk1);
        pair<ld,ld> newP2 = work(xx,yy,m,msk2);
        msk1 = get_mask(newP1.first, newP1.second);
        msk2 = get_mask(newP2.first, newP2.second);
        if(msk1==0 && msk2==0)
        {
            st.push_back(newP1);
            ed.push_back(newP2);
        }


    }

}

int32_t main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (600, 600);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Demo");
    init ();
    solve();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

