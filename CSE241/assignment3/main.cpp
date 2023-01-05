/*
    Author : Abdurrahman BULUT
    Gebze Technical University
    contact: abdurrahmanbulut@yahoo.com
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <string>

using namespace std;
//=================
//pixel struct that represent RGB 
//=================
struct Pixel
{
    int r = 0, g = 0, b = 0;
    Pixel(){}
    Pixel(int red, int green, int blue){
        r = red; g = green; b = blue;
    }
};
//=======================
//ppmImage class
//=======================
class ppmImage
{
public:
    //default and other constructors
    ppmImage();
    ppmImage(string filename);
    ppmImage(int row, int column);

    void print_dimensions();
    int pixel_info(int row, int column , char color);
    void change_pixel_value(int row, int column, char color, int newColor);
    int save_to_file(string fileName) const;
    int read_from_file(string fileName);

    //Getters-setters
    //Inline Functions
    void set_column(int columnValue){column=columnValue;} 
    int get_column()const {return column;}
    void set_row(int rowValue){row=rowValue;}
    int get_row() const {return row;}
    string get_ppm_format(){return ppmFormat;}
    int get_max_pixel(){return maxPixel;}
    void set_max_pixel(int maxPixelValue){maxPixel=maxPixelValue;}
    void set_ppm_format(const string& ppmFormatValue){ppmFormat=ppmFormatValue;}
    int getAnyPixel(int r, int c, char rgb);

    //functions for third assignment
    friend ppmImage operator +(const ppmImage& obj1, const ppmImage& obj2);
    friend ppmImage operator -(const ppmImage& obj1, const ppmImage& obj2);
    friend ostream& operator <<(ostream& term, const ppmImage& obj);
    int& operator () (int row, int column, int color_channel);
    
private:
    //colors represent red green and blue color values
    Pixel colors;
    int column;
    int row;
    int maxPixel;
    string ppmFormat;
    vector<vector<Pixel> >vec;

};

//Standoline Functions prototypes
int read_ppm(const string source_ppm_file_name, ppmImage& destination_object);
int write_ppm(const string destination_ppm_file_name, const ppmImage& source_object);
int swap_channels(ppmImage& image_object_to_be_modified, int swap_choice);
ppmImage single_color(const ppmImage& source, int color_choice);
//PA3 standoline funcs
int test_addition(const string filename_image1, const string filename_image2, const string filename_image3);
int test_subtraction(const string filename_image1, const string filename_image2, const string filename_image3);
int test_print(const string filename_image1);

//========================
//main function
//========================
int main(int argc, char** argv)
{
    // command line arguments
    // a.exe choice_number file_name1 filename2(optional) filename3(optional)
    //argument cannot be less than 3 and more than 5
    if(argc != 4 && argc != 3 && argc != 5)
        exit(1);
    
        
    
    string choice = argv[1];
    string ppm_file_name1 = argv[2];
    string ppm_file_name2;
    string ppm_file_name3;
    //optional filename check
    if(argc == 4 || argc == 5)
        ppm_file_name2 = argv[3];
    //optional filename check
    if(argc == 5) 
         ppm_file_name3 = argv[4]; //optional
    
    if(choice.size() != 1)
        exit(1);
    
    int choice_number = stoi(choice); 
    
    ppmImage myObj;

    if(choice_number == 1)
    {
        
        if(!test_addition(ppm_file_name1, ppm_file_name2, ppm_file_name3)){
            exit(1);
        }
        
    }
    else if(choice_number == 2)
    {
        if(!test_subtraction(ppm_file_name1, ppm_file_name2, ppm_file_name3))
            exit(1);
    }
    else if(choice_number == 3)
    {   //checks whether the file can be opened and whether operation was succesfull.
    
        if(read_ppm(ppm_file_name1, myObj)){            // read ppm_file_name using function read_ppm
            if(swap_channels(myObj, 2)){                // swap red and blue channels
                if(!(write_ppm(ppm_file_name2, myObj))) // write the updated data to a file named "o3.ppm". use write_ppm function.
                    exit(1);
                
            }
            else exit(1);
        }
        else  exit(1); 
    }
    else if(choice_number == 4)
    {   //checks whether the file can be opened and whether operation was succesfull.
        if(read_ppm(ppm_file_name1, myObj)){            // read ppm_file_name using function read_ppm
            if(swap_channels(myObj, 3)){                // swap green and blue channels. use swap_channels function
                if(!(write_ppm(ppm_file_name2, myObj))) // write the updated data to a file named "o4.ppm". use write_ppm function.
                    exit(1);
            }
            else exit(1);
        }
        else  exit(1);
    }    
    else if(choice_number == 5)
    {   //checks whether the file can be opened and whether operation was succesfull.
        if(read_ppm(ppm_file_name1, myObj)){          // read ppm_file_name using function read_ppm
            ppmImage newObj;                          // create a new object which only contains red channel data of the file read. use single_color function
            newObj = single_color(myObj, 1);

            if(!(write_ppm(ppm_file_name2, newObj)))  // write the data of the new object to a file named "o5.ppm". use write_ppm function.
                exit(1);
        }
        else exit(1);
    }   
    else if(choice_number == 6)
    {   //checks whether the file can be opened and whether operation was succesfull.
        if(read_ppm(ppm_file_name1, myObj)){          // read ppm_file_name using function read_ppm
            ppmImage newObj;                          // create a new object which only contains green channel data of the file read. use single_color function
            newObj = single_color(myObj, 2);

            if(!(write_ppm(ppm_file_name2, newObj)))  // write the data of the new object to a file named "o6.ppm". use write_ppm function.
                exit(1);
        }
        else exit(1);
    }
    else if(choice_number == 7)
    {   //checks whether the file can be opened and whether operation was succesfull.
        if(read_ppm(ppm_file_name1, myObj)){         // read ppm_file_name using function read_ppm
            ppmImage newObj;                         // create a new object which only contains blue channel data of the file read. use single_color function
            newObj = single_color(myObj, 3);

            if(!(write_ppm(ppm_file_name2, newObj))) // write the data of the new object to a file named "o7.ppm". use write_ppm function.
                exit(1);
        }
        else exit(1);
    }
    else
    {
        //missing parameter, so exit from the program.
        exit(1);
    }

    return 0;

}
//Default constructor
ppmImage::ppmImage()
{   
    ppmFormat = "P3";
    column = 0;
    row = 0;
    maxPixel = 255;
}
//Constructor ppmImage
//inputs: filename that given by user from user interface
//constructor that read from file and create object with infos from file.
//output: if file can opened succesfully, read it and returns 1, otherwise returns 0
ppmImage::ppmImage(string fileName)
{
    ifstream myFile;
    myFile.open(fileName);
    
    if (myFile.is_open())
    {
        myFile >> ppmFormat >> column >> row >> maxPixel;
        
        if(ppmFormat != "P3")
            exit(1);

        vec.resize(row);
        for (int i = 0; i < row; i++)
            vec[i].resize(column);

        int temp = -1;

        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < column; i++)
            {
                //7th rule of ppm file format, 
                //The maximum color value (Maxval), again in ASCII decimal.
                myFile >> temp;
                if(temp < 0 || temp > 255) exit(1);
                vec[i][j].r = temp;
                myFile >> temp;
                if(temp < 0 || temp > 255) exit(1);
                vec[i][j].g = temp;
                myFile >> temp;
                if(temp < 0 || temp > 255) exit(1);
                vec[i][j].b = temp;
            }
        }
        myFile.close();
    }
    else{
        myFile.close();
        exit(1);
    }

}
//constructor
//inputs: size of image, x and y coordinates.
//it creates an image that  all pixels of it will be white according to size of board
ppmImage::ppmImage(int row, int column)
{

    vec.resize(row);
    for (int i = 0; i < row; i++)
        vec[i].resize(column);

    ppmFormat = "P3";
    this->column = column;
    this->row = row;
    maxPixel = 255;

    for (int i = 0; i < row ; i++)
    {
        for (int j = 0; j < column; j++)
        {
            vec[i][j].r = 255;
            vec[i][j].g = 255;
            vec[i][j].b = 255;
        }
    }
}
//save_to_file member function
//input: filename that enters from user
//it takes all datas from object and stores to file.
//output: returns 1 if it's succesfull , otherwise returns 0
int ppmImage::save_to_file(string fileName) const
{
    ofstream myFile;
    myFile.open(fileName);

    if(myFile.is_open())
    { 
        myFile << ppmFormat << "\n" << column << ' ' << row << "\n" << maxPixel << "\n";
        
        for (int i = 0 ; i < row; i++)
        {
            for (int j = 0; j < column; j++)
            {
                myFile << vec[i][j].r << ' ';
                myFile << vec[i][j].g <<' ';
                myFile << vec[i][j].b;
                
                if(j == column-1)
                    myFile << "\n";
                else
                    myFile <<' ';
            }
        }
        myFile.close();
        return 1;
    }
    else
    {
        myFile.close();
        return 0;
    }
}
//read_from_file member function
//input: filename that enters from user
//it takes all datas from file and stores to object.
//output: returns 1 if it's succesfull , otherwise returns 0
int ppmImage::read_from_file(const string fileName)
{
    ifstream myFile;
    myFile.open(fileName);

    if (myFile.is_open())
    {
        myFile >> ppmFormat >> column >> row >> maxPixel;
        if(ppmFormat != "P3") exit(1);
        
        vec.resize(row);
        for (int i = 0; i < row; i++)
            vec[i].resize(column);

        int temp = -1;

        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < column; j++)
            {
                myFile >> temp;
                if(temp < 0 || temp > 255) exit(1);
                vec[i][j].r = temp;
                myFile >> temp;
                if(temp < 0 || temp > 255) exit(1);
                vec[i][j].g = temp;
                myFile >> temp;
                if(temp < 0 || temp > 255) exit(1);
                vec[i][j].b = temp;
            }
        }
        myFile.close();
        return 1;
    }
    else
    {
        myFile.close();
        return 0;
    }
        
}
//prints dimensions of image
void ppmImage::print_dimensions()
{
    cout << column << ' ' << row << endl;
}
//pixel_info member function
//inputs: row and column of image and color which are RGB .
//For my code structure , all pixel infos are kept in pixel class's object. 
//output: returns pixel information by row and column
int ppmImage::pixel_info(int row, int column, char color)
{   
    if(row > this->row-1 || row < 0 || column > this->column-1 || column < 0)
        exit(1);
    else if(color == 'r')
        return vec[row][column].r;
    else if(color == 'g')
        return vec[row][column].g;
    else if(color == 'b')
        return vec[row][column].b;
    else
        exit(1);
}
//change_pixel_value member function
//change in r and c coordinate pixel's color value from newColor value
void ppmImage::change_pixel_value(int r, int c, char color, int newColorValue)
{
    if (newColorValue <= 255 && newColorValue >= 0 && r < row && c < column)
    {   
       if (color == 'r')
            vec[r][c].r = newColorValue;
        else if (color == 'g')
            vec[r][c].g = newColorValue;
        else if (color == 'b')
            vec[r][c].b = newColorValue;
        else
            exit(1);
    }
    else
        exit(1);
} 

// returns 1 if the operation is successful. otherwise, returns 0.
// reads ppm data from file named as source_ppm_file_name. stores data in destination_object
// which is already created outside of the function.
int read_ppm(const string source_ppm_file_name, ppmImage& myObj)
{   
    return (myObj.read_from_file(source_ppm_file_name));
}

// returns 1 if the operation is successful. otherwise, returns 0.
// writes ppm data from source_object to the file named destination_ppm_file_name.
int write_ppm(const string destination_ppm_file_name, const ppmImage& myObj)
{
     return (myObj.save_to_file(destination_ppm_file_name));   
}

// creates and returns a copy of a new ppmImage object which stores only one color at each
//pixel. This simply takes the source and copies only one color information and stores it in
//a new object. The other color channels are simply going to be zeros.
//if color_choice is 1: red channel is preserved
//if color_choice is 2: green channel is preserved
//if color_choice is 3: blue channel is preserved
ppmImage single_color(const ppmImage& source, int color_choice) 
{
    ppmImage copyObj;
    copyObj = source;

    if(color_choice == 1)
    {
        for (int i = 0; i < copyObj.get_row(); i++)
        {
            for (int j = 0; j < copyObj.get_column(); j++)
            {   
                copyObj.change_pixel_value(i, j, 'g', 0);
                copyObj.change_pixel_value(i, j, 'b', 0);
            } 
        } 
    }
    else if(color_choice == 2)
    {
        for (int i = 0; i < copyObj.get_row(); i++)
        {
            for (int j = 0; j < copyObj.get_column(); j++)
            {
                copyObj.change_pixel_value(i, j, 'r', 0);
                copyObj.change_pixel_value(i, j, 'b', 0);
            } 
        } 
    }
    else if(color_choice == 3)
    {
        for (int i = 0; i < copyObj.get_row(); i++)
        {
            for (int j = 0; j < copyObj.get_column(); j++)
            {
                copyObj.change_pixel_value(i, j, 'g', 0);
                copyObj.change_pixel_value(i, j, 'r', 0);
            } 
        } 
    }
    else
        exit(1);

    return copyObj;
}
//getAnyPixel member function
//inputs: image's row and column infos, and wanted color value name ; r : red, g : green, b: blue
//output: returns specific color value in any pixel
int ppmImage::getAnyPixel(int r, int c, char rgb)
{
        if (r >= row || c >= column || r < 0 || c < 0) 
            exit(1);

        if(rgb == 'r')
            return vec[r][c].r;
        else if(rgb == 'g')
            return vec[r][c].g;
        else if(rgb == 'b')
            return vec[r][c].b;
        else 
            exit(1);
}
// Adds two ppmImage objects pixel-by-pixel. 
// If They are not the same size  it returns an empty ppmImage object.
// If sum of channels will be bigger than 255(max value), it will fix to 255 . 
ppmImage operator +(const ppmImage& obj1, const ppmImage& obj2)
{
    ppmImage sum;

     if(obj1.get_column() != obj2.get_column() || obj1.get_row() != obj2.get_row())
    {
        return sum;        
    } 
    int red = 0;
    int green = 0;
    int blue = 0;
    sum.ppmFormat = obj1.ppmFormat;
    sum.column = obj1.column;
    sum.row = obj1.row;

    sum.vec.resize(obj1.row);
    for (int i = 0; i < obj1.row; i++)
        sum.vec[i].resize(obj1.column);

    for (int i = 0; i < obj1.get_row(); i++)
    {
        for (int j = 0; j < obj1.get_column(); j++)
        {
            red = obj1.vec[i][j].r + obj2.vec[i][j].r; 
            if(red > 255) red = 255;
            sum.vec[i][j].r = red;

            green = obj1.vec[i][j].g + obj2.vec[i][j].g; 
            if(green > 255) green = 255;
            sum.vec[i][j].g = green;

            blue = obj1.vec[i][j].b + obj2.vec[i][j].b; 
            if(blue > 255) blue = 255;
            sum.vec[i][j].b = blue;
        }
    }
    return sum;
    
}
// Subtracts two ppmImage objects pixel-by-pixel. 
// If They are not the same size  it returns an empty ppmImage object.
// If subtract of channels will be lower than 0(min value), it will fix to 0 . 
ppmImage operator -(const ppmImage& obj1, const ppmImage& obj2)
{
    ppmImage diff;

     if(obj1.get_column() != obj2.get_column() || obj1.get_row() != obj2.get_row())
    {
        return diff;        
    } 

    diff.ppmFormat = obj1.ppmFormat;
    diff.column = obj1.column;
    diff.row = obj1.row;

    diff.vec.resize(obj1.row);
    for (int i = 0; i < obj1.row; i++)
        diff.vec[i].resize(obj1.column);

    int red = 0;
    int green = 0;
    int blue = 0;
    
    for (int i = 0; i < obj1.get_row(); i++)
    {
        
        for (int j = 0; j < obj1.get_column(); j++)
        {
            
            red = obj1.vec[i][j].r - obj2.vec[i][j].r; 
            if(red < 0) red = 0;
            diff.vec[i][j].r = red;
            
            green = obj1.vec[i][j].g - obj2.vec[i][j].g; 
            if(green < 0) green = 0;
            diff.vec[i][j].g = green;

            blue = obj1.vec[i][j].b - obj2.vec[i][j].b; 
            if(blue < 0) blue = 0;
            diff.vec[i][j].b = blue;
        }
    }
    return diff;
}
//  Prints image details and image data to stdout.
ostream& operator <<(ostream& term, const ppmImage& obj)
{
        term << obj.ppmFormat << "\n" << obj.column << ' ' << obj.row << "\n" << obj.maxPixel << "\n";
        
        for (int i = 0 ; i < obj.row; i++)
        {
            for (int j = 0; j < obj.column; j++)
            {
                term << obj.vec[i][j].r << ' ';
                term << obj.vec[i][j].g <<' ';
                term << obj.vec[i][j].b;
                
                if(j == obj.column-1)
                    term << "\n";
                else
                    term <<' ';
            }
        }
    return term;
}
// This operator takes three parameters. The first parameter is the index
// of the row number. The second parameter is the index of the column number. The third parameter is the
// color channel. It can either 1, 2 or 3. The upper left corner is (0,0). This operator returns a reference to the pixel value.
int& ppmImage::operator()(int row, int column, int color_channel)
{   
   
    if(color_channel == 1)
       return this->vec[row][column].r;
    else if(color_channel == 2) 
        return this->vec[row][column].g;
    else if(color_channel == 3)
        return this->vec[row][column].b;
    exit(1);
    
}
// returns 1 if the operation is successful. otherwise, returns 0.
// reads images from filename_image1 and filename_image2. Adds them and saves the resulting image to filename_image3
int test_addition(const string filename_image1, const string filename_image2, const string filename_image3)
{
    ppmImage file1_obj, file2_obj;
    
    if(read_ppm(filename_image1, file1_obj)){
        if(read_ppm(filename_image2, file2_obj)){ 
            
            ppmImage sum_obj = file1_obj + file2_obj; 
            if(write_ppm(filename_image3, sum_obj))
            {
                return 1;
            }
        }
    }
    return 0;

}
// returns 1 if the operation is successful. otherwise, returns 0.
// reads images from filename_image1 and filename_image2. Subtracts filename_image2 from  
// filename_image1 saves the resulting image to filename_image3
int test_subtraction(const string filename_image1, const string filename_image2, const string filename_image3)
{
    ppmImage file1_obj, file2_obj;
    
    if(read_ppm(filename_image1, file1_obj)){
        if(read_ppm(filename_image2, file2_obj)){
            
            ppmImage sum_obj = file1_obj - file2_obj;
      
            if(write_ppm(filename_image3, sum_obj))
            {
                
                return 1;
            }
        }
    }
    return 0;
}
// returns 1 if the operation is successful. otherwise, returns 0.
// reads images from filename_image1 and prints it to stdout
int test_print(const string filename_image1)
{   
    ppmImage file1_obj;
    if(read_ppm(filename_image1, file1_obj))
    {    
        cout << file1_obj;
        return 1;
    }
    return 0;
}
// this function swaps the color values of every pixel in a given ppm image.
// this function does not create a new object but modifies the given one.
// if swap_choice is 1: swaps red and green
// if swap_choice is 2: swaps red and blue
// if swap_choice is 3: swaps green and blue
// if swap_choice is not 1, 2 or 3: no swaps (this does not mean that the operation is not
// successful. the function should return 1 in this case if everything is normal)
// returns 1 if the operation is successful. otherwise, returns 0.
int swap_channels(ppmImage& image_object_to_be_modified, int swap_choice)
{   
    if(swap_choice == 1)
    {
        for (int i = 0; i < image_object_to_be_modified.get_row(); i++)
        {   
            for (int j = 0; j < image_object_to_be_modified.get_column(); j++)
            {
                //keep red value in a safe space
                int temp = image_object_to_be_modified(i, j, 1); // 1 -> red
                //change value of red and green
                image_object_to_be_modified.change_pixel_value(i, j, 'r', image_object_to_be_modified(i, j, 2)); //2 -> green
                //put keeped red value to green channel
                image_object_to_be_modified.change_pixel_value(i, j, 'g', temp);
                
            } 
        }
    }
    else if(swap_choice == 2)
    {
        
        for (int i = 0; i < image_object_to_be_modified.get_row(); i++)
        {
            for (int j = 0; j < image_object_to_be_modified.get_column(); j++)
            {   
                //keep red value in a safe space
                int temp = image_object_to_be_modified(i, j, 1); //red
                //change value of red and blue
                image_object_to_be_modified.change_pixel_value(i, j, 'r', image_object_to_be_modified(i, j, 3)); //3 -> blue
                //put keeped red value to blue channel
                image_object_to_be_modified.change_pixel_value(i, j, 'b', temp);
            } 
        } 
    }
    else if(swap_choice == 3)
    {
        for (int i = 0; i < image_object_to_be_modified.get_row(); i++)
        {
            for (int j = 0; j < image_object_to_be_modified.get_column(); j++)
            {
                //keep green value in a safe space
                int temp = image_object_to_be_modified(i, j, 2); //green
                //change value of green and blue
                image_object_to_be_modified.change_pixel_value(i, j, 'g', image_object_to_be_modified(i, j, 3)); //blue
                //put keeped green value to blue channel
                image_object_to_be_modified.change_pixel_value(i, j, 'b', temp);
            } 
        } 
    }
    else
        return 0;
    
    return 1;
}
