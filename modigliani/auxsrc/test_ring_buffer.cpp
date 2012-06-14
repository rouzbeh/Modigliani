#include "../mbase/Ring_buffer.h"

int main()
{
    modigliani_base::Ring_buffer <int> test(5);

    modigliani_base::Size ll =0;
    for (ll= 0; ll < 100; ll++) {
        test.Push(ll);
        test.Show();
    }

}
