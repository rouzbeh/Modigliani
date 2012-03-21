#include "../ntsrc/nt_ring_buffer_obj.h"

int main()
{
    NT_ring_buffer_o <int> test(5);

    NTsize ll =0;
    for (ll= 0; ll < 100; ll++) {
        test.Push(ll);
        test.Show();
    }

}
