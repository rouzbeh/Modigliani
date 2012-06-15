#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

int main()
{
    {
        float package[2];
        ofstream file("test.dat", std::ios::binary);
        for (int ll=0; ll < 100; ll++) {
            package[0] = ll;
            package[1] = (float) ll/sqrt(static_cast<double>(ll));
            file.write( reinterpret_cast<char*>(package), 2*sizeof(float) );
            std::cout << package[0] << "\t" << package[1] << std::endl;
        }
        file.close();
    }
    {
        float tmp[2];
        ifstream data("test.dat", std::ios::binary);
        for (int ll=0; ll < 100; ll++) {
            data.read( reinterpret_cast<char*>(tmp), sizeof(float)*2 );
            std::cout << tmp[0] <<  "\t" << tmp[1] << std::endl;
        }
    }
}
