#include <stdio.h>
#include <math.h>

#define M_PI 3.14159265358979323846

/*
 * main
 *  DESCRIPTION: compute a set of points of a function and print the results
 *  INPUTS: console (int, float float) -- equation related constants
 *  OUTPUTS: console (x, f(x)) -- sequence of values to be printed
 *  RETURN VALUE: 0
 *  SIDE EFFECTS: none
 */
int main()
{
    // Declare variables
    int n;
    float omega1;
    float omega2;
    double pi = M_PI;
    // Prompt user for input
    printf("Enter n, omega1, and omega2, in that order.\n");
    // Get user input
    scanf("%d %f %f", &n, &omega1, &omega2);
    omega1 = (double)omega1;
    omega2 = (double)omega2;
    // Compute function output
    /* for i from 0 to n-1
     *     compute and print xi and f(xi)
     *     use sin() function from math.h
     */
     for(int i=0; i<n; i++){
       double x = (double)i * pi / (double)n;
       double o1x = omega1 * x;
       double o2x = omega2 * x;
       double so1x = sin(o1x);
       double so2x = sin(o2x);
       double f = so1x + so2x/2;

       printf("(%lf, %lf)\n", x, f);
     }
    return 0;
}
