#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

using namespace std;
//=================
//pixel class that represent RGB 
//=================
class pixel
{
public:
    //constructors
    pixel(){};  
    pixel(int rValue, int gValue, int bValue): r(rValue), g(gValue), b(bValue){}
    //getters and setters , inline
    int getRed() const { return r; }   
    int getGreen() const { return g; }
    int getBlue() const { return b; }
    void setRed(const int& color) { r = color; }
    void setGreen(const int& color) { g = color; }
    void setBlue(const int& color) { b = color; }

private:
    //red, green, blue datas
    int r, g, b;
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

    ///////////////////////////
    void set_column(int columnValue){column=columnValue;}
    int get_column(){return column;}
    void set_row(int rowValue){row=rowValue;}
    int get_row(){return row;}
    void set_max_pixel(int maxPixelValue){maxPixel=maxPixelValue;}
    int get_max_pixel(){return maxPixel;}
    void set_ppm_format(const string& ppmFormatValue){ppmFormat=ppmFormatValue;}
    string get_ppm_format(){return ppmFormat;}
    int getAnyPixel(int r, int c, char rgb);

    /////////////////////////////
    //getters and setters. 
    //They are for avoid the use of pixel class-object directl in other functions
    int getG(){return obj.getGreen();}
    int getR(){return obj.getRed();}
    int getB() { return obj.getBlue();}
    void setR(int color) {obj.setRed(color);}
    void setG(int color) { obj.setGreen(color);}
    void setB(int color) { obj.setBlue(color);}
    //////////////////////////77
    
private:
    //An object from pixel class
    pixel obj;
    int column;
    int row;
    int maxPixel;
    string ppmFormat;
    vector<vector<pixel> >vec;

};

//Standoline Functions prototypes
int read_ppm(const string source_ppm_file_name, ppmImage& destination_object);
int write_ppm(const string destination_ppm_file_name, const ppmImage& source_object);
int swap_channels(ppmImage& image_object_to_be_modified, int swap_choice);
ppmImage single_color(const ppmImage& source, int color_choice);

//=======================
//main function
//=======================
int main(int argc, char** argv)
{
    //test.exe number filename -> should be exactly 3 parameters
    if(argc != 3)
        exit(1);

    string fileName = argv[2];
    int choice = stoi(argv[1]);

    ppmImage myObj;
    
    
    if(choice == 1) // if choice number is 1
    {   //checks whether the file can be opened and whether operation was succesfull.
        if(read_ppm(fileName, myObj)){           // read ppm_file_name using function read_ppm
            if(!(write_ppm("o1.ppm", myObj)))    // write the same data without changing anything to a file named "o1.ppm". use write_ppm
                exit(1);
        }
        else exit(1);
    }
    else if(choice == 2)
    {   //checks whether the file can be opened and whether operation was succesfull.
        
         if(read_ppm(fileName, myObj)){
            if(swap_channels(myObj, 1)){
                if(!(write_ppm("o2.ppm", myObj)))
                    exit(1);
            }
            else exit(1);
        }
        else  exit(1); 
    }
    else if(choice == 3)
    {   //checks whether the file can be opened and whether operation was succesfull.
    
        if(read_ppm(fileName, myObj)){               // read ppm_file_name using function read_ppm
            if(swap_channels(myObj, 2)){             // swap red and blue channels
                if(!(write_ppm("o3.ppm", myObj)))    // write the updated data to a file named "o3.ppm". use write_ppm function.
                    exit(1);
            }
            else exit(1);
        }
        else  exit(1); 
    }
    else if(choice == 4)
    {   //checks whether the file can be opened and whether operation was succesfull.
        if(read_ppm(fileName, myObj)){              // read ppm_file_name using function read_ppm
            if(swap_channels(myObj, 3)){            // swap green and blue channels. use swap_channels function
                if(!(write_ppm("o4.ppm", myObj)))   // write the updated data to a file named "o4.ppm". use write_ppm function.
                    exit(1);
            }
            else exit(1);
        }
        else  exit(1);
    }    
    else if(choice == 5)
    {   //checks whether the file can be opened and whether operation was succesfull.
        if(read_ppm(fileName, myObj)){              // read ppm_file_name using function read_ppm
            ppmImage newObj;                        // create a new object which only contains red channel data of the file read. use single_color function
            newObj = single_color(myObj, 1);

            if(!(write_ppm("o5.ppm", newObj)))      // write the data of the new object to a file named "o5.ppm". use write_ppm function.
                exit(1);
        }
        else exit(1);
    }   
    else if(choice == 6)
    {   //checks whether the file can be opened and whether operation was succesfull.
        if(read_ppm(fileName, myObj)){          // read ppm_file_name using function read_ppm
            ppmImage newObj;                    // create a new object which only contains green channel data of the file read. use single_color function
            newObj = single_color(myObj, 2);

            if(!(write_ppm("o6.ppm", newObj)))  // write the data of the new object to a file named "o6.ppm". use write_ppm function.
                exit(1);
        }
        else exit(1);
    }
    else if(choice == 7)
    {   //checks whether the file can be opened and whether operation was succesfull.
        if(read_ppm(fileName, myObj)){              // read ppm_file_name using function read_ppm
            ppmImage newObj;                        // create a new object which only contains blue channel data of the file read. use single_color function
            newObj = single_color(myObj, 3);

            if(!(write_ppm("o7.ppm", newObj)))      // write the data of the new object to a file named "o7.ppm". use write_ppm function.
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
    setR(0);
    setG(0);
    setB(0);
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
                //The maximum color value (Maxval), again in ASCII decimal. Must be less than 65536 and more than zero.
                myFile >> temp;
                if(temp < 0 || temp > 65536) exit(1);
                vec[i][j].setRed(temp);
                myFile >> temp;
                if(temp < 0 || temp > 65536) exit(1);
                vec[i][j].setGreen(temp);
                myFile >> temp;
                if(temp < 0 || temp > 65536) exit(1);
                vec[i][j].setBlue(temp);
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
            vec[i][j].setRed(255);
            vec[i][j].setGreen(255);
            vec[i][j].setBlue(255);
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
                myFile << vec[i][j].getRed() << ' ';
                myFile << vec[i][j].getGreen() <<' ';
                myFile << vec[i][j].getBlue();
                
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
                if(temp < 0 || temp > 65536) exit(1);
                vec[i][j].setRed(temp);
                myFile >> temp;
                if(temp < 0 || temp > 65536) exit(1);
                vec[i][j].setGreen(temp);
                myFile >> temp;
                if(temp < 0 || temp > 65536) exit(1);
                vec[i][j].setBlue(temp);
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
        return vec[row][column].getRed();
    else if(color == 'g')
        return vec[row][column].getGreen();
    else if(color == 'b')
        return vec[row][column].getBlue();
    else
        exit(1);
}
//change_pixel_value member function
//change in r and c coordinate pixel's color value from newColor value
void ppmImage::change_pixel_value(int r, int c, char color, int newColorValue)
{
    if (newColorValue <= 65536 && newColorValue >= 0 && r < row && c < column)
    {   
       if (color == 'r')
            vec[r][c].setRed(newColorValue);
        else if (color == 'g')
            vec[r][c].setGreen(newColorValue);
        else if (color == 'b')
            vec[r][c].setBlue(newColorValue);
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
// this function swaps the color values of every pixel in a given ppm image.
// this function does not create a new object but modifies the given one.
// if swap_choice is 1: swaps red and green
// if swap_choice is 2: swaps red and blue
// if swap_choice is 3: swaps green and blue
// if swap_choice is not 1, 2 or 3: no swaps (this does not mean that the operation is not
// successful. the function should return 1 in this case if everything is normal)
// returns 1 if the operation is successful. otherwise, returns 0.
int swap_channels(ppmImage& myObj, int swap_choice)
{   
    if(swap_choice == 1)
    {
        for (int i = 0; i < myObj.get_row(); i++)
        {   
            for (int j = 0; j < myObj.get_column(); j++)
            {
                int temp = myObj.getAnyPixel(i, j, 'r');
                myObj.change_pixel_value(i, j, 'r', myObj.getAnyPixel(i, j, 'g'));
                myObj.change_pixel_value(i, j, 'g', temp);
            } 
        }
    }
    else if(swap_choice == 2)
    {
        for (int i = 0; i < myObj.get_row(); i++)
        {
            for (int j = 0; j < myObj.get_column(); j++)
            {
                int temp = myObj.getAnyPixel(i, j, 'r');
                myObj.change_pixel_value(i, j, 'r', myObj.getAnyPixel(i, j, 'b'));
                myObj.change_pixel_value(i, j, 'b', temp);
            } 
        } 
    }
    else if(swap_choice == 3)
    {
        for (int i = 0; i < myObj.get_row(); i++)
        {
            for (int j = 0; j < myObj.get_column(); j++)
            {
                int temp = myObj.getAnyPixel(i, j, 'g');
                myObj.change_pixel_value(i, j, 'g', myObj.getAnyPixel(i, j, 'b'));
                myObj.change_pixel_value(i, j, 'b', temp);
            } 
        } 
    }
    else
        return 0;
    
    return 1;
}
// creates and returns a copy of a new ppmImage object which stores only one color at each
//pixel. This simply takes the source and copies only one color information and stores it in
//a new object. The other color channels are simply going to be zeros.

//if color_choice is 1: red channel is preserved
//if color_choice is 2: green channel is preserved
//if color_choice is 3: blue channel is preserved
ppmImage single_color(const ppmImage& myObj, int color_choice) 
{
    ppmImage copyObj;
    copyObj = myObj;

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
                //int temp = myObj.getR();
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
                int temp = copyObj.getG();
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
            return vec[r][c].getRed();
        else if(rgb == 'g')
            return vec[r][c].getGreen();
        else if(rgb == 'b')
            return vec[r][c].getBlue();
        else 
            exit(1);
    }