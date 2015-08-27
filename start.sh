#/bin/bash
###############################################################################
#
# Copyright (c) 2015-2016, Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
###############################################################################


cd ./result

while [ "$?" -eq "0" ]
do
    rm driver.cpp func.cpp init.cpp check.cpp init.h icc-no-opt.log icc-opt.log clang-no-opt.log clang-opt.log
    ../a.out

    icpc init.cpp driver.cpp func.cpp check.cpp -o out -O0 -restrict -std=c++11 -vec-threshold0
    ./out > icc-no-opt.log
    icpc init.cpp driver.cpp func.cpp check.cpp -o out -O3 -restrict -std=c++11 -vec-threshold0
    ./out > icc-opt.log

    clang++ init.cpp driver.cpp func.cpp check.cpp -o out -O0 -std=c++11
    ./out > clang-no-opt.log
    clang++ init.cpp driver.cpp func.cpp check.cpp -o out -O3 -std=c++11
    ./out > clang-opt.log

#    cat test.optrpt
    echo "icc diff"
    diff icc-opt.log icc-no-opt.log
    echo "clang diff"
    diff clang-opt.log clang-no-opt.log
    echo "icc vs. clang diff"
    diff icc-opt.log clang-opt.log

done
