#include <stdio.h>
#include <stdlib.h> //exit()
#include <math.h> // sqrt()

//standoline functions
int draw_triangle(int side1,int side2, int side3);
void type_triangle(int side1,int side2, int side3);
int perimeter_triangle(int side1,int side2, int side3);
double area_triangle(int side1,int side2, int side3, int perimeter);

//=================
//Main function
//=================
int main()
{
    int side1=0, side2=0, side3=0;

    printf("The length of first side:\n");
    scanf("%d", &side1);
    printf("The length of second side:\n");
    scanf("%d", &side2);
    printf("The length of third side:\n");
    scanf("%d", &side3);

    printf("According to the triangle inequality theorem, ");
    if(draw_triangle(side1, side2, side3) == 0){
        printf("this triangle cannot be drawn.");
        exit(1);
    }
    printf("this triangle can be drawn.");
    printf("\n");
    type_triangle(side1, side2, side3);
    printf("\n");
  
    int perimeter = perimeter_triangle(side1, side2, side3);
    printf("The perimeter of the triangle:%d\n", perimeter);

    printf("The area of the triangle:%lf\n", area_triangle(side1, side2, side3, perimeter));


    return 0;
}
//It returns 1 if that triangle can be drawn by looking sides.
//"The Triangle Inequality Theorem states that the sum of any 2 sides of a triangle must be greater than
//the measure of the third side."
int draw_triangle(int side1,int side2, int side3)
{
    if (side1 + side2 > side3 && side1 + side3 > side2 && side3 + side2 > side1) return 1;
    return 0;   
}
//It prints type of triangle by looking at side measures equality
void type_triangle(int side1,int side2, int side3)
{
    if(side1 == side2 && side2 == side3) 
        printf("It is a Equilateral Triangle");
    else if(side1 == side2 || side1 == side3 || side1 == side3) 
        printf("It is a Isosceles Triangle");
    else 
        printf("It is a Scalene Triangle");
}
//Returns perimeter of triangle
int perimeter_triangle(int side1,int side2, int side3)
{
    return (side1 + side2 + side3);
}
//It returns area of triangle
double area_triangle(int side1,int side2, int side3, int perimeter)
{
    double p = perimeter / 2.0;
    return sqrt(p*(p-side1)*(p-side2)*(p-side3));
}
