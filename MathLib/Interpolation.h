

template < class T > void polint( T xa[], T ya[], int n, T x, T *y, T *dy );
//void polint( double xa[], double ya[], int n, double x, double *y, double *dy );


void spline( double x[], double y[], int n, double yp1, double ypn, double y2[] );
void splint( double xa[], double ya[], double y2a[], int n, double x, double *y );
