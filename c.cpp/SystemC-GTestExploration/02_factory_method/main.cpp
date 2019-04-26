#include "factory.h"

#include <systemc>
#include <gtest/gtest.h>

int sc_main(int argc, char* argv[]) {

    factory::get_instance().create();

    testing::InitGoogleTest(&argc, argv);
    int status = RUN_ALL_TESTS();

    factory::get_instance().destroy();

    return status;
}
