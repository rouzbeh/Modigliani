#include "nt_2dim_histogram_obj.h"
#include "nt_histogram_obj.h"
#include "nt_gaussian_rnd_dist_obj.h"


int main()
{

    NT_histogram_o histX(-10,10,11);
    NT_histogram_o histY(-10,10,11);
    NT_2dim_histogram_o histA(11,-10,10,11,-10,10);
    NT_2dim_histogram_o histB(11,-10,10,11,-10,10);
    NT_2dim_histogram_o histC(11,-10,10,11,-10,10);

    NT_gaussian_rnd_dist_o rnd;
    NTreal tmp = 0;
    NTreal tmp2 = 0;
    for (NTsize ll= 0; ll < 1000000; ll++) {
        tmp = rnd.RndVal();
        tmp2 = rnd.RndVal();
        histX.BinValue(tmp);
        histY.BinValue(tmp2);
        histA.BinValue(tmp, 0);
        histB.BinValue(tmp, tmp2);
        histC.BinValue(tmp, tmp);
    }

// 	histX.Show();
    cout << "X entropy is " << histX.ShannonEntropy() << endl;
// 	histY.Show();
    cout << "Y entropy is " << histY.ShannonEntropy() << endl;

//   	histA.Show();
    cout << "Binned " << histA._numBinned() << " and unbinned " << histA._numUnbinned() << endl;
    cout << "Joint entropy is " << histA.ShannonEntropy() << endl;

//   	histB.Show();
    cout << "Binned " << histB._numBinned() << " and unbinned " << histB._numUnbinned() << endl;
    cout << "Joint entropy is " << histB.ShannonEntropy() << endl;

//   	histC.Show();
    cout << "Binned " << histC._numBinned() << " and unbinned " << histC._numUnbinned() << endl;
    cout << "Joint entropy is " << histC.ShannonEntropy() << endl;


    cout << "Mututal inforamtion between X & Y  is " <<histX.ShannonEntropy() + histY.ShannonEntropy()  -histB.ShannonEntropy() << endl;
    return 0;
}

