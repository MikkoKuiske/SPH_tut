#ifndef SPH_LIST_HH
#define SPH_LIST_HH

#include "particle.hh"
#include <string>

using namespace std;

class SPH_list
{
public:
    SPH_list();
    ~SPH_list();


private:
    struct lista_ {
        particle item_;
        lista_* next_ptr_;
    };

    lista_* first_ptr_;
    lista_* last_ptr_;

};

#endif // SPH_LIST_HH
