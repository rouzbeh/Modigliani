#include "../mbase/Ring_buffer.h"

int main()
{
    mbase::Ring_buffer <int> test(5);

    mbase::Msize ll =0;
    for (ll= 0; ll < 100; ll++) {
        test.Push(ll);
        test.Show();
    }

}
